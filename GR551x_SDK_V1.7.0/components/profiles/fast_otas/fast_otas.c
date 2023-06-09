/**
 ****************************************************************************************
 *
 * @file fast_otas.c
 *
 * @brief Over The Air Server Implementation.
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
#include "fast_otas.h"
#include "ble_prf_types.h"
#include "ble_prf_utils.h"
#include "utility.h"

/*
 * DEFINES
 ****************************************************************************************
 */
/**@brief Proprietary UUIDs. */
#define FAST_OTA_SERVICE_UUID           {0x1B, 0xD7, 0x90, 0xEC, 0xE8, 0xB9, 0x75, 0x80, 0x0A, 0x46, 0x44, 0xD3, 0x01, 0x07, 0xED, 0xA6}
#define FAST_OTA_SERVICE_CMD_UUID       {0x1B, 0xD7, 0x90, 0xEC, 0xE8, 0xB9, 0x75, 0x80, 0x0A, 0x46, 0x44, 0xD3, 0x02, 0x07, 0xED, 0xA6}
#define FAST_OTA_SERVICE_DATA_UUID      {0x1B, 0xD7, 0x90, 0xEC, 0xE8, 0xB9, 0x75, 0x80, 0x0A, 0x46, 0x44, 0xD3, 0x03, 0x07, 0xED, 0xA6}

/**@brief Macros for conversion of 128bit to 16bit UUID. */
#define ATT_128_PRIMARY_SERVICE     BLE_ATT_16_TO_128_ARRAY(BLE_ATT_DECL_PRIMARY_SERVICE)
#define ATT_128_CHARACTERISTIC      BLE_ATT_16_TO_128_ARRAY(BLE_ATT_DECL_CHARACTERISTIC)
#define ATT_128_CLIENT_CHAR_CFG     BLE_ATT_16_TO_128_ARRAY(BLE_ATT_DESC_CLIENT_CHAR_CFG)

/*
 * ENUMERATIONS
 ****************************************************************************************
 */
/**@brief FAST_OTA Service Attributes Indexes. */
enum fast_otas_attr_idx_tag
{
    FAST_OTAS_IDX_SVC,

    FAST_OTAS_IDX_CMD_CHAR,
    FAST_OTAS_IDX_CMD_VAL,
    FAST_OTAS_IDX_CMD_CFG,
    FAST_OTAS_IDX_DATA_CHAR,
    FAST_OTAS_IDX_DATA_VAL,

    FAST_OTAS_IDX_NB,
};

/*
 * STRUCT DEFINE
 ****************************************************************************************
 */
struct fast_otas_env_t
{
    fast_otas_init_t fast_otas_init;
    uint16_t     ntf_cfg[FAST_OTAS_CONNECTION_MAX];
    uint16_t    start_hdl;
};


 /*
 * LOCAL FUNCTION DECLARATION
 ****************************************************************************************
 */
