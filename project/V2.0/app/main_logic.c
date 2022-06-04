/*
 * @Author: your name
 * @Date: 2021-12-06 23:26:25
 * @LastEditTime: 2021-12-06 23:28:02
 * @LastEditors: your name
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \demo_emmovo\app\main_logic.c
 */
#include "main_logic.h"
#include "hall.h"
#include "uart.h"
#include "leds.h"
#include "tuya_ble_api.h"
#include "ble.h"
#include "gpio.h"
#include "vibrator.h"
#include "tuya_ble_api.h"
#include "arch.h"
#include "rf.h"
#include "sleep.h"
#include "flash.h"
#include "app_tuya.h"

enum mode_e main_logic_mode = mode_0_idle;
struct mode_param_s mode_param = {.led_sleep = 0, \
																	.led_sleep_tick = 0,	\
																	.power_off = 0,	\
																	.hall_stop_led_trigger = 0,	\
																	.sec_hall_count = 0};
struct key_scan_param_s key_scan_param = { .pwr_btn_on_time = 0, \
																						.pwr_btn_off_time = 0, \
																						.set_btn_on_time = 0, \
																						.set_btn_off_time = 0, \
																						.pwr_btn_last_status = 0,\
																						.set_btn_last_status = 0,\
																						.pwr_btn_mode = idle,
																						.set_btn_mode = idle,};

const uint8_t target_time_level[7] = { 1, 3, 5, 10, 20, 30, 60};   // target_time_level[n] x 60s
const uint8_t target_count_level[7] ={ 1, 3, 5, 10, 20, 30, 50};	// target_count_level[n] x 100

uint8_t hall_cyc = 0;

uint8_t mode_2_end_report_delay = 0;
uint32_t main_sleep_tick_count = 0;
uint32_t led_sleep_tick_count = 0;
bool ble_reset = false;
uint32_t ble_reset_power_off_tick = 0;

extern tuya_ble_device_param_t device_param;
extern bool connection_status;

void mode_param_init(void)
{
		mode_param.calorie = 0;
		mode_param.cyc_count = 0;
		mode_param.time = 0;
		mode_param.led_sleep = 0;
		mode_param.led_sleep_tick = 0;
		mode_param.hall_stop_led_trigger = 0;
		mode_param.sec_hall_count = 0;
}

int main_logic_pwr_btn(void)
{
	return !gpio_get_input(GPIOB_7);
}

int main_logic_set_btn(void)
{
	return !gpio_get_input(GPIOB_2);
}

void main_logic_ready_to_working(void)
{
		if(main_logic_mode == mode_0_ready)
		{
				main_logic_mode = mode_0_working;
		}
		if(main_logic_mode == mode_1_ready)
		{
				main_logic_mode = mode_1_working;
		}
		if(main_logic_mode == mode_2_ready)
		{
				main_logic_mode = mode_2_working;
		}
}

void level_select_to_ready_tick_clear(void)
{
		mode_param.select_tick = 0;
}

void level_select_to_ready_tick(void)
{
		if(mode_param.select_tick < SELECT_TICK_TIME)
		{
				mode_param.select_tick++;
		}
		else
		{
				
				if(main_logic_mode == mode_1_level_select)
				{
						mode_param_init();
						main_logic_vibrator_set(500, 50, 1);
						ble_dp_return_rt_data(0, 0, 0);
						main_logic_mode = mode_1_ready;
					
						ble_dp_return_pause(0);
						ble_dp_return_start();
						
				}
				
				if(main_logic_mode == mode_2_level_select)
				{
						mode_param_init();
						main_logic_vibrator_set(500, 50, 1);
						ble_dp_return_rt_data(0, 0, 0);
						main_logic_mode = mode_2_ready;
					
						ble_dp_return_pause(0);
						ble_dp_return_start();
				}

			}		
}


