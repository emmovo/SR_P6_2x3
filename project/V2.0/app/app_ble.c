
#include "app_ble.h"
#include "tuya_ble_log.h"
#include "tuya_ble_api.h"
#include "app.h"

// #include "flash.h"



#include "main_logic.h"

extern bool connection_status;
extern uint32_t common_weight;

#include "timestamp.h"



bool start_lock = false;


#ifdef	BOARD_BT612_4

uint32_t ble_dp_data_sn = 0;



tuya_ble_status_t tuya_ble_dp_data_report(uint8_t *p_data,uint32_t len)
{
		// return tuya_ble_dp_data_send(ble_dp_data_sn++, 
		// 							DP_SEND_TYPE_ACTIVE,
		// 							DP_SEND_FOR_CLOUD_PANEL,
		// 							DP_SEND_WITHOUT_RESPONSE,
		// 							p_data,
		// 							len);

		tuya_ble_status_t err = 0;

		 err = tuya_ble_dp_data_send(ble_dp_data_sn++, 
									DP_SEND_TYPE_ACTIVE,
									DP_SEND_FOR_CLOUD_PANEL,
									DP_SEND_WITHOUT_RESPONSE,
									p_data,
									len);
	
		uint8_t p = 0;
		for(p = 0; p < len; p++)
		{
				TY_PRINTF("%d", p_data[p]);
		}

		TY_PRINTF("send err: %d", err);

		return err;
}

tuya_ble_status_t tuya_ble_dp_data_with_time_report(uint32_t timestamp,uint8_t *p_data,uint32_t len)
{
	
		uint8_t time_stramp_char[13] = {0};
		uint8_t p = 0;
		uint32_t temp = 1000000000;
		
		for(p = 0; p < 10; p++)
		{
				time_stramp_char[p] = (timestamp / temp);
				timestamp -= time_stramp_char[p] * temp;
			
				time_stramp_char[p] += '0';
				temp = temp / 10;
		}
		
		time_stramp_char[10] = '0';
		time_stramp_char[11] = '0';
		time_stramp_char[12] = '0';
		
		
		return tuya_ble_dp_data_with_time_send(ble_dp_data_sn++,
												DP_SEND_FOR_CLOUD_PANEL,
												DP_TIME_TYPE_UNIX_TIMESTAMP,
												time_stramp_char,
												p_data,
												len);
}

#endif

void ble_dp_return_rt_time(uint16_t time)
{
	
	if (!connection_status)
	{
		return;
	}
	
	uint8_t temp_dp[5] =
	{
		5,
		2,
		2, 
		time >> 8, 
		time,
	};
	tuya_ble_dp_data_report(temp_dp, 5);
	
}

void ble_dp_return_rt_data(uint16_t count, uint16_t time,uint16_t calorie)
{
	
	if (!connection_status)
	{
		return;
	}
	
	uint8_t temp_dp[15] =
	{
		4,
		2,
		2,
		count >> 8,
		count,
		5,
		2,
		2, 
		time >> 8, 
		time,
		6,2,2,calorie>>8,calorie
	};
	tuya_ble_dp_data_report(temp_dp, 15);
	
}

void ble_dp_return_rt_data_no_time(uint16_t count,uint16_t calorie)
{
	
	if (!connection_status)
	{
		return;
	}
	
	uint8_t temp_dp[15] =
	{
		4,
		2,
		2,
		count >> 8,
		count,
		6,2,2,calorie>>8,calorie
	};
	tuya_ble_dp_data_report(temp_dp, 10);
	
}

void ble_dp_return_start(void)
{
	if (!connection_status)
	{
		return;
	}
	uint8_t temp_dp[8] =
	{
		1,
		1,
		1,
		1,
	};
	
	#ifdef debug_uart

	char a[10] = "start\r\n";

	uart_send(a, 10);

	#endif

	tuya_ble_dp_data_report(temp_dp, 4);
}

void ble_dp_return_stop(void)
{
	if (!connection_status)
	{
		return;
	}
	uint8_t temp_dp[8] =
	{
		1,
		1,
		1,
		0,
	};
	
	#ifdef debug_uart

	char a[10] = "stop\r\n";

	uart_send(a, 10);

	#endif

	tuya_ble_dp_data_report(temp_dp, 4);
}

