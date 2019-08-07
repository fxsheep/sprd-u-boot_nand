/* drivers/video/sc8800g/sc8800g_lcd_cd35510.c
 *
 * Support for CD35510 LCD device
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

#include <asm/arch/mfp.h>
#include <asm/arch/asm_generic_gpio.h>
#include <asm/arch/sdram_cfg.h>
#define REG32(x)   (*((volatile uint32 *)(x)))

static unsigned long flash_id_func_cfg[] = {
		MFP_CFG_X(TRACECTRL, AF3, DS1, F_PULL_NONE,   S_PULL_NONE,   IO_IE),//gpio42
};

static int32_t cd35510_init(struct lcd_spec *self)
{
	u32 test1,test2;
	u32 ret;

	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("cd35510_init\n");

	self->ops->lcd_reset(self);
	LCD_DelayMS(120);


	sprd_mfp_config(flash_id_func_cfg, ARRAY_SIZE(flash_id_func_cfg));
	LCD_DelayMS(20);

	if (sprd_gpio_request(NULL, 42)) {
		printf("Failed ro request LCD_ID GPIO_%d \n",	42);
	}
	sprd_gpio_direction_input(NULL, 42);
	ret = sprd_gpio_get(NULL, 42);

	printf("cd35510_init gpio 42 = %d\n", ret);

if(!ret){ //hengkaihui
	printf("lcd init - hkh\n");

// for NT35510 +	CD3.97'  mipi
  send_cmd(0xF000); send_data(0x55); 
  send_cmd(0xF001); send_data(0xAA);
  send_cmd(0xF002); send_data(0x52);
  send_cmd(0xF003); send_data(0x08);
  send_cmd(0xF004); send_data(0x01);

  send_cmd(0xB000); send_data(0x0A); //
  send_cmd(0xB001); send_data(0x0A);
  send_cmd(0xB002); send_data(0x0A);

  send_cmd(0xB600); send_data(0x34); //
  send_cmd(0xB601); send_data(0x34);
  send_cmd(0xB602); send_data(0x34);

  send_cmd(0xB100); send_data(0x0A); // Setting VGL_REG Voltage
  send_cmd(0xB101); send_data(0x0A);
  send_cmd(0xB102); send_data(0x0A);

  send_cmd(0xB700); send_data(0x24); // Setting VGL_REG Voltage
  send_cmd(0xB701); send_data(0x24);
  send_cmd(0xB702); send_data(0x24);

  send_cmd(0xB200); send_data(0x00); // Setting VGL_REG Voltage
  send_cmd(0xB201); send_data(0x00);
  send_cmd(0xB202); send_data(0x00);

  send_cmd(0xB800); send_data(0x24); // Setting VGL_REG Voltage
  send_cmd(0xB801); send_data(0x24);
  send_cmd(0xB802); send_data(0x24);

  send_cmd(0xBF00); send_data(0x01); // VGH Output Voltage

  send_cmd(0xB300); send_data(0x08); // Setting VGL_REG Voltage
  send_cmd(0xB301); send_data(0x08);
  send_cmd(0xB302); send_data(0x08);

  send_cmd(0xB900); send_data(0x34); // Setting VGL_REG Voltage
  send_cmd(0xB901); send_data(0x34);
  send_cmd(0xB902); send_data(0x34);

  send_cmd(0xBA00); send_data(0x24); // Setting VGL_REG Voltage
  send_cmd(0xBA01); send_data(0x24);
  send_cmd(0xBA02); send_data(0x24);

  send_cmd(0xBC00); send_data(0x00); // Setting VGL_REG Voltage
  send_cmd(0xBC01); send_data(0xA3);
  send_cmd(0xBC02); send_data(0x00);

  send_cmd(0xBD00); send_data(0x00); // Setting VGL_REG Voltage
  send_cmd(0xBD01); send_data(0xA3);
  send_cmd(0xBD02); send_data(0x00);

  send_cmd(0xBE00); send_data(0x00); // Setting VGL_REG Voltage
  send_cmd(0xBE01); send_data(0x70); //vcom  7f




 
//Positive Gamma for RED
  send_cmd(0xD100); send_data(0x00);
  send_cmd(0xD101); send_data(0x84);
  send_cmd(0xD102); send_data(0x00);
  send_cmd(0xD103); send_data(0x87);
  send_cmd(0xD104); send_data(0x00);
  send_cmd(0xD105); send_data(0x9F);
  send_cmd(0xD106); send_data(0x00);
  send_cmd(0xD107); send_data(0xbB);
  send_cmd(0xD108); send_data(0x00);
  send_cmd(0xD109); send_data(0xDA);
  send_cmd(0xD10A); send_data(0x01);
  send_cmd(0xD10B); send_data(0x0A);
  send_cmd(0xD10C); send_data(0x01);
  send_cmd(0xD10D); send_data(0x2E);
  send_cmd(0xD10E); send_data(0x01);
  send_cmd(0xD10F); send_data(0x61);
  send_cmd(0xD110); send_data(0x01);
  send_cmd(0xD111); send_data(0x86);
  send_cmd(0xD112); send_data(0x01);
  send_cmd(0xD113); send_data(0xBB);
  send_cmd(0xD114); send_data(0x01);
  send_cmd(0xD115); send_data(0xe2);
  send_cmd(0xD116); send_data(0x02);
  send_cmd(0xD117); send_data(0x19);
  send_cmd(0xD118); send_data(0x02);
  send_cmd(0xD119); send_data(0x43);
  send_cmd(0xD11A); send_data(0x02);
  send_cmd(0xD11B); send_data(0x44);
  send_cmd(0xD11C); send_data(0x02);
  send_cmd(0xD11D); send_data(0x69);
  send_cmd(0xD11E); send_data(0x02);
  send_cmd(0xD11F); send_data(0x8D);
  send_cmd(0xD120); send_data(0x02);
  send_cmd(0xD121); send_data(0xA1);
  send_cmd(0xD122); send_data(0x02);
  send_cmd(0xD123); send_data(0xB9);
  send_cmd(0xD124); send_data(0x02);
  send_cmd(0xD125); send_data(0xC7);
  send_cmd(0xD126); send_data(0x02);
  send_cmd(0xD127); send_data(0xDA);
  send_cmd(0xD128); send_data(0x02);
  send_cmd(0xD129); send_data(0xE7);
  send_cmd(0xD12A); send_data(0x02);
  send_cmd(0xD12B); send_data(0xF9);
  send_cmd(0xD12C); send_data(0x03);
  send_cmd(0xD12D); send_data(0x05);
  send_cmd(0xD12E); send_data(0x03);
  send_cmd(0xD12F); send_data(0x19);
  send_cmd(0xD130); send_data(0x03);
  send_cmd(0xD131); send_data(0x50);
  send_cmd(0xD132); send_data(0x03);
  send_cmd(0xD133); send_data(0xDB);

//Positive Gamma for GREEN
  send_cmd(0xD200); send_data(0x00);
  send_cmd(0xD201); send_data(0x84);
  send_cmd(0xD202); send_data(0x00);
  send_cmd(0xD203); send_data(0x87);
  send_cmd(0xD204); send_data(0x00);
  send_cmd(0xD205); send_data(0x9F);
  send_cmd(0xD206); send_data(0x00);
  send_cmd(0xD207); send_data(0xbB);
  send_cmd(0xD208); send_data(0x00);
  send_cmd(0xD209); send_data(0xDA);
  send_cmd(0xD20A); send_data(0x01);
  send_cmd(0xD20B); send_data(0x0A);
  send_cmd(0xD20C); send_data(0x01);
  send_cmd(0xD20D); send_data(0x2E);
  send_cmd(0xD20E); send_data(0x01);
  send_cmd(0xD20F); send_data(0x61);
  send_cmd(0xD210); send_data(0x01);
  send_cmd(0xD211); send_data(0x86);
  send_cmd(0xD212); send_data(0x01);
  send_cmd(0xD213); send_data(0xBB);
  send_cmd(0xD214); send_data(0x01);
  send_cmd(0xD215); send_data(0xe2);
  send_cmd(0xD216); send_data(0x02);
  send_cmd(0xD217); send_data(0x19);
  send_cmd(0xD218); send_data(0x02);
  send_cmd(0xD219); send_data(0x43);
  send_cmd(0xD21A); send_data(0x02);
  send_cmd(0xD21B); send_data(0x44);
  send_cmd(0xD21C); send_data(0x02);
  send_cmd(0xD21D); send_data(0x69);
  send_cmd(0xD21E); send_data(0x02);
  send_cmd(0xD21F); send_data(0x8D);
  send_cmd(0xD220); send_data(0x02);
  send_cmd(0xD221); send_data(0xA1);
  send_cmd(0xD222); send_data(0x02);
  send_cmd(0xD223); send_data(0xB9);
  send_cmd(0xD224); send_data(0x02);
  send_cmd(0xD225); send_data(0xC7);
  send_cmd(0xD226); send_data(0x02);
  send_cmd(0xD227); send_data(0xDA);
  send_cmd(0xD228); send_data(0x02);
  send_cmd(0xD229); send_data(0xE7);
  send_cmd(0xD22A); send_data(0x02);
  send_cmd(0xD22B); send_data(0xF9);
  send_cmd(0xD22C); send_data(0x03);
  send_cmd(0xD22D); send_data(0x05);
  send_cmd(0xD22E); send_data(0x03);
  send_cmd(0xD22F); send_data(0x19);
  send_cmd(0xD230); send_data(0x03);
  send_cmd(0xD231); send_data(0x50);
  send_cmd(0xD232); send_data(0x03);
  send_cmd(0xD233); send_data(0xDB);

//Positive Gamma for BLUE
  send_cmd(0xD300); send_data(0x00);
  send_cmd(0xD301); send_data(0x84);
  send_cmd(0xD302); send_data(0x00);
  send_cmd(0xD303); send_data(0x87);
  send_cmd(0xD304); send_data(0x00);
  send_cmd(0xD305); send_data(0x9F);
  send_cmd(0xD306); send_data(0x00);
  send_cmd(0xD307); send_data(0xbB);
  send_cmd(0xD308); send_data(0x00);
  send_cmd(0xD309); send_data(0xDA);
  send_cmd(0xD30A); send_data(0x01);
  send_cmd(0xD30B); send_data(0x0A);
  send_cmd(0xD30C); send_data(0x01);
  send_cmd(0xD30D); send_data(0x2E);
  send_cmd(0xD30E); send_data(0x01);
  send_cmd(0xD30F); send_data(0x61);
  send_cmd(0xD310); send_data(0x01);
  send_cmd(0xD311); send_data(0x86);
  send_cmd(0xD312); send_data(0x01);
  send_cmd(0xD313); send_data(0xBB);
  send_cmd(0xD314); send_data(0x01);
  send_cmd(0xD315); send_data(0xe2);
  send_cmd(0xD316); send_data(0x02);
  send_cmd(0xD317); send_data(0x19);
  send_cmd(0xD318); send_data(0x02);
  send_cmd(0xD319); send_data(0x43);
  send_cmd(0xD31A); send_data(0x02);
  send_cmd(0xD31B); send_data(0x44);
  send_cmd(0xD31C); send_data(0x02);
  send_cmd(0xD31D); send_data(0x69);
  send_cmd(0xD31E); send_data(0x02);
  send_cmd(0xD31F); send_data(0x8D);
  send_cmd(0xD320); send_data(0x02);
  send_cmd(0xD321); send_data(0xA1);
  send_cmd(0xD322); send_data(0x02);
  send_cmd(0xD323); send_data(0xB9);
  send_cmd(0xD324); send_data(0x02);
  send_cmd(0xD325); send_data(0xC7);
  send_cmd(0xD326); send_data(0x02);
  send_cmd(0xD327); send_data(0xDA);
  send_cmd(0xD328); send_data(0x02);
  send_cmd(0xD329); send_data(0xE7);
  send_cmd(0xD32A); send_data(0x02);
  send_cmd(0xD32B); send_data(0xF9);
  send_cmd(0xD32C); send_data(0x03);
  send_cmd(0xD32D); send_data(0x05);
  send_cmd(0xD32E); send_data(0x03);
  send_cmd(0xD32F); send_data(0x19);
  send_cmd(0xD330); send_data(0x03);
  send_cmd(0xD331); send_data(0x50);
  send_cmd(0xD332); send_data(0x03);
  send_cmd(0xD333); send_data(0xDB);



//Negative Gamma for RED
  send_cmd(0xD400); send_data(0x00);
  send_cmd(0xD401); send_data(0x84);
  send_cmd(0xD402); send_data(0x00);
  send_cmd(0xD403); send_data(0x87);
  send_cmd(0xD404); send_data(0x00);
  send_cmd(0xD405); send_data(0x9F);
  send_cmd(0xD406); send_data(0x00);
  send_cmd(0xD407); send_data(0xbB);
  send_cmd(0xD408); send_data(0x00);
  send_cmd(0xD409); send_data(0xDA);
  send_cmd(0xD40A); send_data(0x01);
  send_cmd(0xD40B); send_data(0x0A);
  send_cmd(0xD40C); send_data(0x01);
  send_cmd(0xD40D); send_data(0x2E);
  send_cmd(0xD40E); send_data(0x01);
  send_cmd(0xD40F); send_data(0x61);
  send_cmd(0xD410); send_data(0x01);
  send_cmd(0xD411); send_data(0x86);
  send_cmd(0xD412); send_data(0x01);
  send_cmd(0xD413); send_data(0xBB);
  send_cmd(0xD414); send_data(0x01);
  send_cmd(0xD415); send_data(0xe2);
  send_cmd(0xD416); send_data(0x02);
  send_cmd(0xD417); send_data(0x19);
  send_cmd(0xD418); send_data(0x02);
  send_cmd(0xD419); send_data(0x43);
  send_cmd(0xD41A); send_data(0x02);
  send_cmd(0xD41B); send_data(0x44);
  send_cmd(0xD41C); send_data(0x02);
  send_cmd(0xD41D); send_data(0x69);
  send_cmd(0xD41E); send_data(0x02);
  send_cmd(0xD41F); send_data(0x8D);
  send_cmd(0xD420); send_data(0x02);
  send_cmd(0xD421); send_data(0xA1);
  send_cmd(0xD422); send_data(0x02);
  send_cmd(0xD423); send_data(0xB9);
  send_cmd(0xD424); send_data(0x02);
  send_cmd(0xD425); send_data(0xC7);
  send_cmd(0xD426); send_data(0x02);
  send_cmd(0xD427); send_data(0xDA);
  send_cmd(0xD428); send_data(0x02);
  send_cmd(0xD429); send_data(0xE7);
  send_cmd(0xD42A); send_data(0x02);
  send_cmd(0xD42B); send_data(0xF9);
  send_cmd(0xD42C); send_data(0x03);
  send_cmd(0xD42D); send_data(0x05);
  send_cmd(0xD42E); send_data(0x03);
  send_cmd(0xD42F); send_data(0x19);
  send_cmd(0xD430); send_data(0x03);
  send_cmd(0xD431); send_data(0x50);
  send_cmd(0xD432); send_data(0x03);
  send_cmd(0xD433); send_data(0xDB);



//Negative Gamma for GERREN
  send_cmd(0xD500); send_data(0x00);
  send_cmd(0xD501); send_data(0x84);
  send_cmd(0xD502); send_data(0x00);
  send_cmd(0xD503); send_data(0x87);
  send_cmd(0xD504); send_data(0x00);
  send_cmd(0xD505); send_data(0x9F);
  send_cmd(0xD506); send_data(0x00);
  send_cmd(0xD507); send_data(0xbB);
  send_cmd(0xD508); send_data(0x00);
  send_cmd(0xD509); send_data(0xDA);
  send_cmd(0xD50A); send_data(0x01);
  send_cmd(0xD50B); send_data(0x0A);
  send_cmd(0xD50C); send_data(0x01);
  send_cmd(0xD50D); send_data(0x2E);
  send_cmd(0xD50E); send_data(0x01);
  send_cmd(0xD50F); send_data(0x61);
  send_cmd(0xD510); send_data(0x01);
  send_cmd(0xD511); send_data(0x86);
  send_cmd(0xD512); send_data(0x01);
  send_cmd(0xD513); send_data(0xBB);
  send_cmd(0xD514); send_data(0x01);
  send_cmd(0xD515); send_data(0xe2);
  send_cmd(0xD516); send_data(0x02);
  send_cmd(0xD517); send_data(0x19);
  send_cmd(0xD518); send_data(0x02);
  send_cmd(0xD519); send_data(0x43);
  send_cmd(0xD51A); send_data(0x02);
  send_cmd(0xD51B); send_data(0x44);
  send_cmd(0xD51C); send_data(0x02);
  send_cmd(0xD51D); send_data(0x69);
  send_cmd(0xD51E); send_data(0x02);
  send_cmd(0xD51F); send_data(0x8D);
  send_cmd(0xD520); send_data(0x02);
  send_cmd(0xD521); send_data(0xA1);
  send_cmd(0xD522); send_data(0x02);
  send_cmd(0xD523); send_data(0xB9);
  send_cmd(0xD524); send_data(0x02);
  send_cmd(0xD525); send_data(0xC7);
  send_cmd(0xD526); send_data(0x02);
  send_cmd(0xD527); send_data(0xDA);
  send_cmd(0xD528); send_data(0x02);
  send_cmd(0xD529); send_data(0xE7);
  send_cmd(0xD52A); send_data(0x02);
  send_cmd(0xD52B); send_data(0xF9);
  send_cmd(0xD52C); send_data(0x03);
  send_cmd(0xD52D); send_data(0x05);
  send_cmd(0xD52E); send_data(0x03);
  send_cmd(0xD52F); send_data(0x19);
  send_cmd(0xD530); send_data(0x03);
  send_cmd(0xD531); send_data(0x50);
  send_cmd(0xD532); send_data(0x03);
  send_cmd(0xD533); send_data(0xDB);

//Negative Gamma for BLUE
  send_cmd(0xD600); send_data(0x00);
  send_cmd(0xD601); send_data(0x84);
  send_cmd(0xD602); send_data(0x00);
  send_cmd(0xD603); send_data(0x87);
  send_cmd(0xD604); send_data(0x00);
  send_cmd(0xD605); send_data(0x9F);
  send_cmd(0xD606); send_data(0x00);
  send_cmd(0xD607); send_data(0xbB);
  send_cmd(0xD608); send_data(0x00);
  send_cmd(0xD609); send_data(0xDA);
  send_cmd(0xD60A); send_data(0x01);
  send_cmd(0xD60B); send_data(0x0A);
  send_cmd(0xD60C); send_data(0x01);
  send_cmd(0xD60D); send_data(0x2E);
  send_cmd(0xD60E); send_data(0x01);
  send_cmd(0xD60F); send_data(0x61);
  send_cmd(0xD610); send_data(0x01);
  send_cmd(0xD611); send_data(0x86);
  send_cmd(0xD612); send_data(0x01);
  send_cmd(0xD613); send_data(0xBB);
  send_cmd(0xD614); send_data(0x01);
  send_cmd(0xD615); send_data(0xe2);
  send_cmd(0xD616); send_data(0x02);
  send_cmd(0xD617); send_data(0x19);
  send_cmd(0xD618); send_data(0x02);
  send_cmd(0xD619); send_data(0x43);
  send_cmd(0xD61A); send_data(0x02);
  send_cmd(0xD61B); send_data(0x44);
  send_cmd(0xD61C); send_data(0x02);
  send_cmd(0xD61D); send_data(0x69);
  send_cmd(0xD61E); send_data(0x02);
  send_cmd(0xD61F); send_data(0x8D);
  send_cmd(0xD620); send_data(0x02);
  send_cmd(0xD621); send_data(0xA1);
  send_cmd(0xD622); send_data(0x02);
  send_cmd(0xD623); send_data(0xB9);
  send_cmd(0xD624); send_data(0x02);
  send_cmd(0xD625); send_data(0xC7);
  send_cmd(0xD626); send_data(0x02);
  send_cmd(0xD627); send_data(0xDA);
  send_cmd(0xD628); send_data(0x02);
  send_cmd(0xD629); send_data(0xE7);
  send_cmd(0xD62A); send_data(0x02);
  send_cmd(0xD62B); send_data(0xF9);
  send_cmd(0xD62C); send_data(0x03);
  send_cmd(0xD62D); send_data(0x05);
  send_cmd(0xD62E); send_data(0x03);
  send_cmd(0xD62F); send_data(0x19);
  send_cmd(0xD630); send_data(0x03);
  send_cmd(0xD631); send_data(0x50);
  send_cmd(0xD632); send_data(0x03);
  send_cmd(0xD633); send_data(0xDB);

  send_cmd(0xF000); send_data(0x55); 
  send_cmd(0xF001); send_data(0xAA);
  send_cmd(0xF002); send_data(0x52);
  send_cmd(0xF003); send_data(0x08);
  send_cmd(0xF004); send_data(0x00);

  send_cmd(0xB100); send_data(0xFC); 
  send_cmd(0xB101); send_data(0x00);

  send_cmd(0xB600); send_data(0x05); 

  send_cmd(0xB700); send_data(0x70); 
  send_cmd(0xB701); send_data(0x70);

  send_cmd(0xB800); send_data(0x01); 
  send_cmd(0xB801); send_data(0x05);
  send_cmd(0xB802); send_data(0x05); 
  send_cmd(0xB803); send_data(0x05);

  send_cmd(0xBC00); send_data(0x02); //02
  send_cmd(0xBC01); send_data(0x02); //02
  send_cmd(0xBC02); send_data(0x02); //02

  send_cmd(0xBD00); send_data(0x01); 
  send_cmd(0xBD01); send_data(0xc8);//90
  send_cmd(0xBD02); send_data(0x1e); //1c
  send_cmd(0xBD03); send_data(0x1e);//1c
  send_cmd(0xBD04); send_data(0x00);

  send_cmd(0xCB00); send_data(0x02); 
  send_cmd(0xCB01); send_data(0x09);
  send_cmd(0xCB02); send_data(0xBC); 
  send_cmd(0xCB03); send_data(0x02);
  send_cmd(0xCB04); send_data(0x00);
  send_cmd(0xCB05); send_data(0x80); 
  send_cmd(0xCB06); send_data(0x10);
  send_cmd(0xCB07); send_data(0x0C);
  
  send_cmd(0x3A00); send_data(0x77); //24bit

  send_cmd(0x3500); send_data(0x00); 
  send_cmd(0x4400); send_data(0x01); 
  send_cmd(0x4401); send_data(0xde); 
  
  send_cmd(0x1100); // Sleep out
  LCD_DelayMS(150);
  send_cmd(0x2900); // Display On

}
else {  //chuangshi

	printf("lcd init - cs\n");
#if 1 //chuangshi
	send_cmd(0xF000);send_data(0x55);
	send_cmd(0xF001);send_data(0xAA);
	send_cmd(0xF002);send_data(0x52);
	send_cmd(0xF003);send_data(0x08);
	send_cmd(0xF004);send_data(0x01);


	send_cmd(0xB000);send_data(0x0D);
	send_cmd(0xB001);send_data(0x0D);
	send_cmd(0xB002);send_data(0x0D);


	send_cmd(0xB600);send_data(0x34);
	send_cmd(0xB601);send_data(0x34);
	send_cmd(0xB602);send_data(0x34);

	//AVEE -5.2V
	send_cmd(0xB100);send_data(0x0D);
	send_cmd(0xB101);send_data(0x0D);
	send_cmd(0xB102);send_data(0x0D);

	//AVEE ratio
	send_cmd(0xB700);send_data(0x34);
	send_cmd(0xB701);send_data(0x34);
	send_cmd(0xB702);send_data(0x34);

	//VCL ratio
	send_cmd(0xB800);send_data(0x24);
	send_cmd(0xB801);send_data(0x24);
	send_cmd(0xB802);send_data(0x24);

	//VGH 15V (Free pump)
	send_cmd(0xBF00);send_data(0x01);
	send_cmd(0xB300);send_data(0x0F);
	send_cmd(0xB301);send_data(0x0F);
	send_cmd(0xB302);send_data(0x0F);

	//VGH ratio
	send_cmd(0xB900);send_data(0x34);
	send_cmd(0xB901);send_data(0x34);
	send_cmd(0xB902);send_data(0x34);

	//VGL_REG -10V
	send_cmd(0xB500);send_data(0x08);
	send_cmd(0xB501);send_data(0x08);
	send_cmd(0xB502);send_data(0x08);
	send_cmd(0xC200);send_data(0x03);

	//VGLX ratio
	send_cmd(0xBA00);send_data(0x24);
	send_cmd(0xBA01);send_data(0x24);
	send_cmd(0xBA02);send_data(0x24);

	//VGMP/VGSP 4.5V/0V
	send_cmd(0xBC00);send_data(0x00);
	send_cmd(0xBC01);send_data(0x78);
	send_cmd(0xBC02);send_data(0x00);

	//VGMN/VGSN -4.5V/0V
	send_cmd(0xBD00);send_data(0x00);
	send_cmd(0xBD01);send_data(0x78);
	send_cmd(0xBD02);send_data(0x00);

	//VCOM -1.325V
	send_cmd(0xBE00);send_data(0x00);
	send_cmd(0xBE01);send_data(0x8C);//69

	//Gamma Setting
	send_cmd(0xD100);send_data(0x00);
	send_cmd(0xD101);send_data(0x2D);
	send_cmd(0xD102);send_data(0x00);
	send_cmd(0xD103);send_data(0x2E);
	send_cmd(0xD104);send_data(0x00);
	send_cmd(0xD105);send_data(0x32);
	send_cmd(0xD106);send_data(0x00);
	send_cmd(0xD107);send_data(0x44);
	send_cmd(0xD108);send_data(0x00);
	send_cmd(0xD109);send_data(0x53);
	send_cmd(0xD10A);send_data(0x00);

	send_cmd(0xD10B);send_data(0x88);
	send_cmd(0xD10C);send_data(0x00);
	send_cmd(0xD10D);send_data(0xB6);
	send_cmd(0xD10E);send_data(0x00);
	send_cmd(0xD10F);send_data(0xF3);
	send_cmd(0xD110);send_data(0x01);
	send_cmd(0xD111);send_data(0x22);

	send_cmd(0xD112);send_data(0x01);
	send_cmd(0xD113);send_data(0x64);
	send_cmd(0xD114);send_data(0x01);
	send_cmd(0xD115);send_data(0x92);
	send_cmd(0xD116);send_data(0x01);
	send_cmd(0xD117);send_data(0xD4);
	send_cmd(0xD118);send_data(0x02);
	send_cmd(0xD119);send_data(0x07);
	send_cmd(0xD11A);send_data(0x02);
	send_cmd(0xD11B);send_data(0x08);
	send_cmd(0xD11C);send_data(0x02);
	send_cmd(0xD11D);send_data(0x34);
	send_cmd(0xD11E);send_data(0x02);
	send_cmd(0xD11F);send_data(0x5F);
	send_cmd(0xD120);send_data(0x02);
	send_cmd(0xD121);send_data(0x78);
	send_cmd(0xD122);send_data(0x02);
	send_cmd(0xD123);send_data(0x94);
	send_cmd(0xD124);send_data(0x02);
	send_cmd(0xD125);send_data(0xA6);
	send_cmd(0xD126);send_data(0x02);
	send_cmd(0xD127);send_data(0xBB);
	send_cmd(0xD128);send_data(0x02);
	send_cmd(0xD129);send_data(0xCA);
	send_cmd(0xD12A);send_data(0x02);
	send_cmd(0xD12B);send_data(0xDB);
	send_cmd(0xD12C);send_data(0x02);
	send_cmd(0xD12D);send_data(0xE8);
	send_cmd(0xD12E);send_data(0x02);
	send_cmd(0xD12F);send_data(0xF9);
	send_cmd(0xD130);send_data(0x03);
	send_cmd(0xD131);send_data(0x1F);
	send_cmd(0xD132);send_data(0x03);
	send_cmd(0xD133);send_data(0x7F);
	send_cmd(0xD200);send_data(0x00);
	send_cmd(0xD201);send_data(0x2D);
	send_cmd(0xD202);send_data(0x00);
	send_cmd(0xD203);send_data(0x2E);
	send_cmd(0xD204);send_data(0x00);
	send_cmd(0xD205);send_data(0x32);
	send_cmd(0xD206);send_data(0x00);
	send_cmd(0xD207);send_data(0x44);
	send_cmd(0xD208);send_data(0x00);
	send_cmd(0xD209);send_data(0x53);
	send_cmd(0xD20A);send_data(0x00);
	send_cmd(0xD20B);send_data(0x88);

	send_cmd(0xD20C);send_data(0x00);
	send_cmd(0xD20D);send_data(0xB6);
	send_cmd(0xD20E);send_data(0x00);
	send_cmd(0xD20F);send_data(0xF3);

	send_cmd(0xD210);send_data(0x01);
	send_cmd(0xD211);send_data(0x22);
	send_cmd(0xD212);send_data(0x01);
	send_cmd(0xD213);send_data(0x64);
	send_cmd(0xD214);send_data(0x01);
	send_cmd(0xD215);send_data(0x92);
	send_cmd(0xD216);send_data(0x01);
	send_cmd(0xD217);send_data(0xD4);
	send_cmd(0xD218);send_data(0x02);
	send_cmd(0xD219);send_data(0x07);
	send_cmd(0xD21A);send_data(0x02);
	send_cmd(0xD21B);send_data(0x08);
	send_cmd(0xD21C);send_data(0x02);
	send_cmd(0xD21D);send_data(0x34);
	send_cmd(0xD21E);send_data(0x02);
	send_cmd(0xD21F);send_data(0x5F);
	send_cmd(0xD220);send_data(0x02);
	send_cmd(0xD221);send_data(0x78);
	send_cmd(0xD222);send_data(0x02);
	send_cmd(0xD223);send_data(0x94);
	send_cmd(0xD224);send_data(0x02);
	send_cmd(0xD225);send_data(0xA6);
	send_cmd(0xD226);send_data(0x02);
	send_cmd(0xD227);send_data(0xBB);
	send_cmd(0xD228);send_data(0x02);
	send_cmd(0xD229);send_data(0xCA);
	send_cmd(0xD22A);send_data(0x02);
	send_cmd(0xD22B);send_data(0xDB);
	send_cmd(0xD22C);send_data(0x02);
	send_cmd(0xD22D);send_data(0xE8);
	send_cmd(0xD22E);send_data(0x02);
	send_cmd(0xD22F);send_data(0xF9);

	send_cmd(0xD230);send_data(0x03);
	send_cmd(0xD231);send_data(0x1F);
	send_cmd(0xD232);send_data(0x03);
	send_cmd(0xD233);send_data(0x7F);

	send_cmd(0xD300);send_data(0x00);
	send_cmd(0xD301);send_data(0x2D);
	send_cmd(0xD302);send_data(0x00);
	send_cmd(0xD303);send_data(0x2E);
	send_cmd(0xD304);send_data(0x00);
	send_cmd(0xD305);send_data(0x32);
	send_cmd(0xD306);send_data(0x00);
	send_cmd(0xD307);send_data(0x44);
	send_cmd(0xD308);send_data(0x00);
	send_cmd(0xD309);send_data(0x53);
	send_cmd(0xD30A);send_data(0x00);
	send_cmd(0xD30B);send_data(0x88);
	send_cmd(0xD30C);send_data(0x00);
	send_cmd(0xD30D);send_data(0xB6);
	send_cmd(0xD30E);send_data(0x00);
	send_cmd(0xD30F);send_data(0xF3);
	send_cmd(0xD310);send_data(0x01);
	send_cmd(0xD311);send_data(0x22);
	send_cmd(0xD312);send_data(0x01);
	send_cmd(0xD313);send_data(0x64);
	send_cmd(0xD314);send_data(0x01);
	send_cmd(0xD315);send_data(0x92);
	send_cmd(0xD316);send_data(0x01);
	send_cmd(0xD317);send_data(0xD4);
	send_cmd(0xD318);send_data(0x02);
	send_cmd(0xD319);send_data(0x07);
	send_cmd(0xD31A);send_data(0x02);
	send_cmd(0xD31B);send_data(0x08);
	send_cmd(0xD31C);send_data(0x02);
	send_cmd(0xD31D);send_data(0x34);
	send_cmd(0xD31E);send_data(0x02);
	send_cmd(0xD31F);send_data(0x5F);
	send_cmd(0xD320);send_data(0x02);
	send_cmd(0xD321);send_data(0x78);
	send_cmd(0xD322);send_data(0x02);
	send_cmd(0xD323);send_data(0x94);
	send_cmd(0xD324);send_data(0x02);
	send_cmd(0xD325);send_data(0xA6);
	send_cmd(0xD326);send_data(0x02);
	send_cmd(0xD327);send_data(0xBB);
	send_cmd(0xD328);send_data(0x02);
	send_cmd(0xD329);send_data(0xCA);
	send_cmd(0xD32A);send_data(0x02);
	send_cmd(0xD32B);send_data(0xDB);
	send_cmd(0xD32C);send_data(0x02);
	send_cmd(0xD32D);send_data(0xE8);
	send_cmd(0xD32E);send_data(0x02);
	send_cmd(0xD32F);send_data(0xF9);
	send_cmd(0xD330);send_data(0x03);
	send_cmd(0xD331);send_data(0x1F);
	send_cmd(0xD332);send_data(0x03);
	send_cmd(0xD333);send_data(0x7F);
	send_cmd(0xD400);send_data(0x00);
	send_cmd(0xD401);send_data(0x2D);
	send_cmd(0xD402);send_data(0x00);
	send_cmd(0xD403);send_data(0x2E);
	send_cmd(0xD404);send_data(0x00);
	send_cmd(0xD405);send_data(0x32);
	send_cmd(0xD406);send_data(0x00);
	send_cmd(0xD407);send_data(0x44);
	send_cmd(0xD408);send_data(0x00);
	send_cmd(0xD409);send_data(0x53);
	send_cmd(0xD40A);send_data(0x00);
	send_cmd(0xD40B);send_data(0x88);
	send_cmd(0xD40C);send_data(0x00);
	send_cmd(0xD40D);send_data(0xB6);

	send_cmd(0xD40E);send_data(0x00);
	send_cmd(0xD40F);send_data(0xF3);
	send_cmd(0xD410);send_data(0x01);
	send_cmd(0xD411);send_data(0x22);
	send_cmd(0xD412);send_data(0x01);
	send_cmd(0xD413);send_data(0x64);
	send_cmd(0xD414);send_data(0x01);
	send_cmd(0xD415);send_data(0x92);
	send_cmd(0xD416);send_data(0x01);
	send_cmd(0xD417);send_data(0xD4);
	send_cmd(0xD418);send_data(0x02);
	send_cmd(0xD419);send_data(0x07);
	send_cmd(0xD41A);send_data(0x02);
	send_cmd(0xD41B);send_data(0x08);
	send_cmd(0xD41C);send_data(0x02);
	send_cmd(0xD41D);send_data(0x34);
	send_cmd(0xD41E);send_data(0x02);
	send_cmd(0xD41F);send_data(0x5F);
	send_cmd(0xD420);send_data(0x02);
	send_cmd(0xD421);send_data(0x78);
	send_cmd(0xD422);send_data(0x02);
	send_cmd(0xD423);send_data(0x94);
	send_cmd(0xD424);send_data(0x02);
	send_cmd(0xD425);send_data(0xA6);
	send_cmd(0xD426);send_data(0x02);
	send_cmd(0xD427);send_data(0xBB);
	send_cmd(0xD428);send_data(0x02);
	send_cmd(0xD429);send_data(0xCA);
	send_cmd(0xD42A);send_data(0x02);
	send_cmd(0xD42B);send_data(0xDB);
	send_cmd(0xD42C);send_data(0x02);
	send_cmd(0xD42D);send_data(0xE8);
	send_cmd(0xD42E);send_data(0x02);
	send_cmd(0xD42F);send_data(0xF9);
	send_cmd(0xD430);send_data(0x03);
	send_cmd(0xD431);send_data(0x1F);
	send_cmd(0xD432);send_data(0x03);
	send_cmd(0xD433);send_data(0x7F);
	send_cmd(0xD500);send_data(0x00);
	send_cmd(0xD501);send_data(0x2D);
	send_cmd(0xD502);send_data(0x00);
	send_cmd(0xD503);send_data(0x2E);
	send_cmd(0xD504);send_data(0x00);
	send_cmd(0xD505);send_data(0x32);
	send_cmd(0xD506);send_data(0x00);
	send_cmd(0xD507);send_data(0x44);
	send_cmd(0xD508);send_data(0x00);
	send_cmd(0xD509);send_data(0x53);
	send_cmd(0xD50A);send_data(0x00);
	send_cmd(0xD50B);send_data(0x88);
	send_cmd(0xD50C);send_data(0x00);
	send_cmd(0xD50D);send_data(0xB6);
	send_cmd(0xD50E);send_data(0x00);

	send_cmd(0xD50F);send_data(0xF3);
	send_cmd(0xD510);send_data(0x01);
	send_cmd(0xD511);send_data(0x22);
	send_cmd(0xD512);send_data(0x01);
	send_cmd(0xD513);send_data(0x64);
	send_cmd(0xD514);send_data(0x01);
	send_cmd(0xD515);send_data(0x92);
	send_cmd(0xD516);send_data(0x01);
	send_cmd(0xD517);send_data(0xD4);
	send_cmd(0xD518);send_data(0x02);
	send_cmd(0xD519);send_data(0x07);
	send_cmd(0xD51A);send_data(0x02);
	send_cmd(0xD51B);send_data(0x08);
	send_cmd(0xD51C);send_data(0x02);
	send_cmd(0xD51D);send_data(0x34);
	send_cmd(0xD51E);send_data(0x02);
	send_cmd(0xD51F);send_data(0x5F);
	send_cmd(0xD520);send_data(0x02);
	send_cmd(0xD521);send_data(0x78);
	send_cmd(0xD520);send_data(0x02);
	send_cmd(0xD521);send_data(0x78);
	send_cmd(0xD522);send_data(0x02);
	send_cmd(0xD523);send_data(0x94);
	send_cmd(0xD524);send_data(0x02);
	send_cmd(0xD525);send_data(0xA6);
	send_cmd(0xD526);send_data(0x02);
	send_cmd(0xD527);send_data(0xBB);
	send_cmd(0xD528);send_data(0x02);
	send_cmd(0xD529);send_data(0xCA);
	send_cmd(0xD52A);send_data(0x02);
	send_cmd(0xD52B);send_data(0xDB);
	send_cmd(0xD52C);send_data(0x02);
	send_cmd(0xD52D);send_data(0xE8);

	send_cmd(0xD52E);send_data(0x02);
	send_cmd(0xD52F);send_data(0xF9);
	send_cmd(0xD530);send_data(0x03);
	send_cmd(0xD531);send_data(0x1F);
	send_cmd(0xD532);send_data(0x03);
	send_cmd(0xD533);send_data(0x7F);
	send_cmd(0xD600);send_data(0x00);
	send_cmd(0xD601);send_data(0x2D);
	send_cmd(0xD602);send_data(0x00);
	send_cmd(0xD603);send_data(0x2E);
	send_cmd(0xD604);send_data(0x00);
	send_cmd(0xD605);send_data(0x32);
	send_cmd(0xD606);send_data(0x00);
	send_cmd(0xD607);send_data(0x44);

	send_cmd(0xD608);send_data(0x00);
	send_cmd(0xD609);send_data(0x53);
	send_cmd(0xD60A);send_data(0x00);
	send_cmd(0xD60B);send_data(0x88);
	send_cmd(0xD60C);send_data(0x00);
	send_cmd(0xD60D);send_data(0xB6);

	send_cmd(0xD60E);send_data(0x00);
	send_cmd(0xD60F);send_data(0xF3);

	send_cmd(0xD610);send_data(0x01);
	send_cmd(0xD611);send_data(0x22);
	send_cmd(0xD612);send_data(0x01);
	send_cmd(0xD613);send_data(0x64);
	send_cmd(0xD614);send_data(0x01);
	send_cmd(0xD615);send_data(0x92);
	send_cmd(0xD616);send_data(0x01);
	send_cmd(0xD617);send_data(0xD4);
	send_cmd(0xD618);send_data(0x02);
	send_cmd(0xD619);send_data(0x07);

	send_cmd(0xD61A);send_data(0x02);
	send_cmd(0xD61B);send_data(0x08);
	send_cmd(0xD61C);send_data(0x02);
	send_cmd(0xD61D);send_data(0x34);
	send_cmd(0xD61E);send_data(0x02);
	send_cmd(0xD61F);send_data(0x5F);
	send_cmd(0xD620);send_data(0x02);
	send_cmd(0xD621);send_data(0x78);
	send_cmd(0xD622);send_data(0x02);
	send_cmd(0xD623);send_data(0x94);
	send_cmd(0xD624);send_data(0x02);
	send_cmd(0xD625);send_data(0xA6);
	send_cmd(0xD626);send_data(0x02);
	send_cmd(0xD627);send_data(0xBB);
	send_cmd(0xD628);send_data(0x02);
	send_cmd(0xD629);send_data(0xCA);
	send_cmd(0xD62A);send_data(0x02);
	send_cmd(0xD62B);send_data(0xDB);
	send_cmd(0xD62C);send_data(0x02);
	send_cmd(0xD62D);send_data(0xE8);
	send_cmd(0xD62E);send_data(0x02);
	send_cmd(0xD62F);send_data(0xF9);
	send_cmd(0xD630);send_data(0x03);
	send_cmd(0xD631);send_data(0x1F);
	send_cmd(0xD632);send_data(0x03);
	send_cmd(0xD633);send_data(0x7F);

	//LV2 page 0 enable
	send_cmd(0xF000);send_data(0x55);
	send_cmd(0xF001);send_data(0xAA);
	send_cmd(0xF002);send_data(0x52);
	send_cmd(0xF003);send_data(0x08);
	send_cmd(0xF004);send_data(0x00);

	//Display control
	send_cmd(0xB100);send_data(0xCC);
	send_cmd(0xB101);send_data(0x00);

	//Source hold time
	send_cmd(0xB600);send_data(0x05);

	//Gate EQ control
	send_cmd(0xB700);send_data(0x70);
	send_cmd(0xB701);send_data(0x70);

	//Source EQ control(Mode 2)
	send_cmd(0xB800);send_data(0x01);
	send_cmd(0xB801);send_data(0x03);
	send_cmd(0xB802);send_data(0x03);
	send_cmd(0xB803);send_data(0x03);


	//Inversion mode (2-dot)
	send_cmd(0xBC00);send_data(0x02);
	send_cmd(0xBC01);send_data(0x00);
	send_cmd(0xBC02);send_data(0x00);

	//Timing control 4H w/4-delay
	send_cmd(0xC900);send_data(0xD0);
	send_cmd(0xC901);send_data(0x02);
	send_cmd(0xC902);send_data(0x50);
	send_cmd(0xC903);send_data(0x50);
	send_cmd(0xC904);send_data(0x50);

	send_cmd(0x3500);send_data(0x00);
	send_cmd(0x4400); send_data(0x01); 
	send_cmd(0x4401); send_data(0xde); 




	send_cmd(0x1100);
	LCD_DelayMS(120);

	send_cmd(0x2900);
#endif


}

	printf("qiandan init end!.....\n");

	LCD_DelayMS(100);	

	return 0;
}

static int32_t cd35510_set_window(struct lcd_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("qiandan cd35510_set_window: %d, %d, %d, %d\n",left, top, right, bottom);
	
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

static int32_t cd35510_invalidate(struct lcd_spec *self)
{
	LCD_PRINT("cd35510_invalidate\n");

	return self->ops->lcd_set_window(self, 0, 0, 
			self->width - 1, self->height - 1);
	
}

static int32_t cd35510_invalidate_rect(struct lcd_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
	LCD_PRINT("cd35510_invalidate_rect \n");
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

static int32_t cd35510_set_direction(struct lcd_spec *self, uint16_t direction)
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

static int32_t cd35510_enter_sleep(struct lcd_spec *self, uint8_t is_sleep)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
//printk("cj cd35510_enter_sleep = %d \n",is_sleep);

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

static int32_t cd35510_read_id(struct lcd_spec *self)
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

	printf("%s dummy3:0x%x,dummy4:0x%x\n",__func__, dummy3, dummy4);

	return (dummy3<<8)|dummy4;

//	return 0x5510;
}

static struct lcd_operations lcd_cd35510_operations = {
	.lcd_init = cd35510_init,
	.lcd_set_window = cd35510_set_window,
	.lcd_invalidate_rect= cd35510_invalidate_rect,
	.lcd_invalidate = cd35510_invalidate,
	.lcd_set_direction = cd35510_set_direction,
	.lcd_enter_sleep = cd35510_enter_sleep,
	.lcd_readid = cd35510_read_id,
};

static struct timing_mcu lcd_cd35510_timing[] = {
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

static struct info_mcu lcd_cd35510_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 24,
	.timing = lcd_cd35510_timing,
	.ops = NULL,
};

struct lcd_spec lcd_panel_cd35510 = {
	.width = 480,
	.height = 800,
	.mode = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_cd35510_info},
	.ops = &lcd_cd35510_operations,
};
