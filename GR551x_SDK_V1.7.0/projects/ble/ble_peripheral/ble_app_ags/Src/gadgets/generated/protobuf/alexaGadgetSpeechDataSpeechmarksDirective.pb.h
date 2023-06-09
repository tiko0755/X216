/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.2 */

#ifndef PB_ALEXAGADGETSPEECHDATA_ALEXAGADGETSPEECHDATASPEECHMARKSDIRECTIVE_PB_H_INCLUDED
#define PB_ALEXAGADGETSPEECHDATA_ALEXAGADGETSPEECHDATASPEECHMARKSDIRECTIVE_PB_H_INCLUDED
#include <pb.h>
#include "directiveHeader.pb.h"
#include "alexaGadgetSpeechDataSpeechmarksDirectivePayload.pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive {
    bool has_header;
    header_DirectiveHeaderProto header;
    bool has_payload;
    alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto payload;
} alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive;

typedef struct _alexaGadgetSpeechData_SpeechmarksDirectiveProto {
    bool has_directive;
    alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive directive;
} alexaGadgetSpeechData_SpeechmarksDirectiveProto;


/* Initializer values for message structs */
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_init_default {false, alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_init_default}
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_init_default {false, header_DirectiveHeaderProto_init_default, false, alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto_init_default}
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_init_zero {false, alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_init_zero}
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_init_zero {false, header_DirectiveHeaderProto_init_zero, false, alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto_init_zero}

/* Field tags (for use in manual encoding/decoding) */
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_header_tag 1
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_payload_tag 2
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_directive_tag 1

/* Struct field encoding specification for nanopb */
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, MESSAGE,  directive,         1)
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_CALLBACK NULL
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_DEFAULT NULL
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_directive_MSGTYPE alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive

#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, MESSAGE,  header,            1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  payload,           2)
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_CALLBACK NULL
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_DEFAULT NULL
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_header_MSGTYPE header_DirectiveHeaderProto
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_payload_MSGTYPE alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto

extern const pb_msgdesc_t alexaGadgetSpeechData_SpeechmarksDirectiveProto_msg;
extern const pb_msgdesc_t alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_fields &alexaGadgetSpeechData_SpeechmarksDirectiveProto_msg
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_fields &alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_msg

/* Maximum encoded size of messages (where known) */
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_size (61 + header_DirectiveHeaderProto_size)
#define alexaGadgetSpeechData_SpeechmarksDirectiveProto_Directive_size (55 + header_DirectiveHeaderProto_size)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
