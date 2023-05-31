/**
 *****************************************************************************************
 *
 * @file maxeye_cw221x_cmd.c
 *
 * @brief 
 *
 *****************************************************************************************
**/
//#include "app_log.h"
//#include "app_error.h"
//#include "app_i2c.h"

//#include "maxeye_i2c.h"
#include "maxeye_cw221x.h"
#include "maxeye_cw221x_cmd.h"

/*
 * DEFINES
 *****************************************************************************************
 */



/*
 * LOCAL VARIABLE DEFINITIONS
 *****************************************************************************************
 */


/*******************************************************************************   
 Function:       // 
 Description:    // 
 Input:          // None
 Output:         // None  
 Return:         // None
 Others:         // None 
*******************************************************************************/
uint8_t cmd_cw221x(const char *line, XPrint xprint){
    uint8_t id;
    unsigned int x;
    
    if(strncmp(line, "cw221x.get_id", strlen("cw221x.get_id"))==0){
        cw221x_get_chip_id(&id);
        xprint("+ok@cw221x_get_chip_id(0x%02x)", id);
        return 1;
    }
    else if(strncmp(line, "cw221x.get_cap", strlen("cw221x.get_cap"))==0){
        cw221x_get_capacity(&x);
        xprint("+ok@cw221x.get_cap(0x%08x, %d)", x, x);
        return 1;
    }
    else if(strncmp(line, "cw221x.init", strlen("cw221x.init"))==0){
        cw221x_init();
        xprint("+ok@cw221x.init()");
        return 1;
    }

    return 0;
}