static sdk_err_t   fast_otas_init(void);
static void        fast_otas_write_att_cb(uint8_t conn_idx, const gatts_write_req_cb_t *p_param);
static void        fast_otas_read_att_cb(uint8_t conn_idx, const gatts_read_req_cb_t *p_param);
static void        fast_otas_cccd_set_cb(uint8_t conn_idx, uint16_t handle, uint16_t cccd_value);
static void        fast_otas_gatts_cmpl_cb(uint8_t conn_idx, uint8_t status, const ble_gatts_ntf_ind_t *p_ntf_ind);

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
static struct fast_otas_env_t s_fast_otas_env;
static uint16_t          s_char_mask = 0xff;
/**@brief Full FAST_OTAS Database Description - Used to add attributes into the database. */
static const attm_desc_128_t fast_otas_att_db[FAST_OTAS_IDX_NB] = {
    // FAST_OTA service
    [FAST_OTAS_IDX_SVC] = {ATT_128_PRIMARY_SERVICE, READ_PERM_UNSEC, 0, 0},

    // FAST_OTA TX Characteristic Declaration
    [FAST_OTAS_IDX_CMD_CHAR] = {ATT_128_CHARACTERISTIC,READ_PERM_UNSEC, 0, 0},
    // FAST_OTA TX Characteristic Value
    [FAST_OTAS_IDX_CMD_VAL]  = {FAST_OTA_SERVICE_CMD_UUID,
                          WRITE_CMD_PERM_UNSEC | NOTIFY_PERM_UNSEC, 
                          (ATT_VAL_LOC_USER | ATT_UUID_TYPE_SET(UUID_TYPE_128)),
                          FAST_OTAS_MAX_DATA_LEN},

    // FAST_OTA TX Characteristic - Client Characteristic Configuration Descriptor
    [FAST_OTAS_IDX_CMD_CFG]  = {ATT_128_CLIENT_CHAR_CFG,
                          READ_PERM_UNSEC| WRITE_REQ_PERM_UNSEC,
                          0,
                          0},

    // FAST_OTA RX Characteristic Declaration
    [FAST_OTAS_IDX_DATA_CHAR] = {ATT_128_CHARACTERISTIC, READ_PERM_UNSEC, 0, 0 },
     
    // FAST_OTA RX Characteristic Value
    [FAST_OTAS_IDX_DATA_VAL]  = {FAST_OTA_SERVICE_DATA_UUID,
                          WRITE_CMD_PERM_UNSEC,
                          (ATT_VAL_LOC_USER | ATT_UUID_TYPE_SET(UUID_TYPE_128)),
                          FAST_OTAS_MAX_DATA_LEN},
};

/**@brief FAST_OTA Server Task interface required by profile manager. */
static ble_prf_manager_cbs_t fast_otas_tack_cbs = 
{ 
    (prf_init_func_t) fast_otas_init, 
    NULL, 
    NULL, 
};

/**@brief FAST_OTA Server Task Callbacks. */
static gatts_prf_cbs_t fast_otas_cb_func = 
{
    fast_otas_read_att_cb, 
    fast_otas_write_att_cb,
    NULL, 
    fast_otas_gatts_cmpl_cb,
    fast_otas_cccd_set_cb
};

/**@brief FAST_OTA Server Information. */
static const prf_server_info_t fast_otas_prf_info = 
{ 
    .max_connection_nb = FAST_OTAS_CONNECTION_MAX, 
    .manager_cbs = &fast_otas_tack_cbs, 
    .gatts_prf_cbs =&fast_otas_cb_func 
};

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/**
 *****************************************************************************************
 * @brief Initialize FAST_OTA service  create db in att
 *
 * @return Error code to know if profile initialization succeed or not.
 *****************************************************************************************
 */
static sdk_err_t fast_otas_init(void)
{
    // The start hanlde must be set with PRF_INVALID_HANDLE to be allocated automatically by BLE Stack.
    uint16_t          start_hdl       = PRF_INVALID_HANDLE;
    const uint8_t     fast_otas_svc_uuid[] = FAST_OTA_SERVICE_UUID;
    sdk_err_t         error_code      = SDK_SUCCESS;
    gatts_create_db_t gatts_db;

    memset(&gatts_db,0,sizeof(gatts_db));
    
    gatts_db.shdl                  = &start_hdl;
    gatts_db.uuid                  = fast_otas_svc_uuid;
    gatts_db.attr_tab_cfg          = (uint8_t*)&s_char_mask;
    gatts_db.max_nb_attr           = FAST_OTAS_IDX_NB;
    gatts_db.srvc_perm             = SRVC_UUID_TYPE_SET(UUID_TYPE_128);
    gatts_db.attr_tab_type         = SERVICE_TABLE_TYPE_128;
    gatts_db.attr_tab.attr_tab_128 = fast_otas_att_db;

    error_code = ble_gatts_srvc_db_create(&gatts_db);
    if (SDK_SUCCESS == error_code)
    {
        s_fast_otas_env.start_hdl = *(gatts_db.shdl);
    }

    return error_code;
}