void main_logic(void)
{

		uint8_t i;
	
//		uart_printf("%d\r\n",main_sleep_tick_count);
	
//		uart_printf("mode：%d\r\n", main_logic_mode);
	
		if(mode_param.led_sleep_tick < 2000)
		{
				mode_param.led_sleep_tick++;
		}
		
		if(key_scan_param.pwr_btn_mode == single_click)
		{
				//uart_printf("pwr single\r\n");
				
				
				if((main_logic_mode == mode_0_working)||(main_logic_mode == mode_1_working)||(main_logic_mode == mode_2_working)
						||(main_logic_mode == mode_0_pause)||(main_logic_mode == mode_1_pause)||(main_logic_mode == mode_2_pause))
				{
						ledseg_show_digits_blinky_once_triggle();
				}
				else if(main_logic_mode == mode_0_idle)
				{
						if(mode_param.led_sleep == 0)
						{
							main_logic_mode = mode_0_ready;
							main_logic_vibrator_set(500, 300, 1);
							ble_dp_return_start();
							ble_dp_return_pause(0);
						}
						else
						{
							mode_param.led_sleep_tick = 0;
							mode_param.led_sleep = 0;
						}
						
				}
				else if(main_logic_mode == mode_1_idle)
				{

						if(mode_param.led_sleep == 0)
						{
							level_select_to_ready_tick_clear();
							main_logic_mode = mode_1_level_select;
						}
						else
						{
							mode_param.led_sleep_tick = 0;
							mode_param.led_sleep = 0;
						}
				}
				else if(main_logic_mode == mode_2_idle)
				{
						if(mode_param.led_sleep == 0)
						{
							level_select_to_ready_tick_clear();
							main_logic_mode = mode_2_level_select;
						}
						else
						{
							mode_param.led_sleep_tick = 0;
							mode_param.led_sleep = 0;
						}
				}
				else if(main_logic_mode == mode_1_level_select)
				{
						main_logic_target_time_set(target_time_level[mode_param.target_time_level_pointer] * 60);
						ble_dp_return_target_time(target_time_level[mode_param.target_time_level_pointer] * 60);
					
						if(mode_param.target_time_level_pointer > 5)
						{
								mode_param.target_time_level_pointer = 0;
								
						}
						else
						{
								mode_param.target_time_level_pointer++;
						}
						level_select_to_ready_tick_clear();
						
				}
				
				else if(main_logic_mode == mode_2_level_select)
				{
						
						main_logic_target_count_set(target_count_level[mode_param.target_count_level_pointer] * 100);
						ble_dp_return_target_cnt(target_count_level[mode_param.target_count_level_pointer] * 100);
					
						if(mode_param.target_count_level_pointer > 5)
						{
								mode_param.target_count_level_pointer = 0;	
						}
						else
						{
								mode_param.target_count_level_pointer++;
						}
						level_select_to_ready_tick_clear();
					
				}
				else if(main_logic_mode == mode_main_idle)
				{
							mode_param.led_sleep_tick = 0;
							mode_param.led_sleep = 0;
				}

				led_sleep_tick_clear();
				
				key_scan_param.pwr_btn_mode = idle;
		}
		
		if(key_scan_param.pwr_btn_mode == double_click)
		{
				//uart_printf("pwr double\r\n");
				
				main_logic_mode_end();
				
				key_scan_param.pwr_btn_mode = idle;
			
				led_sleep_tick_clear();
		}
		
		if(key_scan_param.pwr_btn_mode == long_press)
		{
//				uart_printf("pwr long\r\n");
				led_sleep_tick_clear();
				key_scan_param.pwr_btn_mode = idle;
			
//				if(main_sleep_tick_count > 3000)
//				{
//						main_sleep_tick_count = 49100;
//				}
				
		}
//		uart_printf("%d\r\n",main_sleep_tick_count);
		
		if(key_scan_param.set_btn_mode == single_click)
		{
				//uart_printf("set single\r\n");
				
				if(main_logic_mode == mode_0_led_sleep_idle)
				{
						main_logic_mode = mode_0_idle;
						led_sleep_tick_clear();
				}
				else if(main_logic_mode == mode_1_led_sleep_idle)
				{
						main_logic_mode = mode_1_idle;
						led_sleep_tick_clear();
				}
				else if(main_logic_mode == mode_2_led_sleep_idle)
				{
						main_logic_mode = mode_2_idle;
						led_sleep_tick_clear();
				}
				
				led_sleep_tick_clear();
				key_scan_param.set_btn_mode = idle;
		}
		
		if(key_scan_param.set_btn_mode == double_click)
		{
			
				//=============================
			
				//uart_printf("set double\r\n");
				
				//uart_send(device_param.auth_key, 32);
				//uart_printf("\r\n");
				//uart_send(device_param.device_id, 16);
				//uart_printf("\r\n");
			
				for(i = 0; i < 6; i++)
				{
						//uart_putchar(hex2Str(device_param.mac_addr.addr[i]));
						//uart_printf(" ");
				}
				
				
				//uart_printf("\r\n");
				//uart_send(device_param.product_id, 8);
				//uart_printf("\r\n");
				
				//=============================
				
				if(main_logic_mode == mode_main_idle)
				{
						ble_dp_return_mode(1);
						main_logic_mode = mode_1_idle;
				}
				else if(main_logic_mode == mode_0_idle)
				{
						ble_dp_return_mode(1);
						main_logic_mode = mode_1_idle;
				}
				else if((main_logic_mode == mode_1_idle)||(main_logic_mode == mode_1_level_select))
				{
						ble_dp_return_mode(2);
						main_logic_mode = mode_2_idle;
				}
				else if((main_logic_mode == mode_2_idle)||(main_logic_mode == mode_2_level_select))
				{
						ble_dp_return_mode(0);
						main_logic_mode = mode_0_idle;
				}
				else if((main_logic_mode == mode_0_pause)||(main_logic_mode == mode_0_working)||(main_logic_mode == mode_0_ready))
				{
						main_logic_mode_end();
						ble_dp_return_mode(1);
						main_logic_mode = mode_1_idle;
				}
				else if((main_logic_mode == mode_1_pause)||(main_logic_mode == mode_1_working)||(main_logic_mode == mode_1_ready))
				{
						main_logic_mode_end();
						ble_dp_return_mode(2);
						main_logic_mode = mode_2_idle;
				}
				else if((main_logic_mode == mode_2_pause)||(main_logic_mode == mode_2_working)||(main_logic_mode == mode_2_ready))
				{
						main_logic_mode_end();
						ble_dp_return_mode(0);
						main_logic_mode = mode_0_idle;
				}
				
				mode_param.led_sleep_tick = 0;
				mode_param.led_sleep = 0;
				
				led_sleep_tick_clear();
				key_scan_param.set_btn_mode = idle;
		}
		
		if(key_scan_param.set_btn_mode == long_press)
		{
				//uart_printf("set long\r\n");
				led_sleep_tick_clear();
				key_scan_param.set_btn_mode = idle;
		}
		
//		uart_printf("mode %d\r\n",main_logic_mode);
		switch(main_logic_mode)
		{
			
			
			case mode_main_idle:
				
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_digits(0);
				}
				else
				{
						ledseg_off();
				}
				
				if(hall_get_cycle())
				{
						mode_param_init();
					
						ble_dp_return_mode(0);
					
						main_logic_vibrator_set(500, 50, 1);
						ble_dp_return_rt_data(0, 0, 0);
						ble_dp_return_pause(0);
						ble_dp_return_start();
					
						main_logic_mode = mode_0_working;
					
				}
				
				
				mode_param.target_count = 100;
				mode_param.target_time = 60;
				mode_param.target_count_level_pointer = 1;
				mode_param.target_time_level_pointer = 1;
				
				mode_param.select_tick = 0;
				
				break;
				
			case  mode_0_idle:
				//uart_printf("mode_0_idle\r\n");
				
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_digits(0);
				}
				else
				{
						ledseg_off();
				}
				
				break;
			
			case mode_0_ready:
				//uart_printf("mode_0_ready\r\n");
			
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_digits_blinky(0);
				}
				else
				{
						ledseg_off();
				}
				
				if(hall_get_cycle())
				{
						main_logic_ready_to_working();
				}
				break;
			case mode_0_working:
				
				if(hall_get_cycle())
				{
						mode_param.cyc_count++;
//						ble_dp_return_rt_data(mode_param.cyc_count, mode_param.time / 164, calorie_cal_main_logic(mode_param.cyc_count, mode_param.time /164));
						ble_dp_return_rt_data_no_time(mode_param.cyc_count,calorie_cal_main_logic(mode_param.cyc_count, mode_param.time / 164));
						//uart_printf("%d\r\n",hall_cyc);
				}
				ledseg_show_digits_blinky_once(mode_param.cyc_count, 300);
				
				break;
			case mode_0_pause:
				ledseg_show_digits_blinky_once(mode_param.cyc_count, 300);
				//uart_printf("mode_0_pause\r\n");
				break;
			case mode_0_end:
				main_logic_mode = mode_0_idle;
				//uart_printf("mode_0_end\r\n");
				break;
			
			
			case mode_1_idle:
				//uart_printf("mode_1_idle\r\n");	
				
			
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_time(mode_param.target_time / 60, mode_param.target_time % 60);
				}
				else
				{
						ledseg_off();
				}
				
				break;
			
			case mode_1_level_select:
				ledseg_show_time_blinky(mode_param.target_time / 60, mode_param.target_time % 60);
				break;
			
			case mode_1_ready:
				//uart_printf("mode_1_ready\r\n");
