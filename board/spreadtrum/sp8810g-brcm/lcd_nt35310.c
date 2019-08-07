/* drivers/video/sc8800g/sc8800g_lcd_nt35310.c
 *
 * Support for NT35310 LCD device
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

static int32_t nt35310_init(struct lcd_spec *self)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("nt35310_init\n");

	self->ops->lcd_reset(self);
	LCD_DelayMS(120);
	
#if 1


//Enter CMD2
send_cmd(0xED);
send_data(0x01);
send_data(0xFE);

//Enter Manufacture Mode    
send_cmd(0xEE);
send_data(0xDE);
send_data(0x21); 
LCD_DelayMS(120);

//Sleep Out
send_cmd(0x11);
LCD_DelayMS(120);
LCD_DelayMS(120);


//Set VCOM
send_cmd(0xC4);
send_data(0x3A); 

send_cmd(0xb3);send_data(0x20);//dai add.


//Enter Page1
send_cmd(0xBF);
send_data(0xAA); 

//Smart Color Manufacture Setting
send_cmd(0xB0);
send_data(0x0D);send_data(0x00);send_data(0x0D);send_data(0x00);send_data(0x11);
send_data(0x00);send_data(0x19);send_data(0x00);send_data(0x21);send_data(0x00);
send_data(0x2D);send_data(0x00);send_data(0x3D);send_data(0x00);send_data(0x5D);
send_data(0x00);send_data(0x5D);send_data(0x00);

send_cmd(0xB1);
send_data(0x80);send_data(0x00);send_data(0x8B);send_data(0x00);send_data(0x96);
send_data(0x00);

send_cmd(0xB2);
send_data(0x00);send_data(0x00);send_data(0x02);send_data(0x00);send_data(0x03);
send_data(0x00);

send_cmd(0xB3);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);

send_cmd(0xB4);
send_data(0x8B);send_data(0x00);send_data(0x96);send_data(0x00);send_data(0xA1);
send_data(0x00);


send_cmd(0xB5);
send_data(0x02);send_data(0x00);send_data(0x03);send_data(0x00);send_data(0x04);
send_data(0x00);


send_cmd(0xB6);
send_data(0x00);send_data(0x00);
send_cmd(0xB7);
send_data(0x3E);send_data(0x00);send_data(0x5E);send_data(0x00);send_data(0x9E);
send_data(0x00);send_data(0x74);send_data(0x00);send_data(0x8C);send_data(0x00);
send_data(0xAC);send_data(0x00);send_data(0xDC);send_data(0x00);send_data(0x70);
send_data(0x00);send_data(0xB9);send_data(0x00);send_data(0xEC);send_data(0x00);
send_data(0xDC);send_data(0x00);

send_cmd(0xB8);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);
send_data(0x00);send_data(0x00);send_data(0x00); 

send_cmd(0xBA);send_data(0x24);send_data(0x00);send_data(0x00);send_data(0x00);

//CABC Manufacture Setting
send_cmd(0xC1);
send_data(0x20);send_data(0x00);send_data(0x54);send_data(0x00);send_data(0xFF);
send_data(0x00); 
send_cmd(0xC2);send_data(0x00);send_data(0x00);send_data(0x04);send_data(0x00); 


send_cmd(0xC3);
send_data(0x3C);send_data(0x00);send_data(0x3A);send_data(0x00);send_data(0x39);
send_data(0x00);send_data(0x37);send_data(0x00);send_data(0x3C);send_data(0x00);
send_data(0x36);send_data(0x00);send_data(0x32);send_data(0x00);send_data(0x2F);
send_data(0x00);send_data(0x2C);send_data(0x00);send_data(0x29);send_data(0x00);
send_data(0x26);send_data(0x00);send_data(0x24);send_data(0x00);send_data(0x24);
send_data(0x00);send_data(0x23);send_data(0x00);send_data(0x3C);send_data(0x00);
send_data(0x36);send_data(0x00);send_data(0x32);send_data(0x00);send_data(0x2F);
send_data(0x00);send_data(0x2C);send_data(0x00);send_data(0x29);send_data(0x00);
send_data(0x26);send_data(0x00);send_data(0x24);send_data(0x00);send_data(0x24);
send_data(0x00);send_data(0x23);send_data(0x00);

send_cmd(0xC4);
send_data(0x62);send_data(0x00);send_data(0x05);send_data(0x00);send_data(0x84);
send_data(0x00);send_data(0xF0);send_data(0x00);send_data(0x18);send_data(0x00);
send_data(0xA4);send_data(0x00);send_data(0x18);send_data(0x00);send_data(0x50);
send_data(0x00);send_data(0x0C);send_data(0x00);send_data(0x17);send_data(0x00);
send_data(0x95);send_data(0x00);send_data(0xF3);send_data(0x00);send_data(0xE6);
send_data(0x00);

send_cmd(0xC5);
send_data(0x32);send_data(0x00);send_data(0x44);send_data(0x00);send_data(0x65);
send_data(0x00);send_data(0x76);send_data(0x00);send_data(0x88);send_data(0x00);
 
send_cmd(0xC6);
send_data(0x20);send_data(0x00);send_data(0x17);send_data(0x00);send_data(0x01);
send_data(0x00); 

//CABC Test Mode Setting
send_cmd(0xC7);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00); 

send_cmd(0xC8);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);

//OSC Manufacture Setting 
send_cmd(0xC9);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);
send_data(0x00); 

send_cmd(0xE0);
send_data(0x01);send_data(0x00);send_data(0x05);send_data(0x00);send_data(0x0F);
send_data(0x00);send_data(0x2F);send_data(0x00);send_data(0x45);send_data(0x00);
send_data(0x54);send_data(0x00);send_data(0x67);send_data(0x00);send_data(0x7C);
send_data(0x00);send_data(0x8C);send_data(0x00);send_data(0x99);send_data(0x00);
send_data(0xA8);send_data(0x00);send_data(0xBA);send_data(0x00);send_data(0xC5);
send_data(0x00);send_data(0xCB);send_data(0x00);send_data(0xD3);send_data(0x00);
send_data(0xDA);send_data(0x00);send_data(0xE3);send_data(0x00);send_data(0xF3);
send_data(0x00);

send_cmd(0xE2);
send_data(0x01);send_data(0x00);send_data(0x05);send_data(0x00);send_data(0x0F);
send_data(0x00);send_data(0x2F);send_data(0x00);send_data(0x45);send_data(0x00);
send_data(0x54);send_data(0x00);send_data(0x67);send_data(0x00);send_data(0x7C);
send_data(0x00);send_data(0x8C);send_data(0x00);send_data(0x99);send_data(0x00);
send_data(0xA8);send_data(0x00);send_data(0xBA);send_data(0x00);send_data(0xC5);
send_data(0x00);send_data(0xCB);send_data(0x00);send_data(0xD3);send_data(0x00);
send_data(0xDA);send_data(0x00);send_data(0xE3);send_data(0x00);send_data(0xF3);
send_data(0x00);

send_cmd(0xE4);
send_data(0x01);send_data(0x00);send_data(0x05);send_data(0x00);send_data(0x0F);
send_data(0x00);send_data(0x2F);send_data(0x00);send_data(0x45);send_data(0x00);
send_data(0x54);send_data(0x00);send_data(0x67);send_data(0x00);send_data(0x7C);
send_data(0x00);send_data(0x8C);send_data(0x00);send_data(0x99);send_data(0x00);
send_data(0xA8);send_data(0x00);send_data(0xBA);send_data(0x00);send_data(0xC5);
send_data(0x00);send_data(0xCB);send_data(0x00);send_data(0xD3);send_data(0x00);
send_data(0xDA);send_data(0x00);send_data(0xE3);send_data(0x00);send_data(0xF3);
send_data(0x00);
                        
send_cmd(0xE1);
send_data(0x00);send_data(0x00);send_data(0x04);send_data(0x00);send_data(0x0F);
send_data(0x00);send_data(0x2F);send_data(0x00);send_data(0x45);send_data(0x00);
send_data(0x54);send_data(0x00);send_data(0x66);send_data(0x00);send_data(0x7D);
send_data(0x00);send_data(0x8B);send_data(0x00);send_data(0x99);send_data(0x00);
send_data(0xA8);send_data(0x00);send_data(0xBA);send_data(0x00);send_data(0xC5);
send_data(0x00);send_data(0xCB);send_data(0x00);send_data(0xD4);send_data(0x00);
send_data(0xD9);send_data(0x00);send_data(0xE3);send_data(0x00);send_data(0xF3);
send_data(0x00);

send_cmd(0xE3);
send_data(0x00);send_data(0x00);send_data(0x04);send_data(0x00);send_data(0x0F);
send_data(0x00);send_data(0x2F);send_data(0x00);send_data(0x45);send_data(0x00);
send_data(0x54);send_data(0x00);send_data(0x66);send_data(0x00);send_data(0x7D);
send_data(0x00);send_data(0x8B);send_data(0x00);send_data(0x99);send_data(0x00);
send_data(0xA8);send_data(0x00);send_data(0xBA);send_data(0x00);send_data(0xC5);
send_data(0x00);send_data(0xCB);send_data(0x00);send_data(0xD4);send_data(0x00);
send_data(0xD9);send_data(0x00);send_data(0xE3);send_data(0x00);send_data(0xF3);
send_data(0x00);

send_cmd(0xE5);
send_data(0x00);send_data(0x00);send_data(0x04);send_data(0x00);send_data(0x0F);
send_data(0x00);send_data(0x2F);send_data(0x00);send_data(0x45);send_data(0x00);
send_data(0x54);send_data(0x00);send_data(0x66);send_data(0x00);send_data(0x7D);
send_data(0x00);send_data(0x8B);send_data(0x00);send_data(0x99);send_data(0x00);
send_data(0xA8);send_data(0x00);send_data(0xBA);send_data(0x00);send_data(0xC5);
send_data(0x00);send_data(0xCB);send_data(0x00);send_data(0xD4);send_data(0x00);
send_data(0xD9);send_data(0x00);send_data(0xE3);send_data(0x00);send_data(0xF3);
send_data(0x00);

send_cmd(0xE6);
send_data(0x21);send_data(0x00);send_data(0x55);send_data(0x00);send_data(0x99);
send_data(0x00);send_data(0x77);send_data(0x00);send_data(0x77);send_data(0x00);
send_data(0x76);send_data(0x00);send_data(0x78);send_data(0x00);send_data(0x98);
send_data(0x00);send_data(0xBB);send_data(0x00);send_data(0x99);send_data(0x00);
send_data(0x66);send_data(0x00);send_data(0x54);send_data(0x00);send_data(0x45);
send_data(0x00);send_data(0x34);send_data(0x00);send_data(0x44);send_data(0x00);
send_data(0x34);send_data(0x00);

send_cmd(0xE7);send_data(0x21);send_data(0x00);send_data(0x55);send_data(0x00);
send_data(0x99);send_data(0x00);send_data(0x77);send_data(0x00);send_data(0x77);
send_data(0x00);send_data(0x76);send_data(0x00);send_data(0x78);send_data(0x00);
send_data(0x98);send_data(0x00);send_data(0xBB);send_data(0x00);send_data(0x99);
send_data(0x00);send_data(0x66);send_data(0x00);send_data(0x54);send_data(0x00);
send_data(0x45);send_data(0x00);send_data(0x34);send_data(0x00);send_data(0x44);
send_data(0x00);send_data(0x34);send_data(0x00);

send_cmd(0xE8);
send_data(0x21);send_data(0x00);send_data(0x55);send_data(0x00);send_data(0x99);
send_data(0x00);send_data(0x77);send_data(0x00);send_data(0x77);send_data(0x00);
send_data(0x76);send_data(0x00);send_data(0x78);send_data(0x00);send_data(0x98);
send_data(0x00);send_data(0xBB);send_data(0x00);send_data(0x99);send_data(0x00);
send_data(0x66);send_data(0x00);send_data(0x54);send_data(0x00);send_data(0x45);
send_data(0x00);send_data(0x34);send_data(0x00);send_data(0x44);send_data(0x00);
send_data(0x34);send_data(0x00);

//Manufacture Setting
send_cmd(0xE9);
send_data(0xAA);send_data(0x00);send_data(0x00);send_data(0x00); 

//Enter Page0
send_cmd(0x00);send_data(0xAA);

//Manufacture Test Setting
send_cmd(0xCF);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);
send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);send_data(0x00);
send_data(0x00);send_data(0x00);
 
send_cmd(0xF0);
send_data(0x00);send_data(0x50);send_data(0x00);send_data(0x00);send_data(0x00);
send_cmd(0xF1);send_data(0x01); 

send_cmd(0xC4);send_data(0x5d);

//Gate Setting
send_cmd(0xF9);
send_data(0x06);send_data(0x10);send_data(0x29);send_data(0x00);

//Code Lock
send_cmd(0xDF);send_data(0x10);
LCD_DelayMS(100);

//Display On
send_cmd(0x29);


//TE On
send_cmd(0x35);send_data(0x00);

send_cmd(0x44);
send_data(0x01);//00
send_data(0xde);//78

send_cmd(0x3A);
//send_data(0x55); 
send_data(0x66); 

send_cmd(0x2c);

printf("qiandan init end!.....\n");


if (0) {
	int i;
	for (i=0; i<320*480/2; i++)
	send_data(0xFF00);
	for (i=0; i< 320*480/2; i++)
	send_data(0x00FF);
}

LCD_DelayMS(100);	


#endif	


	return 0;
}

static int32_t nt35310_set_window(struct lcd_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("qiandan nt35310_set_window: %d, %d, %d, %d\n",left, top, right, bottom);
    
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
/*
	switch (self->direction) {
	case LCD_DIRECT_NORMAL:
	case LCD_DIRECT_ROT_270:
		break;
	case LCD_DIRECT_ROT_90:
	case LCD_DIRECT_MIR_H:
		break;
	case LCD_DIRECT_ROT_180:
	case LCD_DIRECT_MIR_HV:
		break;
	case LCD_DIRECT_MIR_V:
		break;
	default:
		LCD_PRINT("unknown lcd direction!\n");
		break;
	}

	self->info.mcu->ops->send_cmd(0x29);	   //Write display data
*/
	
	send_cmd(0x2C); //Write data	


	return 0;
}

