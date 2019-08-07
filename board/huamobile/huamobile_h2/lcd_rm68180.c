/* drivers/video/sc8800g/sc8800g_lcd_rm68180.c
 *
 * Support for RM68180 LCD device
 *
 * Copyright (C) 2010 Spreadtrum
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <asm/arch/sc8810_lcd.h>

//#define  LCD_DEBUG
#ifdef LCD_DEBUG
#define LCD_PRINT printf
#else
#define LCD_PRINT(...)
#endif

#define SPI_RM68180_Write(a, b) {send_cmd(a); send_data(b);}


static int32_t rm68180_init(struct lcd_spec *self)
{

	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("rm68180_init\n");

	self->ops->lcd_reset(self);
	LCD_DelayMS(120);

	{
	// VCI=2.8V
	//************* Start Initial Sequence **********// /** Select Command Page '1' **/
	SPI_RM68180_Write(0xF000,0x55);
	SPI_RM68180_Write(0xF001,0xAA);
	SPI_RM68180_Write(0xF002,0x52);
	SPI_RM68180_Write(0xF003,0x08);
	SPI_RM68180_Write(0xF004,0x01);
	// Setting AVDD Voltage
	SPI_RM68180_Write(0xB000,0x0D); //AVDD=6V
	SPI_RM68180_Write(0xB001,0x0D);
	SPI_RM68180_Write(0xB002,0x0D);
	// Setting AVEE Voltage
	SPI_RM68180_Write(0xB100,0x0D); //AVEE=-6V
	SPI_RM68180_Write(0xB101,0x0D);
	SPI_RM68180_Write(0xB102,0x0D);
	// Setting VGH Voltag
	SPI_RM68180_Write(0xB300,0x0F);
	SPI_RM68180_Write(0xB301,0x0F);
	SPI_RM68180_Write(0xB302,0x0F);
	// Setting VGL_REG Voltag
	SPI_RM68180_Write(0xB500,0x08);
	SPI_RM68180_Write(0xB501,0x08);
	SPI_RM68180_Write(0xB502,0x08);
	SPI_RM68180_Write(0xB600,0x34);
	SPI_RM68180_Write(0xB601,0x34);
	SPI_RM68180_Write(0xB602,0x34);
	// AVEE Ratio setting
	SPI_RM68180_Write(0xB700,0x34);
	SPI_RM68180_Write(0xB701,0x34);
	SPI_RM68180_Write(0xB702,0x34);
	// Power Control for VCL
	SPI_RM68180_Write(0xB800,0x24); //VCL=-1 x VCI
	SPI_RM68180_Write(0xB801,0x24);
	SPI_RM68180_Write(0xB802,0x24);
	// Setting VGH Ratio
	SPI_RM68180_Write(0xB900,0x34);
	SPI_RM68180_Write(0xB901,0x34);
	SPI_RM68180_Write(0xB902,0x34);
	// VGLX Ratio
	SPI_RM68180_Write(0xBA00,0x24);
	SPI_RM68180_Write(0xBA01,0x24);
	SPI_RM68180_Write(0xBA02,0x24);
	// Setting VGMP and VGSP Voltag
	SPI_RM68180_Write(0xBC00,0x00);
	SPI_RM68180_Write(0xBC01,0x78); //VGMP=5.3V
	SPI_RM68180_Write(0xBC02,0x13);
	// Setting VGMN and VGSN Voltage
	SPI_RM68180_Write(0xBD00,0x00);
	SPI_RM68180_Write(0xBD01,0x78); //VGMN=-5.3V
	SPI_RM68180_Write(0xBD02,0x13);
	// Setting VCOM Offset Voltage
	SPI_RM68180_Write(0xBE00,0x00);
	SPI_RM68180_Write(0xBE01,0x5F); //VCOM=-1.38V
	// Control VGH booster voltage rang
	SPI_RM68180_Write(0xBF00,0x01);
	//Page 1 setting for Gamma setting
	SPI_RM68180_Write(0xF000,0x55);
	SPI_RM68180_Write(0xF001,0xAA);
	SPI_RM68180_Write(0xF002,0x52);
	SPI_RM68180_Write(0xF003,0x08);
	SPI_RM68180_Write(0xF004,0x01);
	//Gamma Setting
	SPI_RM68180_Write(0xD100,0x00);
	SPI_RM68180_Write(0xD101,0x00);
	SPI_RM68180_Write(0xD102,0x00);
	SPI_RM68180_Write(0xD103,0x02);
	SPI_RM68180_Write(0xD104,0x00);
	SPI_RM68180_Write(0xD105,0x14);
	SPI_RM68180_Write(0xD106,0x00);
	SPI_RM68180_Write(0xD107,0x31);
	SPI_RM68180_Write(0xD108,0x00);
	SPI_RM68180_Write(0xD109,0x51);
	SPI_RM68180_Write(0xD10A,0x00);
	SPI_RM68180_Write(0xD10B,0x8C);
	SPI_RM68180_Write(0xD10C,0x00);
	SPI_RM68180_Write(0xD10D,0xBC);
	SPI_RM68180_Write(0xD10E,0x01);
	SPI_RM68180_Write(0xD10F,0x01);
	SPI_RM68180_Write(0xD110,0x01);
	SPI_RM68180_Write(0xD111,0x34);
	SPI_RM68180_Write(0xD112,0x01);
	SPI_RM68180_Write(0xD113,0x7C);
	SPI_RM68180_Write(0xD114,0x01);
	SPI_RM68180_Write(0xD115,0xAE);
	SPI_RM68180_Write(0xD116,0x01);
	SPI_RM68180_Write(0xD117,0xF9);
	SPI_RM68180_Write(0xD118,0x02);
	SPI_RM68180_Write(0xD119,0x32);
	SPI_RM68180_Write(0xD11A,0x02);
	SPI_RM68180_Write(0xD11B,0x33);
	SPI_RM68180_Write(0xD11C,0x02);
	SPI_RM68180_Write(0xD11D,0x65);
	SPI_RM68180_Write(0xD11E,0x02);
	SPI_RM68180_Write(0xD11F,0x96);
	SPI_RM68180_Write(0xD120,0x02);
	SPI_RM68180_Write(0xD121,0xB2);
	SPI_RM68180_Write(0xD122,0x02);
	SPI_RM68180_Write(0xD123,0xD3);
	SPI_RM68180_Write(0xD124,0x02);
	SPI_RM68180_Write(0xD125,0xE7);
	SPI_RM68180_Write(0xD126,0x03);
	SPI_RM68180_Write(0xD127,0x01);
	SPI_RM68180_Write(0xD128,0x03);
	SPI_RM68180_Write(0xD129,0x11);
	SPI_RM68180_Write(0xD12A,0x03);
	SPI_RM68180_Write(0xD12B,0x27);
	SPI_RM68180_Write(0xD12C,0x03);
	SPI_RM68180_Write(0xD12D,0x36);
	SPI_RM68180_Write(0xD12E,0x03);
	SPI_RM68180_Write(0xD12F,0x4D);
	SPI_RM68180_Write(0xD130,0x03);
	SPI_RM68180_Write(0xD131,0x7D);
	SPI_RM68180_Write(0xD132,0x03);
	SPI_RM68180_Write(0xD133,0xFF);
	SPI_RM68180_Write(0xD200,0x00);
	SPI_RM68180_Write(0xD201,0x00);
	SPI_RM68180_Write(0xD202,0x00);
	SPI_RM68180_Write(0xD203,0x02);
	SPI_RM68180_Write(0xD204,0x00);
	SPI_RM68180_Write(0xD205,0x14);
	SPI_RM68180_Write(0xD206,0x00);
	SPI_RM68180_Write(0xD207,0x31);
	SPI_RM68180_Write(0xD208,0x00);
	SPI_RM68180_Write(0xD209,0x51);
	SPI_RM68180_Write(0xD20A,0x00);
	SPI_RM68180_Write(0xD20B,0x8C);
	SPI_RM68180_Write(0xD20C,0x00);
	SPI_RM68180_Write(0xD20D,0xBC);
	SPI_RM68180_Write(0xD20E,0x01);
	SPI_RM68180_Write(0xD20F,0x01);
	SPI_RM68180_Write(0xD210,0x01);
	SPI_RM68180_Write(0xD211,0x34);
	SPI_RM68180_Write(0xD212,0x01);
	SPI_RM68180_Write(0xD213,0x7C);
	SPI_RM68180_Write(0xD214,0x01);
	SPI_RM68180_Write(0xD215,0xAE);
	SPI_RM68180_Write(0xD216,0x01);
	SPI_RM68180_Write(0xD217,0xF9);
	SPI_RM68180_Write(0xD218,0x02);
	SPI_RM68180_Write(0xD219,0x32);
	SPI_RM68180_Write(0xD21A,0x02);
	SPI_RM68180_Write(0xD21B,0x33);
	SPI_RM68180_Write(0xD21C,0x02);
	SPI_RM68180_Write(0xD21D,0x65);
	SPI_RM68180_Write(0xD21E,0x02);
	SPI_RM68180_Write(0xD21F,0x96);
	SPI_RM68180_Write(0xD220,0x02);
	SPI_RM68180_Write(0xD221,0xB2);
	SPI_RM68180_Write(0xD222,0x02);
	SPI_RM68180_Write(0xD223,0xD3);
	SPI_RM68180_Write(0xD224,0x02);
	SPI_RM68180_Write(0xD225,0xE7);
	SPI_RM68180_Write(0xD226,0x03);
	SPI_RM68180_Write(0xD227,0x01);
	SPI_RM68180_Write(0xD228,0x03);
	SPI_RM68180_Write(0xD229,0x11);
	SPI_RM68180_Write(0xD22A,0x03);
	SPI_RM68180_Write(0xD22B,0x27);
	SPI_RM68180_Write(0xD22C,0x03);
	SPI_RM68180_Write(0xD22D,0x36);
	SPI_RM68180_Write(0xD22E,0x03);
	SPI_RM68180_Write(0xD22F,0x4D);
	SPI_RM68180_Write(0xD230,0x03);
	SPI_RM68180_Write(0xD231,0x7D);
	SPI_RM68180_Write(0xD232,0x03);
	SPI_RM68180_Write(0xD233,0xFF);
	SPI_RM68180_Write(0xD300,0x00);
	SPI_RM68180_Write(0xD301,0x00);
	SPI_RM68180_Write(0xD302,0x00);
	SPI_RM68180_Write(0xD303,0x02);
	SPI_RM68180_Write(0xD304,0x00);
	SPI_RM68180_Write(0xD305,0x14);
	SPI_RM68180_Write(0xD306,0x00);
	SPI_RM68180_Write(0xD307,0x31);
	SPI_RM68180_Write(0xD308,0x00);
	SPI_RM68180_Write(0xD309,0x51);
	SPI_RM68180_Write(0xD30A,0x00);
	SPI_RM68180_Write(0xD30B,0x8C);
	SPI_RM68180_Write(0xD30C,0x00);
	SPI_RM68180_Write(0xD30D,0xBC);
	SPI_RM68180_Write(0xD30E,0x01);
	SPI_RM68180_Write(0xD30F,0x01);
	SPI_RM68180_Write(0xD310,0x01);
	SPI_RM68180_Write(0xD311,0x34);
	SPI_RM68180_Write(0xD312,0x01);
	SPI_RM68180_Write(0xD313,0x7C);
	SPI_RM68180_Write(0xD314,0x01);
	SPI_RM68180_Write(0xD315,0xAE);
	SPI_RM68180_Write(0xD316,0x01);
	SPI_RM68180_Write(0xD317,0xF9);
	SPI_RM68180_Write(0xD318,0x02);
	SPI_RM68180_Write(0xD319,0x32);
	SPI_RM68180_Write(0xD31A,0x02);
	SPI_RM68180_Write(0xD31B,0x33);
	SPI_RM68180_Write(0xD31C,0x02);
	SPI_RM68180_Write(0xD31D,0x65);
	SPI_RM68180_Write(0xD31E,0x02);
	SPI_RM68180_Write(0xD31F,0x96);
	SPI_RM68180_Write(0xD320,0x02);
	SPI_RM68180_Write(0xD321,0xB2);
	SPI_RM68180_Write(0xD322,0x02);
	SPI_RM68180_Write(0xD323,0xD3);
	SPI_RM68180_Write(0xD324,0x02);
	SPI_RM68180_Write(0xD325,0xE7);
	SPI_RM68180_Write(0xD326,0x03);
	SPI_RM68180_Write(0xD327,0x01);
	SPI_RM68180_Write(0xD328,0x03);
	SPI_RM68180_Write(0xD329,0x11);
	SPI_RM68180_Write(0xD32A,0x03);
	SPI_RM68180_Write(0xD32B,0x27);
	SPI_RM68180_Write(0xD32C,0x03);
	SPI_RM68180_Write(0xD32D,0x36);
	SPI_RM68180_Write(0xD32E,0x03);
	SPI_RM68180_Write(0xD32F,0x4D);
	SPI_RM68180_Write(0xD330,0x03);
	SPI_RM68180_Write(0xD331,0x7D);
	SPI_RM68180_Write(0xD332,0x03);
	SPI_RM68180_Write(0xD333,0xFF);
	SPI_RM68180_Write(0xD400,0x00);
	SPI_RM68180_Write(0xD401,0x00);
	SPI_RM68180_Write(0xD402,0x00);
	SPI_RM68180_Write(0xD403,0x02);
	SPI_RM68180_Write(0xD404,0x00);
	SPI_RM68180_Write(0xD405,0x14);
	SPI_RM68180_Write(0xD406,0x00);
	SPI_RM68180_Write(0xD407,0x31);
	SPI_RM68180_Write(0xD408,0x00);
	SPI_RM68180_Write(0xD409,0x51);
	SPI_RM68180_Write(0xD40A,0x00);
	SPI_RM68180_Write(0xD40B,0x8C);
	SPI_RM68180_Write(0xD40C,0x00);
	SPI_RM68180_Write(0xD40D,0xBC);
	SPI_RM68180_Write(0xD40E,0x01);
	SPI_RM68180_Write(0xD40F,0x01);
	SPI_RM68180_Write(0xD410,0x01);
	SPI_RM68180_Write(0xD411,0x34);
	SPI_RM68180_Write(0xD412,0x01);
	SPI_RM68180_Write(0xD413,0x7C);
	SPI_RM68180_Write(0xD414,0x01);
	SPI_RM68180_Write(0xD415,0xAE);
	SPI_RM68180_Write(0xD416,0x01);
	SPI_RM68180_Write(0xD417,0xF9);
	SPI_RM68180_Write(0xD418,0x02);
	SPI_RM68180_Write(0xD419,0x32);
	SPI_RM68180_Write(0xD41A,0x02);
	SPI_RM68180_Write(0xD41B,0x33);
	SPI_RM68180_Write(0xD41C,0x02);
	SPI_RM68180_Write(0xD41D,0x65);
	SPI_RM68180_Write(0xD41E,0x02);
	SPI_RM68180_Write(0xD41F,0x96);
	SPI_RM68180_Write(0xD420,0x02);
	SPI_RM68180_Write(0xD421,0xB2);
	SPI_RM68180_Write(0xD422,0x02);
	SPI_RM68180_Write(0xD423,0xD3);
	SPI_RM68180_Write(0xD424,0x02);
	SPI_RM68180_Write(0xD425,0xE7);
	SPI_RM68180_Write(0xD426,0x03);
	SPI_RM68180_Write(0xD427,0x01);
	SPI_RM68180_Write(0xD428,0x03);
	SPI_RM68180_Write(0xD429,0x11);
	SPI_RM68180_Write(0xD42A,0x03);
	SPI_RM68180_Write(0xD42B,0x27);
	SPI_RM68180_Write(0xD42C,0x03);
	SPI_RM68180_Write(0xD42D,0x36);
	SPI_RM68180_Write(0xD42E,0x03);
	SPI_RM68180_Write(0xD42F,0x4D);
	SPI_RM68180_Write(0xD430,0x03);
	SPI_RM68180_Write(0xD431,0x7D);
	SPI_RM68180_Write(0xD432,0x03);
	SPI_RM68180_Write(0xD433,0xFF);
	SPI_RM68180_Write(0xD500,0x00);
	SPI_RM68180_Write(0xD501,0x00);
	SPI_RM68180_Write(0xD502,0x00);
	SPI_RM68180_Write(0xD503,0x02);
	SPI_RM68180_Write(0xD504,0x00);
	SPI_RM68180_Write(0xD505,0x14);
	SPI_RM68180_Write(0xD506,0x00);
	SPI_RM68180_Write(0xD507,0x31);
	SPI_RM68180_Write(0xD508,0x00);
	SPI_RM68180_Write(0xD509,0x51);
	SPI_RM68180_Write(0xD50A,0x00);
	SPI_RM68180_Write(0xD50B,0x8C);
	SPI_RM68180_Write(0xD50C,0x00);
	SPI_RM68180_Write(0xD50D,0xBC);
	SPI_RM68180_Write(0xD50E,0x01);
	SPI_RM68180_Write(0xD50F,0x01);
	SPI_RM68180_Write(0xD510,0x01);
	SPI_RM68180_Write(0xD511,0x34);
	SPI_RM68180_Write(0xD512,0x01);
	SPI_RM68180_Write(0xD513,0x7C);
	SPI_RM68180_Write(0xD514,0x01);
	SPI_RM68180_Write(0xD515,0xAE);
	SPI_RM68180_Write(0xD516,0x01);
	SPI_RM68180_Write(0xD517,0xF9);
	SPI_RM68180_Write(0xD518,0x02);
	SPI_RM68180_Write(0xD519,0x32);
	SPI_RM68180_Write(0xD51A,0x02);
	SPI_RM68180_Write(0xD51B,0x33);
	SPI_RM68180_Write(0xD51C,0x02);
	SPI_RM68180_Write(0xD51D,0x65);
	SPI_RM68180_Write(0xD51E,0x02);
	SPI_RM68180_Write(0xD51F,0x96);
	SPI_RM68180_Write(0xD520,0x02);
	SPI_RM68180_Write(0xD521,0xB2);
	SPI_RM68180_Write(0xD522,0x02);
	SPI_RM68180_Write(0xD523,0xD3);
	SPI_RM68180_Write(0xD524,0x02);
	SPI_RM68180_Write(0xD525,0xE7);
	SPI_RM68180_Write(0xD526,0x03);
	SPI_RM68180_Write(0xD527,0x01);
	SPI_RM68180_Write(0xD528,0x03);
	SPI_RM68180_Write(0xD529,0x11);
	SPI_RM68180_Write(0xD52A,0x03);
	SPI_RM68180_Write(0xD52B,0x27);
	SPI_RM68180_Write(0xD52C,0x03);
	SPI_RM68180_Write(0xD52D,0x36);
	SPI_RM68180_Write(0xD52E,0x03);
	SPI_RM68180_Write(0xD52F,0x4D);
	SPI_RM68180_Write(0xD530,0x03);
	SPI_RM68180_Write(0xD531,0x7D);
	SPI_RM68180_Write(0xD532,0x03);
	SPI_RM68180_Write(0xD533,0xFF);
	SPI_RM68180_Write(0xD600,0x00);
	SPI_RM68180_Write(0xD601,0x00);
	SPI_RM68180_Write(0xD602,0x00);
	SPI_RM68180_Write(0xD603,0x02);
	SPI_RM68180_Write(0xD604,0x00);
	SPI_RM68180_Write(0xD605,0x14);
	SPI_RM68180_Write(0xD606,0x00);
	SPI_RM68180_Write(0xD607,0x31);
	SPI_RM68180_Write(0xD608,0x00);
	SPI_RM68180_Write(0xD609,0x51);
	SPI_RM68180_Write(0xD60A,0x00);
	SPI_RM68180_Write(0xD60B,0x8C);
	SPI_RM68180_Write(0xD60C,0x00);
	SPI_RM68180_Write(0xD60D,0xBC);
	SPI_RM68180_Write(0xD60E,0x01);
	SPI_RM68180_Write(0xD60F,0x01);
	SPI_RM68180_Write(0xD610,0x01);
	SPI_RM68180_Write(0xD611,0x34);
	SPI_RM68180_Write(0xD612,0x01);
	SPI_RM68180_Write(0xD613,0x7C);
	SPI_RM68180_Write(0xD614,0x01);
	SPI_RM68180_Write(0xD615,0xAE);
	SPI_RM68180_Write(0xD616,0x01);
	SPI_RM68180_Write(0xD617,0xF9);
	SPI_RM68180_Write(0xD618,0x02);
	SPI_RM68180_Write(0xD619,0x32);
	SPI_RM68180_Write(0xD61A,0x02);
	SPI_RM68180_Write(0xD61B,0x33);
	SPI_RM68180_Write(0xD61C,0x02);
	SPI_RM68180_Write(0xD61D,0x65);
	SPI_RM68180_Write(0xD61E,0x02);
	SPI_RM68180_Write(0xD61F,0x96);
	SPI_RM68180_Write(0xD620,0x02);
	SPI_RM68180_Write(0xD621,0xB2);
	SPI_RM68180_Write(0xD622,0x02);
	SPI_RM68180_Write(0xD623,0xD3);
	SPI_RM68180_Write(0xD624,0x02);
	SPI_RM68180_Write(0xD625,0xE7);
	SPI_RM68180_Write(0xD626,0x03);
	SPI_RM68180_Write(0xD627,0x01);
	SPI_RM68180_Write(0xD628,0x03);
	SPI_RM68180_Write(0xD629,0x11);
	SPI_RM68180_Write(0xD62A,0x03);
	SPI_RM68180_Write(0xD62B,0x27);
	SPI_RM68180_Write(0xD62C,0x03);
	SPI_RM68180_Write(0xD62D,0x36);
	SPI_RM68180_Write(0xD62E,0x03);
	SPI_RM68180_Write(0xD62F,0x4D);
	SPI_RM68180_Write(0xD630,0x03);
	SPI_RM68180_Write(0xD631,0x7D);
	SPI_RM68180_Write(0xD632,0x03);
	SPI_RM68180_Write(0xD633,0xFF);
	/** Select Command Page '0' **/
	SPI_RM68180_Write(0xF000,0x55);
	SPI_RM68180_Write(0xF001,0xAA);
	SPI_RM68180_Write(0xF002,0x52);
	SPI_RM68180_Write(0xF003,0x08);
	SPI_RM68180_Write(0xF004,0x00);
	SPI_RM68180_Write(0xB500,0x6B); //480*854
	// Display Option Control
	SPI_RM68180_Write(0xB100,0xCC);
	// Vivid Color
	SPI_RM68180_Write(0xB400,0x10);
	// EQ Control Function for Source Driver
	SPI_RM68180_Write(0xB800,0x00);
	SPI_RM68180_Write(0xB801,0x02);
	SPI_RM68180_Write(0xB802,0x02);
	SPI_RM68180_Write(0xB803,0x02);
	// Inversion Driver Control
	SPI_RM68180_Write(0xBC00,0x02);
	SPI_RM68180_Write(0xBC01,0x02);
	SPI_RM68180_Write(0xBC02,0x02);
	SPI_RM68180_Write(0x3500,0x00);//TE Enable
	
	SPI_RM68180_Write(0x4400,0x01);
	SPI_RM68180_Write(0x4401,0xDE);
	
	SPI_RM68180_Write(0x3A00,0x66);//Color Depth
	SPI_RM68180_Write(0x1100,0x00);//Sleep out
	LCD_DelayMS(120);//delay 120ms
	SPI_RM68180_Write(0x2C00,0x00);//Write frame memory
	
	SPI_RM68180_Write(0x2900,0x00);//Display on
	LCD_DelayMS(100);//delay 100ms
	}


	printf("qiandan init end!.....\n");

	LCD_DelayMS(100);	

	return 0;
}

