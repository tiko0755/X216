//  epb_MmBp.h
//  WeChat Embedded Proto Buffer
//
//  Generated by harlliu@tencent.com on 14-11-26.
//  Copyright 2014 Tencent. All rights reserved.
//

//  Version : 1.0.4

#ifndef __EPB_MMBP_H__
#define __EPB_MMBP_H__

#include <stdint.h>
#include <stdbool.h>
#include "epb.h"

typedef enum
{
    ECI_none                 = 0,
    ECI_req_auth             = 10001,
    ECI_req_sendData         = 10002,
    ECI_req_init             = 10003,
    ECI_resp_auth            = 20001,
    ECI_resp_sendData        = 20002,
    ECI_resp_init            = 20003,
    ECI_push_recvData        = 30001,
    ECI_push_switchView      = 30002,
    ECI_push_switchBackgroud = 30003,
    ECI_err_decode           = 29999
} EmCmdId;

typedef enum
{
    EEC_system                        = -1,
    EEC_needAuth                      = -2,
    EEC_sessionTimeout                = -3,
    EEC_decode                        = -4,
    EEC_deviceIsBlock                 = -5,
    EEC_serviceUnAvalibleInBackground = -6,
    EEC_deviceProtoVersionNeedUpdate  = -7,
    EEC_phoneProtoVersionNeedUpdate   = -8,
    EEC_maxReqInQueue                 = -9,
    EEC_userExitWxAccount             = -10
} EmErrorCode;

typedef enum
{
    EAM_md5          = 1,
    EAM_macNoEncrypt = 2,
} EmAuthMethod;

typedef enum
{
    EIRFF_userNickName = 0x1,
    EIRFF_platformType = 0x2,
    EIRFF_model        = 0x4,
    EIRFF_os           = 0x8,
    EIRFF_time         = 0x10,
    EIRFF_timeZone     = 0x20,
    EIRFF_timeString   = 0x40
} EmInitRespFieldFilter;

typedef enum
{
    EIS_deviceChat = 1,
    EIS_autoSync   = 2
} EmInitScence;

typedef enum
{
    EPT_ios     = 1,
    EPT_andriod = 2,
    EPT_wp      = 3,
    EPT_s60v3   = 4,
    EPT_s60v5   = 5,
    EPT_s40     = 6,
    EPT_bb      = 7
} EmPlatformType;

typedef enum
{
    EDDT_manufatureSvr        = 0,
    EDDT_wxWristBand          = 1,
    EDDT_wxDeviceHtmlChatView = 10001
} EmDeviceDataType;

typedef enum
{
    ESVO_enter = 1,
    ESVO_exit  = 2
} EmSwitchViewOp;

typedef enum
{
    EVI_deviceChatView     = 1,
    EVI_deviceChatHtmlView = 2
} EmViewId;

typedef enum
{
    ESBO_enterBackground = 1,
    ESBO_enterForground  = 2,
    ESBO_sleep           = 3
} EmSwitchBackgroundOp;

typedef struct
{
    void *none;
} BaseRequest;

typedef struct
{
    int32_t  err_code;
    bool     has_err_msg;
    CString  err_msg;
} BaseResponse;

typedef struct
{
    void *none;
} BasePush;

typedef struct
{
    BaseRequest *base_request;
    bool         has_md5_device_type_and_device_id;
    Bytes        md5_device_type_and_device_id;
    int32_t      proto_version;
    int32_t      auth_proto;
    EmAuthMethod auth_method;
    bool         has_aes_sign;
    Bytes        aes_sign;
    bool         has_mac_address;
    Bytes        mac_address;
    bool         has_time_zone;
    String       time_zone;
    bool         has_language;
    String       language;
    bool         has_device_name;
    String       device_name;
} AuthRequest;

typedef struct
{
    BaseResponse *base_response;
    CBytes        aes_session_key;
} AuthResponse;

typedef struct
{
    BaseRequest *base_request;
    bool         has_resp_field_filter;
    Bytes        resp_field_filter;
    bool         has_challenge;
    Bytes        challenge;
} InitRequest;

typedef struct
{
    BaseResponse  *base_response;
    uint32_t       user_id_high;
    uint32_t       user_id_low;
    bool           has_challeange_answer;
    uint32_t       challeange_answer;
    bool           has_init_scence;
    EmInitScence   init_scence;
    bool           has_auto_sync_max_duration_second;
    uint32_t       auto_sync_max_duration_second;
    bool           has_user_nick_name;
    CString        user_nick_name;
    bool           has_platform_type;
    EmPlatformType platform_type;
    bool           has_model;
    CString        model;
    bool           has_os;
    CString        os;
    bool           has_time;
    int32_t        time;
    bool           has_time_zone;
    int32_t        time_zone;
    bool           has_time_string;
    CString        time_string;
} InitResponse;

typedef struct
{
    BaseRequest     *base_request;
    Bytes            data;
    bool             has_type;
    EmDeviceDataType type;
} SendDataRequest;

typedef struct
{
    BaseResponse *base_response;
    bool          has_data;
    CBytes        data;
} SendDataResponse;

typedef struct
{
    BasePush        *base_push;
    CBytes           data;
    bool             has_type;
    EmDeviceDataType type;
} RecvDataPush;

typedef struct
{
    BasePush       *base_push;
    EmSwitchViewOp  switch_view_op;
    EmViewId        view_id;
} SwitchViewPush;

typedef struct
{
    BasePush             *base_push;
    EmSwitchBackgroundOp  switch_background_op;
} SwitchBackgroudPush;

BaseResponse *epb_unpack_base_response(const uint8_t *buf, int buf_len);

void epb_unpack_base_response_free(BaseResponse *response);

int epb_auth_request_pack_size(AuthRequest *request);

int epb_pack_auth_request(AuthRequest *request, uint8_t *buf, int buf_len);

AuthResponse *epb_unpack_auth_response(const uint8_t *buf, int buf_len);

void epb_unpack_auth_response_free(AuthResponse *response);

int epb_init_request_pack_size(InitRequest *request);

int epb_pack_init_request(InitRequest *request, uint8_t *buf, int buf_len);

InitResponse *epb_unpack_init_response(const uint8_t *buf, int buf_len);

void epb_unpack_init_response_free(InitResponse *response);

int epb_send_data_request_pack_size(SendDataRequest *request);

int epb_pack_send_data_request(SendDataRequest *request, uint8_t *buf, int buf_len);

SendDataResponse *epb_unpack_send_data_response(const uint8_t *buf, int buf_len);

void epb_unpack_send_data_response_free(SendDataResponse *response);

RecvDataPush *epb_unpack_recv_data_push(const uint8_t *buf, int buf_len);

void epb_unpack_recv_data_push_free(RecvDataPush *push);

SwitchViewPush *epb_unpack_switch_view_push(const uint8_t *buf, int buf_len);

void epb_unpack_switch_view_push_free(SwitchViewPush *push);

SwitchBackgroudPush *epb_unpack_switch_backgroud_push(const uint8_t *buf, int buf_len);

void epb_unpack_switch_backgroud_push_free(SwitchBackgroudPush *push);

#endif
