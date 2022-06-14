/*
 * @Author: emmovo
 * @Date: 2022-06-06 13:36:38
 * @LastEditors: emmovo
 * @LastEditTime: 2022-06-12 14:10:57
 * @FilePath: \skippingRope_phy6222\project\V2.0\app\app.h
 * @Description: 
 * 
 * Copyright (c) 2022 by mingjkl@live.com/emmovo.com, All Rights Reserved. 
 */

 
 #include "types.h"
 
 #include "project_config.h"
 #include "BT612_4_config.h"

 #include "app_timer.h"
 #include "timestamp.h"
 #include "app_flash.h"
 #include "main_logic.h"
 #include "app_btn.h"
 #include "app_bell.h"
 #include "app_adc_btn.h"
 #include "app_soc.h"
 #include "app_seg_lcd.h"
 #include "app_ble.h"


uint16_t app_task(uint8_t task_id, uint16 events);
void app_task_init(uint8_t task_id);

