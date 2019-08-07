/* drivers/video/sc8800g/sc8800g_lcd_ili9805.c
 *
 * Support for ili9805 LCD device
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



static int32_t ili9805_init(struct lcd_spec *self)
{
	u32 test1,test2;
	u32 ret;

	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("ili9805_init\n");

	self->ops->lcd_reset(self);
	LCD_DelayMS(120);


	printf("ili9805_init gpio 42 = %d\n", ret);

#if 0
		{

		//************* Start Initial Sequence **********// 
		send_cmd(0XFF);  //EXTC command Password 
		send_data(0xFF); 
		send_data(0x98); 
		send_data(0x05); 
		send_cmd(0XFD);  // for PFM setting (base on PFM device characteristics) 
		send_data(0x0F);     
		send_data(0x13);     
		send_data(0x44);     
		send_data(0x00);     
		send_cmd(0XF8);  // for PFM setting (base on PFM device characteristics) 
		send_data(0x18);     
		send_data(0x02);     
		send_data(0x02);     
		send_data(0x18);     
		send_data(0x02);     
		send_data(0x02);     
		send_data(0x30);  
		send_data(0x00);     
		send_data(0x00);     
		send_data(0x30);     
		send_data(0x00);     
		send_data(0x00);     
		send_data(0x30);     
		send_data(0x00);     
		send_data(0x00);     
		send_cmd(0XB8);   
		send_data(0x74);    
		send_cmd(0XF1);  //Gate Modulation 
		 
		send_data(0x10);    
		send_cmd(0XF2);   
		send_data(0x00);    
		send_data(0x58);     
		send_data(0x40);     
		send_cmd(0XF3);    
		send_data(0x60);  
		send_data(0x83);  
		send_data(0x04); 
		send_cmd(0XFC);   
		send_data(0x04);   
		send_data(0x0F); 
		send_data(0x01); 
		send_cmd(0XEB);   
		send_data(0x08);     
		   send_data(0x0F); 
		send_cmd(0XE0);  //P-Gamma 
		send_data(0x00);     
		send_data(0x05);     
		send_data(0x0D);     
		send_data(0x12);     
		send_data(0x13);     
		send_data(0x1A);     
		send_data(0x0C);     
		send_data(0x0B);     
		send_data(0x01);     
		send_data(0x06);     
		send_data(0x03);     
		send_data(0x0D);     
		send_data(0x0D);     
		send_data(0X31);     
		send_data(0x2D);     
		send_data(0x00);     
		send_cmd(0XE1);  //N-Gamma 
		send_data(0x00);     
		send_data(0x05);     
		send_data(0x0D);     
		send_data(0x12);     
		send_data(0x12);     
		send_data(0x1A);     
		send_data(0x0D);     
		send_data(0x0B);     
		send_data(0x00);     
		send_data(0x05);     
		send_data(0x03);     
		send_data(0x0D);     
		send_data(0x0D);     
		send_data(0x31);     
		send_data(0x2C);     
		send_data(0x00);     
		send_cmd(0XC1);  //Power Control 1 
		send_data(0x15);    
		send_data(0x30);    
		send_data(0x10);     
		send_data(0x37);    
		
		send_cmd(0xC7);   
		send_data(0xDA);//0xC3
		
		send_cmd(0xB1);       //Frame Rate Control 
		send_data(0x00);   
		send_data(0x12);   
		send_data(0x14);   
		send_cmd(0xB4); 
		send_data(0x02);   
		send_cmd(0xBB);        
		send_data(0x14);   
		send_data(0x55);   
		send_cmd(0x36);        //Memory Access 
		send_data(0x0A);  

		send_cmd(0x35);      
		send_data(0x00); 
		
		send_cmd(0x3A);      
		send_data(0x77);    
		//send_cmd(0X20);    //Display Inv-Off 
		send_cmd(0X21);   //Display Inv-On 
		send_cmd(0xB0);       //RGB I/F Polarity 
		send_data(0x00);   
		send_cmd(0xB6);        
		send_data(0x01);    
		send_cmd(0xC2);   
		send_data(0x11);        
		send_cmd(0xDF);   //For 4002 setting 
		send_data(0x23); 
		send_cmd(0x11);         
		LCD_DelayMS(120);               
		send_cmd(0x29);       
		LCD_DelayMS(20);                  
		send_cmd(0x2C); 
	}