//				ledseg_show_time_blinky(mode_param.target_time / 60, mode_param.target_time % 60);
			
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_time_blinky(mode_param.target_time / 60, mode_param.target_time % 60);
				}
				else
				{
						ledseg_off();
				}
				
				if(hall_get_cycle())
				{
						main_logic_ready_to_working();
				}
				break;
				
			case mode_1_working:
				
				if(hall_get_cycle())
				{
						mode_param.cyc_count++;
//						ble_dp_return_rt_data(mode_param.cyc_count, mode_param.time / 164, calorie_cal_main_logic(mode_param.cyc_count, mode_param.time / 164));
						ble_dp_return_rt_data_no_time(mode_param.cyc_count,calorie_cal_main_logic(mode_param.cyc_count, mode_param.time / 164));
						//uart_printf("%d\r\n", mode_param.cyc_count);
				}
				ledseg_show_digits_blinky_once(mode_param.cyc_count, 300);
				
				if(mode_param.time / 164 > mode_param.target_time)
				{
						main_logic_mode_end();
				}
				break;
				
			case mode_1_pause:
				ledseg_show_digits_blinky_once(mode_param.cyc_count, 300);
				//uart_printf("mode_1_pause\r\n");
				break;
			
			case mode_1_end:
				main_logic_mode = mode_1_idle;
				//uart_printf("mode_1_end\r\n");
				break;
			
			case mode_2_idle:
				//uart_printf("mode_2_idle\r\n");
				
			
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_digits(mode_param.target_count);
				}
				else
				{
						ledseg_off();
				}
				
				break;
			
			case mode_2_level_select:
				ledseg_show_digits_blinky(mode_param.target_count);
				break;
			
			case mode_2_ready:
				//uart_printf("mode_2_ready\r\n");
//				ledseg_show_digits_blinky(mode_param.target_count);
			
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_digits_blinky(mode_param.target_count);
				}
				else
				{
						ledseg_off();
				}
				
				if(hall_get_cycle())
				{
						main_logic_ready_to_working();
				}
				
				break;
			
			case mode_2_working:
				if(hall_get_cycle())
				{
						mode_param.cyc_count++;
					
//					ble_dp_return_rt_data(mode_param.cyc_count, mode_param.time / 164, calorie_cal_main_logic(mode_param.cyc_count, mode_param.time / 164));
					ble_dp_return_rt_data_no_time(mode_param.cyc_count,calorie_cal_main_logic(mode_param.cyc_count, mode_param.time / 164));
					
					if(mode_param.cyc_count == mode_param.target_count)
					{
							main_logic_mode = mode_2_end;
					}
					
				}
				
				ledseg_show_digits_blinky_once(mode_param.cyc_count, 300);
				break;
			
			case mode_2_pause:
				ledseg_show_digits_blinky_once(mode_param.cyc_count, 300);
				//uart_printf("mode_2_pause\r\n");
				break;
			
			case mode_2_end:
				
				mode_2_end_report_delay++;
				if(mode_2_end_report_delay > 10)
				{
						//uart_printf("mode_2_end\r\n");
						mode_2_end_report_delay = 0;
						main_logic_mode_end();
				}
				
				break;
				
//			case mode_0_led_sleep_idle:
//				ledseg_off();
//				break;
//			
//			case mode_1_led_sleep_idle:
//				ledseg_off();
//				break;
//			
//			case mode_2_led_sleep_idle:
//				ledseg_off();
//				break;
			
			default:
				break;
		}	
			
}

