/* drivers/video/sc8800g/sc8800g_lcd_ili9806.c
 *
 * Support for ili9806 LCD device
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



static int32_t ili9806_init(struct lcd_spec *self)
{
	u32 test1,test2;
	u32 ret;

	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("ili9806_init\n");

	self->ops->lcd_reset(self);
	LCD_DelayMS(120);


	printf("ili9806_init gpio 42 = %d\n", ret);

	{ 
		
		//************* Start Initial Sequence **********// 
		send_cmd(0xFF); 	 // EXTC Command Set enable register 
		send_data(0xFF); 
		send_data(0x98); 
		send_data(0x06); 
		 
		send_cmd(0xBA); 	// SPI Interface Setting 
		send_data(0xE0); 
		 
		send_cmd(0xB4);    
		send_data(0x02); 
		send_data(0x02); 
		send_data(0x02); 
			
		send_cmd(0xBC); 	// GIP 1 
		send_data(0x03); 
		send_data(0x0F); 
		send_data(0x63); 
		send_data(0x33); 
		send_data(0x01); 
		send_data(0x01); 
		send_data(0x1B); 
		send_data(0x11); 
		send_data(0x38); 
		send_data(0x73); 
		send_data(0xFF); 
		send_data(0xFF); 
		send_data(0x0A); 
		send_data(0x0A); 
		send_data(0x05); 
		
		send_data(0x00); 
		send_data(0xFF); 
		send_data(0xE2); 
		send_data(0x01); 
		send_data(0x00); 
		send_data(0xC1); 
		 
		send_cmd(0xBD); 	// GIP 2 
		send_data(0x01); 
		send_data(0x23); 
		send_data(0x45); 
		send_data(0x67); 
		send_data(0x01); 
		send_data(0x23); 
		send_data(0x45); 
		send_data(0x67); 
		 
		send_cmd(0xBE); 	// GIP 3 
		send_data(0x00); 
		send_data(0x22); 
		send_data(0x27); 
		send_data(0x6A); 
		send_data(0xBC); 
		send_data(0xD8); 
		send_data(0x92); 
		send_data(0x22); 
		send_data(0x22); 
		 
		send_cmd(0xC7); 	// Vcom 
		send_data(0x64);
		
		send_cmd(0xED); 	// EN_volt_reg 
		send_data(0x7F); 
		send_data(0x0F); 
		send_data(0x00); 
		 
		send_cmd(0xC0); 	// Power Control 1 
		send_data(0x63); 
		send_data(0x0B); 
		send_data(0x02); 
		  
		send_cmd(0xFC); 	// LVGL 
		send_data(0x08); 
		 
		send_cmd(0xDF); 	// Engineering Setting 
		send_data(0x00); 
		send_data(0x00); 
		send_data(0x00); 
		send_data(0x00); 
		send_data(0x00); 
		send_data(0x20); 
		   
		send_cmd(0xF3); 	 // DVDD Voltage Setting 
		send_data(0x74); 
		 
		send_cmd(0xB4); 	 // Display Inversion Control 
		send_data(0x00); 
		send_data(0x00); 
		send_data(0x00); 
		 
		send_cmd(0xF7); 	 // 480x800 
		send_data(0x82); 
		  
		send_cmd(0xB1); 	 // Frame Rate 
		send_data(0x00); 
		send_data(0x12); 
		send_data(0x13); 
		 
		send_cmd(0xF1); 	 // Panel Timing Control 
		send_data(0x29); 
		send_data(0x8A); 
		send_data(0x07); 
		send_cmd(0xF2); 	 //Panel Timing Control 
		send_data(0x40); 
		send_data(0xD0); 
		send_data(0x50); 
		send_data(0x28); 
		send_cmd(0xC1); 	// Power Control 2 
		send_data(0x17); 
		send_data(0x60); 
		send_data(0x60); 
		send_data(0x20); 
		send_cmd(0xE0); 
		send_data(0x00);  //P1 
		send_data(0x16);  //P2 
		send_data(0x21);  //P3 
		send_data(0x0F);  //P4 
		send_data(0x10);  //P5 
		send_data(0x15);  //P6 
		send_data(0x07);  //P7 
		send_data(0x06);  //P8 
		send_data(0x05);  //P9 
		send_data(0x09);  //P10 
		send_data(0x07);  //P11 
		send_data(0x0F);  //P12 
		send_data(0x0F);  //P13 
		send_data(0x35);  //P14 
		send_data(0x31);  //P15 
		send_data(0x00);  //P16 
		 
		send_cmd(0xE1); 
		send_data(0x00);  //P1 
		send_data(0x16);  //P2 
		send_data(0x19);  //P3 
		send_data(0x0C);  //P4 
		send_data(0x0F);  //P5 
		send_data(0x10);  //P6 
		send_data(0x06);  //P7 
		send_data(0x07);  //P8 
		send_data(0x04);  //P9 
		send_data(0x08);  //P10 
		send_data(0x08);  //P11 
		send_data(0x0A);  //P12 
		send_data(0x0B);  //P13 
		send_data(0x24);  //P14 
		send_data(0x20);  //P15 
		send_data(0x00);  //P16 
		 
		send_cmd(0x35);
		send_data(0x00); 

		send_cmd(0x44); 
		send_data(0x01);
		send_data(0xe0);
		
		send_cmd(0x3A);
		send_data(0x77);
					 
		send_cmd(0x11); 	  
		LCD_DelayMS(120); 
		send_cmd(0x29);
		LCD_DelayMS(120);		 
		send_cmd(0x2c); 
			 
				   
	}


	printf("qiandan init end!.....\n");

	LCD_DelayMS(100);	

	return 0;
}