#else
	{
	//************* Start Initial Sequence **********// 
	send_cmd(0XFF);	  //EXTC command Password 
	send_data(0xFF); 
	send_data(0x98); 
	send_data(0x05); 
	send_cmd(0XB8);	 
	send_data(0x74);	
		
	send_cmd(0XFD);  // for PFM setting (base on PFM device characteristics) 
	send_data(0x0F);     
	send_data(0x13);     
	send_data(0x44);     
	send_data(0x00); 
	
	send_cmd(0XF8);	  //PFM
	send_data(0x18); 
	send_data(0x02); 
	send_data(0x02);
	send_data(0x18); 
	send_data(0x02); 
	send_data(0x02);
	send_data(0x30); 
	send_data(0x00); 
	send_data(0x00);
	send_data(0x30); 
	send_data(0x00); 
	send_data(0x00);
	send_data(0x30); 
	send_data(0x00); 
	send_data(0x00);
	
	send_cmd(0XF1); 
	send_data(0x10);	
	send_cmd(0XF2);	//CR/EQ/PC 
	send_data(0x00);	
	send_data(0x58);	 
	send_data(0x41);  
	
	send_cmd(0xF3);
	send_data(0x60);
	send_data(0x83);
	send_data(0x04);
	
	
	send_cmd(0xFC);
	send_data(0x04);
	send_data(0x0F);
	send_data(0x01);
	
	   
	send_cmd(0XEB);	  
	send_data(0x08);	 
	send_data(0x0F); 
	
	send_cmd(0XE0);	 //P-Gamma 
	send_data(0x00);	 
	send_data(0x20);	 
	send_data(0x27);	 
	send_data(0x07);	 
	send_data(0x0e);	 
	send_data(0x11);	 
	send_data(0x7b);	 
	send_data(0x0a);	 
	send_data(0x01);	 
	send_data(0x07);	 
	send_data(0x05);	 
	send_data(0x0d);	 
	send_data(0x0a);	 
	send_data(0X32);	 
	send_data(0x2c);	 
	send_data(0x00);	
	 
	send_cmd(0XE1);	 //N-Gamma 
	send_data(0x00);	 
	send_data(0x19);	 
	send_data(0x1c);	 
	send_data(0x18);	 
	send_data(0x13);	 
	send_data(0x18);	 
	send_data(0x78);	 
	send_data(0x08);	 
	send_data(0x03);	 
	send_data(0x06);	 
	send_data(0x04);	 
	send_data(0x0e);	 
	send_data(0x0b);	 
	send_data(0x2a);	 
	send_data(0x23);	 
	send_data(0x00);  
	   
	send_cmd(0XC1);	  //Power Control 1 
	send_data(0x13);	
	send_data(0x30);	
	send_data(0x10);	
	send_data(0x26);	
	
	send_cmd(0xC7);	 
	send_data(0xDA);   //VCOM 
	
	send_cmd(0xB1);		  //Frame Rate Control 
	send_data(0x00);   
	send_data(0x12);   
	send_data(0x14); 
	
	send_cmd(0xdf); 
	send_data(0x23);
	
	  
	send_cmd(0xB4);		
	send_data(0x02);   
	send_cmd(0xBB);			
	send_data(0x14);   
	send_data(0x55);   
	send_cmd(0x36);	
	send_data(0x0A);	
	
	send_cmd(0x35); 
	send_data(0x00);
	
	send_cmd(0x3A);		
	send_data(0x77);
	
	//send_cmd(0X20);	//Display Inv-Off 
	send_cmd(0X21);	 //Display Inv-On 
	
	send_cmd(0xB0);		  //RGB I/F Polarity 
	send_data(0x00);   
	send_cmd(0xB6);		   
	send_data(0x01);	
	send_cmd(0xDF);		//For 4002 setting 
	send_data(0x23);	
	send_cmd(0x11);		    
	LCD_DelayMS(120);				
	send_cmd(0x29);		  
	LCD_DelayMS(20);				  
	send_cmd(0x2C);
	 

		
	}
#endif

	printf("qiandan init end!.....\n");

	LCD_DelayMS(100);	

	return 0;
}

static int32_t ili9805_set_window(struct lcd_spec *self,
		uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_data = self->info.mcu->ops->send_data;

	LCD_PRINT("qiandan ili9805_set_window: %d, %d, %d, %d\n",left, top, right, bottom);
	
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

static int32_t ili9805_invalidate(struct lcd_spec *self)
{
	LCD_PRINT("ili9805_invalidate\n");

	return self->ops->lcd_set_window(self, 0, 0, 
			self->width - 1, self->height - 1);
	
}

static int32_t ili9805_invalidate_rect(struct lcd_spec *self,
				uint16_t left, uint16_t top,
				uint16_t right, uint16_t bottom)
{
	Send_cmd_data send_cmd_data = self->info.mcu->ops->send_cmd_data;
	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
	LCD_PRINT("ili9805_invalidate_rect \n");

	return self->ops->lcd_set_window(self, left, top, 
			right, bottom);
}

static int32_t ili9805_set_direction(struct lcd_spec *self, uint16_t direction)
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

static int32_t ili9805_enter_sleep(struct lcd_spec *self, uint8_t is_sleep)
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

static int32_t ili9805_read_id(struct lcd_spec *self)
{

	Send_data send_cmd = self->info.mcu->ops->send_cmd;
	Send_data send_data = self->info.mcu->ops->send_data;
	Read_data read_data = self->info.mcu->ops->read_data;

	uint32_t dummy1,dummy2,dummy3,dummy4;

	send_cmd(0xd3);
	dummy1=read_data()&0xFF;
	dummy2=read_data()&0xFF;
	dummy3=read_data()&0xFF;
	dummy4=read_data()&0xFF;


	printf("%s dummy1:0x%x,dummy2:0x%x,dummy3:0x%x,dummy4:0x%x\n",__func__, dummy1, dummy2, dummy3, dummy4);

	return (dummy3<<8)|dummy4;//0x9805

}

static struct lcd_operations lcd_ili9805_operations = {
	.lcd_init = ili9805_init,
	.lcd_set_window = ili9805_set_window,
	.lcd_invalidate_rect= ili9805_invalidate_rect,
	.lcd_invalidate = ili9805_invalidate,
	.lcd_set_direction = ili9805_set_direction,
	.lcd_enter_sleep = ili9805_enter_sleep,
	.lcd_readid = ili9805_read_id,
};

static struct timing_mcu lcd_ili9805_timing[] = {
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
			.wlpw = 20,//35,70	15
			.whpw = 20,//35,70	24

	},
};

static struct info_mcu lcd_ili9805_info = {
	.bus_mode = LCD_BUS_8080,
	.bus_width = 24,
	.timing = lcd_ili9805_timing,
	.ops = NULL,
};

struct lcd_spec lcd_panel_ili9805 = {
	.width = 480,
	.height = 800,
	.mode = LCD_MODE_MCU,
	.direction = LCD_DIRECT_NORMAL,
	.info = {.mcu = &lcd_ili9805_info},
	.ops = &lcd_ili9805_operations,
};
