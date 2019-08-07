/* drivers/video/sc8800g/sc8800g_lcd_otm8009.c
 *
 * Support for OTM8009 LCD device
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
#define printk printf

//#define  LCD_DEBUG
#ifdef LCD_DEBUG
#define LCD_PRINT printk
#else
#define LCD_PRINT(...)
#endif


#include <asm/arch/mfp.h>
#include <asm/arch/asm_generic_gpio.h>
#include <asm/arch/sdram_cfg.h>

static unsigned long flash_id_func_cfg[] = {
		MFP_CFG_X(TRACECTRL, AF3, DS1, F_PULL_NONE,   S_PULL_NONE,   IO_IE),//gpio42
};

static int32_t otm8009_init(struct lcd_spec *self)
{
	u32 ret;

	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("otm8009_init\n");

	self->ops->lcd_reset(self);
	LCD_DelayMS(120);


	sprd_mfp_config(flash_id_func_cfg, ARRAY_SIZE(flash_id_func_cfg));
	LCD_DelayMS(20);

	if (sprd_gpio_request(NULL, 42)) {
		printf("Failed ro request LCD_ID GPIO_%d \n",	42);
	}
	sprd_gpio_direction_input(NULL, 42);
	ret = sprd_gpio_get(NULL, 42);

	printf("otm8009_init gpio 42 = %d\n", ret);

	if(!ret){

#if 1
			printf("init HuaYu LCM!\n");
			send_cmd(0xFF00); send_data(0x80);
			send_cmd(0xFF01); send_data(0x09);
			send_cmd(0xFF02); send_data(0x01);
			
			send_cmd(0xFF80); send_data(0x80);
			send_cmd(0xFF81); send_data(0x09);
			
			send_cmd(0xC480); send_data(0x30);
			send_cmd(0xC48A); send_data(0x40);
			
			send_cmd(0xB390); send_data(0x02);
			
			send_cmd(0xB392); send_data(0x45);
			
			send_cmd(0xC080); send_data(0x00);
			send_cmd(0xC081); send_data(0x58);//5e
			send_cmd(0xC082); send_data(0x00);
			send_cmd(0xC083); send_data(0x14);//20
			send_cmd(0xC084); send_data(0x16);//20
			send_cmd(0xC085); send_data(0x00);
			send_cmd(0xC086); send_data(0x58);//55
			send_cmd(0xC087); send_data(0x14);//10
			send_cmd(0xC088); send_data(0x16);//10
			
			send_cmd(0xC0A2); send_data(0x10);
			send_cmd(0xC0A3); send_data(0x10);
			send_cmd(0xC0A4); send_data(0x08);
			
			send_cmd(0xD800); send_data(0x77);//87   57
			send_cmd(0xD801); send_data(0x77);

			send_cmd(0xd900); send_data(0x3d);//48

			send_cmd(0xC0B4); send_data(0x50);

			send_cmd(0xC181); send_data(0x33);//55
			
			send_cmd(0xC481); send_data(0x02);
			
			send_cmd(0xC489); send_data(0x00);
			
			send_cmd(0xC492); send_data(0x08);
			
			send_cmd(0xC582); send_data(0xF0);
			
			send_cmd(0xC590); send_data(0x96);
			send_cmd(0xC591); send_data(0x76);
			send_cmd(0xC592); send_data(0x06);
			send_cmd(0xC594); send_data(0x44);
			send_cmd(0xC595); send_data(0x44);
			send_cmd(0xC596); send_data(0x46);
			
			send_cmd(0xC5B2); send_data(0x40);
			
			
			send_cmd(0xCFC7); send_data(0x80);
			send_cmd(0xCFC8); send_data(0x01);
			send_cmd(0xCFC9); send_data(0x0A);
			send_cmd(0xCFCA); send_data(0x06);
			
			send_cmd(0xCE80); send_data(0x8b);
			send_cmd(0xCE81); send_data(0x03);
			send_cmd(0xCE82); send_data(0x00);
			
			send_cmd(0xCE83); send_data(0x8a);
			send_cmd(0xCE84); send_data(0x03);
			send_cmd(0xCE85); send_data(0x00);
			
			send_cmd(0xCE86); send_data(0x89);
			send_cmd(0xCE87); send_data(0x03);
			send_cmd(0xCE88); send_data(0x00);
			
			send_cmd(0xCE89); send_data(0x88);
			send_cmd(0xCE8a); send_data(0x03);
			send_cmd(0xCE8b); send_data(0x00);
			
			send_cmd(0xCE90); send_data(0x00);
			send_cmd(0xCE91); send_data(0x00);
			send_cmd(0xCE92); send_data(0x00);
			send_cmd(0xCE93); send_data(0x00);
			send_cmd(0xCE94); send_data(0x00);
			send_cmd(0xCE95); send_data(0x00);
			send_cmd(0xCE96); send_data(0x00);
			send_cmd(0xCE97); send_data(0x00);
			send_cmd(0xCE98); send_data(0x00);
			send_cmd(0xCE99); send_data(0x00);
			send_cmd(0xCE9a); send_data(0x00);
			send_cmd(0xCE9b); send_data(0x00);
			send_cmd(0xCE9c); send_data(0x00);
			send_cmd(0xCE9d); send_data(0x00);
			
			send_cmd(0xCEa0); send_data(0x38);
			send_cmd(0xCEa1); send_data(0x07);
			send_cmd(0xCEa2); send_data(0x03);
			send_cmd(0xCEa3); send_data(0x59);
			send_cmd(0xCEa4); send_data(0x00);
			send_cmd(0xCEa5); send_data(0x00);
			send_cmd(0xCEa6); send_data(0x00);
			send_cmd(0xCEa7); send_data(0x38);
			send_cmd(0xCEa8); send_data(0x06);
			send_cmd(0xCEa9); send_data(0x03);
			send_cmd(0xCEaa); send_data(0x5a);
			send_cmd(0xCEab); send_data(0x00);
			send_cmd(0xCEac); send_data(0x00);
			send_cmd(0xCEad); send_data(0x00);
			
			send_cmd(0xCEb0); send_data(0x38);
			send_cmd(0xCEb1); send_data(0x05);
			send_cmd(0xCEb2); send_data(0x03);
			send_cmd(0xCEb3); send_data(0x5b);
			send_cmd(0xCEb4); send_data(0x00);
			send_cmd(0xCEb5); send_data(0x00);
			send_cmd(0xCEb6); send_data(0x00);
			send_cmd(0xCEb7); send_data(0x38);
			send_cmd(0xCEb8); send_data(0x04);
			send_cmd(0xCEb9); send_data(0x03);
			send_cmd(0xCEba); send_data(0x5c);
			send_cmd(0xCEbb); send_data(0x00);
			send_cmd(0xCEbc); send_data(0x00);
			send_cmd(0xCEbd); send_data(0x00);
			
			send_cmd(0xCEc0); send_data(0x38);
			send_cmd(0xCEc1); send_data(0x03);
			send_cmd(0xCEc2); send_data(0x03);
			send_cmd(0xCEc3); send_data(0x5d);
			send_cmd(0xCEc4); send_data(0x00);
			send_cmd(0xCEc5); send_data(0x00);
			send_cmd(0xCEc6); send_data(0x00);
			send_cmd(0xCEc7); send_data(0x38);
			send_cmd(0xCEc8); send_data(0x02);
			send_cmd(0xCEc9); send_data(0x03);
			send_cmd(0xCEca); send_data(0x5e);
			send_cmd(0xCEcb); send_data(0x00);
			send_cmd(0xCEcc); send_data(0x00);
			send_cmd(0xCEcd); send_data(0x00);
			
			send_cmd(0xCEd0); send_data(0x38);
			send_cmd(0xCEd1); send_data(0x01);
			send_cmd(0xCEd2); send_data(0x03);
			send_cmd(0xCEd3); send_data(0x5f);
			send_cmd(0xCEd4); send_data(0x00);
			send_cmd(0xCEd5); send_data(0x00);
			send_cmd(0xCEd6); send_data(0x00);
			send_cmd(0xCEd7); send_data(0x38);
			send_cmd(0xCEd8); send_data(0x00);
			send_cmd(0xCEd9); send_data(0x03);
			send_cmd(0xCEda); send_data(0x60);
			send_cmd(0xCEdb); send_data(0x00);
			send_cmd(0xCEdc); send_data(0x00);
			send_cmd(0xCEdd); send_data(0x00);
			
			send_cmd(0xCFc6); send_data(0x01);
			send_cmd(0xCFc8); send_data(0x10);
			
			send_cmd(0xCBc0); send_data(0x04);
			send_cmd(0xCBc1); send_data(0x04);
			send_cmd(0xCBc2); send_data(0x00);
			send_cmd(0xCBc3); send_data(0x00);
			send_cmd(0xCBc4); send_data(0x04);
			send_cmd(0xCBc5); send_data(0x04);
			send_cmd(0xCBc6); send_data(0x04);
			send_cmd(0xCBc7); send_data(0x04);
			send_cmd(0xCBc8); send_data(0x04);
			send_cmd(0xCBc9); send_data(0x04);
			send_cmd(0xCBca); send_data(0x00);
			send_cmd(0xCBcb); send_data(0x00);
			send_cmd(0xCBcc); send_data(0x00);
			send_cmd(0xCBcd); send_data(0x00);
			send_cmd(0xCBce); send_data(0x00);
			
			send_cmd(0xCBd0); send_data(0x00);
			send_cmd(0xCBd1); send_data(0x00);
			send_cmd(0xCBd2); send_data(0x00);
			send_cmd(0xCBd3); send_data(0x00);
			send_cmd(0xCBd4); send_data(0x00);
			send_cmd(0xCBd5); send_data(0x04);
			send_cmd(0xCBd6); send_data(0x04);
			send_cmd(0xCBd7); send_data(0x00);
			send_cmd(0xCBd8); send_data(0x00);
			send_cmd(0xCBd9); send_data(0x04);
			send_cmd(0xCBda); send_data(0x04);
			send_cmd(0xCBdb); send_data(0x04);
			send_cmd(0xCBdc); send_data(0x04);
			send_cmd(0xCBdd); send_data(0x04);
			send_cmd(0xCBde); send_data(0x04);
			
			send_cmd(0xCBe0); send_data(0x00);
			send_cmd(0xCBe1); send_data(0x00);
			send_cmd(0xCBe2); send_data(0x00);
			send_cmd(0xCBe3); send_data(0x00);
			send_cmd(0xCBe4); send_data(0x00);
			send_cmd(0xCBe5); send_data(0x00);
			send_cmd(0xCBe6); send_data(0x00);
			send_cmd(0xCBe7); send_data(0x00);
			send_cmd(0xCBe8); send_data(0x00);
			send_cmd(0xCBe9); send_data(0x00);
			
			send_cmd(0xCBf0); send_data(0xFF);
			send_cmd(0xCBf1); send_data(0xFF);
			send_cmd(0xCBf2); send_data(0xFF);
			send_cmd(0xCBf3); send_data(0xFF);
			send_cmd(0xCBf4); send_data(0xFF);
			send_cmd(0xCBf5); send_data(0xFF);
			send_cmd(0xCBf6); send_data(0xFF);
			send_cmd(0xCBf7); send_data(0xFF);
			send_cmd(0xCBf8); send_data(0xFF);
			send_cmd(0xCBf9); send_data(0xFF);
			
			//U2D : 0xCC80 ~ 0xCCAE
			send_cmd(0xCC80); send_data(0x25);
			send_cmd(0xCC81); send_data(0x26);
			send_cmd(0xCC82); send_data(0x00);
			send_cmd(0xCC83); send_data(0x00);
			send_cmd(0xCC84); send_data(0x0d);
			send_cmd(0xCC85); send_data(0x0f);
			send_cmd(0xCC86); send_data(0x09);
			send_cmd(0xCC87); send_data(0x0b);
			send_cmd(0xCC88); send_data(0x01);
			send_cmd(0xCC89); send_data(0x03);
			
			send_cmd(0xCC90); send_data(0x00);
			send_cmd(0xCC91); send_data(0x00);
			send_cmd(0xCC92); send_data(0x00);
			send_cmd(0xCC93); send_data(0x00);
			send_cmd(0xCC94); send_data(0x00);
			send_cmd(0xCC95); send_data(0x00);
			send_cmd(0xCC96); send_data(0x00);
			send_cmd(0xCC97); send_data(0x00);
			send_cmd(0xCC98); send_data(0x00);
			send_cmd(0xCC99); send_data(0x00);
			send_cmd(0xCC9a); send_data(0x25);
			send_cmd(0xCC9b); send_data(0x26);
			send_cmd(0xCC9c); send_data(0x00);
			send_cmd(0xCC9d); send_data(0x00);
			send_cmd(0xCC9e); send_data(0x0E);
			
			send_cmd(0xCCa0); send_data(0x10);
			send_cmd(0xCCa1); send_data(0x0a);
			send_cmd(0xCCa2); send_data(0x0c);
			send_cmd(0xCCa3); send_data(0x02);
			send_cmd(0xCCa4); send_data(0x04);
			send_cmd(0xCCa5); send_data(0x00);
			send_cmd(0xCCa6); send_data(0x00);
			send_cmd(0xCCa7); send_data(0x00);
			send_cmd(0xCCa8); send_data(0x00);
			send_cmd(0xCCa9); send_data(0x00);
			send_cmd(0xCCaa); send_data(0x00);
			send_cmd(0xCCab); send_data(0x00);
			send_cmd(0xCCac); send_data(0x00);
			send_cmd(0xCCad); send_data(0x00);
			send_cmd(0xCCae); send_data(0x00);
			
			
			//D2U : 0xCCB0 ~ 0xCCDE
			send_cmd(0xCCb0); send_data(0x26);
			send_cmd(0xCCb1); send_data(0x25);
			send_cmd(0xCCb2); send_data(0x00);
			send_cmd(0xCCb3); send_data(0x00);
			send_cmd(0xCCb4); send_data(0x0c);
			send_cmd(0xCCb5); send_data(0x0a);
			send_cmd(0xCCb6); send_data(0x10);
			send_cmd(0xCCb7); send_data(0x0e);
			send_cmd(0xCCb8); send_data(0x04);
			send_cmd(0xCCb9); send_data(0x02);
									  
			send_cmd(0xCCC0); send_data(0x00);
			send_cmd(0xCCC1); send_data(0x00);
			send_cmd(0xCCC2); send_data(0x00);
			send_cmd(0xCCC3); send_data(0x00);
			send_cmd(0xCCC4); send_data(0x00);
			send_cmd(0xCCC5); send_data(0x00);
			send_cmd(0xCCC6); send_data(0x00);
			send_cmd(0xCCC7); send_data(0x00);
			send_cmd(0xCCC8); send_data(0x00);
			send_cmd(0xCCC9); send_data(0x00);
			send_cmd(0xCCCa); send_data(0x26);
			send_cmd(0xCCCb); send_data(0x25);
			send_cmd(0xCCCc); send_data(0x00);
			send_cmd(0xCCCd); send_data(0x00);
			send_cmd(0xCCCe); send_data(0x0b);
									  
			send_cmd(0xCCd0); send_data(0x09);
			send_cmd(0xCCd1); send_data(0x0f);
			send_cmd(0xCCd2); send_data(0x0d);
			send_cmd(0xCCd3); send_data(0x03);
			send_cmd(0xCCd4); send_data(0x01);
			send_cmd(0xCCd5); send_data(0x00);
			send_cmd(0xCCd6); send_data(0x00);
			send_cmd(0xCCd7); send_data(0x00);
			send_cmd(0xCCd8); send_data(0x00);
			send_cmd(0xCCd9); send_data(0x00);
			send_cmd(0xCCda); send_data(0x00);
			send_cmd(0xCCdb); send_data(0x00);
			send_cmd(0xCCdc); send_data(0x00);
			send_cmd(0xCCdd); send_data(0x00);
			send_cmd(0xCCde); send_data(0x00);
			
			send_cmd(0xE100); send_data(0x00);
			send_cmd(0xE101); send_data(0x04);
			send_cmd(0xE102); send_data(0x0c);
			send_cmd(0xE103); send_data(0x12);
			send_cmd(0xE104); send_data(0x0B);
			send_cmd(0xE105); send_data(0x1E);
			send_cmd(0xE106); send_data(0x0E);
			send_cmd(0xE107); send_data(0x0E);
			send_cmd(0xE108); send_data(0x00);
			send_cmd(0xE109); send_data(0x05);
			send_cmd(0xE10A); send_data(0x02);
			send_cmd(0xE10B); send_data(0x07);
			send_cmd(0xE10C); send_data(0x0F);
			send_cmd(0xE10D); send_data(0x22);
			send_cmd(0xE10E); send_data(0x1E);
			send_cmd(0xE10F); send_data(0x16);
			
			send_cmd(0xE200); send_data(0x00);
			send_cmd(0xE201); send_data(0x05);
			send_cmd(0xE202); send_data(0x0c);
			send_cmd(0xE203); send_data(0x12);
			send_cmd(0xE204); send_data(0x0B);
			send_cmd(0xE205); send_data(0x1E);
			send_cmd(0xE206); send_data(0x0F);
			send_cmd(0xE207); send_data(0x0D);
			send_cmd(0xE208); send_data(0x00);
			send_cmd(0xE209); send_data(0x04);
			send_cmd(0xE20A); send_data(0x02);
			send_cmd(0xE20B); send_data(0x07);
			send_cmd(0xE20C); send_data(0x0E);
			send_cmd(0xE20D); send_data(0x21);
			send_cmd(0xE20E); send_data(0x1E);
			send_cmd(0xE20F); send_data(0x16);
			
			send_cmd(0xFF00); send_data(0xFF);
			send_cmd(0xFF01); send_data(0xFF);
			send_cmd(0xFF02); send_data(0xFF);
			
		//	send_cmd(0x3600);  send_data(0x00);
			send_cmd(0x3500);  send_data(0x00);
			send_cmd(0x4400);  send_data(0x01);
			send_cmd(0x4401);  send_data(0x22);
				
#if CONFIG_HUAMOBILE_LCD_BUS_WIDTH < 24
					send_cmd(0x3A00); send_data(0x66);	
#else
					send_cmd(0x3A00); send_data(0x77);	
#endif
	
			send_cmd(0x1100);
			LCD_DelayMS(150);
			
			send_cmd(0x2900);
			LCD_DelayMS(20);
			
			send_cmd(0x2A00); send_data(0x00);
			send_cmd(0x2A01); send_data(0x00);
			send_cmd(0x2A02); send_data(0x01);
			send_cmd(0x2A03); send_data(0xDF);
			
			send_cmd(0x2B00); send_data(0x00);
			send_cmd(0x2B01); send_data(0x00);
			send_cmd(0x2B02); send_data(0x03);
			send_cmd(0x2B03); send_data(0x55);
			
			
#endif



	} else {
		printf("init TXD LCM!\n");
		
		send_cmd(0xFF00); send_data(0x80);
		send_cmd(0xFF01); send_data(0x09);
		send_cmd(0xFF02); send_data(0x01);

		send_cmd(0xFF80); send_data(0x80);
		send_cmd(0xFF81); send_data(0x09);

		send_cmd(0xFF03); send_data(0x01);

		send_cmd(0xD800); send_data(0x6C);
		send_cmd(0xD801); send_data(0x6C);

		send_cmd(0xC0B4); send_data(0x50);

		send_cmd(0xC582); send_data(0xA3);

		send_cmd(0xC181); send_data(0x66);//33

		send_cmd(0xC1A1); send_data(0x08);

		send_cmd(0xC0A3); send_data(0x1B);

		send_cmd(0xC481); send_data(0x83);

		send_cmd(0xC590); send_data(0xD6);
		send_cmd(0xC591); send_data(0xA7);
		send_cmd(0xC592); send_data(0x01);

		send_cmd(0xC5B1); send_data(0xA9);

		send_cmd(0xD900); send_data(0x25);//30//19

		send_cmd(0xE100); send_data(0x00);
		send_cmd(0xE101); send_data(0x04);
		send_cmd(0xE102); send_data(0x0c);
		send_cmd(0xE103); send_data(0x12);
		send_cmd(0xE104); send_data(0x0B);
		send_cmd(0xE105); send_data(0x1E);
		send_cmd(0xE106); send_data(0x0E);
		send_cmd(0xE107); send_data(0x0E);
		send_cmd(0xE108); send_data(0x00);
		send_cmd(0xE109); send_data(0x05);
		send_cmd(0xE10A); send_data(0x02);
		send_cmd(0xE10B); send_data(0x07);
		send_cmd(0xE10C); send_data(0x0F);
		send_cmd(0xE10D); send_data(0x22);
		send_cmd(0xE10E); send_data(0x1E);
		send_cmd(0xE10F); send_data(0x16);

		send_cmd(0xE200); send_data(0x00);
		send_cmd(0xE201); send_data(0x05);
		send_cmd(0xE202); send_data(0x0c);
		send_cmd(0xE203); send_data(0x12);
		send_cmd(0xE204); send_data(0x0B);
		send_cmd(0xE205); send_data(0x1E);
		send_cmd(0xE206); send_data(0x0F);
		send_cmd(0xE207); send_data(0x0D);
		send_cmd(0xE208); send_data(0x00);
		send_cmd(0xE209); send_data(0x04);
		send_cmd(0xE20A); send_data(0x02);
		send_cmd(0xE20B); send_data(0x07);
		send_cmd(0xE20C); send_data(0x0E);
		send_cmd(0xE20D); send_data(0x21);
		send_cmd(0xE20E); send_data(0x1E);
		send_cmd(0xE20F); send_data(0x16);

		send_cmd(0xB3A7); send_data(0x00);

		send_cmd(0xB392); send_data(0x45);

		send_cmd(0xB390); send_data(0x02);

		send_cmd(0xC090); send_data(0x00);
		send_cmd(0xC091); send_data(0x44);
		send_cmd(0xC092); send_data(0x00);
		send_cmd(0xC093); send_data(0x00);
		send_cmd(0xC094); send_data(0x00);
		send_cmd(0xC095); send_data(0x03);

		send_cmd(0xC1A6); send_data(0x00);
		send_cmd(0xC1A7); send_data(0x00);
		send_cmd(0xC1A8); send_data(0x00);

		send_cmd(0xCE80); send_data(0x87);
		send_cmd(0xCE81); send_data(0x03);
		send_cmd(0xCE82); send_data(0x00);
		send_cmd(0xCE83); send_data(0x86);
		send_cmd(0xCE84); send_data(0x03);
		send_cmd(0xCE85); send_data(0x00);

		send_cmd(0xCE90); send_data(0x33);
		send_cmd(0xCE91); send_data(0x54);
		send_cmd(0xCE92); send_data(0x00);
		send_cmd(0xCE93); send_data(0x33);
		send_cmd(0xCE94); send_data(0x55);
		send_cmd(0xCE95); send_data(0x00);

		send_cmd(0xCEA0); send_data(0x38);
		send_cmd(0xCEA1); send_data(0x03);
		send_cmd(0xCEA2); send_data(0x03);
		send_cmd(0xCEA3); send_data(0x58);
		send_cmd(0xCEA4); send_data(0x00);
		send_cmd(0xCEA5); send_data(0x00);
		send_cmd(0xCEA6); send_data(0x00);
		send_cmd(0xCEA7); send_data(0x38);
		send_cmd(0xCEA8); send_data(0x02);
		send_cmd(0xCEA9); send_data(0x03);
		send_cmd(0xCEAA); send_data(0x59);
		send_cmd(0xCEAB); send_data(0x00);
		send_cmd(0xCEAC); send_data(0x00);
		send_cmd(0xCEAD); send_data(0x00);

		send_cmd(0xCEB0); send_data(0x38);
		send_cmd(0xCEB1); send_data(0x01);
		send_cmd(0xCEB2); send_data(0x03);
		send_cmd(0xCEB3); send_data(0x5A);
		send_cmd(0xCEB4); send_data(0x00);
		send_cmd(0xCEB5); send_data(0x00);
		send_cmd(0xCEB6); send_data(0x00);
		send_cmd(0xCEB7); send_data(0x38);
		send_cmd(0xCEB8); send_data(0x00);
		send_cmd(0xCEB9); send_data(0x03);
		send_cmd(0xCEBA); send_data(0x5B);
		send_cmd(0xCEBB); send_data(0x00);
		send_cmd(0xCEBC); send_data(0x00);
		send_cmd(0xCEBD); send_data(0x00);

		send_cmd(0xCEC0); send_data(0x30);
		send_cmd(0xCEC1); send_data(0x00);
		send_cmd(0xCEC2); send_data(0x03);
		send_cmd(0xCEC3); send_data(0x5C);
		send_cmd(0xCEC4); send_data(0x00);
		send_cmd(0xCEC5); send_data(0x00);
		send_cmd(0xCEC6); send_data(0x00);
		send_cmd(0xCEC7); send_data(0x30);
		send_cmd(0xCEC8); send_data(0x01);
		send_cmd(0xCEC9); send_data(0x03);
		send_cmd(0xCECA); send_data(0x5D);
		send_cmd(0xCECB); send_data(0x00);
		send_cmd(0xCECC); send_data(0x00);
		send_cmd(0xCECD); send_data(0x00);

		send_cmd(0xCED0); send_data(0x38);
		send_cmd(0xCED1); send_data(0x05);
		send_cmd(0xCED2); send_data(0x03);
		send_cmd(0xCED3); send_data(0x5E);
		send_cmd(0xCED4); send_data(0x00);
		send_cmd(0xCED5); send_data(0x00);
		send_cmd(0xCED6); send_data(0x00);
		send_cmd(0xCED7); send_data(0x38);
		send_cmd(0xCED8); send_data(0x04);
		send_cmd(0xCED9); send_data(0x03);
		send_cmd(0xCEDA); send_data(0x5F);
		send_cmd(0xCEDB); send_data(0x00);
		send_cmd(0xCEDC); send_data(0x00);
		send_cmd(0xCEDD); send_data(0x00);

		send_cmd(0xCFC0); send_data(0x00);
		send_cmd(0xCFC1); send_data(0x00);
		send_cmd(0xCFC2); send_data(0x00);
		send_cmd(0xCFC3); send_data(0x00);
		send_cmd(0xCFC4); send_data(0x00);
		send_cmd(0xCFC5); send_data(0x00);
		send_cmd(0xCFC6); send_data(0x01);
		send_cmd(0xCFC7); send_data(0x80);
		send_cmd(0xCFC8); send_data(0x00);
		send_cmd(0xCFC9); send_data(0x09);

		send_cmd(0xCBC0); send_data(0x00);
		send_cmd(0xCBC1); send_data(0x04);
		send_cmd(0xCBC2); send_data(0x04);
		send_cmd(0xCBC3); send_data(0x04);
		send_cmd(0xCBC4); send_data(0x04);
		send_cmd(0xCBC5); send_data(0x00);
		send_cmd(0xCBC6); send_data(0x00);
		send_cmd(0xCBC7); send_data(0x04);
		send_cmd(0xCBC8); send_data(0x04);
		send_cmd(0xCBC9); send_data(0x04);
		send_cmd(0xCBCA); send_data(0x04);
		send_cmd(0xCBCB); send_data(0x00);
		send_cmd(0xCBCC); send_data(0x00);
		send_cmd(0xCBCD); send_data(0x00);
		send_cmd(0xCBCE); send_data(0x00);

		send_cmd(0xCBD0); send_data(0x00);
		send_cmd(0xCBD1); send_data(0x00);
		send_cmd(0xCBD2); send_data(0x00);
		send_cmd(0xCBD3); send_data(0x00);
		send_cmd(0xCBD4); send_data(0x00);
		send_cmd(0xCBD5); send_data(0x00);
		send_cmd(0xCBD6); send_data(0x04);
		send_cmd(0xCBD7); send_data(0x04);
		send_cmd(0xCBD8); send_data(0x04);
		send_cmd(0xCBD9); send_data(0x04);
		send_cmd(0xCBDA); send_data(0x00);
		send_cmd(0xCBDB); send_data(0x00);
		send_cmd(0xCBDC); send_data(0x04);
		send_cmd(0xCBDD); send_data(0x04);
		send_cmd(0xCBDE); send_data(0x04);

		send_cmd(0xCBE0); send_data(0x04);
		send_cmd(0xCBE1); send_data(0x00);
		send_cmd(0xCBE2); send_data(0x00);
		send_cmd(0xCBE3); send_data(0x00);
		send_cmd(0xCBE4); send_data(0x00);
		send_cmd(0xCBE5); send_data(0x00);
		send_cmd(0xCBE6); send_data(0x00);
		send_cmd(0xCBE7); send_data(0x00);
		send_cmd(0xCBE8); send_data(0x00);
		send_cmd(0xCBE9); send_data(0x00);

		send_cmd(0xCC80); send_data(0x00);
		send_cmd(0xCC81); send_data(0x26);
		send_cmd(0xCC82); send_data(0x25);
		send_cmd(0xCC83); send_data(0x02);
		send_cmd(0xCC84); send_data(0x06);
		send_cmd(0xCC85); send_data(0x00);
		send_cmd(0xCC86); send_data(0x00);
		send_cmd(0xCC87); send_data(0x0A);
		send_cmd(0xCC88); send_data(0x0E);
		send_cmd(0xCC89); send_data(0x0C);

		send_cmd(0xCC90); send_data(0x10);
		send_cmd(0xCC91); send_data(0x00);
		send_cmd(0xCC92); send_data(0x00);
		send_cmd(0xCC93); send_data(0x00);
		send_cmd(0xCC94); send_data(0x00);
		send_cmd(0xCC95); send_data(0x00);
		send_cmd(0xCC96); send_data(0x00);
		send_cmd(0xCC97); send_data(0x00);
		send_cmd(0xCC98); send_data(0x00);
		send_cmd(0xCC99); send_data(0x00);
		send_cmd(0xCC9A); send_data(0x00);
		send_cmd(0xCC9B); send_data(0x26);
		send_cmd(0xCC9C); send_data(0x25);
		send_cmd(0xCC9D); send_data(0x01);
		send_cmd(0xCC9E); send_data(0x05);

		send_cmd(0xCCA0); send_data(0x00);
		send_cmd(0xCCA1); send_data(0x00);
		send_cmd(0xCCA2); send_data(0x09);
		send_cmd(0xCCA3); send_data(0x0D);
		send_cmd(0xCCA4); send_data(0x0B);
		send_cmd(0xCCA5); send_data(0x0F);
		send_cmd(0xCCA6); send_data(0x00);
		send_cmd(0xCCA7); send_data(0x00);
		send_cmd(0xCCA8); send_data(0x00);
		send_cmd(0xCCA9); send_data(0x00);
		send_cmd(0xCCAA); send_data(0x00);
		send_cmd(0xCCAB); send_data(0x00);
		send_cmd(0xCCAC); send_data(0x00);
		send_cmd(0xCCAD); send_data(0x00);
		send_cmd(0xCCAE); send_data(0x00);

		send_cmd(0xCCB0); send_data(0x00);
		send_cmd(0xCCB1); send_data(0x25);
		send_cmd(0xCCB2); send_data(0x26);
		send_cmd(0xCCB3); send_data(0x05);
		send_cmd(0xCCB4); send_data(0x01);
		send_cmd(0xCCB5); send_data(0x00);
		send_cmd(0xCCB6); send_data(0x00);
		send_cmd(0xCCB7); send_data(0x0D);
		send_cmd(0xCCB8); send_data(0x09);
		send_cmd(0xCCB9); send_data(0x0B);

		send_cmd(0xCCC0); send_data(0x0F);
		send_cmd(0xCCC1); send_data(0x00);
		send_cmd(0xCCC2); send_data(0x00);
		send_cmd(0xCCC3); send_data(0x00);
		send_cmd(0xCCC4); send_data(0x00);
		send_cmd(0xCCC5); send_data(0x00);
		send_cmd(0xCCC6); send_data(0x00);
		send_cmd(0xCCC7); send_data(0x00);
		send_cmd(0xCCC8); send_data(0x00);
		send_cmd(0xCCC9); send_data(0x00);
		send_cmd(0xCCCA); send_data(0x00);
		send_cmd(0xCCCB); send_data(0x25);
		send_cmd(0xCCCC); send_data(0x26);
		send_cmd(0xCCCD); send_data(0x06);
		send_cmd(0xCCCE); send_data(0x02);

		send_cmd(0xCCD0); send_data(0x00);
		send_cmd(0xCCD1); send_data(0x00);
		send_cmd(0xCCD2); send_data(0x0E);
		send_cmd(0xCCD3); send_data(0x0A);
		send_cmd(0xCCD4); send_data(0x0C);
		send_cmd(0xCCD5); send_data(0x10);
		send_cmd(0xCCD6); send_data(0x00);
		send_cmd(0xCCD7); send_data(0x00);
		send_cmd(0xCCD8); send_data(0x00);
		send_cmd(0xCCD9); send_data(0x00);
		send_cmd(0xCCDA); send_data(0x00);
		send_cmd(0xCCDB); send_data(0x00);
		send_cmd(0xCCDC); send_data(0x00);
		send_cmd(0xCCDD); send_data(0x00);
		send_cmd(0xCCDE); send_data(0x00);

		send_cmd(0xFF00); send_data(0xFF);
		send_cmd(0xFF01); send_data(0xFF);
		send_cmd(0xFF02); send_data(0xFF);

		send_cmd(0x2A00);
		send_data(0x00);
		send_cmd(0x2A01);
		send_data(0x00);
		send_cmd(0x2A02);
		send_data(0x01);
		send_cmd(0x2A03);	
		send_data(0xDF);

		send_cmd(0x2B00);
		send_data(0x00);
		send_cmd(0x2B01);
		send_data(0x00);
		send_cmd(0x2B02);	
		send_data(0x08);
		send_cmd(0x2B03);	
		send_data(0x53);

		send_cmd(0x4400); send_data(0x01); 
		send_cmd(0x4401); send_data(0xde); 


		send_cmd(0x3A00); send_data(0x66); 

		send_cmd(0x3500); send_data(0x00); 

		send_cmd(0x1100);
		LCD_DelayMS(150);

		send_cmd(0x2900);
		LCD_DelayMS(200);

}




printf("qiandan init end!.....\n");


LCD_DelayMS(100);	



	return 0;
}

static int32_t otm8009_set_window(struct lcd_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("qiandan otm8009_set_window: %d, %d, %d, %d\n",left, top, right, bottom);
#if 0    
	send_cmd(0x2A); // col
	send_data((left >> 8));
	send_data((left & 0xFF));
	send_data((right >> 8));
	send_data((right & 0xFF));

	send_cmd(0x2B); // row
	send_data((top >> 8));
	send_data((top & 0xFF));
	send_data((bottom >> 8));
	send_data((bottom & 0xFF));

	send_cmd(0x2C); //Write data	

#else

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

#endif

	return 0;
}

static int32_t otm8009_invalidate(struct lcd_spec *self)
{
	LCD_PRINT("otm8009_invalidate\n");

	return self->ops->lcd_set_window(self, 0, 0, 
			self->width - 1, self->height - 1);
	
}

static int32_t otm8009_invalidate_rect(struct lcd_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
	LCD_PRINT("otm8009_invalidate_rect \n");
	// TE scaneline
	//send_cmd_data(0x000b, (top >> 8));
	//send_cmd_data(0x000c, (top & 0xff));
#if 0
	send_cmd(0x4400); // TE scanline
	send_data((top >> 8));
	send_cmd(0x4401); // TE scanline
	send_data((top & 0xFF));	
#endif
	return self->ops->lcd_set_window(self, left, top, 
			right, bottom);
}

static int32_t otm8009_set_direction(struct lcd_spec *self, uint16_t direction)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("qiandan hx8369_set_direction\n");
#if 0
	send_cmd(0x36);

	switch (direction) {
	case LCD_DIRECT_NORMAL:
		send_data(0);
		break;
	case LCD_DIRECT_ROT_90:
		send_data(0xA0);
		break;
	case LCD_DIRECT_ROT_180:
		send_data(0x60);
		break;
	case LCD_DIRECT_ROT_270:
		send_data(0xB0);
		break;
	case LCD_DIRECT_MIR_H:
		send_data(0x40);
		break;
	case LCD_DIRECT_MIR_V:
		send_data(0x10);
		break;
	case LCD_DIRECT_MIR_HV:
		send_data(0xE0);
		break;
	default:
		LCD_PRINT("unknown lcd direction!\n");
		send_data(0x0);
		direction = LCD_DIRECT_NORMAL;
		break;
	}
#else
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

#endif
	self->direction = direction;
	
	return 0;
}

static int32_t otm8009_enter_sleep(struct lcd_spec *self, uint8_t is_sleep)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
//printk("cj otm8009_enter_sleep = %d \n",is_sleep);

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
		LCD_DelayMS(120); 
		send_cmd(0x2900);
		LCD_DelayMS(120); 
	}

	return 0;
}

static int32_t otm8009_read_id(struct lcd_spec *self)
{

	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
	Read_data read_data = self->info.mcu->ops->read_data;

	uint32_t dummy0,dummy1,dummy2,dummy3,dummy4;

	send_cmd(0xA100);
	dummy0=read_data();
	dummy1=read_data();
	dummy2=read_data();
	dummy3=read_data();
	dummy4=read_data();
	printk("%s: dummy0:0x%x,dummy1:0x%x,dummy2:0x%x,dummy3:0x%x,dummy4:0x%x\n",__func__, dummy0,dummy1,dummy2,dummy3,dummy4);
	printk("id = %x\n", ((dummy3<<8)|dummy4));
	return (dummy3<<8)|dummy4;

}

static struct lcd_operations lcd_otm8009_operations = {
	.lcd_init = otm8009_init,
	.lcd_set_window = otm8009_set_window,
	.lcd_invalidate_rect= otm8009_invalidate_rect,
	.lcd_invalidate = otm8009_invalidate,
	.lcd_set_direction = otm8009_set_direction,
	.lcd_enter_sleep = otm8009_enter_sleep,
	.lcd_readid = otm8009_read_id,
};

static struct timing_mcu lcd_otm8009_timing[] = {
[LCD_REGISTER_TIMING] = {                    // read/write register timing
		.rcss = 20,//15,  // 15ns
		.rlpw = 50,//60,
		.rhpw = 50,//60,
		.wcss = 5,
		.wlpw = 20,//35,
		.whpw = 20,//35,
	},
[LCD_GRAM_TIMING] = {                    // read/write gram timing
		.rcss = 20,//15,  // 15ns
		.rlpw = 50,//60,
		.rhpw = 50,//60,
		.wcss = 5,//10,
		.wlpw = 20,//50,//35,
		.whpw = 20,//50,//35,
	},
};

static struct info_mcu lcd_otm8009_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 18,
	.timing = lcd_otm8009_timing,
	.ops = NULL,
};

struct lcd_spec lcd_panel_otm8009 = {
	.width = 480,
	.height = 854,
	.mode = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_otm8009_info},
	.ops = &lcd_otm8009_operations,
};
