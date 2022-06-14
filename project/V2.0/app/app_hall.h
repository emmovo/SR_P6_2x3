/*
 * @Author: emmovo
 * @Date: 2022-06-12 16:47:03
 * @LastEditors: emmovo
 * @LastEditTime: 2022-06-12 16:47:23
 * @FilePath: \skippingRope_phy6222\project\V2.0\app\app_hall.h
 * @Description: 
 * 
 * Copyright (c) 2022 by mingjkl@live.com/emmovo.com, All Rights Reserved. 
 */
#ifndef __hall_h
#define __hall_h

#include "types.h"
#include "stdbool.h"

void hall_init(void);
bool hall_get_1_status(void);
bool hall_get_2_status(void);
bool hall_get_3_status(void);
uint8_t hall_get_all_status(void);

bool hall_get_cycle(void);
void hall_reset_cycle(void);

#endif
