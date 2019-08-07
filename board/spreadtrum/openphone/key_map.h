#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_

#include <linux/key_code.h>
#include <boot_mode.h>

//#define KEYPAD_DEBUG
static uint32_t board_key_map[]={
    0x27, KEY_FASTBOOT, //home page
    0x42, KEY_RECOVERY, //back
    0x41, KEY_UPDATE, //volume up
    0x30, KEY_ENGTEST, //menu
};

#define CONFIG_KEYPAD_ROW_CNT 8
#define CONFIG_KEYPAD_COL_CNT 8
#define CONFIG_KEYPAD_LONG_CNT 0xc
#define CONFIG_KEYPAD_DEBOUNCE_CNT 0x5
#endif //_KEY_MAP_H_
