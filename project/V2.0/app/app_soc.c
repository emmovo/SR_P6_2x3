/*
 * @Author: emmovo
 * @Date: 2022-06-06 13:36:38
 * @LastEditors: emmovo mingjkl@live.com
 * @LastEditTime: 2022-06-19 23:21:14
 * @FilePath: \skippingRope_phy6222\project\V2.0\app\app_soc.c
 * @Description: 
 * 
 * Copyright (c) 2022 by mingjkl@live.com/emmovo.com, All Rights Reserved. 
 */

#include <string.h>

#include "tuya_ble_main.h"
#include "tuya_ble_log.h"
#include "tuya_ble_api.h"


#include "OSAL_Tasks.h"

#include "app.h"

#include "ty_adc.h"
#include "adc.h"
#include "OSAL.h"
#include "log.h"
#include "ty_system.h"
#include "app_soc.h"


/*********************************************************************
 * LOCAL CONSTANT
 */
#define APP_SOC_MAX_SAMPLE_POINT    64
#define APP_SOC_PHY_ADC_PIN_IDX     2  // Notes: It MUST be updated accordingly in case changing channel.
#define APP_SOC_ADC_CH  ADC_CH1N_P11

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLE
 */
static uint16_t app_soc_debug[APP_SOC_MAX_SAMPLE_POINT];
static uint8_t  app_soc_channel_done_flag = 0; 
static uint16_t app_soc_g_adc_value = 0;
 
static adc_Cfg_t app_soc_phy_adc_cfg = {
    .channel = ADC_BIT(APP_SOC_ADC_CH),
    .is_continue_mode = FALSE,
    .is_differential_mode = 0x00,
    .is_high_resolution = 0x01,
}; 



static void adc_evt(adc_Evt_t* pev)
{
    float value = 0;
    int i = 0;
    bool is_high_resolution = FALSE;
    bool is_differential_mode = FALSE;
    uint8_t ch = 0;

    if((pev->type != HAL_ADC_EVT_DATA) || (pev->ch < 2)) {
        return;
    }
		
		

    osal_memcpy(app_soc_debug, pev->data, 2*(pev->size));
    app_soc_channel_done_flag |= BIT(pev->ch);
		
		

    if(app_soc_channel_done_flag == app_soc_phy_adc_cfg.channel) {
			
			
        if(app_soc_channel_done_flag & BIT(APP_SOC_PHY_ADC_PIN_IDX)) {
					
            is_high_resolution = (app_soc_phy_adc_cfg.is_high_resolution & BIT(APP_SOC_PHY_ADC_PIN_IDX))?TRUE:FALSE;
            is_differential_mode = (app_soc_phy_adc_cfg.is_differential_mode & BIT(APP_SOC_PHY_ADC_PIN_IDX))?TRUE:FALSE;
				
					
            value = hal_adc_value_cal((adc_CH_t)APP_SOC_PHY_ADC_PIN_IDX, app_soc_debug, pev->size, is_high_resolution, is_differential_mode);

            app_soc_g_adc_value = (uint16_t)(value*1000);
        }

        app_soc_channel_done_flag = 0;
    }
}


static void app_soc_adcMeasureTask( void )
{
    int ret;
    uint8_t batt_ch = APP_SOC_ADC_CH;
    GPIO_Pin_e pin;

    pin = s_pinmap[batt_ch];
	
	
    hal_gpio_cfg_analog_io(pin, Bit_DISABLE);
    hal_gpio_write(pin, 1);  // lbl
    ret = hal_adc_config_channel(app_soc_phy_adc_cfg, adc_evt);
    hal_gpio_cfg_analog_io(pin, Bit_ENABLE);

    if(ret) {
        LOG("ret = %d\n",ret);
        return;
    }

    hal_adc_start();
}


uint32_t app_soc_ty_adc_init(ty_adc_t* p_adc)
{
    p_adc->channel = APP_SOC_ADC_CH;
    p_adc->value = 0;
    hal_adc_init();
    return 0;
}

uint32_t app_soc_ty_adc_stop(ty_adc_t* p_adc)
{
    hal_adc_stop();
    return 0;
}

ty_adc_t app_soc_p;

uint32_t app_soc_ty_adc_start(ty_adc_t* p_adc)
{
    int i = 1000;
	
    app_soc_ty_adc_init(p_adc);
    app_soc_g_adc_value = 0;
    app_soc_adcMeasureTask();
    
    while(i--) {
        ty_system_delay_ms(1);
        if(app_soc_g_adc_value != 0) {
            p_adc->value = app_soc_g_adc_value;
            app_soc_ty_adc_stop(p_adc);
            return 0;
        }
    }

    app_soc_ty_adc_stop(p_adc);
    
    return 1;
}

