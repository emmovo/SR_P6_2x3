/*
 * @Author: emmovo mingjkl@live.com
 * @Date: 2022-06-04 17:26:22
 * @LastEditors: emmovo
 * @LastEditTime: 2022-06-20 16:17:44
 * @FilePath: \V2.0\app\timestamp.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __timestamp_h
#define __timestamp_h


#include "tuya_ble_log.h"

uint32_t timestamp_set_by_ble(uint8_t *data);
void timestamp_update(void);
uint32_t timestamp_get(void);

void timestamp_updata_by_ble(uint32_t timestamp);

#endif
