/*
 * @Author: emmovo mingjkl@live.com
 * @Date: 2022-06-04 17:28:45
 * @LastEditors: emmovo
 * @LastEditTime: 2022-06-20 09:35:28
 * @FilePath: \V2.0\app\app_timer.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */


#include "timer.h"        // timer definition
#include "tuya_ble_log.h"
#include "pwm.h"
#include "main_logic.h"

#include "app_timer.h"

volatile uint32_t sys_10ms_tick = 0;

void timer_init(void)
{
}



uint32_t timer_get_systick(void)
{
//	return (REG_AHB0_ICU_TIME_COUNT) & 0xfffffff;
}



void tick_10ms_timer_handler(uint8_t evt)
{
	sys_10ms_tick++;
//	main_param_time_increase();
//	main_logic_key_scan();
//	led_blinky_ctrl();
//	level_select_to_ready_tick();
//	main_sleep_tick();
}

void tick_10ms_init(void)
{
		hal_timer_init(tick_10ms_timer_handler);
		hal_timer_set(AP_TIMER_ID_5, 10000);
}



 
uint32_t tick_10ms_get(void)
{
	return sys_10ms_tick;
}


/// @} TIMER
