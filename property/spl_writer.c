#include "../nand_fdl/fdl-2/inc/fdl_nand.h"
#include "asm/arch/sci_types.h"
#include <asm/arch/regs_nfc.h>
#include <linux/mtd/mtd.h>
#include <nand.h>
#include <linux/mtd/nand.h>
//#include <jffs2/jffs2.h>
//#include <parsemtdparts.h>
//#include <cmd_def.h>

#define CONFIG_SYS_SPL_ECC_POS		8
#define MAX_SPL_SIZE    0x4000

struct bootloader_header
{
	u32 check_sum;
	u32 page_type; //page type 0-512,1-1K,2-2k,3-4k,4-8K
	u32 acycle; // 3, 4, 5
	u32 bus_width; //0 ,1
	u32 advance; // 0, 1
	u32 magic_num; //0xaa55a5a5
	u32 spare_size; //spare part sise for one sector
	u32 ecc_mode; //0--1bit, 1--2bit,2--4bit,3--8bit,4--12bit, 5--16bit, 6--24bit
	u32 ecc_pos; // ecc postion at spare part
	u32 sct_size; //sector size;
	u32 sct_per_page; //sector per page
	u32 ecc_value[11];
};

/******************************************************************************
 * make the checksum of one packet
 ******************************************************************************/
unsigned short CheckSum(const unsigned int *src, int len)
{
    unsigned int   sum = 0;
    unsigned short *src_short_ptr = PNULL;

    while (len > 3)
    {
        sum += *src++;
        len -= 4;
    }

    src_short_ptr = (unsigned short *) src;

    if (0 != (len&0x2))
    {
        sum += * (src_short_ptr);
        src_short_ptr++;
    }

    if (0 != (len&0x1))
    {
        sum += * ( (unsigned char *) (src_short_ptr));
    }

    sum  = (sum >> 16) + (sum & 0x0FFFF);
    sum += (sum >> 16);

    return (unsigned short) (~sum);
}
u32 get_nand_page_type(u32 pg_sz)
{
	u32 pg_type = 0;
	switch(pg_sz)
	{
	case 512:
		pg_type = 0;
		break;
	case 1024:
		pg_type = 1;
		break;
	case 2048:
		pg_type = 2;
		break;
	case 4096:
		pg_type = 3;
		break;
	case 892:
		pg_type = 4;
		break;		
	default:
		while(1);
		break;								
	}
	return pg_type;
}
extern unsigned short CheckSum(const unsigned int *src, int len);
void set_header_info(u8 *bl_data, struct mtd_info *nand, int ecc_pos)
{
	struct bootloader_header *header;
	struct nand_chip *chip = nand->priv;
	struct sc8810_ecc_param param;
	u8 ecc[44];
	header = (struct bootloader_header *)(bl_data + BOOTLOADER_HEADER_OFFSET);
	memset(header, 0, sizeof(struct bootloader_header));
	memset(ecc, 0, sizeof(ecc));
#if 1
	header->page_type = get_nand_page_type(nand->writesize);
	if (chip->options & NAND_BUSWIDTH_16)	{
		header->bus_width = 1;
	}
	if(nand->writesize > 512)	{
		header->advance = 1;
		/* One more address cycle for devices > 128MiB */
		if (chip->chipsize > (128 << 20))		{
			header->acycle = 5;
		}
		else	 {
			header->acycle = 4;
		}
	}
	else{
		header->advance = 0;
		/* One more address cycle for devices > 32MiB */
		if (chip->chipsize > (32 << 20)) {
			header->acycle = 3;
		}
		else	{
			header->acycle = 3;
		}
	}
	header->magic_num = 0xaa55a5a5;
	header->spare_size = (nand->oobsize/chip->ecc.steps);
	header->ecc_mode = ecc_mode_convert(chip->eccbitmode);
	header->ecc_pos = ecc_pos;
	header->sct_size = (nand->writesize/chip->ecc.steps);
	header->sct_per_page = chip->ecc.steps;
	header->check_sum = CheckSum((unsigned int *)(bl_data + BOOTLOADER_HEADER_OFFSET + 4), (NAND_PAGE_LEN - BOOTLOADER_HEADER_OFFSET - 4));

	param.mode = 24;
	param.ecc_num = 1;
	param.sp_size = sizeof(ecc);
	param.ecc_pos = 0;
	param.m_size = chip->ecc.size;
	param.p_mbuf = (u8 *)bl_data;
	param.p_sbuf = ecc;
	sc8810_ecc_encode(&param);
	memcpy(header->ecc_value, ecc, sizeof(ecc));
#endif	
}
int nand_write_spl_page(u8 *buf, struct mtd_info *mtd, u32 pg, u32 ecc_pos)
{
	int eccsteps;
	u32 eccsize;
	struct nand_chip *chip = mtd->priv;
	int eccbytes = chip->ecc.bytes;
	u32 i;
	u32 page;
	u32 spare_per_sct;
	uint8_t *ecc_calc = chip->buffers->ecccalc;
	eccsteps = chip->ecc.steps;
	eccsize = chip->ecc.size;
	spare_per_sct = mtd->oobsize / eccsteps;
	memset(chip->buffers->ecccode, 0xff, mtd->oobsize);
	
	page = (int)(pg >> chip->page_shift);
	
	chip->cmdfunc(mtd, NAND_CMD_SEQIN, 0x00, page);
	
	for (i = 0; i < eccsteps; i ++, buf += eccsize) {
		chip->ecc.hwctl(mtd, NAND_ECC_WRITE);
		chip->write_buf(mtd, buf, eccsize);
		chip->ecc.calculate(mtd, buf, &ecc_calc[0]);
		memcpy(chip->buffers->ecccode + i * spare_per_sct + ecc_pos, &ecc_calc[0], eccbytes);
	}
	chip->write_buf(mtd, chip->buffers->ecccode, mtd->oobsize);
	chip->cmdfunc(mtd, NAND_CMD_PAGEPROG, -1, -1);
	chip->waitfunc(mtd, chip);
	
	return NAND_SUCCESS;
}
int nand_write_spl(u8 *buf, struct mtd_info *mtd)
{
	u32 i;
	u32 pg_start;
	u32 pg_end;
	u32 pg;
	u8 * data;
	int ret = NAND_SUCCESS;

	//struct nand_chip *chip = mtd->priv;
	set_header_info(buf, mtd, CONFIG_SYS_SPL_ECC_POS);
	/* erase spl block in nand_start_write(), so erase spl block code is deleted */
	
	extern int nand_scan_block(int block, int erasesize, int writesize);
	ret = nand_scan_block(0,mtd->erasesize,mtd->writesize);
	if(ret)
		return -1;

	/* write spl to flash*/
	for (i = 0; i < 3; i++) {
		pg_start = i * MAX_SPL_SIZE;
		pg_end = (i + 1) * MAX_SPL_SIZE;
		data = buf;
		for(pg  = pg_start; pg < pg_end; pg += mtd->writesize) {
			ret = nand_write_spl_page(data, mtd, pg, CONFIG_SYS_SPL_ECC_POS);
			data += mtd->writesize;
		}
	}
	
	return ret;	
}