static int32_t rm68180_set_window(struct lcd_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("qiandan rm68180_set_window: %d, %d, %d, %d\n",left, top, right, bottom);
	
	send_cmd(0x2A00); // col
	send_data((left >> 8));
	send_cmd(0x2A01); // col	
	send_data((left & 0xFF));
	send_cmd(0x2A02); // col	
	send_data((right >> 8));
	send_cmd(0x2A03); // col	
	send_data((right & 0xFF));

	send_cmd(0x2B00); // row
	send_data((top >> 8));
	send_cmd(0x2B01); // row	
	send_data((top & 0xFF));
	send_cmd(0x2B02); // row		
	send_data((bottom >> 8));
	send_cmd(0x2B03); // row		
	send_data((bottom & 0xFF));

	send_cmd(0x2C00); //Write data	

	return 0;
}

static int32_t rm68180_invalidate(struct lcd_spec *self)
{
	LCD_PRINT("rm68180_invalidate\n");

	return self->ops->lcd_set_window(self, 0, 0, 
			self->width - 1, self->height - 1);
	
}

static int32_t rm68180_invalidate_rect(struct lcd_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
	LCD_PRINT("rm68180_invalidate_rect \n");
	// TE scaneline
	//send_cmd_data(0x000b, (top >> 8));
	//send_cmd_data(0x000c, (top & 0xff));
#if 0
	send_cmd(0x44); // TE scanline
	send_data((top >> 8));
	send_data((top & 0xFF));	
#endif
	return self->ops->lcd_set_window(self, left, top, 
			right, bottom);
}

