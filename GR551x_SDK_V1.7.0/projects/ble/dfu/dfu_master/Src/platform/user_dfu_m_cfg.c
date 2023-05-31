/**
 ****************************************************************************************
 *
 * @file user_dfu_m_cfg.c
 *
 * @brief User DFU Master Config implementation.
 *
 ****************************************************************************************
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
 ****************************************************************************************
 */
#include "user_dfu_m_cfg.h"
#include "user_gui.h"
#include "user_periph_setup.h"
#include "user_app.h"
#include "hal_flash.h"
#include <string.h>

/*
 * LOCAL FUNCTION DECLARATION
 ****************************************************************************************
 */
static void dfu_m_get_img_info(dfu_img_info_t *p_img_info);
static void dfu_m_get_img_data(uint32_t addr, uint8_t *p_data, uint16_t length);
static void dfu_m_event_handler(dfu_m_event_t event, uint8_t pre);

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
static dfu_m_func_cfg_t s_dfu_m_func_cfg = 
{
    .dfu_m_get_img_info  = dfu_m_get_img_info,
    .dfu_m_get_img_data  = dfu_m_get_img_data,
    .dfu_m_send_data     = uart_data_send,
    .dfu_m_event_handler = dfu_m_event_handler,
};

static dfu_img_info_t dfu_img_info;

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static void dfu_m_get_img_info(dfu_img_info_t *img_info)
{
    memcpy(img_info, &dfu_img_info, sizeof(dfu_img_info_t));
}

static void dfu_m_get_img_data(uint32_t addr, uint8_t *p_data, uint16_t length)
{
    bool flash_security_status = false;
    uint32_t sys_security = sys_security_enable_status_check();
    if(sys_security)
    {
        flash_security_status = hal_flash_get_security();
        hal_flash_set_security(false);
    }
    hal_flash_read(addr, p_data, length);
    if(sys_security)
    {
        hal_flash_set_security(flash_security_status);   
    }
}

static void dfu_m_event_handler(dfu_m_event_t event, uint8_t pre)
{
    switch(event)
    {
        case PRO_START_SUCCESS:
            user_gui_program_start();
            break;

        case PRO_FLASH_SUCCESS:
            user_gui_programing(pre);
            break;

        case PRO_END_SUCCESS:
            user_gui_program_end();
            break;

        default:
            break;
    }
}



/*
 * GLOBAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */
void user_dfu_m_start(dfu_img_info_t *img_info)
{
    memcpy(&dfu_img_info, img_info, sizeof(dfu_img_info_t));
    master_timer_start();
    bool security = sys_security_enable_status_check();
    dfu_m_program_start(security, true);
}


void user_dfu_m_init(uint8_t dfu_mode, uint16_t once_send_size)
{
    if (DFU_MODE_UART == dfu_mode)
    {
        s_dfu_m_func_cfg.dfu_m_send_data = uart_data_send;
    }
    else if (DFU_MODE_BLE == dfu_mode)
    {
        s_dfu_m_func_cfg.dfu_m_send_data =ble_data_send;
    }

    dfu_m_init(&s_dfu_m_func_cfg, once_send_size);
}


