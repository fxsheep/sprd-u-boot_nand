/* drivers/video/sc8810/lcd_hx8369.c
 *
 * Support for hx8369 LCD device
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

#include <linux/kernel.h>
#include <linux/delay.h>

#include "lcd.h"

//#define  LCD_DEBUG
#ifdef LCD_DEBUG
#define LCD_PRINT printk
#else
#define LCD_PRINT(...)
#endif

static int32_t hx8369_init(struct lcd_spec *self)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("hx8369_init\n");

	send_cmd(0xB9); // SET password 
	send_data(0xFF); 
	send_data(0x83); 
	send_data(0x69); 

	send_cmd(0xB1);  //Set Power 
	send_data(0x9D);  //9D 
	send_data(0x00); //00
	send_data(0x34); //34
	send_data(0x07); //07
	send_data(0x00); //00 
	send_data(0x0B); //0B
	send_data(0x0B); //0B 
	send_data(0x1A); //1A
	send_data(0x22); //22
	send_data(0x3F); //3F 
	send_data(0x3F); //3F
	send_data(0x01); //01 
	send_data(0x23); //23
	send_data(0x01); //01
	send_data(0xE6); //E6
	send_data(0xE6); //E6
	send_data(0xE6); //E6
	send_data(0xE6); //E6
	send_data(0xE6); //E6

	send_cmd(0xB2);  // SET Display  480x800 
	send_data(0x00); //00 
	send_data(0x20);    
	send_data(0x03); //03 
	send_data(0x03); //03   
	send_data(0x70);   
	send_data(0x00); 
	send_data(0xFF); 
	send_data(0x00);   
	send_data(0x00);   
	send_data(0x00); 
	send_data(0x00);   
	send_data(0x03);   
	send_data(0x03); 
	send_data(0x00); 
	send_data(0x01); 

	send_cmd(0xB4);  // SET Display 480x800 
	send_data(0x01); 
	send_data(0x18); 
	send_data(0x80); 
	send_data(0x06); 
	send_data(0x02); 

	//send_cmd(0xB0);//OSC  
	//send_data(0x00);   
	//send_data(0x0B);//05   42HZ  07 50HZ  0B 100% 67HZ


	send_cmd(0xB6);  // SET VCOM 
	send_data(0x4A); 
	send_data(0x4A); 

	send_cmd(0xD5);  // SET GIP 
	send_data(0x00); 
	send_data(0x03); 
	send_data(0x03); 
	send_data(0x00); 
	send_data(0x01); 
	send_data(0x02); //04

	send_data(0x28); 
	send_data(0x70); 
	send_data(0x11); 
	send_data(0x13); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x40); 
	send_data(0x06); 
	send_data(0x51); 
	send_data(0x07); 
	send_data(0x00); 
	send_data(0x00); 
	send_data(0x41); 
	send_data(0x06); 
	send_data(0x50); 
	send_data(0x07); 
	send_data(0x07); 
	send_data(0x0F); 
	send_data(0x04); 
	send_data(0x00); 

	send_cmd(0xE0);  // Set Gamma 
	send_data(0x00); 
	send_data(0x01); 
	send_data(0x04); 
	send_data(0x23); 
	send_data(0x22); 
	send_data(0x3F); 
	send_data(0x13); 
	send_data(0x39); 
	send_data(0x06); 
	send_data(0x0B); 
	send_data(0x0E); 
	send_data(0x12); 
	send_data(0x15); 
	send_data(0x13); 
	send_data(0x15); 
	send_data(0x13); 
	send_data(0x1B); 
	send_data(0x00); 
	send_data(0x01); 
	send_data(0x04); 
	send_data(0x23); 
	send_data(0x22); 
	send_data(0x3F); 
	send_data(0x13); 
	send_data(0x39); 
	send_data(0x06); 
	send_data(0x0B); 
	send_data(0x0E); 
	send_data(0x12); 
	send_data(0x15); 
	send_data(0x13); 
	send_data(0x15); 
	send_data(0x13); 
	send_data(0x1B); 
	send_cmd(0x35);   // TE on
	send_data(0x00);                      
	//send_cmd(0x34); // TE off

	send_cmd(0x3A);   // set CSEL 
	send_data(0x07);  // CSEL=0x06, 16bit-color CSEL=0x06, 18bit-color CSEL=0x07, 24bit-color 

	/*
	//16bit 65K
		send_cmd(0x2D);        //Look up table//                   
	for(i=0;i<64;i++) 
			send_data(8*i);//RED                                               
	for(i=0;i<64;i++) 
			send_data(4*i);//Green                                         
	for(i=0;i<64;i++) 
			send_data(8*i);//Blue 
	*/
	
	/*
	//18bit 262K
		send_cmd(0x2D);        //Look up table//                   
	for(i=0;i<64;i++) 
			send_data(4*i);//RED                                               
	for(i=0;i<64;i++) 
			send_data(4*i);//Green                                         
	for(i=0;i<64;i++) 
			send_data(4*i);//Blue 
	*/
	//24 bit don't need to set 2DH 

	send_cmd(0x11); //Sleep Out 
	mdelay(120); //120ms
	
	if (0) { //  for test the lcd
		int i;
		send_cmd(0x2C); //Write data
		for (i = 0; i < 480*800/3; i++)
			send_data(0xff);
		for (i = 0; i < 480*800/3; i++)
			send_data(0xff00);
       	for (i = 0; i < 480*800/3; i++)
			send_data(0xff0000);
	}
	send_cmd(0x29); //Display On 
	mdelay(120); //120ms
	send_cmd(0x2C); //Write data
	//mdelay(120); //120ms
	LCD_PRINT("hx8369_init: end\n");


	return 0;
}