static int32_t rm68180_set_direction(struct lcd_spec *self, uint16_t direction)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("qiandan hx8369_set_direction\n");

	switch (direction) {
	case LCD_DIRECT_NORMAL:
		break;
	case LCD_DIRECT_ROT_90:
		break;
	case LCD_DIRECT_ROT_180:
		break;
	case LCD_DIRECT_ROT_270:
		break;
	case LCD_DIRECT_MIR_H:
		break;
	case LCD_DIRECT_MIR_V:
		break;
	case LCD_DIRECT_MIR_HV:
		break;
	default:
		LCD_PRINT("unknown lcd direction!\n");
		direction = LCD_DIRECT_NORMAL;
		break;
	}

	self->direction = direction;
	
	return 0;
}

static int32_t rm68180_enter_sleep(struct lcd_spec *self, uint8_t is_sleep)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
//printk("cj rm68180_enter_sleep = %d \n",is_sleep);

	if(is_sleep) {
		//Sleep In
		send_cmd(0x2800);
		LCD_DelayMS(120); 
		send_cmd(0x1000);
		LCD_DelayMS(120); 
	}
	else {
		//Sleep Out
		send_cmd(0x1100);
		LCD_DelayMS(200); 
		send_cmd(0x2900);
		LCD_DelayMS(120); 
	}

	return 0;
}