void main_logic_onekey(void)
{
		uint8_t i;
	
//		uart_printf("%d\r\n",main_sleep_tick_count);
	
//		uart_printf("mode：%d\r\n", main_logic_mode);
	
		if(mode_param.led_sleep_tick < 2000)
		{
				mode_param.led_sleep_tick++;
		}
		
		if(key_scan_param.pwr_btn_mode == single_click)
		{
				//uart_printf("pwr single\r\n");
				
				
				if((main_logic_mode == mode_0_working)||(main_logic_mode == mode_1_working)||(main_logic_mode == mode_2_working)
						||(main_logic_mode == mode_0_pause)||(main_logic_mode == mode_1_pause)||(main_logic_mode == mode_2_pause))
				{
						ledseg_show_digits_blinky_once_triggle();
				}
				else if(main_logic_mode == mode_0_idle)
				{
						if(mode_param.led_sleep == 0)
						{	
							ble_dp_return_mode(1);
							main_logic_mode = mode_1_idle;
							
							
						}
						else
						{
							mode_param.led_sleep_tick = 0;
							mode_param.led_sleep = 0;
						}
					
						
						
				}
				else if(main_logic_mode == mode_1_idle)
				{

						if(mode_param.led_sleep == 0)
						{
							ble_dp_return_mode(2);
							main_logic_mode = mode_2_idle;
							
							main_logic_target_time_set(target_time_level[0] * 60);
							ble_dp_return_target_time(target_time_level[0] * 60);
						}
						else
						{
							mode_param.led_sleep_tick = 0;
							mode_param.led_sleep = 0;
						}
					
						
				}
				else if(main_logic_mode == mode_2_idle)
				{
						if(mode_param.led_sleep == 0)
						{
							ble_dp_return_mode(0);
							main_logic_mode = mode_0_idle;
							
							main_logic_target_count_set(target_count_level[0] * 100);
							ble_dp_return_target_cnt(target_count_level[0] * 100);
						}
						else
						{
							mode_param.led_sleep_tick = 0;
							mode_param.led_sleep = 0;
						}
					
						
				}
				else if(main_logic_mode == mode_1_level_select)
				{
						main_logic_target_time_set(target_time_level[mode_param.target_time_level_pointer] * 60);
						ble_dp_return_target_time(target_time_level[mode_param.target_time_level_pointer] * 60);
					
						if(mode_param.target_time_level_pointer > 5)
						{
								mode_param.target_time_level_pointer = 0;
								
						}
						else
						{
								mode_param.target_time_level_pointer++;
						}
						level_select_to_ready_tick_clear();
						
				}
				
				else if(main_logic_mode == mode_2_level_select)
				{
						
						main_logic_target_count_set(target_count_level[mode_param.target_count_level_pointer] * 100);
						ble_dp_return_target_cnt(target_count_level[mode_param.target_count_level_pointer] * 100);
					
						if(mode_param.target_count_level_pointer > 5)
						{
								mode_param.target_count_level_pointer = 0;	
						}
						else
						{
								mode_param.target_count_level_pointer++;
						}
						level_select_to_ready_tick_clear();
					
				}
				else if(main_logic_mode == mode_main_idle)
				{
							mode_param.led_sleep_tick = 0;
							mode_param.led_sleep = 0;
					
						ble_dp_return_mode(1);
						main_logic_mode = mode_1_idle;
				}

				led_sleep_tick_clear();
				
				key_scan_param.pwr_btn_mode = idle;
		}
		
		if(key_scan_param.pwr_btn_mode == double_click)
		{
				//uart_printf("pwr double\r\n");
				
				main_logic_mode_end();
				
				key_scan_param.pwr_btn_mode = idle;
			
				led_sleep_tick_clear();
		}
		
		if(key_scan_param.pwr_btn_mode == long_press)
		{
//				uart_printf("pwr long\r\n");
				led_sleep_tick_clear();
				key_scan_param.pwr_btn_mode = idle;
			
//				if(main_sleep_tick_count > 3000)
//				{
//						main_sleep_tick_count = 49100;
//				}
				
		}
//		uart_printf("%d\r\n",main_sleep_tick_count);
		
		if(key_scan_param.set_btn_mode == single_click)
		{
				//uart_printf("set single\r\n");
				
				if(main_logic_mode == mode_0_led_sleep_idle)
				{
						main_logic_mode = mode_0_idle;
						led_sleep_tick_clear();
				}
				else if(main_logic_mode == mode_1_led_sleep_idle)
				{
						main_logic_mode = mode_1_idle;
						led_sleep_tick_clear();
				}
				else if(main_logic_mode == mode_2_led_sleep_idle)
				{
						main_logic_mode = mode_2_idle;
						led_sleep_tick_clear();
				}
				
				led_sleep_tick_clear();
				key_scan_param.set_btn_mode = idle;
		}
		
		if(key_scan_param.set_btn_mode == double_click)
		{
			
				//=============================
			
				//uart_printf("set double\r\n");
				
				//uart_send(device_param.auth_key, 32);
				//uart_printf("\r\n");
				//uart_send(device_param.device_id, 16);
				//uart_printf("\r\n");
			
				for(i = 0; i < 6; i++)
				{
						//uart_putchar(hex2Str(device_param.mac_addr.addr[i]));
						//uart_printf(" ");
				}
				
				
				//uart_printf("\r\n");
				//uart_send(device_param.product_id, 8);
				//uart_printf("\r\n");
				
				//=============================
				
				if(main_logic_mode == mode_main_idle)
				{
						ble_dp_return_mode(1);
						main_logic_mode = mode_1_idle;
				}
				else if(main_logic_mode == mode_0_idle)
				{
						ble_dp_return_mode(1);
						main_logic_mode = mode_1_idle;
				}
				else if((main_logic_mode == mode_1_idle)||(main_logic_mode == mode_1_level_select))
				{
						ble_dp_return_mode(2);
						main_logic_mode = mode_2_idle;
				}
				else if((main_logic_mode == mode_2_idle)||(main_logic_mode == mode_2_level_select))
				{
						ble_dp_return_mode(0);
						main_logic_mode = mode_0_idle;
				}
				else if((main_logic_mode == mode_0_pause)||(main_logic_mode == mode_0_working)||(main_logic_mode == mode_0_ready))
				{
						main_logic_mode_end();
						ble_dp_return_mode(1);
						main_logic_mode = mode_1_idle;
				}
				else if((main_logic_mode == mode_1_pause)||(main_logic_mode == mode_1_working)||(main_logic_mode == mode_1_ready))
				{
						main_logic_mode_end();
						ble_dp_return_mode(2);
						main_logic_mode = mode_2_idle;
				}
				else if((main_logic_mode == mode_2_pause)||(main_logic_mode == mode_2_working)||(main_logic_mode == mode_2_ready))
				{
						main_logic_mode_end();
						ble_dp_return_mode(0);
						main_logic_mode = mode_0_idle;
				}
				
				mode_param.led_sleep_tick = 0;
				mode_param.led_sleep = 0;
				
				led_sleep_tick_clear();
				key_scan_param.set_btn_mode = idle;
		}
		
		if(key_scan_param.set_btn_mode == long_press)
		{
				//uart_printf("set long\r\n");
				led_sleep_tick_clear();
				key_scan_param.set_btn_mode = idle;
		}
		
//		uart_printf("mode %d\r\n",main_logic_mode);
		switch(main_logic_mode)
		{
			
			
			case mode_main_idle:
				
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_digits(0);
				}
				else
				{
						ledseg_off();
				}
				
				if(hall_get_cycle())
				{
						mode_param_init();
					
						ble_dp_return_mode(0);
					
						main_logic_vibrator_set(500, 50, 1);
						ble_dp_return_rt_data(0, 0, 0);
						ble_dp_return_pause(0);
						ble_dp_return_start();
					
						main_logic_mode = mode_0_working;
					
				}
				
				
				mode_param.target_count = 100;
				mode_param.target_time = 60;
				mode_param.target_count_level_pointer = 1;
				mode_param.target_time_level_pointer = 1;
				
				mode_param.select_tick = 0;
				
				break;
				
			case  mode_0_idle:
				//uart_printf("mode_0_idle\r\n");
				
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_digits(0);
				}
				else
				{
						ledseg_off();
				}
				
				if(hall_get_cycle())
				{
						main_logic_mode = mode_0_ready;
						main_logic_vibrator_set(500, 300, 1);
						ble_dp_return_start();
						ble_dp_return_pause(0);
				}
				
				
				break;
			
			case mode_0_ready:
				//uart_printf("mode_0_ready\r\n");
			
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_digits_blinky(0);
				}
				else
				{
						ledseg_off();
				}
				
