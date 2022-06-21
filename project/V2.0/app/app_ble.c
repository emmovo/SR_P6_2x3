
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


#if (TUYA_BLE_PROTOCOL_VERSION_HIGN == 0x04)

uint32_t ble_dp_data_sn = 0;



tuya_ble_status_t tuya_ble_dp_data_report(uint8_t *p_data,uint32_t len)
{

		uint8_t temp_data[30];
		tuya_ble_status_t err = 0;

		temp_data[0] = p_data[0];
		temp_data[1] = p_data[1];
		temp_data[2] = 0;

		memcpy(temp_data + 3, p_data + 2, len - 2);

		 err = tuya_ble_dp_data_send(ble_dp_data_sn++, 
									DP_SEND_TYPE_ACTIVE,
									DP_SEND_FOR_CLOUD_PANEL,
									DP_SEND_WITH_RESPONSE,
									temp_data,
									len + 1);

#ifdef	APP_DEBUG_LOG

		TY_PRINTF("\r");
		TY_PRINTF("=====================================");		
		TY_HEXDUMP("mcu send dp data:", temp_data, len + 1);	
	TY_PRINTF("send err: %d sn: %d", err, ble_dp_data_sn);
		TY_PRINTF("=====================================");
		TY_PRINTF("\r");

#endif

		return err;
}

tuya_ble_status_t tuya_ble_dp_data_with_time_report(uint32_t timestamp,uint8_t *p_data,uint32_t len)
{
	
		uint8_t time_stramp_char[13] = {0};
		uint8_t p = 0;
		uint32_t temp = 1000000000;
		
		uint8_t temp_data[30];
		
		temp_data[0] = p_data[0];
		temp_data[1] = p_data[1];
		temp_data[2] = 0;

		memcpy(temp_data + 3, p_data + 2, len - 2);
		
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
												temp_data,
												len + 1);
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

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata rt time %d   >>>>>>", time);	

	#endif
	tuya_ble_dp_data_report(temp_dp, 5);
	
}

void ble_dp_return_rt_data(uint16_t count, uint16_t time,uint16_t calorie)
{
	
	if (!connection_status)
	{
		return;
	}

#if (TUYA_BLE_PROTOCOL_VERSION_HIGN == 0x04)
	
	uint8_t temp_dp_count[5] =
	{
		4,
		2,
		2,
		count >> 8,
		count,
	};

	tuya_ble_dp_data_report(temp_dp_count, 5);

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata rt count %d   >>>>>>", count);	

	#endif

	uint8_t temp_dp_time[5] =
	{
		5,
		2,
		2, 
		time >> 8, 
		time,
	};
	tuya_ble_dp_data_report(temp_dp_time, 15);

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata rt time %d   >>>>>>", time);	

	#endif


	uint8_t temp_dp_calorie[15] =
	{
		6,
		2,
		2,
		calorie>>8,
		calorie
	};
	tuya_ble_dp_data_report(temp_dp_calorie, 5);

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata rt calorie %d   >>>>>>", calorie);	

	#endif

#else

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

#endif
	
}

void ble_dp_return_rt_data_no_time(uint16_t count,uint16_t calorie)
{
	
	if (!connection_status)
	{
		return;
	}

#if (TUYA_BLE_PROTOCOL_VERSION_HIGN == 0x04)
	
	uint8_t temp_dp_count[5] =
	{
		4,
		2,
		2,
		count >> 8,
		count,
	};

	tuya_ble_dp_data_report(temp_dp_count, 5);

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata rt count %d   >>>>>>", count);	

	#endif

	uint8_t temp_dp_calorie[5] =
	{
		6,
		2,
		2,
		calorie>>8,
		calorie
	};
	tuya_ble_dp_data_report(temp_dp_calorie, 5);

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata rt calorie %d   >>>>>>", calorie);	

	#endif

#else

	uint8_t temp_dp[10] =
	{
		4,
		2,
		2,
		count >> 8,
		count,
		6,2,2,calorie>>8,calorie
	};
	tuya_ble_dp_data_report(temp_dp, 10);

#endif
	
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
	
	tuya_ble_dp_data_report(temp_dp, 4);

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata start   >>>>>>");	

	#endif
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
	

	tuya_ble_dp_data_report(temp_dp, 4);

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata stop   >>>>>>");	

	#endif
}

