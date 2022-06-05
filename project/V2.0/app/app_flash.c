/*
 * @Author: emmovo mingjkl@live.com
 * @Date: 2022-06-05 10:28:02
 * @LastEditors: emmovo mingjkl@live.com
 * @LastEditTime: 2022-06-05 13:46:46
 * @FilePath: \V2.0\app\app_flash.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */



#include "tuya_ble_log.h"
#include "app_flash.h"
#include "ty_flash.h"

#define WEIGHT_DATA_FLASH_ADDR	0x11210000

void flash_init(void)
{
	ty_flash_init();
}


uint8_t common_weight_init(void)
{
	uint8_t weight_data[4];
	int err = 0;
	
	ty_flash_read(WEIGHT_DATA_FLASH_ADDR, weight_data, 4);

	TY_PRINTF("flash %x %x %d", weight_data[0], weight_data[1], weight_data[2]);

	if((weight_data[0] == 0xA1)&&(weight_data[1] == 0x55))
	{
		return weight_data[2];
	}
	else
	{
		ty_flash_erase(WEIGHT_DATA_FLASH_ADDR, 4);
		weight_data[0] = 0xA1;
		weight_data[1] = 0x55;
		weight_data[2] = 75;

		ty_flash_write(WEIGHT_DATA_FLASH_ADDR, weight_data, 4);

		return 75;
	}

}


		// //=====test
	
		// WaitMs(1000);
		// 	ty_flash_init();
	
		// uint8_t test_buffer[2];
	
		// int err = 0;
	
		// err = ty_flash_read(0x11200000, test_buffer, 2);
	
		// if(!err)
		// {
		// 		TY_PRINTF("flash read success %d", err);
		// }
		// else
		// {
		// 		TY_PRINTF("flash read error %d", err);
		// }
	
	
	
		// if(test_buffer[0] != 0x23)
		// {
		// 		ty_flash_erase(0x11200000, 2);
		// 		test_buffer[0] = 0x23;
		// 		test_buffer[1] = 0xA1;
		// 		ty_flash_write(0x11200000, test_buffer, 2);
		// 		TY_PRINTF("flash wtiter");
		// }
		
		// test_buffer[0] = 0xFF;
		// test_buffer[1] = 0xFF;
	
		// ty_flash_read(0x11200000, test_buffer, 2);
		
		// TY_PRINTF("flash read %x %x", test_buffer[0], test_buffer[1]);
		// //========


/// @} TIMER