static int32_t nt35310_invalidate(struct lcd_spec *self)
{
	LCD_PRINT("nt35310_invalidate\n");

	return self->ops->lcd_set_window(self, 0, 0, 
			self->width - 1, self->height - 1);
	
}

static int32_t nt35310_invalidate_rect(struct lcd_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
	LCD_PRINT("nt35310_invalidate_rect \n");
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

static int32_t nt35310_set_direction(struct lcd_spec *self, uint16_t direction)
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

static int32_t nt35310_enter_sleep(struct lcd_spec *self, uint8_t is_sleep)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
//printk("cj nt35310_enter_sleep = %d \n",is_sleep);

	if(is_sleep) {
		//Sleep In
		send_cmd(0x28);
		LCD_DelayMS(120); 
		send_cmd(0x10);
		LCD_DelayMS(120); 
	}
	else {
		//Sleep Out
		send_cmd(0x11);
		LCD_DelayMS(120); 
		send_cmd(0x29);
		LCD_DelayMS(120); 
	}
	
	return 0;
}

static int32_t nt35310_read_id(struct lcd_spec *self)
{

	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
	Read_data read_data = self->info.mcu->ops->read_data;

#if 0
	int32_t   chip_id = 0;

	send_cmd(0xB9);
	send_data(0xFF);
	send_data(0x83);
	send_data(0x57);

	send_cmd(0xD0);
	chip_id = read_data();
	LCD_PRINT("nt35310_read_id   3rdparty chip_id = %d!\n", chip_id);
	//return chip_id;
#endif	
uint32_t dummy1,dummy2,dummy3,dummy4;

send_cmd(0xED);
send_data(0x01);
send_data(0xFE);

send_cmd(0xD4);

dummy1=read_data();
dummy2=read_data();
dummy3=read_data();
dummy4=read_data();

printk("dummy1:0x%x,dummy2:0x%x,dummy3:0x%x,dummy4:0x%x\n",dummy1,dummy2,dummy3,dummy4);
printk("%d\n", ((dummy3<<8)|dummy4));

printk("qiandan %s\n", __func__);
return (dummy3<<8)|dummy4;

#if 0

	uint32_t dummy1,dummy2,dummy3,dummy4;
	self->info.mcu->ops->send_cmd(0xD3);

	dummy1 = (self->info.mcu->ops->read_data())&0xff;
	dummy2 = (self->info.mcu->ops->read_data())&0xff;
	dummy3= (self->info.mcu->ops->read_data())&0xff;
	dummy4= (self->info.mcu->ops->read_data())&0xff;

	printk("dummy1:0x%x,dummy2:0x%x,dummy3:0x%x,dummy4:0x%x\n",dummy1,dummy2,dummy3,dummy4);
	printk("%d\n", ((dummy3<<8)|dummy4));
	return (dummy3<<8)|dummy4;
#endif


	//return 0x57;
}

