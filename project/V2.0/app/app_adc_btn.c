/*
 * @Author: emmovo
 * @Date: 2022-06-12 10:14:35
 * @LastEditors: emmovo
 * @LastEditTime: 2022-06-12 10:45:02
 * @FilePath: \skippingRope_phy6222\project\V2.0\app\app_adc_btn.c
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
#include "app_adc_btn.h"




/*********************************************************************
 * LOCAL CONSTANT
 */
#define APP_ADC_BTN_MAX_SAMPLE_POINT    64
#define APP_ADC_BTN_PHY_ADC_PIN_IDX     5  // Notes: It MUST be updated accordingly in case changing channel.

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLE
 */
static uint16_t app_adc_btn_debug[APP_ADC_BTN_MAX_SAMPLE_POINT];
static uint8_t  app_adc_btn_channel_done_flag = 0; 
static uint16_t app_adc_btn_g_adc_value = 0;
 
static adc_Cfg_t app_adc_btn_phy_adc_cfg = {
    .channel = ADC_BIT(ADC_CH2P_P14),
    .is_continue_mode = FALSE,
    .is_differential_mode = 0x00,
    .is_high_resolution = 0x00,
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
		
		

    osal_memcpy(app_adc_btn_debug, pev->data, 2*(pev->size));
    app_adc_btn_channel_done_flag |= BIT(pev->ch);
		
		

    if(app_adc_btn_channel_done_flag == app_adc_btn_phy_adc_cfg.channel) {
			
			
        if(app_adc_btn_channel_done_flag & BIT(APP_ADC_BTN_PHY_ADC_PIN_IDX)) {
					
            is_high_resolution = (app_adc_btn_phy_adc_cfg.is_high_resolution & BIT(APP_ADC_BTN_PHY_ADC_PIN_IDX))?TRUE:FALSE;
            is_differential_mode = (app_adc_btn_phy_adc_cfg.is_differential_mode & BIT(APP_ADC_BTN_PHY_ADC_PIN_IDX))?TRUE:FALSE;
				
					
            value = hal_adc_value_cal((adc_CH_t)APP_ADC_BTN_PHY_ADC_PIN_IDX, app_adc_btn_debug, pev->size, is_high_resolution, is_differential_mode);

            app_adc_btn_g_adc_value = (uint16_t)(value*1000);
        }

        app_adc_btn_channel_done_flag = 0;
    }
}


static void app_adc_btn_adcMeasureTask( void )
{
    int ret;
    uint8_t batt_ch = ADC_CH2P_P14;
    GPIO_Pin_e pin;

    pin = s_pinmap[batt_ch];
	
	
    hal_gpio_cfg_analog_io(pin, Bit_DISABLE);
    hal_gpio_write(pin, 1);  // lbl
    ret = hal_adc_config_channel(app_adc_btn_phy_adc_cfg, adc_evt);
    hal_gpio_cfg_analog_io(pin, Bit_ENABLE);

    if(ret) {
        LOG("ret = %d\n",ret);
        return;
    }

    hal_adc_start();
}


uint32_t app_adc_btn_ty_adc_init(ty_adc_t* p_adc)
{
    p_adc->channel = ADC_CH2P_P14;
    p_adc->value = 0;
    hal_adc_init();
    return 0;
}

uint32_t app_adc_btn_ty_adc_stop(ty_adc_t* p_adc)
{
    hal_adc_stop();
    return 0;
}

ty_adc_t app_adc_btn_p;

uint32_t app_adc_btn_ty_adc_start(ty_adc_t* p_adc)
{
    int i = 1000;
	
    app_adc_btn_ty_adc_init(p_adc);
    app_adc_btn_g_adc_value = 0;
    app_adc_btn_adcMeasureTask();
    
    while(i--) {
        ty_system_delay_ms(1);
        if(app_adc_btn_g_adc_value != 0) {
            p_adc->value = app_adc_btn_g_adc_value;
            app_adc_btn_ty_adc_stop(p_adc);
            return 0;
        }
    }

    app_adc_btn_ty_adc_stop(p_adc);
    
    return 1;
}

uint32_t app_adc_btn_ty_adc_value_get(void)
{
		if(!app_adc_btn_ty_adc_start(&app_adc_btn_p))
		{
				return app_adc_btn_p.value;
		}
		else
		{
				return 0;
		}
}





