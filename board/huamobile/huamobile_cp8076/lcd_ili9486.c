/* drivers/video/sc8800g/sc8800g_lcd_ili9328.c
 *
 * Support for ILI9328 LCD device
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

#define  LCD_DEBUG
#ifdef LCD_DEBUG
#define LCD_PRINT printk
#else
#define LCD_PRINT(...)
#endif

#define LCD_WIDTH  320
#define LCD_HEIGHT 480
static int32_t ili9486_init(struct lcd_spec *self)
{
	LCD_PRINT("ili9486_init\n");
	/* reset the lcd */
	
	self->ops->lcd_reset(self);
	LCD_DelayMS(120);

	//************* Start Initial Sequence **********//
	self->info.mcu->ops->send_cmd(0xF1);
	self->info.mcu->ops->send_data(0x36);
	self->info.mcu->ops->send_data(0x04);
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x3C);
	self->info.mcu->ops->send_data(0x0F);
	self->info.mcu->ops->send_data(0x8F);

	self->info.mcu->ops->send_cmd(0xF2);
	self->info.mcu->ops->send_data(0x18);
	self->info.mcu->ops->send_data(0xA3);
	self->info.mcu->ops->send_data(0x12);
	self->info.mcu->ops->send_data(0x02);
	self->info.mcu->ops->send_data(0xB2);
	self->info.mcu->ops->send_data(0x12);
	self->info.mcu->ops->send_data(0xFF);
	self->info.mcu->ops->send_data(0x10);
	self->info.mcu->ops->send_data(0x00);

	self->info.mcu->ops->send_cmd(0XF8);
	self->info.mcu->ops->send_data(0x21);
	self->info.mcu->ops->send_data(0x04);

	self->info.mcu->ops->send_cmd(0xF9);
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x08);

	self->info.mcu->ops->send_cmd(0x36);
	self->info.mcu->ops->send_data(0x08);

	self->info.mcu->ops->send_cmd(0x3A);
	self->info.mcu->ops->send_data(0x66); //18bit write 0x66 & 16bit wrire 0x55

	self->info.mcu->ops->send_cmd(0xB1);
	self->info.mcu->ops->send_data(0xB0);
	self->info.mcu->ops->send_data(0x11);

	self->info.mcu->ops->send_cmd(0xB4);
	self->info.mcu->ops->send_data(0x02);

	self->info.mcu->ops->send_cmd(0xB6);
	self->info.mcu->ops->send_data(0x02);
	self->info.mcu->ops->send_data(0x22);
	self->info.mcu->ops->send_data(0x3B);

	self->info.mcu->ops->send_cmd(0xC0);
	self->info.mcu->ops->send_data(0x0a);
	self->info.mcu->ops->send_data(0x0a);

	self->info.mcu->ops->send_cmd(0xC1);
	self->info.mcu->ops->send_data(0x44);


	self->info.mcu->ops->send_cmd(0XC5);
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x50);

	self->info.mcu->ops->send_cmd(0xE0);
	self->info.mcu->ops->send_data(0x0F);
	self->info.mcu->ops->send_data(0x18);
	self->info.mcu->ops->send_data(0x18);
	self->info.mcu->ops->send_data(0x0b);
	self->info.mcu->ops->send_data(0x0e);
	self->info.mcu->ops->send_data(0x09);
	self->info.mcu->ops->send_data(0x48);
	self->info.mcu->ops->send_data(0xa9);
	self->info.mcu->ops->send_data(0x3a);//38
	self->info.mcu->ops->send_data(0x0a);
	self->info.mcu->ops->send_data(0x14);
	self->info.mcu->ops->send_data(0x06);
	self->info.mcu->ops->send_data(0x0d);
	self->info.mcu->ops->send_data(0x09);
	self->info.mcu->ops->send_data(0x07);

	self->info.mcu->ops->send_cmd(0XE1);
	self->info.mcu->ops->send_data(0x08);
	self->info.mcu->ops->send_data(0x36);
	self->info.mcu->ops->send_data(0x32);
	self->info.mcu->ops->send_data(0x09);
	self->info.mcu->ops->send_data(0x0a);
	self->info.mcu->ops->send_data(0x04);
	self->info.mcu->ops->send_data(0x49);//47
	self->info.mcu->ops->send_data(0x65);
	self->info.mcu->ops->send_data(0x37);
	self->info.mcu->ops->send_data(0x06);
	self->info.mcu->ops->send_data(0x10);
	self->info.mcu->ops->send_data(0x04);
	self->info.mcu->ops->send_data(0x27);//29
	self->info.mcu->ops->send_data(0x27);
	self->info.mcu->ops->send_data(0x02);

	self->info.mcu->ops->send_cmd(0x35);
	self->info.mcu->ops->send_data(0x00);

	self->info.mcu->ops->send_cmd(0x44);
	self->info.mcu->ops->send_data(0x00);
	self->info.mcu->ops->send_data(0x78);

	self->info.mcu->ops->send_cmd(0x11);
	LCD_DelayMS(120);
	self->info.mcu->ops->send_cmd(0x29);
	
	self->info.mcu->ops->send_cmd(0x2C);

//huamobile add 
#if 0
	int i;
	for(i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++)
		self->info.mcu->ops->send_data(0x0);