static struct lcd_operations lcd_nt35310_operations = {
	.lcd_init = nt35310_init,
	.lcd_set_window = nt35310_set_window,
	.lcd_invalidate_rect= nt35310_invalidate_rect,
	.lcd_invalidate = nt35310_invalidate,
	.lcd_set_direction = nt35310_set_direction,
	.lcd_enter_sleep = nt35310_enter_sleep,
	.lcd_readid = nt35310_read_id,
};

static struct timing_mcu lcd_nt35310_timing[] = {
[LCD_REGISTER_TIMING] = {                    // read/write register timing
		.rcss = 25,//15,  // 15ns
		.rlpw = 70,//60,
		.rhpw = 70,//60,
		.wcss = 10,
		.wlpw = 50,//35,
		.whpw = 50,//35,
	},
[LCD_GRAM_TIMING] = {                    // read/write gram timing
		.rcss = 25,//15,  // 15ns
		.rlpw = 70,//60,
		.rhpw = 70,//60,
		.wcss = 0,//10,
		.wlpw = 35,//50,//35,
		.whpw = 35,//50,//35,
	},
};

static struct info_mcu lcd_nt35310_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 18,
	.timing = lcd_nt35310_timing,
	.ops = NULL,
};

struct lcd_spec lcd_panel_nt35310 = {
	.width = 320,
	.height = 480,
	.mode = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_nt35310_info},
	.ops = &lcd_nt35310_operations,
};
