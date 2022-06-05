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

uint32_t timestamp_get(void)
{
	return ble_unix_time;
}