#endif
//huamobile add 

	return 0;
}

static int32_t ili9486_set_window(struct lcd_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
LCD_PRINT("hx9486_invalidate_rect : (%d, %d, %d, %d)\n",left, top, right, bottom);

	self->info.mcu->ops->send_cmd(0x2a);       //column address set
    	self->info.mcu->ops->send_data(left>>8); //start address 0x00,0x00
    	self->info.mcu->ops->send_data(left&0xff);    //start address 0x00,0x00
    	self->info.mcu->ops->send_data(right>>8);   //end address 0x00,0x00
    	self->info.mcu->ops->send_data(right&0xff);      //end address 0x00,0x7f 128
    
    	self->info.mcu->ops->send_cmd(0x2b);               //row address set
    	self->info.mcu->ops->send_data(top>>8); //start address 0x00,0x00
    	self->info.mcu->ops->send_data(top&0xff);    //start address 0x00,0x00
    	self->info.mcu->ops->send_data(bottom>>8);   //end address 0x00,0x00
    	self->info.mcu->ops->send_data(bottom&0xff);      //end address 0x00,0x9f 160
    

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
  	self->info.mcu->ops->send_cmd(0x29);       //Write display data
	
    	self->info.mcu->ops->send_cmd(0x2C);       //Write display data
	return 0;
}

static int32_t ili9486_invalidate(struct lcd_spec *self)
{
	LCD_PRINT("ili9486_invalidate width:%d,height:%d\n",self->width,self->height);

	return self->ops->lcd_set_window(self, 0, 0, 
			self->width-1, self->height-1);
	
}

static int32_t ili9486_invalidate_rect(struct lcd_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{

	LCD_PRINT("hx9486_invalidate_rect : (%d, %d, %d, %d)\n",left, top, right, bottom);

	return self->ops->lcd_set_window(self, left, top, 
			right, bottom);
}



static int32_t ili9486_set_direction(struct lcd_spec *self, uint16_t direction)
{
	LCD_PRINT("ili9486_set_direction\n");

	switch (direction) {
	case LCD_DIRECT_NORMAL:
		break;
	case LCD_DIRECT_ROT_90:
		break;
	case LCD_DIRECT_ROT_180:
	case LCD_DIRECT_MIR_HV:
		break;
	case LCD_DIRECT_ROT_270:
		break;
	case LCD_DIRECT_MIR_H:
		break;
	case LCD_DIRECT_MIR_V:
		break;
	default:
		LCD_PRINT("unknown lcd direction!\n");
		direction = LCD_DIRECT_NORMAL;
		break;
	}

	self->direction = direction;
	
	return 0;
}

static int32_t ili9486_enter_sleep(struct lcd_spec *self, uint8_t is_sleep)
{
	if(is_sleep) {
	self->info.mcu->ops->send_cmd(0x28);
	LCD_DelayMS(10);
	self->info.mcu->ops->send_cmd(0x10); // Set_address_mode
	LCD_DelayMS(120);

	}
	else{
	self->info.mcu->ops->send_cmd(0x11); // Set_address_mode
	LCD_DelayMS(120);
	self->info.mcu->ops->send_cmd(0x29);
	}
	return 0;
}

static uint32_t ili9486_readid(struct lcd_spec *self)
{
	uint32_t dummy1,dummy2,dummy3,dummy4;
	self->info.mcu->ops->send_cmd(0xD3);

	dummy1 = (self->info.mcu->ops->read_data())&0xff;
	dummy2 = (self->info.mcu->ops->read_data())&0xff;
	dummy3= (self->info.mcu->ops->read_data())&0xff;
	dummy4= (self->info.mcu->ops->read_data())&0xff;

	printk("dummy1:0x%x,dummy2:0x%x,dummy3:0x%x,dummy4:0x%x\n",dummy1,dummy2,dummy3,dummy4);
	return (dummy3<<8)|dummy4;
}

static struct lcd_operations lcd_ili9486_operations = {
	.lcd_init = ili9486_init,
	.lcd_set_window = ili9486_set_window,
	.lcd_invalidate = ili9486_invalidate,
	.lcd_invalidate_rect = ili9486_invalidate_rect,
	.lcd_set_direction = ili9486_set_direction,
	.lcd_enter_sleep     = ili9486_enter_sleep,
	.lcd_readid          = ili9486_readid,
};

static struct timing_mcu lcd_ili9486_timing[] = {
	[LCD_REGISTER_TIMING] = {					 // read/write register timing
			.rcss = 15,  // 15ns
			.rlpw = 60,
			.rhpw = 60,
			.wcss = 10,
			.wlpw = 35,
			.whpw = 35,
		},
	[LCD_GRAM_TIMING] = {					 // read/write gram timing
			.rcss = 15,  // 15ns
			.rlpw = 60,
			.rhpw = 60,
			.wcss = 10,
			.wlpw = 35,
			.whpw = 35,
		},
};

static struct info_mcu lcd_ili9486_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 18,
	.timing = lcd_ili9486_timing,
	.ops = NULL,
};

struct lcd_spec lcd_panel_ili9486 = {
	.width = LCD_WIDTH,
	.height = LCD_HEIGHT,
	.mode = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_ili9486_info},
	.ops = &lcd_ili9486_operations,
};