void ble_dp_return_state(bool state)
{
	if (!connection_status)
	{
		return;
	}
	uint8_t temp_dp[8] =
	{
		1,
		1,
		1,
		state,
		2,
		1,
		1,
		0,
	};
	
	#ifdef debug_uart

	char a[10] = "start\r\n";
	char b[10] = "stop\r\n";
	if(state)
	{
			uart_send(a, 10);
	}
	else
	{
			uart_send(b, 10);
	}

	#endif

	tuya_ble_dp_data_report(temp_dp, 8);
}

void ble_dp_return_pause(bool pause)
{
	if (!connection_status)
	{
		return;
	}
	uint8_t temp_dp[8] =
	{
		2,
		1,
		1,
		pause,
	};
	
	#ifdef debug_uart
	
	char a[10] = "pause\r\n";
	char b[10] = "go on\r\n";
	if(pause)
	{
			uart_send(a, 10);
	}
	else
	{
			uart_send(b, 10);
	}
	
	#endif
	
	tuya_ble_dp_data_report(temp_dp, 4);

}

void ble_dp_return_mode(uint8_t mode)
{
	if (!connection_status)
	{
		return;
	}
	uint8_t temp_dp[4] =
	{
		3,
		4,
		1,
		mode,
	};
	
	
	#ifdef debug_uart
	
	char a[10] = "mode 0\r\n";
	char b[10] = "mode 1\r\n";
	char c[10] = "mode 2\r\n";
	if(mode == 0)
	{
			uart_send(a, 10);
	}
	if(mode == 1)
	{
			uart_send(b, 10);
	}
	if(mode == 2)
	{
			uart_send(c, 10);
	}
	
	#endif
	tuya_ble_dp_data_report(temp_dp, 4);
}

bool ble_finish_dp_offline_exist = false;

void ble_dp_return_finish(uint8_t mode, uint16_t total_cnt, uint16_t total_time,uint16_t total_calorie)
{
	
	
	static uint8_t of_mode;
	static uint16_t of_cnt, of_time, of_cal;
		
	
	if (!connection_status)
	{
		TUYA_BLE_LOG_INFO(" offline data set \r\n");
		of_mode = mode;
		of_cnt = total_cnt;
		of_time = total_time;
		of_cal = total_calorie;
	
		ble_finish_dp_offline_exist = true;
		
		return;
	}
	if (connection_status&&ble_finish_dp_offline_exist)
	{
		
		TUYA_BLE_LOG_INFO("Return offline data\r\n");
		
		ble_finish_dp_offline_exist = false;
		ble_dp_return_finish(of_mode, of_cnt, of_time, of_cal);
		return;
	}
	
	
	
	uint8_t temp_dp[27] =
	{
		1,
		1,
		1,
		0,
		2, 
		1,
		 1,
		 0,
		3,
		 4, 
		1,
		 mode,
		8,
		 2,
		 2,
		 total_cnt >> 8,
		total_cnt,
		9,
		 2,
		 2,
		 total_time >> 8,
		 total_time,
			10,
		2,2,total_calorie>>8,total_calorie
	};
	
	if(total_cnt == 0)
	{
		temp_dp[15] = 0;
		temp_dp[16] = 0;
		temp_dp[20] = 0;
		temp_dp[21] = 0;
		temp_dp[25] = 0;
		temp_dp[26] = 0;
	}
	
	TUYA_BLE_LOG_INFO("BLE Return finish:%d,%d,%d,%d\r\n", mode, total_cnt, total_time, total_calorie);
	
	tuya_ble_dp_data_with_time_report(timestamp_get(), temp_dp, 27);
}

void ble_dp_return_target_time(uint16_t time)
{
	if (!connection_status)
	{
		return;
	}
	uint8_t temp_dp[5] =
	{
		14,
		2,
		2,
		time>>8,
		time
	};
	tuya_ble_dp_data_report(temp_dp, 5);
}
void ble_dp_return_target_cnt(uint16_t cnt)
{
	if (!connection_status)
	{
		return;
	}
	uint8_t temp_dp[5] =
	{
		15,
		2,
		2,
		cnt>>8,
		cnt
	};
	tuya_ble_dp_data_report(temp_dp, 5);
}
void ble_dp_return_bat(uint8_t level)
{
	if (!connection_status)
	{
		return;
	}
	uint8_t temp_dp[4] =
	{
		16,
		4,
		1,
		level
	};
	
	#ifdef debug_uart
	
	char a[10] = "bat\r\n";

	uart_send(a, 10);
	
	#endif
	
	tuya_ble_dp_data_report(temp_dp, 4);
}


uint8_t rx_data[100] = { 0 };
uint8_t rx_data_len = 0;
bool rxed = false;

