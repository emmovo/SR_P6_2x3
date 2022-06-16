/*
 * @Author: emmovo
 * @Date: 2022-06-12 13:50:07
 * @LastEditors: emmovo
 * @LastEditTime: 2022-06-16 14:40:39
 * @FilePath: \V2.0\app\app_seg_lcd.c
 * @Description: 
 * 
 * Copyright (c) 2022 by mingjkl@live.com/emmovo.com, All Rights Reserved. 
 */


#include "tuya_ble_log.h"
#include "app_seg_lcd.h"
#include "gpio.h"
#include "app.h"

const static bool num_to_seg_table[10][7] = {{1,1,1,1,1,1,0},		// num 0
								{0,1,1,0,0,0,0},		// num 1
								{1,1,0,1,1,0,1},		// num 2
								{1,1,1,1,0,0,1},		// num 3
								{0,1,1,0,0,1,1},		// num 4
								{1,0,1,1,0,1,1},		// num 5
								{1,0,1,1,1,1,1},		// num 6
								{1,1,1,0,0,0,0},		// num 7
								{1,1,1,1,1,1,1},		// num 8
								{1,1,1,1,0,1,1}};		// num 9

const static uint8_t num_seg_pin_table[8][9]	= {{4, 11, 11, 4, 10, 10, 4},	// cyc thousand digit seg
									{3, 13, 13, 3, 12, 12, 3},	// cyc hundred  digit seg
									{2, 15, 15, 2, 14, 14, 2},	// cyc ten digit seg
									{1, 17, 17, 1, 16, 16, 1},	// cyc single digit seg
									{4, 11, 11, 11, 10, 10, 4},	// min ten digit seg
									{3, 13, 13, 13, 12, 12, 3},	// sec single digit seg
									{2, 15, 15, 15, 14, 14, 2},	// min ten digit seg
									{1, 17, 17, 17, 16, 16, 1}};	// sec single digit seg

const static uint8_t mcu_seg_pin_map_table[18] = {SEG_LCD_PIN1, SEG_LCD_PIN2, SEG_LCD_PIN3, SEG_LCD_PIN4,
									0xFF, 0xFF, 0xFF, 0xFF,
									0xFF, SEG_LCD_PIN10, SEG_LCD_PIN11, SEG_LCD_PIN12,
									SEG_LCD_PIN13, SEG_LCD_PIN14, SEG_LCD_PIN15, SEG_LCD_PIN16,
									SEG_LCD_PIN17, SEG_LCD_PIN18};


const static uint8_t mcu_seg_com_map_table[5] = {SEG_LCD_COM1, SEG_LCD_COM2, SEG_LCD_COM3, SEG_LCD_COM4 ,SEG_LCD_COM5};



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



struct  lcd_display_param_s
{
	bool lcd_enable;
	bool backlight_enable;
	bool time_display;
	bool cycle_display;
	bool ble_connect;
	uint8_t soc_status;
	uint8_t time_min;
	uint8_t time_sec;
	uint16_t cyc_cnt;
	uint8_t mode_display;
	bool seg_polar_dir;
};

struct lcd_display_param_s lcd_display_param;

void lcd_display_param_init(void)
{
	lcd_display_param.lcd_enable = false;
	lcd_display_param.backlight_enable = false;
	lcd_display_param.time_display = false;
	lcd_display_param.cycle_display = false;
	lcd_display_param.ble_connect = false;
	lcd_display_param.soc_status = 0;
	lcd_display_param.time_min = 0;
	lcd_display_param.time_sec = 0;
	lcd_display_param.cyc_cnt = 0;
	lcd_display_param.mode_display = 0;
	lcd_display_param.seg_polar_dir = false;
}


void lcd_display_task(void)
{
	uint8_t p_com = 0;
	uint8_t p_num_seg_pos = 0;
	uint8_t p_seg = 0;

	bool seg_polar;
	bool com_polar;

	seg_polar = lcd_display_param.seg_polar_dir ? true : false;
	com_polar = lcd_display_param.seg_polar_dir ? false : true;

	for(p_com = 0; p_com < 5; p_com++)
	{
		hal_gpio_write(mcu_seg_com_map_table[p_com], com_polar);

		for(p_num_seg_pos = 0; p_num_seg_pos < 8; p_num_seg_pos++)
		{
			for(p_seg = 0; p_seg < 7; p_seg++)
			{
				// hal_gpio_write(mcu_seg_pin_map_table[num_seg_pin_table[p_num_seg_pos][p_seg]], \
				// 				num_to_seg_table[lcd_display_param.cyc_cnt][seg] ? seg_polar : com_polar);
			}
		}

	}
}


