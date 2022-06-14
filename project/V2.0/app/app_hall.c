/*
 * @Author: emmovo
 * @Date: 2022-06-12 16:47:03
 * @LastEditors: emmovo
 * @LastEditTime: 2022-06-12 17:00:14
 * @FilePath: \skippingRope_phy6222\project\V2.0\app\app_hall.c
 * @Description: 
 * 
 * Copyright (c) 2022 by mingjkl@live.com/emmovo.com, All Rights Reserved. 
 */
#include "app_hall.h"
#include "gpio.h"
#include "tuya_ble_log.h"
#include "timer.h"
#include "main_logic.h"
#include "app.h"

void hall_init(void)
{
	hal_gpio_init();
	hal_gpio_pin_init(HALL_1_PIN, GPIO_INPUT);
	hal_gpio_pull_set(HALL_1_PIN, GPIO_PULL_UP);
}

bool hall_get_1_status(void)
{
	return !hal_gpio_read(HALL_1_PIN);
}


uint8_t hall_get_all_status(void)
{
	return !hal_gpio_read(HALL_1_PIN);
}

static uint8_t trig_cnt = 0,init_hall=0;
static bool is_init_set = false,is_other_tmp_trig=false;

extern struct mode_param_s mode_param;

uint8_t hall_sleep_countdown = 0;

bool hall_get_cycle(void)
{
	static uint32_t tick = 0;
	
	uint8_t temp = hall_get_all_status();
	static uint8_t last_hall = 0;
	
	if ( temp == 3 ||temp==0)
	{
		if (is_init_set == false)
		{
			is_init_set = true;
			init_hall = temp;
		}
		if (temp != init_hall)
		{
			is_other_tmp_trig = true;
		}
			
		if (temp == init_hall&&trig_cnt >= 2&&is_other_tmp_trig)
		{
			is_other_tmp_trig = false;
			trig_cnt = 0;
			
			mode_param.sec_hall_count++;
			mode_param.hall_stop_led_trigger = 0;
			
			
			if(hall_sleep_countdown < 2)
			{
					hall_sleep_countdown++;
			}
			else
			{
					main_sleep_tick_clear();
					hall_sleep_countdown = 0;
			}
			
			
			return true;
		}
		return false;
	}
	if (temp != last_hall)
	{
		temp = last_hall;
		trig_cnt++;
	}
	
		
	return false;
}

