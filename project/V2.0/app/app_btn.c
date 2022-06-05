/*
 * @Author: emmovo mingjkl@live.com
 * @Date: 2022-06-05 10:28:02
 * @LastEditors: emmovo mingjkl@live.com
 * @LastEditTime: 2022-06-05 11:24:27
 * @FilePath: \V2.0\app\app_flash.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */



#include "tuya_ble_log.h"
#include "app_btn.h"
#include "gpio.h"

void btn_init(void)
{
	hal_gpio_init();
	hal_gpio_pin_init(GPIO_P14, GPIO_INPUT);
	hal_gpio_pull_set(GPIO_P14, GPIO_PULL_UP);
}