void ble_dp_return_state(bool state)
{
	if (!connection_status)
	{
		return;
	}


#if (TUYA_BLE_PROTOCOL_VERSION_HIGN == 0x04)

	if(state == true)
	{
		ble_dp_return_start();
	}
	else
	{
		ble_dp_return_stop();
	}

	ble_dp_return_pause(false);

#else

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
	
	tuya_ble_dp_data_report(temp_dp, 8);

#endif

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
	
	#ifdef	APP_DEBUG_LOG
	
	if(pause == true)
	{
		TY_PRINTF(" >>>>>>   mcu updata pause   >>>>>>");
	}
	else
	{
		TY_PRINTF(" >>>>>>   mcu updata go on   >>>>>>");
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

	tuya_ble_dp_data_report(temp_dp, 4);

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata mode %d   >>>>>>", mode);
	
	#endif
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

	
	
#if (TUYA_BLE_PROTOCOL_VERSION_HIGN == 0x04)

	ble_dp_return_stop();
	ble_dp_return_pause(false);
	ble_dp_return_mode(mode);

	uint32_t timestamp_valse;

	timestamp_valse = timestamp_get();

	TY_PRINTF("===== timestamp %d =====", timestamp_valse);

	if(total_cnt == 0)
	{
		total_time = 0;
		total_calorie = 0;
	}

	uint8_t temp_dp_count[5] = 
	{
		8,
		2,
		2,
		total_cnt >> 8,
		total_cnt,
	};

	tuya_ble_dp_data_with_time_report(timestamp_valse, temp_dp_count, 5);

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata total count %d   >>>>>>", total_cnt);
	
	#endif

	uint8_t temp_dp_time[5] = 
	{
		9,
		2,
		2,
		total_time >> 8,
		total_time,
	};

	tuya_ble_dp_data_with_time_report(timestamp_valse, temp_dp_time, 5);

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata total count %d   >>>>>>", total_time);
	
	#endif

	uint8_t temp_dp_calorie[5] = 
	{
		10,
		2,
		2,
		total_calorie>>8,
		total_calorie
	};

	tuya_ble_dp_data_with_time_report(timestamp_valse, temp_dp_calorie, 5);

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata total calorie %d   >>>>>>", total_calorie);
	
	#endif


#else

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

#endif
	

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

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata target time %d   >>>>>>", time);
	
	#endif

	
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

	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata target count %d   >>>>>>", cnt);
	
	#endif
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
	
	#ifdef	APP_DEBUG_LOG

	TY_PRINTF(" >>>>>>   mcu updata bat %d   >>>>>>", level);
	
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
	

	#ifdef	APP_DEBUG_LOG

		TY_PRINTF("\r");
		TY_PRINTF("=====================================");		
		TY_HEXDUMP("mcu receive dp data:", rx_data, len);	
		TY_PRINTF("=====================================");
		TY_PRINTF("\r");

	#endif


#if (TUYA_BLE_PROTOCOL_VERSION_HIGN == 0x04)

	memcpy(rx_data + 2, rx_data + 3, len - 3);

#endif
		
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
				
				#ifdef	APP_DEBUG_LOG

				TY_PRINTF(" <<<<<<   mcu receive end   <<<<<<");
	
				#endif

				main_logic_mode_end();

			}
			else if (rx_data[3] == 1)
			{
				#ifdef	APP_DEBUG_LOG

				TY_PRINTF(" <<<<<<   mcu receive start   <<<<<<");
	
				#endif
				main_logic_mode_start();
				
			}
			break;
		}
	case 2:
		{

			
			if(rx_data[3])
			{
					#ifdef	APP_DEBUG_LOG

					TY_PRINTF(" <<<<<<   mcu receive pause   <<<<<<");
	
					#endif
					main_logic_mode_pause();
	
			}
			else
			{
					#ifdef	APP_DEBUG_LOG

					TY_PRINTF(" <<<<<<   mcu receive go on   <<<<<<");
	
					#endif
					main_logic_mode_continue();
					
			}
			


			break;
		}
	case 3:
		{

				#ifdef	APP_DEBUG_LOG

				TY_PRINTF(" <<<<<<   mcu receive mode %d  <<<<<<", rx_data[3]);
	
				#endif

				ble_mode_switch(rx_data[3]);
			
				
	
			break;
		}
	case 14:
		{
			uint16_t time = (uint16_t)(rx_data[5] << 8 | rx_data[6]);

			#ifdef	APP_DEBUG_LOG

				TY_PRINTF(" <<<<<<   mcu target time %d  <<<<<<", time);
	
			#endif

			
			main_logic_target_time_set(time);
			
			tuya_ble_dp_data_report(rx_data, 7);

			
			break;
		}
	case 15:
		{
			#ifdef	APP_DEBUG_LOG

				TY_PRINTF(" <<<<<<   mcu target time %d  <<<<<<", (uint16_t)(rx_data[5] << 8 | rx_data[6]));
	
			#endif

			main_logic_target_count_set((uint16_t)(rx_data[5] << 8 | rx_data[6]));
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

