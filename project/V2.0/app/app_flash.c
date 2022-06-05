/*
 * @Author: emmovo mingjkl@live.com
 * @Date: 2022-06-05 10:28:02
 * @LastEditors: emmovo mingjkl@live.com
 * @LastEditTime: 2022-06-05 10:30:44
 * @FilePath: \V2.0\app\app_flash.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */



#include "tuya_ble_log.h"
#include "app_flash.h"
#include "ty_flash.h"

void flash_init(void)
{
	ty_flash_init();
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
