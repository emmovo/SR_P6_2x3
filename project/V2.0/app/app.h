/*
 * @Author: emmovo mingjkl@live.com
 * @Date: 2022-06-04 12:38:27
 * @LastEditors: emmovo
 * @LastEditTime: 2022-06-07 10:15:12
 * @FilePath: \V2.0\app\app.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
 
 #include "types.h"

 #include "app_timer.h"
 #include "timestamp.h"
 #include "app_flash.h"
 #include "main_logic.h"
 #include "app_btn.h"
 #include "app_bell.h"


uint16_t app_task(uint8_t task_id, uint16 events);
void app_task_init(uint8_t task_id);