static int32_t ili9806_set_window(struct lcd_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("qiandan ili9806_set_window: %d, %d, %d, %d\n",left, top, right, bottom);
	
	self->info.mcu->ops->send_cmd(0x2a);
    self->info.mcu->ops->send_data(left>>8);
    self->info.mcu->ops->send_data(left&0xff);
    self->info.mcu->ops->send_data(right>>8);
    self->info.mcu->ops->send_data(right&0xff);
    
    self->info.mcu->ops->send_cmd(0x2b); 
    self->info.mcu->ops->send_data(top>>8);
    self->info.mcu->ops->send_data(top&0xff);
    self->info.mcu->ops->send_data(bottom>>8);
    self->info.mcu->ops->send_data(bottom&0xff);
  	self->info.mcu->ops->send_cmd(0x29);
	
    self->info.mcu->ops->send_cmd(0x2C);


	return 0;
}

static int32_t ili9806_invalidate(struct lcd_spec *self)
{
	LCD_PRINT("ili9806_invalidate\n");

	return self->ops->lcd_set_window(self, 0, 0, 
			self->width - 1, self->height - 1);
	
}

static int32_t ili9806_invalidate_rect(struct lcd_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
	LCD_PRINT("ili9806_invalidate_rect \n");

	return self->ops->lcd_set_window(self, left, top, 
			right, bottom);
}

static int32_t ili9806_set_direction(struct lcd_spec *self, uint16_t direction)
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

static int32_t ili9806_enter_sleep(struct lcd_spec *self, uint8_t is_sleep)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;

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
		LCD_DelayMS(200); 
		send_cmd(0x29);
		LCD_DelayMS(120); 
	}

	return 0;
}

static int32_t ili9806_read_id(struct lcd_spec *self)
{

	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
	Read_data read_data = self->info.mcu->ops->read_data;

	uint32_t dummy1,dummy2,dummy3,dummy4;
	int i = 0;
	for(i=0; i<5; i++) {
		send_cmd(0xd3);
		dummy1=read_data()&0xFF;
		dummy2=read_data()&0xFF;
		dummy3=read_data()&0xFF;
		dummy4=read_data()&0xFF;


		printf("%s dummy1:0x%x,dummy2:0x%x,dummy3:0x%x,dummy4:0x%x\n",__func__, dummy1, dummy2, dummy3, dummy4);
		if((dummy3==0x98) && (dummy4==0x6))
			break;
	}
	return (dummy3<<8)|dummy4;//0x9806

}

static struct lcd_operations lcd_ili9806_operations = {
	.lcd_init = ili9806_init,
	.lcd_set_window = ili9806_set_window,
	.lcd_invalidate_rect= ili9806_invalidate_rect,
	.lcd_invalidate = ili9806_invalidate,
	.lcd_set_direction = ili9806_set_direction,
	.lcd_enter_sleep = ili9806_enter_sleep,
	.lcd_readid = ili9806_read_id,
};

static struct timing_mcu lcd_ili9806_timing[] = {
	[LCD_REGISTER_TIMING] = {					 // read/write register timing
			.rcss = 25,//15,  // 15ns
			.rlpw = 70,//60,90	70
			.rhpw = 70,//60,90	70
			.wcss = 10,//20 20	10
			.wlpw = 50,//35,70	 50
			.whpw = 50,//35,70	 50
		},
	[LCD_GRAM_TIMING] = {					 // read/write gram timing
			.rcss = 25,//15,  // 15ns
			.rlpw = 70,//60,90	70
			.rhpw = 70,//60,90	70
			.wcss = 0,//20
			.wlpw = 70,//35,70	15
			.whpw = 70,//35,70	24

	},
};

static struct info_mcu lcd_ili9806_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 24,
	.timing = lcd_ili9806_timing,
	.ops = NULL,
};

struct lcd_spec lcd_panel_ili9806 = {
	.width = 480,
	.height = 800,
	.mode = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_ili9806_info},
	.ops = &lcd_ili9806_operations,
};
