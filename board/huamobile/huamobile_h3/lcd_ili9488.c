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
static int32_t ili9488_init(struct lcd_spec *self)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("ili9488_init\n");
	/* reset the lcd */
	
	self->ops->lcd_reset(self);
	LCD_DelayMS(120);

	//************* Start Initial Sequence **********//

	{
		printf("ili9488 new code\n");
		send_cmd(0x36);
		send_data(0x08);
		
		send_cmd(0x3A);
		send_data(0x66);
		
		send_cmd(0xB0);
		send_data(0x80);
		
		send_cmd(0xB1);
		send_data(0xA0);
		
		send_cmd(0xB4);
		send_data(0x02);
		
		send_cmd(0xB6);
		send_data(0x02);
		send_data(0x22);
		send_data(0x3B);
		
		send_cmd(0xC0);
		send_data(0x11);
		send_data(0x09);
		
		send_cmd(0xC1);
		send_data(0x31);
		
		send_cmd(0xC5);
		send_data(0x00);
		send_data(0x24);
		send_data(0x80);
		

#if 1
		//gamma 2.2
		send_cmd(0XE0);
		send_data(0x00);
		send_data(0x07);
		send_data(0x12);
		send_data(0x0B);
		send_data(0x18);

		send_data(0x0B);
		send_data(0x3F);
		send_data(0x9B);
		send_data(0x4B);
		send_data(0x0B);

		send_data(0x0F);
		send_data(0x0B);
		send_data(0x15);
		send_data(0x17);
		send_data(0x0F);

		send_cmd(0XE1);
		send_data(0x00);
		send_data(0x16);
		send_data(0x1B);
		send_data(0x02);
		send_data(0x0f);

		send_data(0x06);
		send_data(0x34);
		send_data(0x46);
		send_data(0x48);
		send_data(0x04);

		send_data(0x0D);
		send_data(0x0D);
		send_data(0x35);
		send_data(0x36);
		send_data(0x0F);
#else
		//gamma 2.0
        send_cmd(0XE0);
		send_data(0x00);
		send_data(0x0a);
		send_data(0x16);
		send_data(0x0a);
		send_data(0x18);

		send_data(0x0b);
		send_data(0x45);
		send_data(0x7a);
		send_data(0x50);
		send_data(0x08);

		send_data(0x0b);
		send_data(0x09);
		send_data(0x1d);
		send_data(0x1d);
		send_data(0x0F);

		send_cmd(0XE1);
		send_data(0x00);
		send_data(0x1c);
		send_data(0x1f);
		send_data(0x02);
		send_data(0x0f);

		send_data(0x05);
		send_data(0x34);
		send_data(0x34);
		send_data(0x46);
		send_data(0x02);

		send_data(0x09);
		send_data(0x08);
		send_data(0x32);
		send_data(0x38);
		send_data(0x0F);
#endif
		
		
		send_cmd(0xE9);
		send_data(0x00);
		
		send_cmd(0xF7);
		send_data(0xA9);
		send_data(0x51);
		send_data(0x2C);
		send_data(0x82);
		
		send_cmd(0x35);
		send_data(0x00);
		
		send_cmd(0x44);
		send_data(0x00);//0x01
		send_data(0x00);//0x2c
	
		send_cmd(0x3A);
		send_data(0x66);
	
		
		send_cmd(0x11);
		LCD_DelayMS(120);
		
		send_cmd(0x29);
	
		LCD_DelayMS(120);
		send_cmd(0x2c);
		
		
	
	
	
		}


	return 0;
}

static int32_t ili9488_set_window(struct lcd_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
LCD_PRINT("hx9488_invalidate_rect : (%d, %d, %d, %d)\n",left, top, right, bottom);

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

static int32_t ili9488_invalidate(struct lcd_spec *self)
{
	LCD_PRINT("ili9488_invalidate width:%d,height:%d\n",self->width,self->height);

	return self->ops->lcd_set_window(self, 0, 0, 
			self->width-1, self->height-1);
	
}

static int32_t ili9488_invalidate_rect(struct lcd_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{

	LCD_PRINT("hx9488_invalidate_rect : (%d, %d, %d, %d)\n",left, top, right, bottom);

	return self->ops->lcd_set_window(self, left, top, 
			right, bottom);
}



static int32_t ili9488_set_direction(struct lcd_spec *self, uint16_t direction)
{
	LCD_PRINT("ili9488_set_direction\n");

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

static int32_t ili9488_enter_sleep(struct lcd_spec *self, uint8_t is_sleep)
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

static uint32_t ili9488_readid(struct lcd_spec *self)
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

static struct lcd_operations lcd_ili9488_operations = {
	.lcd_init = ili9488_init,
	.lcd_set_window = ili9488_set_window,
	.lcd_invalidate = ili9488_invalidate,
	.lcd_invalidate_rect = ili9488_invalidate_rect,
	.lcd_set_direction = ili9488_set_direction,
	.lcd_enter_sleep     = ili9488_enter_sleep,
	.lcd_readid          = ili9488_readid,
};

static struct timing_mcu lcd_ili9488_timing[] = {
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

static struct info_mcu lcd_ili9488_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 18,
	.timing = lcd_ili9488_timing,
	.ops = NULL,
};

struct lcd_spec lcd_panel_ili9488 = {
	.width = LCD_WIDTH,
	.height = LCD_HEIGHT,
	.mode = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_ili9488_info},
	.ops = &lcd_ili9488_operations,
};
