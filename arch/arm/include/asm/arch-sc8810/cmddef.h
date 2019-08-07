/******************************************************************************
 ** File Name:      cmddef.h                                                  *
 ** Author:         Richard Yang                                              *
 ** DATE:           20/09/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file includes some common message type defines for   *
 **                 other files.                                              * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 20/09/2001     Richard.Yang     Create.                                   *
 ** 25/12/2001     Lliu             Add LOG_FILTER_F and log filter cmd set   *
 ** 01/06/2003     Eric.zhou        Add Channel server CMD and Time update    *
 ** 					CMD.				                                  *	
 ** 16/09/2003	   Xueliang.Wang    Add a mode RESET_MODE to MCU_MODE_E       *
 ** 20/05/2004     Zhemin.Lin       Add RPC cmd for digital camera(CR9397)    *
 ******************************************************************************/
#ifndef _CMDDEF_H
#define _CMDDEF_H
#include "sci_types.h"
#include "tasks_id.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
// This is the communication frame head
typedef struct msg_head_tag
{
    uint32  seq_num;      // Message sequence number, used for flow control
	uint16  len;          // The totoal size of the packet "sizeof(MSG_HEAD_T)
                          // + packet size"
	uint8   type;         // Main command type
	uint8   subtype;      // Sub command type
} MSG_HEAD_T;

#define CM_COPY_MSG_HEAD(msg, num, total_len, main_type, sub_type) \
((MSG_HEAD_T *) msg)->seq_num = num;  \
((MSG_HEAD_T *) msg)->len     = total_len;\
((MSG_HEAD_T *) msg)->type    = main_type;\
((MSG_HEAD_T *) msg)->subtype = sub_type;

//Message Type define start
#define     MSG_REQ_REP_START   0
#define     DIAG_COMMAND_OFFSET	0
typedef enum
{
    // Query commands
    DIAG_SWVER_F             = 0,  // Information regarding MS software

    DIAG_SOFTKEY_F           = 1,  // Soft key command
    DIAG_LOG_F               = 2,
    DIAG_AT_F                = 3,
    DIAG_GET_SYS_INFO_F      = 4,  // Get system information.
    DIAG_SYSTEM_F            = 5,

    DIAG_CHIP_TEST_F         = 6,  // Direct chip test operation 
    DIAG_POWER_F             = 7,  // Power on/off MS
    DIAG_SIM_F               = 8,  // Sim card command
    DIAG_PS_F                = 9,
    DIAG_RF_F                = 10,
    DIAG_MCU_F               = 11,
    DIAG_CHANGE_MODE_F       = 12, // Set MCU mode: normal, test or calibration
    DIAG_NVITEM_F            = 13,
    DIAG_AT_CHLSRV_F         = 14, // AT Command from channel server
      
    DIAG_ADC_F               = 15, // read ADC result      
    DIAG_PIN_TEST_F          = 16, // for Auto pin-test      
    DIAG_CURRENT_TEST_F      = 17,
    DIAG_SIM_CARD_TEST_F     = 18, // Test sim card
    DIAG_AUDIO_TEST_F        = 19, // test audio part
    DIAG_KEYPAD_TEST_F       = 20, // test keypad
    DIAG_LCM_TEST_F          = 21, // test lcd module
    DIAG_VIBRATOR_TEST_F     = 22, // test vibrator

    DIAG_RPC_EFS_F           = 23,
    DIAG_RPC_DC_F            = 24,
    DIAG_PRODUCTION_TEST_EXTEND_F = 25,     // Product Test Extend Command.
    DIAG_FORCE_ON_CELLS_F    = 26,
    DIAG_RPC_FFS_F           = 27,
    DIAG_GET_MCU_MODE_F      = 28, //query mcu mode
    DIAG_CALIBRATION_NOISE_F = 29, // calibration noise check
    DIAG_POWER_SUPPLY_F      = 30, 
    DIAG_RF_QUERY_F          = 31,
    DIAG_CALIBRATION         = 32,  
    DIAG_CALIBRATION_BT_F    = 33, // Bluetooth test
    DIAG_TP_F                = 34, 
    DIAG_GPS_F               = 35,
    DIAG_TD_NV               = 36, //DIAG_TD_NV=36, read/write the TD calibration parameter
    DIAG_TV_PARAM            = 37, // add for TV chip 08-03-07
    DIAG_READ_ASSERT_F       = 40, //read assert info from NV item. 

    DIAG_LAYER1_F            = 50, // Read or write paramter by Layer1
    DIAG_WIFI_TEST_F     = 54,      //wifi test
      
    DIAG_SENSOR_REG_F        = 90, // sensor register w/r tool
    DIAG_FUN_CALL_F          = 92,
    DIAG_DIRECT_NV           = 94,
    DIAG_DIRECT_PHSCHK       = 95,   
    DIAG_ENDIAN_CHECK_F      = 97,
    REQ_MAX_F                = 98                  
} diag_cmd_code_enum_type;


