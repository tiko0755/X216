/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.2 */

#ifndef PB_DEVICE_PB_H_INCLUDED
#define PB_DEVICE_PB_H_INCLUDED
#include <pb.h>
#include "common.pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _GetDeviceFeatures {
    char dummy_field;
} GetDeviceFeatures;

typedef struct _GetDeviceInformation {
    char dummy_field;
} GetDeviceInformation;

typedef struct _DeviceFeatures {
    uint64_t features;
    uint64_t device_attributes;
} DeviceFeatures;

typedef struct _DeviceInformation {
    char serial_number[20];
    char name[16];
    pb_size_t supported_transports_count;
    Transport supported_transports[2];
    char device_type[22];
} DeviceInformation;


/* Initializer values for message structs */
#define DeviceInformation_init_default           {"", "", 0, {_Transport_MIN, _Transport_MIN}, ""}
#define GetDeviceInformation_init_default        {0}
#define DeviceFeatures_init_default              {0, 0}
#define GetDeviceFeatures_init_default           {0}
#define DeviceInformation_init_zero              {"", "", 0, {_Transport_MIN, _Transport_MIN}, ""}
#define GetDeviceInformation_init_zero           {0}
#define DeviceFeatures_init_zero                 {0, 0}
#define GetDeviceFeatures_init_zero              {0}

/* Field tags (for use in manual encoding/decoding) */
#define DeviceFeatures_features_tag              1
#define DeviceFeatures_device_attributes_tag     2
#define DeviceInformation_serial_number_tag      1
#define DeviceInformation_name_tag               2
#define DeviceInformation_supported_transports_tag 3
#define DeviceInformation_device_type_tag        4

/* Struct field encoding specification for nanopb */
#define DeviceInformation_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   serial_number,     1) \
X(a, STATIC,   SINGULAR, STRING,   name,              2) \
X(a, STATIC,   REPEATED, UENUM,    supported_transports,   3) \
X(a, STATIC,   SINGULAR, STRING,   device_type,       4)
#define DeviceInformation_CALLBACK NULL
#define DeviceInformation_DEFAULT NULL

#define GetDeviceInformation_FIELDLIST(X, a) \

#define GetDeviceInformation_CALLBACK NULL
#define GetDeviceInformation_DEFAULT NULL

#define DeviceFeatures_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT64,   features,          1) \
X(a, STATIC,   SINGULAR, UINT64,   device_attributes,   2)
#define DeviceFeatures_CALLBACK NULL
#define DeviceFeatures_DEFAULT NULL

#define GetDeviceFeatures_FIELDLIST(X, a) \

#define GetDeviceFeatures_CALLBACK NULL
#define GetDeviceFeatures_DEFAULT NULL

extern const pb_msgdesc_t DeviceInformation_msg;
extern const pb_msgdesc_t GetDeviceInformation_msg;
extern const pb_msgdesc_t DeviceFeatures_msg;
extern const pb_msgdesc_t GetDeviceFeatures_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define DeviceInformation_fields &DeviceInformation_msg
#define GetDeviceInformation_fields &GetDeviceInformation_msg
#define DeviceFeatures_fields &DeviceFeatures_msg
#define GetDeviceFeatures_fields &GetDeviceFeatures_msg

/* Maximum encoded size of messages (where known) */
#define DeviceInformation_size                   65
#define GetDeviceInformation_size                0
#define DeviceFeatures_size                      22
#define GetDeviceFeatures_size                   0

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
