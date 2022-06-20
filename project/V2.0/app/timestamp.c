/*
 * @Author: emmovo
 * @Date: 2022-06-06 13:36:38
 * @LastEditors: emmovo
 * @LastEditTime: 2022-06-20 16:17:26
 * @FilePath: \V2.0\app\timestamp.c
 * @Description: 
 * 
 * Copyright (c) 2022 by mingjkl@live.com/emmovo.com, All Rights Reserved. 
 */
#include "timestamp.h"
#include "app_timer.h"

#include "math.h"

volatile uint32_t ble_unix_time = 0;
volatile uint32_t start_tick = 0;

static uint32_t ppow(uint32_t y);

uint32_t timestamp_set_by_ble(uint8_t *data)
{
	
	start_tick = timer_get_systick();

	ble_unix_time = 0;
	for(uint8_t i = 0 ; i < 10 ; i++)
	{
		ble_unix_time += (*(data + i) - '0')*(uint32_t)pow(10, 9 - i);
	}
	
	return ble_unix_time;
}

static uint32_t ppow(uint32_t y)
{
	uint32_t sum = 10;
	uint32_t m; 
	if (y == 0) return 1; 
	for (m = 1; m < y; m++) sum *= 10; 
	return sum;
}

void timestamp_update(void)
{
	if (ble_unix_time != 0&&(timer_get_systick() - start_tick) > 2000)
	{
		ble_unix_time += ((timer_get_systick() - start_tick) / 2000);
		start_tick = timer_get_systick();
	}
}

#if (TUYA_BLE_PROTOCOL_VERSION_HIGN == 0x04)

void timestamp_updata_by_ble(uint32_t timestamp)
{
	ble_unix_time = timestamp;
}

#endif

uint32_t timestamp_get(void)
{
	return ble_unix_time;
}

