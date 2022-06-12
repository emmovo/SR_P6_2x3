/*
 * @Author: emmovo
 * @Date: 2022-06-12 13:50:07
 * @LastEditors: emmovo
 * @LastEditTime: 2022-06-12 15:14:13
 * @FilePath: \skippingRope_phy6222\project\V2.0\app\app_seg_lcd.c
 * @Description: 
 * 
 * Copyright (c) 2022 by mingjkl@live.com/emmovo.com, All Rights Reserved. 
 */


#include "tuya_ble_log.h"
#include "app_seg_lcd.h"
#include "gpio.h"
#include "app.h"


void app_seg_lcd_backlight_on(void)
{
	hal_gpio_write(SEG_LCD_BACKLIGHT_PIN, 1);
}

void app_seg_lcd_backlight_off(void)
{
	hal_gpio_write(SEG_LCD_BACKLIGHT_PIN, 0);
}

void app_seg_lcd_init(void)
{
	hal_gpio_init();

	hal_gpio_pin_init(SEG_LCD_BACKLIGHT_PIN, GPIO_OUTPUT);
	hal_gpio_pull_set(SEG_LCD_BACKLIGHT_PIN, GPIO_PULL_DOWN);

	hal_gpio_pin_init(SEG_LCD_PIN1, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_PIN2, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_PIN3, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_PIN4, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_PIN10, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_PIN11, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_PIN12, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_PIN13, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_PIN14, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_PIN15, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_PIN16, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_PIN17, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_PIN18, GPIO_OUTPUT);

	hal_gpio_pin_init(SEG_LCD_COM1, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_COM2, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_COM3, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_COM4, GPIO_OUTPUT);
	hal_gpio_pin_init(SEG_LCD_COM5, GPIO_OUTPUT);



	hal_gpio_pull_set(SEG_LCD_PIN1, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_PIN2, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_PIN3, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_PIN4, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_PIN10, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_PIN11, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_PIN12, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_PIN13, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_PIN14, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_PIN15, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_PIN16, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_PIN17, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_PIN18, GPIO_PULL_DOWN);

	hal_gpio_pull_set(SEG_LCD_COM1, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_COM2, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_COM3, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_COM4, GPIO_PULL_DOWN);
	hal_gpio_pull_set(SEG_LCD_COM5, GPIO_PULL_DOWN);

}

bool app_seg_lcd_test_status = 0;



void app_seg_lcd_test(void)
{
	if(app_seg_lcd_test_status == 0)
	{

		hal_gpio_write(SEG_LCD_PIN1, 0);
		hal_gpio_write(SEG_LCD_PIN2, 0);
		hal_gpio_write(SEG_LCD_PIN3, 0);
		hal_gpio_write(SEG_LCD_PIN4, 0);
		hal_gpio_write(SEG_LCD_PIN10, 0);
		hal_gpio_write(SEG_LCD_PIN11, 0);
		hal_gpio_write(SEG_LCD_PIN12, 0);
		hal_gpio_write(SEG_LCD_PIN13, 0);
		hal_gpio_write(SEG_LCD_PIN14, 0);
		hal_gpio_write(SEG_LCD_PIN15, 0);
		hal_gpio_write(SEG_LCD_PIN16, 0);
		hal_gpio_write(SEG_LCD_PIN17, 0);
		hal_gpio_write(SEG_LCD_PIN18, 0);

		hal_gpio_write(SEG_LCD_COM1, 1);
		hal_gpio_write(SEG_LCD_COM2, 1);
		hal_gpio_write(SEG_LCD_COM3, 1);
		hal_gpio_write(SEG_LCD_COM4, 1);
		hal_gpio_write(SEG_LCD_COM5, 1);

		app_seg_lcd_backlight_on();

		app_seg_lcd_test_status = 1;
	}
	else
	{

		hal_gpio_write(SEG_LCD_PIN1, 1);
		hal_gpio_write(SEG_LCD_PIN2, 1);
		hal_gpio_write(SEG_LCD_PIN3, 1);
		hal_gpio_write(SEG_LCD_PIN4, 1);
		hal_gpio_write(SEG_LCD_PIN10, 1);
		hal_gpio_write(SEG_LCD_PIN11, 1);
		hal_gpio_write(SEG_LCD_PIN12, 1);
		hal_gpio_write(SEG_LCD_PIN13, 1);
		hal_gpio_write(SEG_LCD_PIN14, 1);
		hal_gpio_write(SEG_LCD_PIN15, 1);
		hal_gpio_write(SEG_LCD_PIN16, 1);
		hal_gpio_write(SEG_LCD_PIN17, 1);
		hal_gpio_write(SEG_LCD_PIN18, 1);

		hal_gpio_write(SEG_LCD_COM1, 0);
		hal_gpio_write(SEG_LCD_COM2, 0);
		hal_gpio_write(SEG_LCD_COM3, 0);
		hal_gpio_write(SEG_LCD_COM4, 0);
		hal_gpio_write(SEG_LCD_COM5, 0);

		app_seg_lcd_backlight_off();

		app_seg_lcd_test_status = 0;
	}
}