uint32_t app_soc_ty_adc_value_get(void)
{
		if(!app_soc_ty_adc_start(&app_soc_p))
		{
				return app_soc_p.value;
		}
		else
		{
				return 0;
		}
}


void battery_init(void)
{
	//chrg
	// gpio_config(GPIOD_3, INPUT, PULL_HIGH);
	
	// //adc init
	// adc_init(4, 1);
	
	// uint16_t temp = adc_get_value();
	
}

uint8_t last_bat_level = 0;

void battery_check(void)
{
	static uint32_t tick = 0;
	static bool last_chrg = 0;
	uint8_t digit = 0;
	uint8_t ledseg_data[4] = { 0 };
// 	if(gpio_get_input(GPIOD_3) == 0)
// 	{
// 		bool led_lit = 0;
// 		last_chrg = 1;
		
// 		uint32_t lit_tick = 0;
// 		led_set(1);
// 		uint8_t offset = 0;
// 		while (1)
// 		{
// 			uint16_t temp = adc_get_value();
// 			memset(ledseg_data, 0, 4);
			
// 			if (temp < 220)
// 			{
// 				if (offset == 0)
// 				{
// 					offset = 0;
// 				}
// 			}
// 			else if (temp < 235)
// 			{
// 				if (offset < 1)
// 				{
// 					offset = 1; 
// 				}
// 			}
// 			else if (temp < 250)
// 			{
// 				if (offset < 2)
// 				{
// 					offset = 2; 
// 				}
// 			}
// 			else
// 			{
// 				offset = 3;
// 			}
			
// 			memset(ledseg_data, LEDS_LETTER_0, offset + 1);
// 			if (led_lit == 0&&timer_get_systick() - lit_tick > 2000)
// 			{
// 				lit_tick = timer_get_systick();
// 				led_lit = 1;
// 				ledseg_write_display(ledseg_data);
// 			}
// 			else if (led_lit == 1&&timer_get_systick() - lit_tick > 2000)
// 			{
// 				ledseg_data[offset] = 0;
// 				lit_tick = timer_get_systick();
// 				led_lit = 0;
// 				ledseg_write_display(ledseg_data);
// 			}
// 			if (gpio_get_input(GPIOD_3) == 1)
// 			{
// 				platform_reset(RESET_NO_ERROR);
// 			}
// 			bat_val = 4;
// 			if (timer_get_systick() - ble_upload_ticks > 1200000)
// 			{
// 				ble_dp_return_bat(bat_val);
// 				ble_upload_ticks = timer_get_systick();
// 			}
// 			rwip_schedule();
// 			wdt_feed(0xffff);
// 		}
		
// 	}	
// 	//check battery
// 	if(initial == false)
// 	{
// 		uint16_t temp = adc_get_value();
		
		
		
// 		if (temp < 168)	//2.688
// 		{
// 			vibrator_set(1);
// 			led_set(1);
// 			Delay_ms(2000);
// 			vibrator_set(0);
// 			led_set(0);
// 			power_off();
// 		}
// 		else if (temp < 230)	//3.68
// 		{
// 			bat_val = 0;
// 			ble_uploaded = 0;
// 		}
// 		else if(temp < 240)	//3.84
// 		{
// 			bat_val = 1;
// 			ble_uploaded = 0;
// 		}
// 		else
// 		{
// 			bat_val = 2;
// 			ble_uploaded = 0;
// 		}
// 		if (!ble_uploaded)
// 		{
// 			last_bat_level = bat_val;
// 			ble_dp_return_bat(bat_val);
// 			ble_uploaded = 1;
// 		}
		
// 		tick = timer_get_systick();
// 		initial = true;
// 	}
	
	
		
		
// 	if(timer_get_systick() - tick > 20000)
// 	{
		
		
// 		uint16_t temp = adc_get_value();
		
// //		UART_PRINTF("bat %d\r\n", temp);
		
// 		if (temp < 168)		//3.1
// 		{
// 			vibrator_set(1);
// 			led_set(1);
// 			Delay_ms(2000);
// 			vibrator_set(0);
// 			led_set(0);
// 			power_off();
			
// 		}
// 		else if (temp < 0xcf)		//3.3
// 		{
// 			bat_val = 0;
// 			ble_uploaded = 0;
// 		}
// 		else if(temp < 0xf0)		//3.84
// 		{
// 			bat_val = 1;
// 			ble_uploaded = 0;
// 		}
// 		else
// 		{
// 			bat_val = 2;
// 			ble_uploaded = 0;
// 		}
// 		if (last_bat_level != bat_val)
// 		{
// 			ble_dp_return_bat(bat_val);
// 			last_bat_level = bat_val;
// 			ble_uploaded = 1;
// 		}
		
// 		tick = timer_get_systick();
// 	}
}

void Battery_Upload_BLE(void)
{
		ble_dp_return_bat(2);	//test;
//	ble_dp_return_bat(bat_val);
}