/**
 *****************************************************************************************
 * @brief Handles reception of the attribute info request message.
 *
 * @param[in] conn_idx: Connection index
 * @param[in] p_param:  The parameters of the read request.
 *****************************************************************************************
 */
static void fast_otas_read_att_cb(uint8_t conn_idx, const gatts_read_req_cb_t *p_param)
{
    gatts_read_cfm_t cfm;
    uint8_t          handle     = p_param->handle;
    uint8_t          tab_index  = 0;

    tab_index = prf_find_idx_by_handle(handle, s_fast_otas_env.start_hdl, FAST_OTAS_IDX_NB, (uint8_t*)&s_char_mask);

    cfm.handle = handle;
    cfm.status = BLE_SUCCESS;

    switch(tab_index)
    {
        case FAST_OTAS_IDX_CMD_CFG:
            cfm.length = sizeof(uint16_t);
            cfm.value = (uint8_t *)(&s_fast_otas_env.ntf_cfg[conn_idx]);
            break;
        
        default:
            cfm.length = 0;
            cfm.status = BLE_ATT_ERR_INVALID_HANDLE;
            break;
    }

    ble_gatts_read_cfm(conn_idx,&cfm);
}

/**
 *****************************************************************************************
 * @brief Handles reception of the write request.
 *
 * @param[in] conn_idx: of connection index
 * @param[in] p_param: Pointer to the parameters of the write request.
 *****************************************************************************************
 */
static void fast_otas_write_att_cb(uint8_t conn_idx, const gatts_write_req_cb_t *p_param)
{
    gatts_write_cfm_t cfm;
    uint8_t           handle     = p_param->handle;
    uint8_t           tab_index  = 0;
    uint16_t          cccd_value = 0;
    fast_otas_evt_t        event;

    tab_index = prf_find_idx_by_handle(handle, s_fast_otas_env.start_hdl, FAST_OTAS_IDX_NB, (uint8_t*)&s_char_mask);

    cfm.handle = handle;
    cfm.status = BLE_SUCCESS;

    switch(tab_index)
    {
            
        case FAST_OTAS_IDX_CMD_VAL:
            if(le32toh(&p_param->value[0]) == FAST_OTA_PATTERN_VALUE)
            { 
                if(s_fast_otas_env.fast_otas_init.evt_handler != NULL)
                {
                    event.conn_idx = conn_idx;
                    event.evt_type = FAST_OTAS_EVT_CMD_RECEIVE_DATA;
                    event.p_data = (uint8_t*)&p_param->value[4];
                    event.length = p_param->length-4;
                    
                    s_fast_otas_env.fast_otas_init.evt_handler(&event);
                }
            }
            
            break;

        case FAST_OTAS_IDX_CMD_CFG:
            cccd_value = le16toh(&p_param->value[0]);
            if(s_fast_otas_env.fast_otas_init.evt_handler != NULL)
            {
                event.conn_idx = conn_idx;
                event.evt_type = (cccd_value == PRF_CLI_START_NTF) ?\
                                    FAST_OTAS_EVT_CMD_NOTIFICATION_ENABLED :
                                    FAST_OTAS_EVT_CMD_NOTIFICATION_DISABLED;
                s_fast_otas_env.fast_otas_init.evt_handler(&event);
            }
            s_fast_otas_env.ntf_cfg[conn_idx] = cccd_value;
            break;
            
         case FAST_OTAS_IDX_DATA_VAL:
            if(s_fast_otas_env.fast_otas_init.evt_handler != NULL)
            {
                event.conn_idx = conn_idx;
                event.evt_type = FAST_OTAS_EVT_DATA_RECEIVE_DATA;
                event.p_data = (uint8_t*)p_param->value;
                event.length = p_param->length;
                
                s_fast_otas_env.fast_otas_init.evt_handler(&event);
            }
            
            break;
            
            

        default:
            cfm.status = BLE_ATT_ERR_INVALID_HANDLE;
            break;
    }

    ble_gatts_write_cfm(conn_idx,&cfm);
}