//				if(hall_get_cycle())
//				{
//						main_logic_ready_to_working();
//				}
				
				main_logic_ready_to_working();
				
				break;
			case mode_0_working:
				
				if(hall_get_cycle())
				{
						mode_param.cyc_count++;
//						ble_dp_return_rt_data(mode_param.cyc_count, mode_param.time / 164, calorie_cal_main_logic(mode_param.cyc_count, mode_param.time /164));
						ble_dp_return_rt_data_no_time(mode_param.cyc_count,calorie_cal_main_logic(mode_param.cyc_count, mode_param.time / 164));
						//uart_printf("%d\r\n",hall_cyc);
				}
				ledseg_show_digits_blinky_once(mode_param.cyc_count, 300);
				
				break;
			case mode_0_pause:
				ledseg_show_digits_blinky_once(mode_param.cyc_count, 300);
				//uart_printf("mode_0_pause\r\n");
				break;
			case mode_0_end:
				main_logic_mode = mode_0_idle;
				//uart_printf("mode_0_end\r\n");
				break;
			
			
			case mode_1_idle:
				//uart_printf("mode_1_idle\r\n");	
				
			
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_time(mode_param.target_time / 60, mode_param.target_time % 60);
				}
				else
				{
						ledseg_off();
				}
				
				if(hall_get_cycle())
				{
						main_logic_mode = mode_1_ready;
						main_logic_vibrator_set(500, 300, 1);
						ble_dp_return_start();
						ble_dp_return_pause(0);
				}
				
				break;
			
			case mode_1_level_select:
				ledseg_show_time_blinky(mode_param.target_time / 60, mode_param.target_time % 60);
				break;
			
			case mode_1_ready:
				//uart_printf("mode_1_ready\r\n");
//				ledseg_show_time_blinky(mode_param.target_time / 60, mode_param.target_time % 60);
			
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_time_blinky(mode_param.target_time / 60, mode_param.target_time % 60);
				}
				else
				{
						ledseg_off();
				}
				
//				if(hall_get_cycle())
//				{
//						main_logic_ready_to_working();
//				}
				
				main_logic_ready_to_working();
				
				break;
				
			case mode_1_working:
				
				if(hall_get_cycle())
				{
						mode_param.cyc_count++;
//						ble_dp_return_rt_data(mode_param.cyc_count, mode_param.time / 164, calorie_cal_main_logic(mode_param.cyc_count, mode_param.time / 164));
						ble_dp_return_rt_data_no_time(mode_param.cyc_count,calorie_cal_main_logic(mode_param.cyc_count, mode_param.time / 164));
						//uart_printf("%d\r\n", mode_param.cyc_count);
				}
				ledseg_show_digits_blinky_once(mode_param.cyc_count, 300);
				
				if(mode_param.time / 164 > mode_param.target_time)
				{
						main_logic_mode_end();
				}
				break;
				
			case mode_1_pause:
				ledseg_show_digits_blinky_once(mode_param.cyc_count, 300);
				//uart_printf("mode_1_pause\r\n");
				break;
			
			case mode_1_end:
				main_logic_mode = mode_1_idle;
				//uart_printf("mode_1_end\r\n");
				break;
			
			case mode_2_idle:
				//uart_printf("mode_2_idle\r\n");
				
			
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_digits(mode_param.target_count);
				}
				else
				{
						ledseg_off();
				}
				
				if(hall_get_cycle())
				{
						main_logic_mode = mode_2_ready;
						main_logic_vibrator_set(500, 300, 1);
						ble_dp_return_start();
						ble_dp_return_pause(0);
				}
				
				break;
			
			case mode_2_level_select:
				ledseg_show_digits_blinky(mode_param.target_count);
				break;
			
			case mode_2_ready:
				//uart_printf("mode_2_ready\r\n");
