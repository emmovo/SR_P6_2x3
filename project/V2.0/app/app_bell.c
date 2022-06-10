/*
 * @Author: emmovo
 * @Date: 2022-06-07 10:10:58
 * @LastEditors: emmovo
 * @LastEditTime: 2022-06-07 10:14:33
 * @FilePath: \V2.0\app\app_bell.c
 * @Description: 
 * 
 * Copyright (c) 2022 by mingjkl@live.com/emmovo.com, All Rights Reserved. 
 */



#include "tuya_ble_log.h"
#include "app_bell.h"

#include "pwm.h"


void app_bell_start(void)
{

	hal_pwm_init(PWM_CH0, PWM_CLK_NO_DIV, PWM_CNT_UP, PWM_POLARITY_RISING);

	hal_pwm_set_count_val(PWM_CH0, 5000, 10000);

	hal_pwm_open_channel(PWM_CH0, GPIO_P11);

	hal_pwm_start();

}

void app_bell_stop(void)
{
	hal_pwm_stop();
}

