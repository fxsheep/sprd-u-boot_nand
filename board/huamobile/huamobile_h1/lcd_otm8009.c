/* u-boot/spreadtrum/sp8810g-brcm/lcd_otm8009.c
 *
 * Support for otm8009 LCD device
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
#define LCD_PRINT printk
#else
#define LCD_PRINT(...)
#endif

static int32_t otm8009_init(struct lcd_spec *self)
{

	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("otm8009_init\n");

	self->ops->lcd_reset(self);
	LCD_DelayMS(120);
	
#if 0
	
		{
		printf("8076 BY otm8009");
		send_cmd(0xff00);
		send_data(0x80);
	
		send_cmd(0xff01);
		send_data(0x09);
	
		send_cmd(0xff02);
		send_data(0x01);
	
		send_cmd(0xff80);
		send_data(0x80);
	
		send_cmd(0xff81);
		send_data(0x09);
	
		send_cmd(0xff03);
		send_data(0x01);
	
		send_cmd(0xd800);
		send_data(0x43);
	
		send_cmd(0xd801);
		send_data(0x43);
	
		send_cmd(0xc5b1);
		send_data(0xa9);
	
		send_cmd(0xc590);
		send_data(0x96);
	
		send_cmd(0xc591);
		send_data(0x84);
	
		send_cmd(0xc592);
		send_data(0x00);
	
		send_cmd(0xc582);
		send_data(0xa3);
	
		send_cmd(0xc181);
		send_data(0x66);
	
		send_cmd(0xc1a1);
		send_data(0x08);
	
		send_cmd(0xc0a2);
		send_data(0x02);
	
		send_cmd(0xc0a3);
		send_data(0x1b);
	
		send_cmd(0xc481);
		send_data(0x83);
	
		send_cmd(0xc489);
		send_data(0x00);
	
		send_cmd(0xb3a1);
		send_data(0x10);
	
		send_cmd(0xc0b4);
		send_data(0x50);
	
		send_cmd(0x3600);
		send_data(0x00);
	
		send_cmd(0xc090);
		send_data(0x00);
	
		send_cmd(0xc091);
		send_data(0x44);
	
		send_cmd(0xc092);
		send_data(0x00);
	
		send_cmd(0xc093);
		send_data(0x00);
	
		send_cmd(0xc094);
		send_data(0x00);
	
		send_cmd(0xc095);
		send_data(0x03);
	
		send_cmd(0xc1a6);
		send_data(0x00);
	
		send_cmd(0xc1a7);
		send_data(0x00);
	
		send_cmd(0xc1a8);
		send_data(0x00);
	
		send_cmd(0xce80);
		send_data(0x87);
	
		send_cmd(0xce81);
		send_data(0x03);
	
		send_cmd(0xce82);
		send_data(0x14);
	
		send_cmd(0xce83);
		send_data(0x86);
	
		send_cmd(0xce84);
		send_data(0x03);
	
		send_cmd(0xce85);
		send_data(0x14);
	
		send_cmd(0xce90);
		send_data(0x33);
	
		send_cmd(0xce91);
		send_data(0x1e);
	
		send_cmd(0xce92);
		send_data(0x14);
	
		send_cmd(0xce93);
		send_data(0x33);
	
		send_cmd(0xce94);
		send_data(0x1f);
	
		send_cmd(0xce95);
		send_data(0x14);
	
		send_cmd(0xcea0);
		send_data(0x38);
	
		send_cmd(0xcea1);
		send_data(0x03);
	
		send_cmd(0xcea2);
		send_data(0x03);
	
		send_cmd(0xcea3);
		send_data(0x1c);
	
		send_cmd(0xcea4);
		send_data(0x00);
	
		send_cmd(0xcea5);
		send_data(0x14);
	
		send_cmd(0xcea6);
		send_data(0x00);
	
		send_cmd(0xcea7);
		send_data(0x38);
	
		send_cmd(0xcea8);
		send_data(0x02);
	
		send_cmd(0xcea9);
		send_data(0x03);
	
		send_cmd(0xceaa);
		send_data(0x1d);
	
		send_cmd(0xceab);
		send_data(0x00);
	
		send_cmd(0xceac);
		send_data(0x14);
	
		send_cmd(0xcead);
		send_data(0x00);
	
		send_cmd(0xceb0);
		send_data(0x38);
	
		send_cmd(0xceb1);
		send_data(0x01);
	
		send_cmd(0xceb2);
		send_data(0x03);
	
		send_cmd(0xceb3);
		send_data(0x1e);
	
		send_cmd(0xceb4);
		send_data(0x00);
	
		send_cmd(0xceb5);
		send_data(0x14);
	
		send_cmd(0xceb6);
		send_data(0x00);
	
		send_cmd(0xceb7);
		send_data(0x38);
	
		send_cmd(0xceb8);
		send_data(0x00);
	
		send_cmd(0xceb9);
		send_data(0x03);
	
		send_cmd(0xceba);
		send_data(0x1f);
	
		send_cmd(0xcebb);
		send_data(0x00);
	
		send_cmd(0xcebc);
		send_data(0x14);
	
		send_cmd(0xcebd);
		send_data(0x00);
	
		send_cmd(0xcec0);
		send_data(0x30);
	
		send_cmd(0xcec1);
		send_data(0x00);
	
		send_cmd(0xcec2);
		send_data(0x03);
	
		send_cmd(0xcec3);
		send_data(0x20);
	
		send_cmd(0xcec4);
		send_data(0x00);
	
		send_cmd(0xcec5);
		send_data(0x14);
	
		send_cmd(0xcec6);
		send_data(0x00);
	
		send_cmd(0xcec7);
		send_data(0x30);
	
		send_cmd(0xcec8);
		send_data(0x01);
	
		send_cmd(0xcec9);
		send_data(0x03);
	
		send_cmd(0xceca);
		send_data(0x21);
	
		send_cmd(0xcecb);
		send_data(0x00);
	
		send_cmd(0xcecc);
		send_data(0x14);
	
		send_cmd(0xcecd);
		send_data(0x00);
	
		send_cmd(0xced0);
		send_data(0x30);
	
		send_cmd(0xced1);
		send_data(0x02);
	
		send_cmd(0xced2);
		send_data(0x03);
	
		send_cmd(0xced3);
		send_data(0x22);
	
		send_cmd(0xced4);
		send_data(0x00);
	
		send_cmd(0xced5);
		send_data(0x14);
	
		send_cmd(0xced6);
		send_data(0x00);
	
		send_cmd(0xced7);
		send_data(0x30);
	
		send_cmd(0xced8);
		send_data(0x03);
	
		send_cmd(0xced9);
		send_data(0x03);
	
		send_cmd(0xceda);
		send_data(0x23);
	
		send_cmd(0xcedb);
		send_data(0x00);
	
		send_cmd(0xcedc);
		send_data(0x14);
	
		send_cmd(0xcedd);
		send_data(0x00);
	
		send_cmd(0xcfc6);
		send_data(0x01);
	
		send_cmd(0xcfc7);
		send_data(0x80);
	
		send_cmd(0xcfc9);
		send_data(0x10);
	
		send_cmd(0xcbc0);
		send_data(0x00);
	
		send_cmd(0xcbc1);
		send_data(0x04);
	
		send_cmd(0xcbc2);
		send_data(0x04);
	
		send_cmd(0xcbc3);
		send_data(0x04);
	
		send_cmd(0xcbc4);
		send_data(0x04);
	
		send_cmd(0xcbc5);
		send_data(0x00);
	
		send_cmd(0xcbc6);
		send_data(0x00);
	
		send_cmd(0xcbc7);
		send_data(0x04);
	
		send_cmd(0xcbc8);
		send_data(0x04);
	
		send_cmd(0xcbc9);
		send_data(0x04);
	
		send_cmd(0xcbca);
		send_data(0x04);
	
		send_cmd(0xcbcb);
		send_data(0x00);
	
		send_cmd(0xcbcc);
		send_data(0x00);
	
		send_cmd(0xcbcd);
		send_data(0x00);
	
		send_cmd(0xcbce);
		send_data(0x00);
	
		send_cmd(0xcbd0);
		send_data(0x00);
	
		send_cmd(0xcbd1);
		send_data(0x00);
	
		send_cmd(0xcbd2);
		send_data(0x00);
	
		send_cmd(0xcbd3);
		send_data(0x00);
	
		send_cmd(0xcbd4);
		send_data(0x00);
	
		send_cmd(0xcbd5);
		send_data(0x00);
	
		send_cmd(0xcbd6);
		send_data(0x04);
	
		send_cmd(0xcbd7);
		send_data(0x04);
	
		send_cmd(0xcbd8);
		send_data(0x04);
	
		send_cmd(0xcbd9);
		send_data(0x04);
	
		send_cmd(0xcbda);
		send_data(0x00);
	
		send_cmd(0xcbdb);
		send_data(0x00);
	
		send_cmd(0xcbdc);
		send_data(0x04);
	
		send_cmd(0xcbdd);
		send_data(0x04);
	
		send_cmd(0xcbde);
		send_data(0x04);
	
		send_cmd(0xcbe0);
		send_data(0x04);
	
		send_cmd(0xcbe1);
		send_data(0x00);
	
		send_cmd(0xcbe2);
		send_data(0x00);
	
		send_cmd(0xcbe3);
		send_data(0x00);
	
		send_cmd(0xcbe4);
		send_data(0x00);
	
		send_cmd(0xcbe5);
		send_data(0x00);
	
		send_cmd(0xcbe6);
		send_data(0x00);
	
		send_cmd(0xcbe7);
		send_data(0x00);
	
		send_cmd(0xcbe8);
		send_data(0x00);
	
		send_cmd(0xcbe9);
		send_data(0x00);
	
		send_cmd(0xcc80);
		send_data(0x00);
	
		send_cmd(0xcc81);
		send_data(0x26);
	
		send_cmd(0xcc82);
		send_data(0x25);
	
		send_cmd(0xcc83);
		send_data(0x02);
	
		send_cmd(0xcc84);
		send_data(0x06);
	
		send_cmd(0xcc85);
		send_data(0x00);
	
		send_cmd(0xcc86);
		send_data(0x00);
	
		send_cmd(0xcc87);
		send_data(0x0a);
	
		send_cmd(0xcc88);
		send_data(0x0e);
	
		send_cmd(0xcc89);
		send_data(0x0c);
	
		send_cmd(0xcc90);
		send_data(0x10);
	
		send_cmd(0xcc91);
		send_data(0x00);
	
		send_cmd(0xcc92);
		send_data(0x00);
	
		send_cmd(0xcc93);
		send_data(0x00);
	
		send_cmd(0xcc94);
		send_data(0x00);
	
		send_cmd(0xcc95);
		send_data(0x00);
	
		send_cmd(0xcc96);
		send_data(0x00);
	
		send_cmd(0xcc97);
		send_data(0x00);
	
		send_cmd(0xcc98);
		send_data(0x00);
	
		send_cmd(0xcc99);
		send_data(0x00);
	
		send_cmd(0xcc9a);
		send_data(0x00);
	
		send_cmd(0xcc9b);
		send_data(0x26);
	
		send_cmd(0xcc9c);
		send_data(0x25);
	
		send_cmd(0xcc9d);
		send_data(0x01);
	
		send_cmd(0xcc9e);
		send_data(0x05);
	
		send_cmd(0xcca0);
		send_data(0x00);
	
		send_cmd(0xcca1);
		send_data(0x00);
	
		send_cmd(0xcca2);
		send_data(0x09);
	
		send_cmd(0xcca3);
		send_data(0x0d);
	
		send_cmd(0xcca4);
		send_data(0x0b);
	
		send_cmd(0xcca5);
		send_data(0x0f);
	
		send_cmd(0xcca6);
		send_data(0x00);
	
		send_cmd(0xcca7);
		send_data(0x00);
	
		send_cmd(0xcca8);
		send_data(0x00);
	
		send_cmd(0xcca9);
		send_data(0x00);
	
		send_cmd(0xccaa);
		send_data(0x00);
	
		send_cmd(0xccab);
		send_data(0x00);
	
		send_cmd(0xccac);
		send_data(0x00);
	
		send_cmd(0xccad);
		send_data(0x00);
	
		send_cmd(0xccae);
		send_data(0x00);
	
		send_cmd(0xccb0);
		send_data(0x00);
	
		send_cmd(0xccb1);
		send_data(0x25);
	
		send_cmd(0xccb2);
		send_data(0x26);
	
		send_cmd(0xccb3);
		send_data(0x05);
	
		send_cmd(0xccb4);
		send_data(0x01);
	
		send_cmd(0xccb5);
		send_data(0x00);
	
		send_cmd(0xccb6);
		send_data(0x00);
	
		send_cmd(0xccb7);
		send_data(0x0f);
	
		send_cmd(0xccb8);
		send_data(0x0b);
	
		send_cmd(0xccb9);
		send_data(0x0d);
	
		send_cmd(0xccc0);
		send_data(0x09);
	
		send_cmd(0xccc1);
		send_data(0x00);
	
		send_cmd(0xccc2);
		send_data(0x00);
	
		send_cmd(0xccc3);
		send_data(0x00);
	
		send_cmd(0xccc4);
		send_data(0x00);
	
		send_cmd(0xccc5);
		send_data(0x00);
	
		send_cmd(0xccc6);
		send_data(0x00);
	
		send_cmd(0xccc7);
		send_data(0x00);
	
		send_cmd(0xccc8);
		send_data(0x00);
	
		send_cmd(0xccc9);
		send_data(0x00);
	
		send_cmd(0xccca);
		send_data(0x00);
	
		send_cmd(0xcccb);
		send_data(0x25);
	
		send_cmd(0xcccc);
		send_data(0x26);
	
		send_cmd(0xcccd);
		send_data(0x06);
	
		send_cmd(0xccce);
		send_data(0x02);
	
		send_cmd(0xccd0);
		send_data(0x00);
	
		send_cmd(0xccd1);
		send_data(0x00);
	
		send_cmd(0xccd2);
		send_data(0x10);
	
		send_cmd(0xccd3);
		send_data(0x0c);
	
		send_cmd(0xccd4);
		send_data(0x0e);
	
		send_cmd(0xccd5);
		send_data(0x0a);
	
		send_cmd(0xccd6);
		send_data(0x00);
	
		send_cmd(0xccd7);
		send_data(0x00);
	
		send_cmd(0xccd8);
		send_data(0x00);
	
		send_cmd(0xccd9);
		send_data(0x00);
	
		send_cmd(0xccda);
		send_data(0x00);
	
		send_cmd(0xccdb);
		send_data(0x00);
	
		send_cmd(0xccdc);
		send_data(0x00);
	
		send_cmd(0xccdd);
		send_data(0x00);
	
		send_cmd(0xccde);
		send_data(0x00);
	
		send_cmd(0xd900);
		send_data(0x31);
	
		send_cmd(0xe100);
		send_data(0x06);
	
		send_cmd(0xe101);
		send_data(0x07);
	
		send_cmd(0xe102);
		send_data(0x0e);
	
		send_cmd(0xe103);
		send_data(0x0d);
	
		send_cmd(0xe104);
		send_data(0x07);
	
		send_cmd(0xe105);
		send_data(0x16);
	
		send_cmd(0xe106);
		send_data(0x0c);
	
		send_cmd(0xe107);
		send_data(0x0c);
	
		send_cmd(0xe108);
		send_data(0x02);
	
		send_cmd(0xe109);
		send_data(0x06);
	
		send_cmd(0xe10a);
		send_data(0x05);
	
		send_cmd(0xe10b);
		send_data(0x07);
	
		send_cmd(0xe10c);
		send_data(0x0f);
	
		send_cmd(0xe10d);
		send_data(0x2b);
	
		send_cmd(0xe10e);
		send_data(0x27);
	
		send_cmd(0xe10f);
		send_data(0x0d);
	
		send_cmd(0xe200);
		send_data(0x06);
	
		send_cmd(0xe201);
		send_data(0x07);
	
		send_cmd(0xe202);
		send_data(0x0e);
	
		send_cmd(0xe203);
		send_data(0x0d);
	
		send_cmd(0xe204);
		send_data(0x07);
	
		send_cmd(0xe205);
		send_data(0x16);
	
		send_cmd(0xe206);
		send_data(0x0c);
	
		send_cmd(0xe207);
		send_data(0x0c);
	
		send_cmd(0xe208);
		send_data(0x02);
	
		send_cmd(0xe209);
		send_data(0x06);
	
		send_cmd(0xe20a);
		send_data(0x05);
	
		send_cmd(0xe20b);
		send_data(0x07);
	
		send_cmd(0xe20c);
		send_data(0x0f);
	
		send_cmd(0xe20d);
		send_data(0x2b);
	
		send_cmd(0xe20e);
		send_data(0x27);
	
		send_cmd(0xe20f);
		send_data(0x0d);
	
		send_cmd(0x3500); send_data(0x00);
		
		send_cmd(0x4400); send_data(0x01); 
		send_cmd(0x4401); send_data(0xDE); 
	
		send_cmd(0x3A00); send_data(0x77);
	
		send_cmd(0x1100); send_data(0x00);
		LCD_DelayMS(120);//delay 120ms
	
		send_cmd(0x2900); send_data(0x00);
		LCD_DelayMS(100);//delay 100ms
	
		send_cmd(0x2C00); send_data(0x00);
	
		}
#endif


	{
			printf("init h1 QiJia otm8009a!\n");
	
			send_cmd(0xFF00); send_data(0x80);
			send_cmd(0xFF01); send_data(0x09);
			send_cmd(0xFF02); send_data(0x01);
	
			send_cmd(0xFF80); send_data(0x80);
			send_cmd(0xFF81); send_data(0x09);
	
			send_cmd(0xc0b4); send_data(0x50);
	
			send_cmd(0xc582); send_data(0xa3);
	
			send_cmd(0xc590); send_data(0x96);
			send_cmd(0xc591); send_data(0x87);
	
			send_cmd(0xd800); send_data(0x97);
			send_cmd(0xd801); send_data(0x97);
	
			send_cmd(0xd900); send_data(0x4E);
	
			send_cmd(0xc181); send_data(0x66);
	
			send_cmd(0xc1a1); send_data(0x08);
	
			send_cmd(0xc489); send_data(0x08);
	
			send_cmd(0xc0a3); send_data(0x00);
	
			send_cmd(0xc481); send_data(0x83);
	
			send_cmd(0xc592); send_data(0x01);
	
			send_cmd(0xc5b1); send_data(0xa9);
	
			send_cmd(0xcfc7); send_data(0x02);
	
			send_cmd(0xb390); send_data(0x00);
	
			send_cmd(0xb392); send_data(0x05);
	
			send_cmd(0xc080); send_data(0x00);
			send_cmd(0xc081); send_data(0x58);
			send_cmd(0xc082); send_data(0x00);
			send_cmd(0xc083); send_data(0x15);
			send_cmd(0xc084); send_data(0x15);
			send_cmd(0xc085); send_data(0x00);
			send_cmd(0xc086); send_data(0x58);
			send_cmd(0xc087); send_data(0x15);
			send_cmd(0xc088); send_data(0x15);
	
			send_cmd(0xc090); send_data(0x00);
			send_cmd(0xc091); send_data(0x44);
			send_cmd(0xc092); send_data(0x00);
			send_cmd(0xc093); send_data(0x00);
			send_cmd(0xc094); send_data(0x00);
			send_cmd(0xc095); send_data(0x03);
	
			send_cmd(0xc1a6); send_data(0x00);
			send_cmd(0xc1a7); send_data(0x00);
			send_cmd(0xc1a8); send_data(0x00);
	
			send_cmd(0xce80); send_data(0x87);
			send_cmd(0xce81); send_data(0x03);
			send_cmd(0xce82); send_data(0x00);
			send_cmd(0xce83); send_data(0x86);
			send_cmd(0xce84); send_data(0x03);
			send_cmd(0xce85); send_data(0x00);
			send_cmd(0xce86); send_data(0x85);
			send_cmd(0xce87); send_data(0x03);
			send_cmd(0xce88); send_data(0x00);
			send_cmd(0xce89); send_data(0x84);
			send_cmd(0xce8a); send_data(0x03);
			send_cmd(0xce8b); send_data(0x00);
	
			send_cmd(0xce90); send_data(0x33);
			send_cmd(0xce91); send_data(0x1c);
			send_cmd(0xce92); send_data(0x00);
			send_cmd(0xce93); send_data(0x33);
			send_cmd(0xce94); send_data(0x1d);
			send_cmd(0xce95); send_data(0x00);
			send_cmd(0xce96); send_data(0x33);
			send_cmd(0xce97); send_data(0x1e);
			send_cmd(0xce98); send_data(0x00);
			send_cmd(0xce99); send_data(0x33);
			send_cmd(0xce9a); send_data(0x1f);
			send_cmd(0xce9b); send_data(0x00);
			send_cmd(0xce9c); send_data(0x00);
			send_cmd(0xce9d); send_data(0x00);
	
			send_cmd(0xcea0); send_data(0x38);
			send_cmd(0xcea1); send_data(0x05);
			send_cmd(0xcea2); send_data(0x03);
			send_cmd(0xcea3); send_data(0x20);
			send_cmd(0xcea4); send_data(0x00);
			send_cmd(0xcea5); send_data(0x00);
			send_cmd(0xcea6); send_data(0x00);
			send_cmd(0xcea7); send_data(0x38);
			send_cmd(0xcea8); send_data(0x04);
			send_cmd(0xcea9); send_data(0x03);
			send_cmd(0xceaa); send_data(0x21);
			send_cmd(0xceab); send_data(0x00);
			send_cmd(0xceac); send_data(0x00);
			send_cmd(0xcead); send_data(0x00);
	
			send_cmd(0xceb0); send_data(0x38);
			send_cmd(0xceb1); send_data(0x03);
			send_cmd(0xceb2); send_data(0x03);
			send_cmd(0xceb3); send_data(0x22);
			send_cmd(0xceb4); send_data(0x00);
			send_cmd(0xceb5); send_data(0x00);
			send_cmd(0xceb6); send_data(0x00);
			send_cmd(0xceb7); send_data(0x38);
			send_cmd(0xceb8); send_data(0x02);
			send_cmd(0xceb9); send_data(0x03);
			send_cmd(0xceba); send_data(0x23);
			send_cmd(0xcebb); send_data(0x00);
			send_cmd(0xcebc); send_data(0x00);
			send_cmd(0xcebd); send_data(0x00);
	
			send_cmd(0xcec0); send_data(0x38);
			send_cmd(0xcec1); send_data(0x01);
			send_cmd(0xcec2); send_data(0x03);
			send_cmd(0xcec3); send_data(0x24);
			send_cmd(0xcec4); send_data(0x00);
			send_cmd(0xcec5); send_data(0x00);
			send_cmd(0xcec6); send_data(0x00);
			send_cmd(0xcec7); send_data(0x38);
			send_cmd(0xcec8); send_data(0x00);
			send_cmd(0xcec9); send_data(0x03);
			send_cmd(0xceca); send_data(0x25);
			send_cmd(0xcecb); send_data(0x00);
			send_cmd(0xcecc); send_data(0x00);
			send_cmd(0xcecd); send_data(0x00);
	
			send_cmd(0xced0); send_data(0x30);
			send_cmd(0xced1); send_data(0x00);
			send_cmd(0xced2); send_data(0x03);
			send_cmd(0xced3); send_data(0x26);
			send_cmd(0xced4); send_data(0x00);
			send_cmd(0xced5); send_data(0x00);
			send_cmd(0xced6); send_data(0x00);
			send_cmd(0xced7); send_data(0x30);
			send_cmd(0xced8); send_data(0x01);
			send_cmd(0xced9); send_data(0x03);
			send_cmd(0xceda); send_data(0x27);
			send_cmd(0xcedb); send_data(0x00);
			send_cmd(0xcedc); send_data(0x00);
			send_cmd(0xcedd); send_data(0x00);
	
			send_cmd(0xcbc3); send_data(0x04);
			send_cmd(0xcbc4); send_data(0x04);
			send_cmd(0xcbc5); send_data(0x04);
			send_cmd(0xcbc6); send_data(0x04);
			send_cmd(0xcbc7); send_data(0x04);
			send_cmd(0xcbc8); send_data(0x04);
			send_cmd(0xcbc9); send_data(0x04);
			send_cmd(0xcbca); send_data(0x04);
	
			send_cmd(0xcbd8); send_data(0x04);
			send_cmd(0xcbd9); send_data(0x04);
			send_cmd(0xcbda); send_data(0x04);
			send_cmd(0xcbdb); send_data(0x04);
			send_cmd(0xcbdc); send_data(0x04);
			send_cmd(0xcbdd); send_data(0x04);
			send_cmd(0xcbde); send_data(0x04);
	
			send_cmd(0xcbe0); send_data(0x04);
	
			send_cmd(0xcc83); send_data(0x03);
			send_cmd(0xcc84); send_data(0x01);
			send_cmd(0xcc85); send_data(0x09);
			send_cmd(0xcc86); send_data(0x0b);
			send_cmd(0xcc87); send_data(0x0d);
			send_cmd(0xcc88); send_data(0x0f);
			send_cmd(0xcc89); send_data(0x05);
	
			send_cmd(0xcc90); send_data(0x07);
	
			send_cmd(0xcc9d); send_data(0x04);
			send_cmd(0xcc9e); send_data(0x02);
	
			send_cmd(0xcca0); send_data(0x0a);
			send_cmd(0xcca1); send_data(0x0c);
			send_cmd(0xcca2); send_data(0x0e);
			send_cmd(0xcca3); send_data(0x10);
			send_cmd(0xcca4); send_data(0x06);
			send_cmd(0xcca5); send_data(0x08);
	
			send_cmd(0xccb3); send_data(0x06);
			send_cmd(0xccb4); send_data(0x08);
			send_cmd(0xccb5); send_data(0x0a);
			send_cmd(0xccb6); send_data(0x10);
			send_cmd(0xccb7); send_data(0x0e);
			send_cmd(0xccb8); send_data(0x0c);
			send_cmd(0xccb9); send_data(0x04);
	
			send_cmd(0xccc0); send_data(0x02);
	
			send_cmd(0xcccd); send_data(0x05);
			send_cmd(0xcccd); send_data(0x07);
	
			send_cmd(0xccd0); send_data(0x09);
			send_cmd(0xccd1); send_data(0x0f);
			send_cmd(0xccd2); send_data(0x0d);
			send_cmd(0xccd3); send_data(0x0b);
			send_cmd(0xccd4); send_data(0x03);
			send_cmd(0xccd5); send_data(0x01);
	
			send_cmd(0xE100); send_data(0x08);
			send_cmd(0xE101); send_data(0x0a);
			send_cmd(0xE102); send_data(0x0F);
			send_cmd(0xE103); send_data(0x0d);
			send_cmd(0xE104); send_data(0x08);
			send_cmd(0xE105); send_data(0x1f);
			send_cmd(0xE106); send_data(0x0f);
			send_cmd(0xE107); send_data(0x0f);
			send_cmd(0xE108); send_data(0x00);
			send_cmd(0xE109); send_data(0x04);
			send_cmd(0xE10a); send_data(0x00);
			send_cmd(0xE10b); send_data(0x07);
			send_cmd(0xE10c); send_data(0x0e);
			send_cmd(0xE10d); send_data(0x1c);
			send_cmd(0xE10e); send_data(0x1a);
			send_cmd(0xE10f); send_data(0x12);
	
			send_cmd(0xE200); send_data(0x08);
			send_cmd(0xE201); send_data(0x0a);
			send_cmd(0xE202); send_data(0x0F);
			send_cmd(0xE203); send_data(0x0d);
			send_cmd(0xE204); send_data(0x08);
			send_cmd(0xE205); send_data(0x1f);
			send_cmd(0xE206); send_data(0x0f);
			send_cmd(0xE207); send_data(0x0f);
			send_cmd(0xE208); send_data(0x00);
			send_cmd(0xE209); send_data(0x04);
			send_cmd(0xE20a); send_data(0x01);
			send_cmd(0xE20b); send_data(0x07);
			send_cmd(0xE20c); send_data(0x0e);
			send_cmd(0xE20d); send_data(0x1c);
			send_cmd(0xE20e); send_data(0x19);
			send_cmd(0xE20f); send_data(0x12);
	
			send_cmd(0xFF00); send_data(0xff);
			send_cmd(0xFF01); send_data(0xff);
			send_cmd(0xFF02); send_data(0xff);
			
			send_cmd(0x3500); send_data(0x00);
			
			send_cmd(0x4400); send_data(0x01); 
			send_cmd(0x4401); send_data(0xDE); 
			
			send_cmd(0x3A00); send_data(0x77);
			
			send_cmd(0x1100); 
			LCD_DelayMS(150);
			send_cmd(0x2900); 
			LCD_DelayMS(150);
			send_cmd(0x2C00); 
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
	send_cmd(0x44); // TE scanline
	send_data((top >> 8));
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

static int32_t otm8009_enter_sleep(struct lcd_spec *self, uint8_t is_sleep)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;

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
	printf("%s: dummy0:0x%x,dummy1:0x%x,dummy2:0x%x,dummy3:0x%x,dummy4:0x%x\n",__func__, dummy0,dummy1,dummy2,dummy3,dummy4);
	printf("id = %x\n", ((dummy3<<8)|dummy4));
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
		.rlpw = 50,//60,90  70
		.rhpw = 50,//60,90  70
		.wcss = 5,//20 20  10
		.wlpw = 20,//35,70   50
		.whpw = 20,//35,70   50
	},
[LCD_GRAM_TIMING] = {                    // read/write gram timing
		.rcss = 20,//15,  // 15ns
		.rlpw = 50,//60,90  70
		.rhpw = 50,//60,90  70
		.wcss = 5,//20
		.wlpw = 20,//35,70  15
		.whpw = 20,//35,70  24
	},
};

static struct info_mcu lcd_otm8009_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 24,
	.timing = lcd_otm8009_timing,
	.ops = NULL,
};

struct lcd_spec lcd_panel_otm8009 = {
	.width = 480,
	.height = 800,
	.mode = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_otm8009_info},
	.ops = &lcd_otm8009_operations,
};
