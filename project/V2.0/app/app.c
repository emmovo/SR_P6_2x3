/*
 * @Author: emmovo mingjkl@live.com
 * @Date: 2022-06-04 12:38:27
 * @LastEditors: emmovo
 * @LastEditTime: 2022-06-12 13:13:15
 * @FilePath: \skippingRope_phy6222\project\V2.0\app\app.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */


#include <string.h>

#include "tuya_ble_main.h"

#include "tuya_ble_log.h"


#include "tuya_ble_api.h"



// #include "battery.h"

// #include "flash.h"

// #include "wdt.h"

//#include "timestamp.h"
#include "main_logic.h"

#include "OSAL.h"
#include "OSAL_Tasks.h"
#include "osal_cbTimer.h"

#include "app.h"
#include "ty_flash.h"

#include "ty_adc.h"



volatile uint32_t common_weight = 0;

bool connection_status = 0;


void app(void)
{

// 	btn_app();
// 	battery_check();
// 	wdt_feed(0xffff);
	
// 	if(start_lock == false)


// #ifdef Non_intelligent_version
// 		main_logic_onekey();
// #endif


 		main_logic();



// 	connection_show();
// 	ble_app();
// 	timestamp_update();
	
// 	extern bool ble_finish_dp_offline_exist;
// 	if (ble_finish_dp_offline_exist
// 		&&connection_status
// 		&&timestamp_get() != 0)
// 	{
// 		ble_dp_return_finish(0, 0, 0, 0);
// 	}
	
}



void connection_show(void)
{
	static uint32_t led_tick = 0;
	static bool led_lit = 0;
	
	// if (connection_status == 0)
	// {
	// 	if (tick_10ms_get() - led_tick > 100)
	// 	{
	// 		if (led_lit == 1)
	// 		{
	// 			led_lit = 0;
	// 			led_set(0);
	// 		}
	// 		else
	// 		{
	// 			led_lit = 1;
	// 			led_set(1);
	// 		}
	// 		led_tick = tick_10ms_get();
	// 	}
	// }
	
}

ty_adc_t adc_p;

uint16 app_task(uint8 task_id, uint16 events)
{
	
		//=====test

//		TY_PRINTF("adc:	%d", app_soc_ty_adc_value_get());
		WaitMs(2000);
	
		app_seg_lcd_test();
//		
		//========
		app();
		return 1;
}

void app_task_init(uint8 task_id)
{
	
		app_seg_lcd_init();
	
		osal_set_event(task_id, 1);
}


