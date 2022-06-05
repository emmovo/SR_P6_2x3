/*
 * @Author: emmovo mingjkl@live.com
 * @Date: 2022-06-04 17:28:45
 * @LastEditors: emmovo mingjkl@live.com
 * @LastEditTime: 2022-06-04 17:30:24
 * @FilePath: \V2.0\app\timer.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */


#ifndef _TIMER_H_
#define _TIMER_H_


#include <stdint.h>      // standard integer definition
#include <stdbool.h>


uint32_t timer_get_systick(void);


void tick_10ms_init(void);
uint32_t tick_10ms_get(void);


void pwm4_isr(unsigned char ucChannel);



/// @} TIMER

#endif /* _TIMER_H_ */
