/**
 *****************************************************************************************
 *
 * @file ias_c.c
 *
 * @brief Immediate Alert Service Client Implementation.
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
#include "ias_c.h"
#include "ble_prf_utils.h"
#include "utility.h"
#include <string.h>

/*
 * STRUCT DEFINE
 *****************************************************************************************
 */
/**@brief Immediate Alert Service environment variable. */
struct ias_c_env_t
{
    ias_c_handles_t     handles;            /**< Handles of IAS characteristics which will be got for peer. */
    ias_c_evt_handler_t evt_handler;        /**< Handler of IAS Client event  */
    uint8_t             prf_id;             /**< IAS Client profile id. */
};

/*
 * LOCAL FUNCTION DECLARATION
 *****************************************************************************************
 */
static void ias_c_att_write_cb(uint8_t conn_idx, uint8_t status, uint16_t handle);
static void ias_c_srvc_browse_cb(uint8_t conn_idx, uint8_t status, const ble_gattc_browse_srvc_t *p_browse_srvc);

/*
 * LOCAL VARIABLE DEFINITIONS
 *****************************************************************************************
 */
static struct ias_c_env_t s_ias_c_env;     /**< Immediate Alert Service Client environment variable. */

/**@brief Immediate Alert Service Client interface required by profile manager. */
static ble_prf_manager_cbs_t ias_c_mgr_cbs =
{
    NULL,
    NULL,
    NULL
};

/**@brief Immediate Alert Service GATT Client Callbacks. */
static gattc_prf_cbs_t ias_c_gattc_cbs =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ias_c_att_write_cb,
    NULL,
    ias_c_srvc_browse_cb,
    NULL,
};

/**@brief Immediate Alert Service Client Information. */
static const prf_client_info_t ias_c_prf_info =
{
    .max_connection_nb = IAS_C_CONNECTION_MAX,
    .manager_cbs       = &ias_c_mgr_cbs,
    .gattc_prf_cbs     = &ias_c_gattc_cbs
};

/*
 * LOCAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */
/**
 *****************************************************************************************
 * @brief This callback function will be called when receiving read response.
 *
 * @param[in] conn_idx:   The connection index.
 * @param[in] status:     The status of GATTC operation.
 * @param[in] handle:     The handle of attribute.
 *****************************************************************************************
 */
static void ias_c_att_write_cb(uint8_t conn_idx, uint8_t status, uint16_t handle)
{
    ias_c_evt_t ias_c_evt;

    ias_c_evt.conn_idx = conn_idx;
    ias_c_evt.evt_type = IAS_C_EVT_INVALID;

    if (handle == s_ias_c_env.handles.ias_alert_level_handle)
    {
        ias_c_evt.evt_type = (BLE_SUCCESS == status) ?\
                             IAS_C_EVT_ALERT_LEVEL_SET_SUCCESS :\
                             IAS_C_EVT_ALERT_LEVEL_SET_ERR;

        if (s_ias_c_env.evt_handler)
        {
            s_ias_c_env.evt_handler(&ias_c_evt);
        }
    }
}

/**
 *****************************************************************************************
 * @brief This callback function will be called when receiving browse service indication.
 *
 * @param[in] conn_idx:      The connection index.
 * @param[in] status:        The status of GATTC operation.
 * @param[in] p_browse_srvc: The information of service browse.
 *****************************************************************************************
 */
static void ias_c_srvc_browse_cb(uint8_t conn_idx, uint8_t status, const ble_gattc_browse_srvc_t *p_browse_srvc)
{
    ias_c_evt_t  ias_c_evt;
    uint16_t     uuid_disc;
    uint16_t     handle_disc;

    ias_c_evt.conn_idx = conn_idx;
    ias_c_evt.evt_type = IAS_C_EVT_DISCOVERY_FAIL;

    if(BLE_GATT_ERR_BROWSE_NO_ANY_MORE == status)
    {
        return;
    }

    if (BLE_SUCCESS == status)
    {
        uuid_disc = p_browse_srvc->uuid[0] | p_browse_srvc->uuid[1] << 8;

        if (BLE_ATT_SVC_IMMEDIATE_ALERT == uuid_disc)
        {
            s_ias_c_env.handles.ias_srvc_start_handle = p_browse_srvc->start_hdl;
            s_ias_c_env.handles.ias_srvc_end_handle   = p_browse_srvc->end_hdl;

            for (uint32_t i = 0; i < (p_browse_srvc->end_hdl - p_browse_srvc->start_hdl); i++)
            {
                uuid_disc   = p_browse_srvc->info[i].attr.uuid[0] | p_browse_srvc->info[i].attr.uuid[1] << 8;
                handle_disc = p_browse_srvc->start_hdl + i + 1;

                if (BLE_GATTC_BROWSE_ATTR_VAL == p_browse_srvc->info[i].attr_type)
                {
                    if (BLE_ATT_CHAR_ALERT_LEVEL == uuid_disc)
                    {
                        s_ias_c_env.handles.ias_alert_level_handle = handle_disc;
                    }
                }
                else if (BLE_GATTC_BROWSE_NONE == p_browse_srvc->info[i].attr_type)
                {
                    break;
                }
            }

            ias_c_evt.evt_type = IAS_C_EVT_DISCOVERY_COMPLETE;
        }
    }

    if (s_ias_c_env.evt_handler)
    {
        s_ias_c_env.evt_handler(&ias_c_evt);
    }
}

/*
 * GLOBAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */
sdk_err_t ias_client_init(ias_c_evt_handler_t evt_handler)
{
    if (NULL == evt_handler)
    {
        return SDK_ERR_POINTER_NULL;
    }

    memset(&s_ias_c_env, 0, sizeof(s_ias_c_env));
    s_ias_c_env.evt_handler = evt_handler;

    return ble_client_prf_add(&ias_c_prf_info, &s_ias_c_env.prf_id);
}

sdk_err_t ias_c_disc_srvc_start(uint8_t conn_idx)
{
    uint8_t target_uuid[2];

    target_uuid[0] = LO_U16(BLE_ATT_SVC_IMMEDIATE_ALERT);
    target_uuid[1] = HI_U16(BLE_ATT_SVC_IMMEDIATE_ALERT);

    const ble_uuid_t ias_service_uuid =
    {
        .uuid_len = 2,
        .uuid     = target_uuid,
    };

    return ble_gattc_prf_services_browse(s_ias_c_env.prf_id, conn_idx, &ias_service_uuid);
}

sdk_err_t ias_c_alert_level_set(uint8_t conn_idx, ias_c_alert_level_t alert_level)
{
    gattc_write_no_resp_t write_attr_value;


    if (BLE_ATT_INVALID_HDL == s_ias_c_env.handles.ias_alert_level_handle)
    {
        return SDK_ERR_INVALID_HANDLE;
    }

    write_attr_value.signed_write = false;
    write_attr_value.handle  = s_ias_c_env.handles.ias_alert_level_handle;
    write_attr_value.length  = sizeof(uint8_t);
    write_attr_value.p_value = (uint8_t *)&alert_level;

    return ble_gattc_prf_write_no_resp(s_ias_c_env.prf_id, conn_idx, &write_attr_value);
}

