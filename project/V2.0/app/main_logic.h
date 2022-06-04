/*
 * @Author: your name
 * @Date: 2021-12-06 23:26:25
 * @LastEditTime: 2021-12-06 23:36:34
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \demo_emmovo\app\main_logic.h
 */
#ifndef __MAIN_LOGIC_h
#define __MAIN_LOGIC_h

#include "stdint.h"
#include "stdbool.h"


#define LONG_PRESS_TIME 100
#define DOUBLE_PRESS_INTERVAL_TIME 30
#define SELECT_TICK_TIME	500
#define LED_SLEEP_TICK_TIME 1000

enum mode_e
{
    mode_0_idle,		
		mode_0_ready,		
    mode_0_working,
    mode_0_pause,
    mode_0_end,
    mode_1_idle,
    mode_1_level_select,
    mode_1_ready,
    mode_1_working,
    mode_1_pause,
    mode_1_end,
    mode_2_idle,
    mode_2_level_select,
    mode_2_ready,
    mode_2_working,
    mode_2_pause,
    mode_2_end,
		mode_main_idle,
		mode_0_led_sleep_idle,
		mode_1_led_sleep_idle,
		mode_2_led_sleep_idle,
};

struct mode_param_s
{
		uint16_t cyc_count;
		uint32_t time;
		uint32_t calorie;
		uint32_t target_count;
		uint32_t target_time;
		uint8_t	target_count_level_pointer;
		uint8_t target_time_level_pointer;
		uint16_t select_tick;
		uint16_t led_sleep_tick;
		uint8_t  led_sleep;
		uint8_t power_off;
		uint8_t hall_stop_led_trigger;
		uint8_t sec_hall_count;
};

enum key_scan_mode_e
{
		single_click,
		pre_single_click,
		double_click,
		long_press,
		idle,
};

struct key_scan_param_s
{
		uint16_t pwr_btn_on_time;
		uint16_t pwr_btn_off_time;
		uint8_t pwr_btn_last_status;
		enum key_scan_mode_e pwr_btn_mode;
		uint16_t set_btn_on_time;
		uint16_t set_btn_off_time;
		uint8_t set_btn_last_status;
		enum key_scan_mode_e set_btn_mode;
		uint8_t pwr_btn_reset_cnt;
		uint8_t pwr_btn_reset_cnt_clear_interval_time;
		
};

enum authorization_status
{
		authorization_not_completed,
		authorized,
		wait_for_reset,
		authorizing,
		authorization_timeout,
		authorization_waiting,
};


void main_logic(void);
void main_logic_onekey(void);
void main_logic_mode_set(enum mode_e mode);

void main_logic_mode_start(void);
void main_logic_mode_end(void);

void main_logic_mode_pause(void);
void main_logic_mode_continue(void);

void main_param_time_increase(void);

uint16_t calorie_cal_main_logic(uint16_t count, uint32_t second);

void main_logic_target_time_set(uint32_t target_time);

void main_logic_target_count_set(uint32_t target_count);

void ble_mode_switch(uint8_t mode);
void main_logic_key_scan(void);

void level_select_to_ready_tick_clear(void);
void level_select_to_ready_tick(void);
void main_sleep_tick_clear(void);
void main_sleep_tick(void);
void led_sleep_tick(void);
void led_sleep_tick_clear(void);

void authorization_completion_flag_flash_write(void);
void authorization_status_flash_read(void);
enum authorization_status authorization_completion_flag(void);

void authorization_completion_flag_set(enum authorization_status status);

#endif
