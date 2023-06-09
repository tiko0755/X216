#include "user_gui.h"
#include "gui_include.h"
#include "st7735.h"
#include "bsp.h"
#include "boards.h"
#include "ancs_protocol.h"
#include "gr55xx_sys.h"
#include "app_log.h"

#define BACK_COLOR      0xffff     /**< white. */
#define DISPLAY_COLOR   0xf800     /**< blue. */

/*
 * LOCAL FUNCTION DECLARATION
 ****************************************************************************************
 */
/**@brief Font code. */
static const uint8_t code_16x32[][16] =
{
    {0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x03,0x07,0x06,0x06,0x06,0x0E,0x0C},
    {0x0C,0x0C,0x1F,0x18,0x18,0x18,0x38,0x30,0x30,0x70,0xFC,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xE0,0xE0,0xE0,0xE0,0x70,0x70},
    {0x70,0x70,0xF8,0x38,0x38,0x38,0x38,0x1C,0x1C,0x1C,0x3F,0x00,0x00,0x00,0x00,0x00},/*"A",0*/

    {0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x3C,0x3E,0x3E,0x3E,0x3F,0x37,0x37,0x33,0x33},
    {0x31,0x31,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0xFC,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x8C,0x8C,0xCC},
    {0xCC,0xEC,0xEC,0xFC,0x7C,0x7C,0x3C,0x3C,0x3C,0x1C,0x1C,0x00,0x00,0x00,0x00,0x00},/*"N",1*/

    {0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x0C,0x1C,0x38,0x38,0x30,0x70,0x70,0x70},
    {0x70,0x70,0x70,0x70,0x70,0x38,0x38,0x38,0x1C,0x0E,0x03,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0xF6,0x1E,0x0E,0x06,0x03,0x03,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x06,0x0E,0x1C,0xF0,0x00,0x00,0x00,0x00,0x00},/*"C",2*/

    {0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x0E,0x1C,0x38,0x38,0x38,0x38,0x1C,0x1F,0x07},
    {0x01,0x00,0x00,0x00,0x00,0x30,0x30,0x18,0x1C,0x1E,0x1B,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0xF6,0x1E,0x0E,0x06,0x06,0x00,0x00,0x00,0x00,0xE0},
    {0xF8,0x7C,0x1E,0x0F,0x07,0x07,0x07,0x07,0x0E,0x1C,0xF8,0x00,0x00,0x00,0x00,0x00},/*"S",3*/

};


void app_key_evt_handler(uint8_t key_id, app_key_click_type_t key_click_type)
{
    uint16_t uid;
    if (key_click_type == APP_KEY_SINGLE_CLICK)
    {
        if (BSP_KEY_OK_ID == key_id)
        {
            pwr_mgmt_mode_set(PMR_MGMT_IDLE_MODE);
            uid = ancs_get_uid();

            ancs_notify_attr_get(uid, ANCS_NOTIF_ATTR_ID_APP_IDENTIFIER);
            ancs_notify_attr_get(uid, ANCS_NOTIF_ATTR_ID_TITLE);
            ancs_notify_attr_get(uid, ANCS_NOTIF_ATTR_ID_SUBTITLE);
            ancs_notify_attr_get(uid, ANCS_NOTIF_ATTR_ID_MESSAGE);
            ancs_notify_attr_get(uid, ANCS_NOTIF_ATTR_ID_MESSAGE_SIZE);
            ancs_notify_attr_get(uid, ANCS_NOTIF_ATTR_ID_DATE);
            ancs_notify_attr_get(uid, ANCS_NOTIF_ATTR_ID_POSITIVE_ACTION_LABEL);
            ancs_notify_attr_get(uid, ANCS_NOTIF_ATTR_ID_NEGATIVE_ACTION_LABEL);
        }
        else if (BSP_KEY_LEFT_ID == key_id)
        {
            APP_LOG_INFO("pressed key left");
            uid = ancs_get_uid();
            if (uid > 0)
            {
                ancs_action_perform(uid, ACTION_ID_NEGATIVE);
            }
        }
        else if (BSP_KEY_RIGHT_ID == key_id)
        {
            APP_LOG_INFO("pressed key right");
            uid = ancs_get_uid();

            ancs_action_perform(uid, ACTION_ID_POSITIVE);
        }
    }
}


void user_key_init(void)
{
    bsp_key_init();
}


/*
 * GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 */
void user_gui_init(void)
{
    gui_init();
    gui_set_refresh_mem(true);
    gui_fill_mem(BACK_COLOR);
    gui_rectangle(24,44,104,84,DISPLAY_COLOR);
    gui_set_color(DISPLAY_COLOR,BACK_COLOR);
    gui_put_string16_32(32,48,code_16x32,4);
    gui_refresh();
}