//				ledseg_show_digits_blinky(mode_param.target_count);
			
				if(mode_param.led_sleep_tick < LED_SLEEP_TICK_TIME)
				{
						mode_param.led_sleep_tick++;
				}
				else
				{
						mode_param.led_sleep = 1;
				}
				if(mode_param.led_sleep == 0)
				{
						ledseg_show_digits_blinky(mode_param.target_count);
				}
				else
				{
						ledseg_off();
				}
				
//				if(hall_get_cycle())
//				{
//						main_logic_ready_to_working();
//				}
				
				main_logic_ready_to_working();
				
				break;
			
			case mode_2_working:
				if(hall_get_cycle())
				{
						mode_param.cyc_count++;
					
//					ble_dp_return_rt_data(mode_param.cyc_count, mode_param.time / 164, calorie_cal_main_logic(mode_param.cyc_count, mode_param.time / 164));
					ble_dp_return_rt_data_no_time(mode_param.cyc_count,calorie_cal_main_logic(mode_param.cyc_count, mode_param.time / 164));
					
					if(mode_param.cyc_count == mode_param.target_count)
					{
							main_logic_mode = mode_2_end;
					}
					
				}
				
				ledseg_show_digits_blinky_once(mode_param.cyc_count, 300);
				break;
			
			case mode_2_pause:
				ledseg_show_digits_blinky_once(mode_param.cyc_count, 300);
				//uart_printf("mode_2_pause\r\n");
				break;
			
			case mode_2_end:
				
				mode_2_end_report_delay++;
				if(mode_2_end_report_delay > 10)
				{
						//uart_printf("mode_2_end\r\n");
						mode_2_end_report_delay = 0;
						main_logic_mode_end();
				}
				
				break;
				
//			case mode_0_led_sleep_idle:
//				ledseg_off();
//				break;
//			
//			case mode_1_led_sleep_idle:
//				ledseg_off();
//				break;
//			
//			case mode_2_led_sleep_idle:
//				ledseg_off();
//				break;
			
			default:
				break;
		}	

			
}


void ble_mode_switch(uint8_t mode)
{
		if((main_logic_mode == mode_0_idle)||(main_logic_mode == mode_1_idle)||(main_logic_mode == mode_2_idle)||(main_logic_mode == mode_main_idle)
			||(main_logic_mode == mode_1_level_select)||(main_logic_mode == mode_2_level_select))
		{
				if(mode == 0)
				{
						main_logic_mode = mode_0_idle;
						ble_dp_return_mode(mode);
				}
				if(mode == 1)
				{
						main_logic_mode = mode_1_idle;
						ble_dp_return_target_time(mode_param.target_time);
						ble_dp_return_mode(mode);
				}
				if(mode == 2)
				{
						main_logic_mode = mode_2_idle;
						ble_dp_return_target_cnt(mode_param.target_count);
						ble_dp_return_mode(mode);
				}

		}
		
		mode_param.led_sleep_tick = 0;
		mode_param.led_sleep = 0;
}
void main_logic_mode_set(enum mode_e mode)
{
		main_logic_mode = mode;
		
}

void main_logic_mode_start(void)
{
		mode_param_init();
	
		if(main_logic_mode == mode_0_idle)
		{
				tuya_ble_time_req(0);
				main_logic_vibrator_set(500, 50, 1);
				ble_dp_return_rt_data(0, 0, 0);
				main_logic_mode = mode_0_ready;
				
				ble_dp_return_pause(0);
				ble_dp_return_start();
				
		}
		
		if(main_logic_mode == mode_1_idle)
		{
				tuya_ble_time_req(0);
				main_logic_vibrator_set(500, 50, 1);
				ble_dp_return_rt_data(0, 0, 0);
				main_logic_mode = mode_1_ready;
			
				ble_dp_return_pause(0);
				ble_dp_return_start();
				
		}
		
		if(main_logic_mode == mode_2_idle)
		{
				tuya_ble_time_req(0);
				main_logic_vibrator_set(500, 50, 1);
				ble_dp_return_rt_data(0, 0, 0);
				main_logic_mode = mode_2_ready;
			
				ble_dp_return_pause(0);
				ble_dp_return_start();
				
		}
}

void main_logic_mode_end(void)
{
		if((main_logic_mode == mode_0_working)||(main_logic_mode == mode_0_pause)||(main_logic_mode == mode_0_ready))
		{
				main_logic_mode = mode_0_idle;
				ble_dp_return_finish(0, mode_param.cyc_count, mode_param.time / 164, calorie_cal_main_logic(mode_param.cyc_count, mode_param.time /164));
//				ble_dp_return_stop();
				ble_dp_return_rt_data(0, 0, 0);
				ledseg_show_digits(mode_param.cyc_count);
				main_logic_vibrator_set(500, 300, 3);
				
		}
		
		if((main_logic_mode == mode_1_working)||(main_logic_mode == mode_1_pause)||(main_logic_mode == mode_1_ready))
		{
				main_logic_mode = mode_1_idle;
				if(mode_param.time / 164 > mode_param.target_time) mode_param.time = mode_param.target_time * 164;
				ble_dp_return_finish(1, mode_param.cyc_count, mode_param.time / 164, calorie_cal_main_logic(mode_param.cyc_count, mode_param.time / 164));
//				ble_dp_return_stop();
				ledseg_show_digits(mode_param.cyc_count);
				main_logic_vibrator_set(500, 300, 3);
				ble_dp_return_rt_data(0, 0, 0);
				
		}
		
		if((main_logic_mode == mode_2_working)||(main_logic_mode == mode_2_pause)||(main_logic_mode == mode_2_ready)||(main_logic_mode == mode_2_end))
		{
				main_logic_mode = mode_2_idle;
				if(mode_param.cyc_count > mode_param.target_count) mode_param.cyc_count = mode_param.target_count;
				ble_dp_return_finish(2, mode_param.cyc_count, mode_param.time / 164, calorie_cal_main_logic(mode_param.cyc_count, mode_param.time / 164));
//				ble_dp_return_stop();
				ledseg_show_digits(mode_param.cyc_count);
				main_logic_vibrator_set(500, 300, 3);
				ble_dp_return_rt_data(0, 0, 0);
				
		}
		
		if(main_logic_mode == mode_main_idle)
		{
				main_logic_mode = mode_main_idle;
				ble_dp_return_finish(0, 0, 0, 0);
//				ble_dp_return_stop();
				
		}
		led_sleep_tick_clear();
		mode_param.led_sleep_tick = 0;
		mode_param.led_sleep = 0;
		mode_param_init();
}

