#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_

#include <linux/key_code.h>
#include <boot_mode.h>

//#define KEYPAD_DEBUG
static uint32_t board_key_map[]={

    0x00, KEY_RECOVERY,//DOWN
    0x01, KEY_FASTBOOT,//CAM
    0x10, KEY_ENGTEST,//UP

};

#define CONFIG_KEYPAD_ROW_CNT 5
#define CONFIG_KEYPAD_COL_CNT 5
#define CONFIG_KEYPAD_LONG_CNT 0xc
#define CONFIG_KEYPAD_DEBOUNCE_CNT 0x5
#endif //_KEY_MAP_H_