//yujun.ke add for layer1 to diag signals
typedef enum
{
    L1_DIAG_RD_CALI_PARAM_CNF = DIAG<<8,
    L1_DIAG_WR_CALI_PARAM_CNF
}SIGNALS_TO_DIAG_E;



	#define     MSG_REG         99
	#define     MSG_UNREG       100
#define     MSG_REQ_REP_END     100

#define     MSG_REQ_START       101
	typedef enum
	{
	  PS_REQ_F    = 101,     // Protocol stack request message.
      ATC_CMD_R,             // ATC command to ATC module
	  IP_PACKET_R = 103,
	  PPP_PACKET_R,          // PPP packet request for ppp link!
	  DIAG_AT_R 
	} bs_env_cmd_enum_type;
#define     MSG_REQ_END         150

#define     MSG_ANONY_START     151
#define     MSG_LINK_IND        0       // Added by Leo.Feng
	                                    // Notify ChannelServer Connect / Disconnect with MS
	typedef enum
	{
        DIAG_LOG_A             = 152,   // log message
        PS_L2_A,                        // L2 layer anonymous message.
//		L1_IND_A,                       // L1 anonymous message.
		IP_PACKET_A            = 154,
		DIAG_AT_A			   = 155,   // AT Command message
		PPP_PACKET_A,
		DIAG_MOBILETV_DSP_LOG_F = 157, 
		DIAG_AUTOTEST_INFO_F    = 158   // For Autotest INFORMATION		
	} anony_enum_type;

#define     MSG_CONSOLE_A       159
#define     DIAG_CONSOLE_MS     159

#define     MSG_TOOLS_RESERVED  209
#define     MSG_ANONY_END       210
#define     MSG_TIME_OUT        211
#define     MSG_BUF_FULL        212
#define     MSG_SEND_OK         213
#define     MSG_CRC_ERROR       214
#define     MSG_LINK_ERROR      215
#define     MSG_UNKNOWN_STRUCT  216

// 
#define     MSG_BOARD_ASSERT    255
typedef enum
{
    // Normal assert information, subtype of ASSERT message.
    NORMAL_INFO,
    // Dump memory data.
    DUMP_MEM_DATA,
    // Dump memory end.
    DUMP_MEM_END,
    // Dump sme buff memory end
    DUMP_SME_BUFF_MEM_END
} ASSERT_DATA_TYPE_E;


/* Log filter command sets  */
typedef enum
{
    //LOG_GET_INFO,
    LOG_GET_FILTER = 1,
    LOG_SET_FILTER,
    LOG_SET_FLOW_LOG,
    ARM_LOG_ENABLE,
    ARM_LOG_DISABLE,
    DSP_LOG_ENABLE,
    DSP_LOG_DISABLE
} log_cmd_set_enum_type;

/* PS related command sets  */
typedef enum
{
    CELL_INFO_F,
    CAMP_STATUS_F
} PS_CMD_SET_ENUM_TYPE;