static int32_t rm68180_read_id(struct lcd_spec *self)
{

	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
	Read_data read_data = self->info.mcu->ops->read_data;

	uint32_t dummy1,dummy2,dummy3,dummy4;
	send_cmd(0xF000);
	send_data(0x55);
	send_cmd(0xF001);
	send_data(0xAA);
	send_cmd(0xF002);
	send_data(0x52);
	send_cmd(0xF003);
	send_data(0x08);
	send_cmd(0xF004);
	send_data(0x01);

	send_cmd(0xc500);
	dummy3=read_data();
	send_cmd(0xc501);
	dummy4=read_data();

	printf("%s dummy3:0x%x,dummy4:0x%x, id=0x%x\n",__func__, dummy3, dummy4, (dummy3<<8)|dummy4);

	return (dummy3<<8)|dummy4;

}

static struct lcd_operations lcd_rm68180_operations = {
	.lcd_init = rm68180_init,
	.lcd_set_window = rm68180_set_window,
	.lcd_invalidate_rect= rm68180_invalidate_rect,
	.lcd_invalidate = rm68180_invalidate,
	.lcd_set_direction = rm68180_set_direction,
	.lcd_enter_sleep = rm68180_enter_sleep,
	.lcd_readid = rm68180_read_id,
};

static struct timing_mcu lcd_rm68180_timing[] = {
	[LCD_REGISTER_TIMING] = {					 // read/write register timing
			.rcss = 25,//15,  // 15ns
			.rlpw = 90,//60,90	70
			.rhpw = 90,//60,90	70
			.wcss = 20,//20 20	10
			.wlpw = 70,//35,70	 50
			.whpw = 70,//35,70	 50
		},
	[LCD_GRAM_TIMING] = {					 // read/write gram timing
			.rcss = 25,//15,  // 15ns
			.rlpw = 90,//60,90	70
			.rhpw = 90,//60,90	70
			.wcss = 5,//20
			.wlpw = 20,//35,70	15
			.whpw = 20,//35,70	24

	},
};

static struct info_mcu lcd_rm68180_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 18,
	.timing = lcd_rm68180_timing,
	.ops = NULL,
};

struct lcd_spec lcd_panel_rm68180 = {
	.width = 480,
	.height = 854,
	.mode = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_rm68180_info},
	.ops = &lcd_rm68180_operations,
};