void main_logic_mode_pause(void)
{
		if(main_logic_mode == mode_0_working)
		{
				main_logic_mode = mode_0_pause;
				ble_dp_return_pause(1);
		}
		
		if(main_logic_mode == mode_1_working)
		{
				main_logic_mode = mode_1_pause;
				ble_dp_return_pause(1);
		}
		
		if(main_logic_mode == mode_2_working)
		{
				main_logic_mode = mode_2_pause;
				ble_dp_return_pause(1);
		}
		
		if(main_logic_mode == mode_main_idle)
		{
				ble_dp_return_finish(0, 0, 0, 0);
				ble_dp_return_stop();
				main_logic_mode = mode_main_idle;
		}
}

void main_logic_mode_continue(void)
{
		if(main_logic_mode == mode_0_pause)
		{
				main_logic_mode = mode_0_working;
				ble_dp_return_pause(0);
		}
		
		if(main_logic_mode == mode_1_pause)
		{
				main_logic_mode = mode_1_working;
				ble_dp_return_pause(0);
		}
		
		if(main_logic_mode == mode_2_pause)
		{
				main_logic_mode = mode_2_working;
				ble_dp_return_pause(0);
		}
		
}

extern uint32_t common_weight;

uint16_t calorie_cal_main_logic(uint16_t count, uint32_t second)
{
	uint8_t met_time = count / second;
	uint8_t met = 0;
	if (met_time < 1)
	{
		met = 7;
	}
	else if (met_time < 3)
	{
		met = 9;
	}
	else
	{
		met = 10;
	}
	uint16_t calorie = (met*common_weight * 3.5*count) / 20000;
	
	return calorie;
	
	
}


void main_logic_key_scan(void)
{

		if(main_logic_pwr_btn() == 1)
		{
				if(key_scan_param.pwr_btn_last_status == 0)
				{
						key_scan_param.pwr_btn_on_time = 0;
				}
				key_scan_param.pwr_btn_on_time++;
				
				if(key_scan_param.pwr_btn_on_time > LONG_PRESS_TIME)
				{
						key_scan_param.pwr_btn_mode = long_press;
				}
				key_scan_param.pwr_btn_last_status = 1;
				
				main_sleep_tick_clear();
				led_sleep_tick_clear();
				
				
		}
		else
		{
				if(key_scan_param.pwr_btn_last_status == 1)
				{
					
						if(key_scan_param.pwr_btn_mode == pre_single_click)
						{
								key_scan_param.pwr_btn_mode = double_click;
								key_scan_param.pwr_btn_off_time = 0;
						}
						if(key_scan_param.pwr_btn_mode == idle)
						{
								key_scan_param.pwr_btn_mode = pre_single_click;
						}
						
						
						
#ifdef Non_intelligent_version
						
//						if(connection_status == 1)
						{
								//uart_printf("%d \r\n",key_scan_param.pwr_btn_reset_cnt);
								
								key_scan_param.pwr_btn_reset_cnt_clear_interval_time = 0;
								
								key_scan_param.pwr_btn_reset_cnt++;
								
								if(key_scan_param.pwr_btn_reset_cnt == 5)
								{
									
										key_scan_param.pwr_btn_reset_cnt_clear_interval_time = 0;
									
										ble_reset = true;
									
										tuya_ble_device_factory_reset();
									
										ledseg_show_all_zero();
										main_logic_vibrator_set(500, 300, 1);	
								}
						}
						

#endif
						
				}
				
				key_scan_param.pwr_btn_reset_cnt_clear_interval_time++;
						
#ifdef Non_intelligent_version		

//				if(connection_status == 1)
				{
					if(key_scan_param.pwr_btn_reset_cnt_clear_interval_time > 50)
					{
								key_scan_param.pwr_btn_reset_cnt = 0;
								key_scan_param.pwr_btn_reset_cnt_clear_interval_time = 0;
					}
				}
						
				
				
#endif
				
				if(key_scan_param.pwr_btn_mode == pre_single_click)
				{
						key_scan_param.pwr_btn_off_time++;
					
						if(key_scan_param.pwr_btn_off_time > DOUBLE_PRESS_INTERVAL_TIME)
						{
								key_scan_param.pwr_btn_mode = single_click;
								key_scan_param.pwr_btn_off_time = 0;
						}
				}
				
				
				key_scan_param.pwr_btn_last_status = 0;
		}
		
		
		if(main_logic_set_btn() == 1)
		{
				if(key_scan_param.set_btn_last_status == 0)
				{
						key_scan_param.set_btn_on_time = 0;
				}
				key_scan_param.set_btn_on_time++;
				
				if(key_scan_param.set_btn_on_time > LONG_PRESS_TIME)
				{
						key_scan_param.set_btn_mode = long_press;
				}
				key_scan_param.set_btn_last_status = 1;
				
				main_sleep_tick_clear();
				led_sleep_tick_clear();
		}
		else
		{
				if(key_scan_param.set_btn_last_status == 1)
				{
					
						if(key_scan_param.set_btn_mode == pre_single_click)
						{
								key_scan_param.set_btn_mode = double_click;
								key_scan_param.set_btn_off_time = 0;
						}
						if(key_scan_param.set_btn_mode == idle)
						{
								key_scan_param.set_btn_mode = pre_single_click;
						}
				}
				
				if(key_scan_param.set_btn_mode == pre_single_click)
				{
						key_scan_param.set_btn_off_time++;
					
						if(key_scan_param.set_btn_off_time > DOUBLE_PRESS_INTERVAL_TIME)
						{
								key_scan_param.set_btn_mode = single_click;
								key_scan_param.set_btn_off_time = 0;
						}
				}
				
				
				key_scan_param.set_btn_last_status = 0;
		}
}


