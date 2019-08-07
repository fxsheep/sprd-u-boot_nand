#ifndef _DDR_PARA_H_
#define _DDR_PARA_H_

#include <asm/arch/sdram_cfg.h>
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))


/* emc config */
struct sc8810_ddr_reset_para ddr_config_table[] =
{
	{0xad, 0xbc, 0x90, 0x55, 0x54, 400000000, 2, 2, 0, 2, 13, 0x818},
	{0x2c, 0xbc, 0x90, 0x55, 0x56, 400000000, 2, 2, 1, 2, 14, 0x818}, // MT29C4G48MAZBAAKS
	//{0xec, 0xbc, 0x00, 0x66, 0x56, 400000000, 2, 2, 0, 2, 12, 0x818},
	//{0x2c, 0xb3, 0x90, 0x66, 0x64, 400000000, 2, 2, 0, 2, 12, 0x818}
};
struct sc8810_ddr_reset_data ddr_reset_data = {

	.ddr_para = ddr_config_table,
	.size     = ARRAY_SIZE(ddr_config_table),
};
#endif //_DDR_PARA_H_