typedef enum
{
	DIAG_STACK_INFO_F,
	DIAG_MEMORY_INFO_F,
	DIAG_BACKTRACE_F,
    DIAG_MEM_ALLOC_INFO_F,
    DIAG_ASSERT_MS
} system_info_cmd_set_enum_type;

typedef enum
{
	DIAG_NORMAL_KEY_MSG_INFO_F,		// both send key down and up msg
	DIAG_KEY_MSG_DOWN_INFO_F,		// only send key down msg
	DIAG_KEY_MSG_UP_INFO_F			// only send key up msg
} softkey_cmd_set_enum_type;

/* BATTERY related command type sets*/
typedef enum
{
    BATT_VOLTAGE_F
}POWER_SUPPLY_TYPE;

/* AT Like Command ERROR type sets  */
#define		ATCMD_ERROR_TYPE_BASE	0
typedef enum
{
	ATCMD_SUCCESS		= ATCMD_ERROR_TYPE_BASE,
	ATCMD_BADCOMMAND,
	ATCMD_BADPARAMETER,
    ATCMD_BADADDRESS,
    ATCMD_BADLENGTH,
    ATCMD_BADADDRLEN,
    ATCMD_REPOWERON,
    ATCMD_REPOWEROFF
} ATCMD_ERROR_TYPE_E;

typedef enum
{
    MCU_READB,
    MCU_READW,
    MCU_READD,
    MCU_WRITEB,
    MCU_WRITEW,
    MCU_WRITED
} DIRECT_CHIP_TEST_TYPE_E;

typedef enum
{
    NVITEM_READ,				/*0*/
    NVITEM_WRITE,
    NVITEM_PRODUCT_CTRL_READ,
    NVITEM_PRODUCT_CTRL_WRITE,
    NVITEM_PRODUCT_CTRL_ERASE,
    NVITEM_GETLENGTH,			/*5*/
    NVITEM_DELETE
} NVITEM_ACCESS_E;

#define MAX_WRITE_SIZE 100

typedef struct
{
    int * addr;
} CHIP_READ_T;

typedef struct
{
    int * addr;
    int buffer[MAX_WRITE_SIZE];
} CHIP_WRITE_T;
/* Message type define end*/

typedef enum
{
    POWER_ON, 
    POWER_OFF
} POWER_SET_E;

typedef enum
{
    NORMAL_MODE                         = 0,
    LAYER1_TEST_MODE                    = 1,
    ASSERT_BACK_MODE                    = 2,
    CALIBRATION_MODE                    = 3,
    DSP_CODE_DOWNLOAD_BACK              = 4,
    DSP_CODE_DOWNLOAD_BACK_CALIBRATION  = 5,
    BOOT_RESET_MODE                     = 6,
    PRODUCTION_MODE                     = 7,
    RESET_MODE                          = 8,
    CALIBRATION_POST_MODE               = 9,
    PIN_TEST_MODE                       = 10,
    IQC_TEST_MODE                       = 11,
    WATCHDOG_RESET_MODE                 = 12,

    CALIBRATION_NV_ACCESS_MODE          = 13,
    CALIBRATION_POST_NO_LCM_MODE        = 14,

    TD_CALIBRATION_POST_MODE            = 15,
    TD_CALIBRATION_MODE                 = 16,
    TD_CALIBRATION_POST_NO_LCM_MODE     = 17,
    AUTO_TEST_MODE       		= 18,
    
    MODE_MAX_TYPE,

    MODE_MAX_MASK                       = 0x1f	
    
} MCU_MODE_E;

/*
*indicate that current is usb mcft mode
*maybe used only when changmode by pc tools
*/
#define MCU_MODE_USB  0x80

typedef enum
{
    MISC_TM_UPDATE =1,
    MISC_RESERVED 
} MISC_CMD_E;


