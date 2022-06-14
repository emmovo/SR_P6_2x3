/*
 * @Author: emmovo
 * @Date: 2022-06-12 17:29:31
 * @LastEditors: emmovo
 * @LastEditTime: 2022-06-12 17:30:09
 * @FilePath: \skippingRope_phy6222\project\V2.0\app\app_ble.h
 * @Description: 
 * 
 * Copyright (c) 2022 by mingjkl@live.com/emmovo.com, All Rights Reserved. 
 */
#ifndef __ble_h
#define __ble_h

#include "types.h"
#include "tuya_ble_log.h"

void ble_dp_return_rt_data(uint16_t count, uint16_t time, uint16_t calorie);

void ble_dp_return_state(bool state);
void ble_dp_return_mode(uint8_t mode);
void ble_dp_return_finish(uint8_t mode, uint16_t total_cnt, uint16_t total_time, uint16_t total_calorie);
void ble_dp_return_target_cnt(uint16_t cnt);
void ble_dp_return_target_time(uint16_t time);
void ble_app(void);

void ble_receive_dp(uint8_t *data, uint8_t len);
void ble_mode_pause_func_reg(void(*func)(uint8_t pause));
void ble_mode_stop_func_reg(void(*func)(void));
void ble_mode_start_func_reg(void(*func)(void));
void ble_dp_return_bat(uint8_t level);
void ble_dp_return_pause(bool pause);

void ble_dp_return_start(void);
void ble_dp_return_stop(void);
void ble_dp_return_rt_time(uint16_t time);
void ble_dp_return_rt_data_no_time(uint16_t count,uint16_t calorie);
#endif
