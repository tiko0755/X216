/**
 *****************************************************************************************
 *
 * @file user_periph_setup.c
 *
 * @brief  User Periph Init Function Implementation.
 *
 *****************************************************************************************
 * @attention
  #####Copyright (c) 2019 GOODIX
  All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
  * Neither the name of GOODIX nor the names of its contributors may be used
    to endorse or promote products derived from this software without
    specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************************
 */

/*
 * INCLUDE FILES
 *****************************************************************************************
 */
#include "user_periph_setup.h"
#include "gr55xx_sys.h"
#include "app_log.h"
#include "app_assert.h"
#include "hal_flash.h"
#include "custom_config.h"
#include "gr55xx_pwr.h"
#include "dfu_port.h"
#include "bsp.h"

#include "fault_trace.h"

#include "maxeye_uart.h"
#include "maxeye_i2c.h"
#include "maxeye_io_it.h"
#include "maxeye_gpio.h"


#include "maxeye_dfu.h"

/*
 * LOCAL VARIABLE DEFINITIONS
 *****************************************************************************************
 */
/**@brief Bluetooth device address. */
// static const uint8_t  s_bd_addr[SYS_BD_ADDR_LEN] = {0x13, 0x00, 0xcf, 0x3e, 0xcb, 0xea};
uint8_t s_bond_erase_enable = false;

/*
 * GLOBAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */
#if 0 //fault test
static void app_log_assert_init(void)
{
  app_log_init_t log_init;
  log_init.filter.level = APP_LOG_LVL_DEBUG;
  log_init.fmt_set[APP_LOG_LVL_ERROR] = APP_LOG_FMT_ALL & (~APP_LOG_FMT_TAG);
  log_init.fmt_set[APP_LOG_LVL_WARNING] = APP_LOG_FMT_LVL;
  log_init.fmt_set[APP_LOG_LVL_INFO] = APP_LOG_FMT_LVL;
  log_init.fmt_set[APP_LOG_LVL_DEBUG] = APP_LOG_FMT_LVL;
  app_log_init(&log_init, bsp_uart_send, bsp_uart_flush);
  fault_trace_db_init();
}
#endif


void app_periph_init(void)
{
    // SYS_SET_BD_ADDR(s_bd_addr);
    
//log
    bsp_log_init();

    // app_log_assert_init();

//中断配置
    maxeye_wlc_int_cfg();
    maxeye_charge_int_cfg();
    maxeye_g_int_cfg();
    maxeye_touch_int_cfg();
    maxeye_meter_int_cfg();  
    maxeye_wakeup_ble_pin_cfg();

//I2C 配置 
    app_i2c0_master_init();
    
//打码MCU
    maxeye_code_uart_init();
    mcu_pin_init();

//其他pin
    pencil_pin_init();

//dfu   
    maxeye_dfu_init();

//pwr
    pwr_mgmt_mode_set(PMR_MGMT_SLEEP_MODE);
}