/**
 *****************************************************************************************
 * @brief Handles reception of the cccd recover request.
 *
 * @param[in]: conn_idx:   Connection index
 * @param[in]: handle:     The handle of cccd attribute.
 * @param[in]: cccd_value: The value of cccd attribute.
 *****************************************************************************************
 */
static void fast_otas_cccd_set_cb(uint8_t conn_idx, uint16_t handle, uint16_t cccd_value)
{
    uint8_t           tab_index  = 0;
    fast_otas_evt_t        event;

    if (!prf_is_cccd_value_valid(cccd_value))
    {
        return;
    }

    tab_index = prf_find_idx_by_handle(handle, s_fast_otas_env.start_hdl, FAST_OTAS_IDX_NB, (uint8_t*)&s_char_mask);

    switch(tab_index)
    {
        case FAST_OTAS_IDX_CMD_CFG:
            if(s_fast_otas_env.fast_otas_init.evt_handler != NULL)
            {
                event.conn_idx = conn_idx;
                event.evt_type = (cccd_value == PRF_CLI_START_NTF) ?\
                                    FAST_OTAS_EVT_CMD_NOTIFICATION_ENABLED :
                                    FAST_OTAS_EVT_CMD_NOTIFICATION_DISABLED;
                s_fast_otas_env.fast_otas_init.evt_handler(&event);
            }
            s_fast_otas_env.ntf_cfg[conn_idx] = cccd_value;
            break;

        default:
            break;
    }
}

/**
 *****************************************************************************************
 * @brief Handles reception of the complete event.
 *
 * @param[in] conn_idx: Connection index
 * @param[in] p_param:  Pointer to the parameters of the complete event.
 *
 * @return If the event was consumed or not.
 *****************************************************************************************
 */
static void fast_otas_gatts_cmpl_cb(uint8_t conn_idx, uint8_t status, const ble_gatts_ntf_ind_t *p_ntf_ind)
{
    if(s_fast_otas_env.fast_otas_init.evt_handler != NULL)
    {
        fast_otas_evt_t event;

        event.conn_idx = conn_idx;
        if(status == BLE_SUCCESS)
        {
            if(p_ntf_ind->type == BLE_GATT_NOTIFICATION)
            {
                event.evt_type = FAST_OTAS_EVT_CMD_NOTIFY_COMPLETE;
                s_fast_otas_env.fast_otas_init.evt_handler(&event);
            }
        }
    }
 }

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
sdk_err_t fast_otas_notify_cmd_data(uint8_t conn_idx,uint8_t* p_data,uint16_t len)
{
    sdk_err_t   error_code = SDK_ERR_NTF_DISABLED;
    gatts_noti_ind_t send_cmd;

    if(s_fast_otas_env.ntf_cfg[conn_idx] == PRF_CLI_START_NTF)
    {
        // Fill in the parameter structure
        send_cmd.type = BLE_GATT_NOTIFICATION;
        send_cmd.handle = prf_find_handle_by_idx(FAST_OTAS_IDX_CMD_VAL, s_fast_otas_env.start_hdl, (uint8_t*)&s_char_mask);
        // pack measured value in database
        send_cmd.length = len;
        send_cmd.value  = p_data;
        // send notification to peer device
        error_code = ble_gatts_noti_ind(conn_idx,&send_cmd);
    }   
    return error_code;
}


sdk_err_t fast_otas_service_init(fast_otas_init_t *p_fast_otas_init)
{
    if (NULL == p_fast_otas_init)
    {
        return SDK_ERR_POINTER_NULL;
    }

    s_fast_otas_env.fast_otas_init.evt_handler = p_fast_otas_init->evt_handler;

    return ble_server_prf_add(&fast_otas_prf_info);
}