uint32_t key_power_off_tick = 0;

void main_sleep_tick_clear(void)
{
		main_sleep_tick_count = 0;
}

void main_sleep_tick(void)
{
	
		
		
		main_sleep_tick_count++;
		if(main_sleep_tick_count / 164 == 300)
		{
				main_logic_mode_end();
		}
		else if((main_sleep_tick_count / 164 > 302))
		{
				
			
//				uart_printf("%d\r\n", main_sleep_tick_count);
			
				if (gpio_get_input(GPIOD_3) == 0)
				{
//						uart_printf("on\r\n");
						main_sleep_tick_count = 0;
				}
				else
				{
//						uart_printf("off\r\n");
						main_logic_vibrator_set(500, 300, 1);
						power_off();
				}
		}
		
		if(mode_param.power_off == 1)
		{
				key_power_off_tick++;
				if(key_power_off_tick == 5)
				{
						main_logic_mode_end();
				}
				if(key_power_off_tick / 164 > 1)
				{
						main_logic_vibrator_set(500, 300, 1);
						power_off();
				}
		}
		
		if(ble_reset == true)
		{
			ble_reset_power_off_tick++;
			
			//uart_printf("reset\r\n");
			
			rwip_schedule();
			
			if(ble_reset_power_off_tick > 10)
			{
											
				tuya_ble_device_reset();
				
				ble_reset = 0;
			}
			
		}
}



void led_sleep_tick_clear(void)
{
		led_sleep_tick_count = 0;
}

void led_sleep_tick(void)
{
		if(led_sleep_tick_count < 2000)
		{
				led_sleep_tick_count++;
		}
		
	
		
		if(led_sleep_tick_count > 1640)
		{
				if(main_logic_mode == mode_0_idle)
				{
						main_logic_mode = mode_0_led_sleep_idle;
				}
				
				if(main_logic_mode == mode_1_idle)
				{
						main_logic_mode = mode_1_led_sleep_idle;
				}
				
				if(main_logic_mode == mode_2_idle)
				{
						main_logic_mode = mode_2_led_sleep_idle;
				}
				if(main_logic_mode == mode_main_idle)
				{
						main_logic_mode = mode_0_led_sleep_idle;
//						uart_printf("debug\r\n");
				}
		}
		else
		{
//				if(main_logic_mode == mode_0_led_sleep_idle)
//				{
//						main_logic_mode = mode_0_idle;
//				}
//				
//				if(main_logic_mode == mode_1_led_sleep_idle)
//				{
//						main_logic_mode = mode_1_idle;
//				}
//				
//				if(main_logic_mode == mode_2_led_sleep_idle)
//				{
//						main_logic_mode = mode_2_idle;
//				}

		}
}

void main_param_time_increase(void)
{
		if((main_logic_mode == mode_0_working)||(main_logic_mode == mode_2_working)||(main_logic_mode == mode_1_working))
		{
				mode_param.time++;
				if(mode_param.time % 164 == 0)
				{
//						ble_dp_return_rt_data(mode_param.cyc_count, mode_param.time / 164, calorie_cal_main_logic(mode_param.cyc_count, mode_param.time / 164));
					
						ble_dp_return_rt_time(mode_param.time / 164);
						if(mode_param.sec_hall_count == 0)
						{
								if(mode_param.hall_stop_led_trigger < 10)
								{
										mode_param.hall_stop_led_trigger++;
								}
								
								if(mode_param.hall_stop_led_trigger == 2)
								{
										ledseg_show_digits_blinky_once_triggle();
								}
						}
						
//						uart_printf("led %d %d\r\n",mode_param.sec_hall_count, mode_param.hall_stop_led_trigger);
						mode_param.sec_hall_count = 0;
				}
		}
}

void main_logic_target_time_set(uint32_t target_time)
{
		mode_param.target_time = target_time;
}

void main_logic_target_count_set(uint32_t target_count)
{
		mode_param.target_count = target_count;
}





enum authorization_status authorization_completed = authorization_not_completed;

void authorization_completion_flag_flash_write(void)
{
		flash_writeword(FLASH_SPACE_TYPE_MAIN, (0x27600/4) + 4,0xA5);
}


void authorization_status_flash_read(void)
{
		uint32_t status = 0, reset = 0;
	
		status = flash_readword(FLASH_SPACE_TYPE_MAIN, (0x27600 / 4) + 4);
	
		reset = flash_readword(FLASH_SPACE_TYPE_MAIN, (0x27600 / 4) + 5);
	
//		uart_printf("status:%d %d\r\n", status, reset);
	
		if(status == 0xA5) 
		{
				if(reset == 0xA7)
				{
						authorization_completed = authorized;
				}
				else
				{
						authorization_completed = wait_for_reset;
						flash_writeword(FLASH_SPACE_TYPE_MAIN, (0x27600/4) + 5,0xA7);
				}
		}
		
//		authorization_completed = authorized;

}

enum authorization_status authorization_completion_flag(void)
{
		return authorization_completed;
}


void authorization_completion_flag_set(enum authorization_status status)
{
		authorization_completed = status;
}