void ble_mode_pause_func(uint8_t pause);
void ble_mode_stop_func(void);
void ble_mode_start_func(void);


void ble_receive_dp(uint8_t *data, uint8_t len)
{
	memcpy(rx_data, data, len);
	rx_data_len = len;
	rxed = true;
	
		#ifdef debug_uart
		uint8_t i;
	
		uart_putchar("receive:");
		for(i = 0; i < len; i++)
		{
				uart_putchar(hex2Str(data[i]));
				uart_putchar(" ");
		}
		uart_putchar("\r\n");
		
		#endif
		
		TY_HEXDUMP("BLE dp data:", rx_data, rx_data_len);
	
	TUYA_APP_LOG_HEXDUMP_INFO("BLE dp data:", rx_data, rx_data_len);
		
}




void ble_app(void)
{
	if (rxed == false)
	{
		return;
	}
	switch (rx_data[0])
	{
	case 1:
		{
			if (rx_data[3] == 0)
			{
				// main_logic_mode_end();
				
					#ifdef debug_uart
	
					char a[10] = "ble_stop\r\n";

					uart_send(a, 10);
					
					#endif

			}
			else if (rx_data[3] == 1)
			{

				// main_logic_mode_start();
				
					#ifdef debug_uart
	
					char a[10] = "b_start\r\n";

					uart_send(a, 10);
					
					#endif
			}
			break;
		}
	case 2:
		{

			
			if(rx_data[3])
			{
					// main_logic_mode_pause();
			}
			else
			{
					// main_logic_mode_continue();
			}
			
								#ifdef debug_uart
	
					char a[10] = "pause ";
					a[7] = rx_data[3] + '0';
					uart_send(a, 10);
					
					#endif
			break;
		}
	case 3:
		{

			
				ble_mode_switch(rx_data[3]);
			
				#ifdef debug_uart
	
				char a[10] = "mode 0\r\n";
				char b[10] = "mode 1\r\n";
				char c[10] = "mode 2\r\n";
			
				if(rx_data[3] == 0)
				{
						uart_send(a, 10);
				}
				if(rx_data[3] == 1)
				{
						uart_send(b, 10);
				}
				if(rx_data[3] == 2)
				{
						uart_send(c, 10);
				}
				
				#endif
	
			break;
		}
	case 14:
		{
			uint16_t time = (uint16_t)(rx_data[5] << 8 | rx_data[6]);
			
			// main_logic_target_time_set(time);
			
			// tuya_ble_dp_data_report(rx_data, 7);

			
			break;
		}
	case 15:
		{
			// main_logic_target_count_set((uint16_t)(rx_data[5] << 8 | rx_data[6]));
			tuya_ble_dp_data_report(rx_data, 7);
			break;
		}
	case 17:
		{
			
			// common_weight =flash_readword(FLASH_SPACE_TYPE_MAIN, 0x27600/4);
			// if (common_weight != 0xffffffff)
			// {
			// 	TUYA_BLE_LOG_INFO("erase flash\r\n");
			// 	flash_erase_sector(FLASH_SPACE_TYPE_MAIN, 0x27600/4);
			// }
			
			// common_weight = ((uint32_t)rx_data[3] << 24 |
			// 	(uint32_t)rx_data[4] << 16 |
			// 	(uint32_t)rx_data[5] << 8 |
			// 	(uint32_t)rx_data[6]);
			
			// flash_writeword(FLASH_SPACE_TYPE_MAIN, 0x27600/4,common_weight);
			// TUYA_BLE_LOG_INFO("write flash\r\n");
			// tuya_ble_dp_data_report(rx_data, 7);
			// TUYA_BLE_LOG_INFO("Weight set to:%d\r\n", common_weight);
			break;
		}
	}
	rxed = false;
}

extern bool start_lock;

void(*mode_pause_func)(uint8_t pause) = NULL;

void(*mode_stop_func)(void) = NULL;

void(*mode_start_func)(void) = NULL;

void ble_mode_pause_func_reg(void(*func)(uint8_t pause))
{
	mode_pause_func = func;
}

void ble_mode_pause_func(uint8_t pause)
{
	mode_pause_func(pause);
}

void ble_mode_stop_func_reg(void(*func)(void))
{
	mode_stop_func = func;
}
void ble_mode_stop_func(void)
{
	mode_stop_func();
	
}

void ble_mode_start_func_reg(void(*func)(void))
{
	mode_start_func = func;
}
void ble_mode_start_func(void)
{
	mode_start_func();
	start_lock = false;
}