typedef enum
{
    ADC_0  = 0x1,
    ADC_1  = 0x2,
    ADC_2  = 0x3,
    ADC_3  = 0x4,    
    ADC_4  = 0x5,
    ADC_5 = 0x6, 
    ADC_6 = 0x7,
    ADC_0_HIGH_VOLTAGE = 0x81,
    ADC_1_HIGH_VOLTAGE = 0x82,
    ADC_2_HIGH_VOLTAGE = 0x83,
    ADC_3_HIGH_VOLTAGE = 0x84, 
    ADC_4_HIGH_VOLTAGE = 0x85, 
    ADC_5_HIGH_VOLTAGE = 0x86,
    ADC_6_HIGH_VOLTAGE = 0x87
}ADC_SOURCE_E;

// @lin.liu (2003-08-25), CR: ms4267 
typedef struct
{
    MSG_HEAD_T   msg_head;
    uint16       result;     // the result that sent back to PC
}MSG_ADC_T;

#define MAX_TEST_PIN_NUM       292

//#define PIN_STATE_UNDEFINED      0x0
//#define PIN_STATE_PASS           0x1
//#define PIN_STATE_FAIED          0x2
//#define PIN_STATE_NOT_CHECKED    0x3

// We use 2bits for one pin.
// state[n][1:0] :  state of pin 4*n + 0
// state[n][3:2] :  state of pin 4*n + 1
// state[n][5:4] :  state of pin 4*n + 2
// state[n][7:6] :  state of pin 4*n + 3
//
typedef struct
{
    MSG_HEAD_T   msg_head;
    uint16       pin_num;           // the pin number of the module.( include PASS, FAILED, NOT_CHECKED )
    uint8        pin_state[ (MAX_TEST_PIN_NUM) / 4 + 2]; // pin state.
}MSG_PIN_TEST_T;
// End. @lin.liu

typedef struct
{
    MSG_HEAD_T   msg_head;     // type is DIAG_CALIBRATION_NOISE_F
    union
    {
        struct{
	    	uint32       EAR_noise_count;
    		uint32       speaker_noise_count;
    	}result;
    	struct{
    	    uint32       check_time;  // the time to check the noise count( ms )
    	    uint32       reserved;
    	}input;
    }param;
}MSG_CALIBRATION_NOISE_T;

typedef enum
{
    CURRENT_TEST_STOP                   = 0,
    CURRENT_TEST_RF_CLOSED              = 1,
    CURRENT_TEST_DEEP_SLEEP             = 2,
    CURRENT_TEST_LED_ON                 = 3,
    CURRENT_TEST_VIBRATOR_ON            = 4,
    CURRENT_TEST_RX_MIN_PEAK_VALUE      = 5,
    CURRENT_TEST_TX_MAX_PEAK_VALUE      = 6,
    CURRENT_TEST_CHARGING               = 7,
    CURRENT_TEST_LED_OFF                = 8,
    CURRENT_TEST_VIBRATOR_OFF           = 9,
     CURRENT_TEST_DEEP_GET_SLEEP_FLAG   = 0xA,
    CURRENT_TEST_DEEP_SLEEP_FLAG_ENABLE = 0xB,
    CURRENT_TEST_DEEP_SLEEP_FLAG_DISABLE= 0xC,
    CURRENT_TEST_DEEP_SLEEP_WAKEUP      = 0xF // Be careful that the higer 8 bit is used for parameter of how long it sleeps
    
}CURRENT_TEST_E;

typedef enum
{
    REG_READ, 
    REG_WRITE
} REG_RW_E;

typedef enum
{
	UNIT8_TYPE=1,
	UNIT16_TYPE, 
	UINT32_TYPE
} REG_TYPE_MODE_E;

typedef enum
{
	REG_READ_SUCCESS,    // successful
	REG_READ_ADDR_ERR,   // address cann't access 
	REG_READ_OTHER_ERR  // other error
}READ_RESULT_MODE_E;

typedef enum
{
	REG_WRITE_SUCCESS,   // successful
	REG_WRITE_ADDR_ERR,  // address cann't access 
	REG_WRITE_VALUE_ERR, // value error 
	REG_WRITE_OTHER_ERR // other error
} WRITE_RESULT_MODE_E;