static int32_t hx8369_set_window(struct lcd_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("hx8369_set_window\n");
    
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

	return 0;
}


static int32_t hx8369_invalidate(struct lcd_spec *self)
{
	LCD_PRINT("hx8369_invalidate\n");

	return self->ops->lcd_set_window(self, 0, 0, 
			self->width-1, self->height-1);
	
}

static int32_t hx8369_invalidate_rect(struct lcd_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	LCD_PRINT("hx8369_invalidate_rect \n");

	return self->ops->lcd_set_window(self, left, top, 
			right, bottom);
}

static int32_t hx8369_set_direction(struct lcd_spec *self, uint16_t direction)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("hx8369_set_direction\n");
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

	self->direction = direction;
	
	return 0;
}

static int32_t hx8369_enter_sleep(struct lcd_spec *self, uint8_t is_sleep)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;

	if(is_sleep) {
		//Sleep In
		send_cmd(0x28);
		mdelay(120); 
		send_cmd(0x10);
		mdelay(120); 
	}
	else {
		//Sleep Out
		send_cmd(0x11);
		mdelay(120); 
		send_cmd(0x29);
		mdelay(120); 
	}
	return 0;
}

static int32_t hx8369_read_id(struct lcd_spec *self)
{
	int32_t id  = 0;
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Read_data read_data = self->info.mcu->ops->read_data;

	//Get ID
	send_cmd(0xD0);
	read_data(); //Dummy read
	id = read_data(); //The value is 0x69h
	read_data(); //Dummy read

	return id;
}

static struct lcd_operations lcd_hx8369_operations = {
	.lcd_init            = hx8369_init,
	.lcd_set_window      = hx8369_set_window,
	.lcd_invalidate      = hx8369_invalidate,
	.lcd_invalidate_rect = hx8369_invalidate_rect,
	.lcd_set_direction   = hx8369_set_direction,
	.lcd_enter_sleep     = hx8369_enter_sleep,
	.lcd_readid          = hx8369_read_id,
};

static struct timing_mcu lcd_hx8369_timing = {
	.rcss = 360,
	.rlpw = 90,
	.rhpw = 360,
	.wcss = 40,
	.wlpw = 160,
	.whpw = 160,
};

static struct info_mcu lcd_hx8369_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 24,
	.timing = &lcd_hx8369_timing,
	.ops = NULL,
};

struct lcd_spec lcd_panel_hx8369 = {
	.width = 480,
	.height = 800,
	.mode = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_hx8369_info},
	.ops = &lcd_hx8369_operations,
};