typedef struct
{
	uint32       mode;     // MODE_ENUM
	uint32       addr;     // register address
	uint32       value;    // register value
}MSG_REG_T, *MSG_REG_PTR;

typedef struct
{
	uint32       func_id;     //

}MSG_FUNC_CALL_T, *MSG_FUNC_CALL_PTR;

#define MAX_FAILED_PIN_NUM    64
typedef struct
{
    MSG_HEAD_T      msg_head;
    uint16          result;
    uint8           failed_pin[ MAX_FAILED_PIN_NUM + 4]; // +4 for overwrite check
}MSG_PRODUCT_TEST_T;




//@lin.liu(2004-05-09). CR8889
//#define the subtype of type(DIAG_RPC_EFS_R).
typedef enum
{
    RPC_EFS_CLOSE,
    RPC_EFS_GET_LENGTH,
    RPC_EFS_CREATE,
    RPC_EFS_SEEK,
    RPC_EFS_READ,
    RPC_EFS_WRITE,
    RPC_EFS_DELETE,
    RPC_EFS_RENAME,
    RPC_EFS_CHECK_EXIST,
    RPC_EFS_FIND_FIRST,
    RPC_EFS_FIND_NEXT,
    RPC_EFS_CREATE_DIR,
    RPC_EFS_DELETE_DIR,
    RPC_EFS_FOPEN,
    RPC_EFS_FCLOSE,
    RPC_EFS_FREAD,
    RPC_EFS_FWRITE,
    RPC_EFS_FSEEK,
    RPC_EFS_FTELL,
    RPC_EFS_FEOF
}RPC_EFS_FUNCTION_E;
//End CR8889

//@Zhemin.Lin(2004-05-20), CR9397
typedef enum
{
	RPC_DC_OPEN = 0,
	RPC_DC_CLOSE,
	RPC_DC_SETBYPASSMODE,
	RPC_DC_SETPOWERSAVEMODE,
	RPC_DC_SETPREVIEWWINSIZE,
	RPC_DC_MOVEPREVIEWIN,
	RPC_DC_STARTPREVIEW,
	RPC_DC_STOPPREVIEW,
	RPC_DC_SNAPSHOT,
	RPC_DC_DISPJPEG,
	RPC_DC_LOADOSD,
	RPC_DC_ZOOMIN,
	RPC_DC_ZOOMOUT,
	RPC_DC_SETSWIVELMODE,
	RPC_DC_SETOVERLAYMODE,
	RPC_DC_GETERRORID,
	RPC_DC_SETJPEGSIZE,
	RPC_DC_SETJPEGLEVEL,
	RPC_DC_SETLIGHTSOURCE,
	RPC_DC_SETLIGHTINTENSITY,
	RPC_DC_BRIGTENOBJ,
	RPC_DC_ENABLEANTIBANDING,
	RPC_DC_ENABLESTROBE,
	RPC_DC_SETIMGEFFECT,
	RPC_DC_ADJUSTCONTRAST,
	RPC_DC_ADJUST_CR,
	RPC_DC_ADJUST_CB,
	RPC_DC_ADJUST_BRIGHT
}RPC_DC_FUNCTION_E;

//End CR9397


typedef enum
{
    RPC_FFS_CLOSE,
    RPC_FFS_GET_LENGTH,
    RPC_FFS_CREATE,
    RPC_FFS_SEEK,
    RPC_FFS_READ,
    RPC_FFS_WRITE,
    RPC_FFS_DELETE,
    RPC_FFS_RENAME,
    RPC_FFS_CHECK_EXIST,
    RPC_FFS_FIND_FIRST,
    RPC_FFS_FIND_NEXT,
    RPC_FFS_CREATE_DIR,
    RPC_FFS_DIR,
    RPC_FFS_CD,
    RPC_FFS_DELETE_DIR,
    RPC_FFS_FOPEN,
    RPC_FFS_FCLOSE,
    RPC_FFS_FREAD,
    RPC_FFS_FWRITE,
    RPC_FFS_FSEEK,
    RPC_FFS_FTELL,
    RPC_FFS_FEOF,
    RPC_FFS_FORMAT,
    RPC_FFS_FREE_SPACE,
    RPC_FFS_USED_SPACE
}RPC_FFS_FUNCTION_E;

//
// The subtype of DIAG_FORCE_ON_CELLS_F Command
//
typedef enum
{
    FORCE_ON_CELLS          = 0,
    CANCEL_FORCE_ON_CELLS   = 1
}FORCE_ON_CELLS_TYPE_E;


#define FORCE_ON_CELLS_NUM_MAX       6

typedef struct
{
    MSG_HEAD_T      msg_head;
    
    // The param is valid when the subtype is FORCE_ON_CELLS
    uint16          cell_number_and_status;
    uint16          cell_list[ FORCE_ON_CELLS_NUM_MAX ];
	uint16			sim_no;
}MSG_FORCE_ON_CELLS_T;

typedef struct
{
    MSG_HEAD_T      msg_head;
    
    uint32          batt_voltage;
}MSG_POWER_SUPPLY_T;


typedef enum _TOOL_TV_MSG_ID_E
{
    TOOL_TV_SET_FREQ = 0,              // ����Ƶ��
    TOOL_TV_GET_DEMODULATION,          // ��ý���ļ���Ϣ
    TOOL_TV_GET_FREQERROR,             // ���Ƶ��ƫ��
    TOOL_TV_GET_RSSI,                  // ���RSSI��С
    TOOL_TV_GET_BLOCKERROR,            // ��ý��յ����ܿ�����������ܿ���
    TOOL_TV_SET_REGISTER,              // д��Ĵ���ֵ
    TOOL_TV_GET_REGISTER,              // ��ȡ�Ĵ���ֵ
    TOOL_TV_GET_PLL,                   // ��ȡPLL��������Ϣ
    TOOL_TV_RESET_MODE,                // ����SC6600Vģʽ
    TOOL_TV_GET_MODE,                  // ��ѯ��ǰSC6600V��ģʽ
    TOOL_TV_BB_SHOW_PICTURE,           // ����LCD
    TOOL_TV_START_RAM_ROM_TEST,        // ����RAM/ROM
    TOOL_TV_DISPLAY_PICTURE,           // ��ʾͼƬ
    TOOL_TV_PERIPHERAL_TEST,           // Peripheral����
    TOOL_TV_START_AV_DECODE_TEST,      // ����Audio Vedio Decode����
    TOOL_TV_GET_AV_DECODE_TEST_STATE,  // ��ѯ����״̬,ȷ���Ƿ�������
    TOOL_TV_GET_AV_DECODE_TEST_RESULT, // Audio Vedio Decode���Խ��
    TOOL_TV_START_CMMB_DEMOD,          // ����CMMB Demod����
    TOOL_TV_START_DATA_DISPLAY,        // ��ʼ���ݻط�
    TOOL_TV_LCD_DISPLAY,               // LCD��ʾ����
    TOOL_TV_READ_NV,                   // ��NV����
    TOOL_TV_WRITE_NV,                  // дNV����
    TOOL_TV_GET_SERVICE_INFO,          // ��ȡƵ�㼰Ƶ����Ϣ
    TOOL_TV_PLAY_BY_PARAM,             // ���ݸ���Ƶ�㼰Ƶ����Ϣ���в���
    TOOL_TV_SEARCH_NETWORK,            // ����Ƶ��
    TOOL_TV_SMD_CHECK,                 // SMD���

    TOOL_TV_CLOSE_WAITING_WIN,         // �رյȴ�����(�ǹ�������)
    TOOL_TV_CMD_INVALID                // ��Ч����
} TOOL_TV_MSG_ID_E;
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
#endif      /*End cmddef.h*/
