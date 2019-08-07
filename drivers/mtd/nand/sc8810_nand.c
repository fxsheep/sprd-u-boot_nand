#include <config.h>
#include <common.h>
#include <asm/arch/bits.h>
#include <asm/arch/chip_drv_config_extern.h>
#include <asm/arch/regs_nfc.h>
#include <asm/arch/regs_cpc.h>
#include <asm/arch/sdram_cfg.h>
#include <nand.h>
#include <asm/io.h>
#include <linux/mtd/nand.h>

#ifdef CONFIG_NAND_SPL
#define printf(arg...) do{}while(0)
#endif

#ifndef NELEM
#define NELEM(a) \
	((int)(sizeof(a) / sizeof((a)[0])))
#endif

#define	NFC_ECC_EVENT  		1
#define	NFC_DONE_EVENT		2
#define	NFC_TX_DMA_EVENT	4
#define	NFC_RX_DMA_EVENT	8
#define	NFC_ERR_EVENT		16
#define	NFC_TIMEOUT_EVENT	32
#define NFC_TIMEOUT_VAL		0x1000000

/* 2 bit correct, sc8810 support 1, 2, 4, 8, 12,14, 24 */
#define CONFIG_SYS_NAND_ECC_MODE	2
/* Number of ECC bytes per OOB - S3C6400 calculates 4 bytes ECC in 1-bit mode */
#define CONFIG_SYS_NAND_ECCBYTES	4
/* Size of the block protected by one OOB (Spare Area in Samsung terminology) */
#define CONFIG_SYS_NAND_ECCSIZE	512

struct sc8810_nand_timing_param {
	u8 acs_time;
	u8 rwh_time;
	u8 rwl_time;
	u8 acr_time;
	u8 rr_time;
	u8 ceh_time;
};
struct sc8810_nand_info {
	struct clk	*clk;
	struct nand_chip *chip;
	unsigned int cfg0_setting;
	unsigned int ecc0_cfg_setting;
	unsigned int ecc1_cfg_setting;
	u8 	asy_cle; //address cycles, can be set 3, 4, 5
	u8	advance;// advance property, can be set 0, 1
	u8	bus_width; //bus width, can be 0 or 1
	u8	ecc_mode; // ecc mode can be 1, 2, 4, 8, 12, 16,24
	u8  	mc_ins_num; // micro instruction number
	u8	mc_addr_ins_num; //micro address instruction number
	u16	ecc_postion; //ecc postion
	u16 	b_pointer; // nfc buffer pointer
	u16 	addr_array[5];// the addrss of the flash to operation
};

struct sc8810_nand_page_oob {
	u8 id[5];
	u16 pagesize;
	u16 oobsize; /* total oob size */
	u16 eccsize; /* per ??? bytes data for ecc calcuate once time */
	u16 eccbit; /* ecc level per eccsize */
};

struct nand_spec_str{
	u8 id[5];
    struct sc8810_nand_timing_param timing_cfg;
};

static struct nand_spec_str *ptr_nand_spec = NULL;

#define NF_MC_CMD_ID	(0xFD)
#define NF_MC_ADDR_ID	(0xF1)
#define NF_MC_WAIT_ID	(0xF2)
#define NF_MC_RWORD_ID	(0xF3)
#define NF_MC_RBLK_ID	(0xF4)
#define NF_MC_WWORD_ID	(0xF6)
#define NF_MC_WBLK_ID	(0xF7)
#define NF_MC_DEACTV_ID	(0xF9)
#define NF_MC_NOP_ID	(0xFA)
#define NF_PARA_20M        	0x7ac05      //trwl = 0  trwh = 0
#define NF_PARA_40M        	0x7ac15      //trwl = 1  trwh = 0
#define NF_PARA_53M        	0x7ad26      //trwl = 2  trwh = 1
#define NF_PARA_80M        	0x7ad37      //trwl = 3  trwh = 1
#define NF_PARA_DEFAULT    	0x7ad77      //trwl = 7  trwh = 1

#define REG_AHB_CTL0		       		(*((volatile unsigned int *)(AHB_CTL0)))
#define REG_AHB_SOFT_RST				(*((volatile unsigned int *)(AHB_SOFT_RST)))

#define REG_GR_NFC_MEM_DLY                      (*((volatile unsigned int *)(GR_NFC_MEM_DLY)))

static int mtderasesize = 0;
static int mtdwritesize = 0;
static int mtdoobsize = 0;

static struct sc8810_nand_info g_info ={0};
static nand_ecc_modes_t sprd_ecc_mode = NAND_ECC_NONE;
static __attribute__((aligned(4))) unsigned char io_wr_port[NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE];
static char nand_id_table[5]={0};
static int  re_oob_layout=0;

struct nand_ecclayout _nand_oob_64 = {
	.eccbytes = 24,
	.eccpos = {
		   40, 41, 42, 43, 44, 45, 46, 47,
		   48, 49, 50, 51, 52, 53, 54, 55,
		   56, 57, 58, 59, 60, 61, 62, 63},
	.oobfree = {
		{.offset = 2,
		 .length = 38}}
};

struct nand_ecclayout _nand_oob_64_4bit = {
	.eccbytes = 28,
	.eccpos = {
	           36, 37, 38, 39, 40, 41, 42, 43,
                   44, 45, 46, 47, 48, 49, 50, 51,
                   52, 53, 54, 55, 56, 57, 58, 59,
                   60, 61, 62, 63},
	.oobfree = {
		{.offset = 2,
		 .length = 34}}
};


static struct nand_ecclayout nand_oob_128 = {
	.eccbytes = 48,
	.eccpos = {
		    80,  81,  82,  83,  84,  85,  86,  87,
		    88,  89,  90,  91,  92,  93,  94,  95,
		    96,  97,  98,  99, 100, 101, 102, 103,
		   104, 105, 106, 107, 108, 109, 110, 111,
		   112, 113, 114, 115, 116, 117, 118, 119,
		   120, 121, 122, 123, 124, 125, 126, 127},
	.oobfree = {
		{.offset = 2,
		 .length = 78}}
};

static struct nand_ecclayout _nand_oob_128 = {
	.eccbytes = 56,
	.eccpos = {
		72, 73, 74, 75, 76, 77, 78, 79,
		80,  81,  82,  83,  84,  85,  86,  87,
		88,  89,  90,  91,  92,  93,  94,  95,
		96,  97,  98,  99, 100, 101, 102, 103,
		104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119,
		120, 121, 122, 123, 124, 125, 126, 127},
	.oobfree = {
		{.offset = 2,
		.length = 70}}
};

static struct nand_ecclayout _nand_oob_224 = {
	.eccbytes = 112,
	.eccpos = {
		112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125,
		126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
		140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153,
		154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
		168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
		182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195,
		196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
		210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223},
	.oobfree = {
		{.offset = 2,
		.length = 110}}
};

static struct nand_ecclayout _nand_oob_256 = {
	.eccbytes = 112,
	.eccpos = {
		144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157,
		158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171,
		172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185,
		186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
		200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
		214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
		228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
		242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255},
	.oobfree = {
		{.offset = 2,
		.length = 142}}
};

#if 0
struct sc8810_nand_timing_param nand_timing =
{
	50,
	30,
	40,
	40,
	40,
	50
};
#endif

/* only for special 4kpage or 8kpage nand flash, no 2kpage or normal 4kpage or normal 8kpage */
static struct sc8810_nand_page_oob nand_config_table[] =
{
	//{0xec, 0xbc, 0x00, 0x66, 0x56, 4096, 128, 512, 4},
	//{0x2c, 0xb3, 0x90, 0x66, 0x64, 4096, 224, 512, 8},
	//{0x2c, 0xbc, 0x90, 0x66, 0x54, 4096, 224, 512, 8},
	//{0xec, 0xb3, 0x01, 0x66, 0x5a, 4096, 128, 512, 4},
    //{0xec, 0xbc, 0x00, 0x6a, 0x56, 4096, 256, 512, 8},
    {{0x2c, 0xbc, 0x90, 0x55, 0x56}, 2048, 64,  512, 4},
    //{0x2c, 0xb3, 0xd1, 0x55, 0x56, 2048, 64,  512, 4},
	//{0xc8, 0xbc, 0x90, 0x55, 0x54, 2048, 64,  512, 4}
};

/* some nand id could not be calculated the pagesize by mtd, replace it with a known id which has the same format. */
static unsigned char nand_id_replace_table[][10] =
{
    {0xad, 0xb3, 0x91, 0x11, 0x00, /*replace with*/ 0xec, 0xbc, 0x00, 0x66, 0x56},
    {0xad, 0xbc, 0x90, 0x11, 0x00, /*replace with*/ 0xec, 0xbc, 0x00, 0x66, 0x56}
};


#ifdef CONFIG_DDR_PARA
/* emc config */
extern struct sc8810_ddr_reset_data ddr_reset_data ;
#endif

static const struct nand_spec_str nand_spec_table[] = {
    //{0x2c, 0xb3, 0xd1, 0x55, 0x5a, {10, 10, 12, 10, 20, 50}},// MT29C8G96MAAFBACKD-5, MT29C4G96MAAHBACKD-5
    //{0x2c, 0xba, 0x80, 0x55, 0x50, {10, 10, 12, 10, 20, 50}},// MT29C2G48MAKLCJA-5 IT
    {{0x2c, 0xbc, 0x90, 0x55, 0x56}, {10, 10, 12, 10, 20, 50}},// KTR0405AS-HHg1, KTR0403AS-HHg1, MT29C4G96MAZAPDJA-5 IT
   //{0x98, 0xac, 0x90, 0x15, 0x76, {12, 10, 12, 10, 20, 50}},// TYBC0A111392KC
    //{0x98, 0xbc, 0x90, 0x55, 0x76, {12, 10, 12, 10, 20, 50}},// TYBC0A111430KC, KSLCBBL1FB4G3A, KSLCBBL1FB2G3A

    //{0xad, 0xbc, 0x90, 0x11, 0x00, {25, 15, 25, 10, 20, 50}},// H9DA4VH4JJMMCR-4EMi, H9DA4VH2GJMMCR-4EM
    {{0xad, 0xbc, 0x90, 0x55, 0x54}, {25, 15, 25, 10, 20, 50}},//
    //{0xec, 0xb3, 0x01, 0x66, 0x5a, {21, 10, 21, 10, 20, 50}},// KBY00U00VA-B450
    //{0xec, 0xbc, 0x00, 0x55, 0x54, {21, 10, 21, 10, 20, 50}},// KA100O015M-AJTT
    //{0xec, 0xbc, 0x00, 0x6a, 0x56, {21, 10, 21, 10, 20, 50}},// K524G2GACH-B050
    //{0xec, 0xbc, 0x01, 0x55, 0x48, {21, 15, 21, 10, 20, 50}},// KBY00N00HM-A448
};


void read_chip_id(void);
static void nfc_reg_write(unsigned int addr, unsigned int value)
{
	writel(value, addr);
}
static unsigned int nfc_reg_read(unsigned int addr)
{
	return readl(addr);
}

#ifdef CONFIG_NAND_SPL
void * memcpy(void *dest, const void *src, size_t count)
{
        unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
        char *d8, *s8;

        /* while all data is aligned (common case), copy a word at a time */
        if ( (((ulong)dest | (ulong)src) & (sizeof(*dl) - 1)) == 0) {
                while (count >= sizeof(*dl)) {
                        *dl++ = *sl++;
                        count -= sizeof(*dl);
                }
        }
        /* copy the reset one byte at a time */
        d8 = (char *)dl;
        s8 = (char *)sl;
        while (count--)
                *d8++ = *s8++;

        return dest;
}
#endif

int nand_chip_id_match(const u8 *id1, const u8 *id2)
{
	const u8 *id1_end;

	for (id1_end = id1 + 5; id1 < id1_end; id1++, id2++) {
		if (*id1 != *id2) {
			return 0;
		}
	}

	return 1;
}

static void  nfc_mcr_inst_init(void)
{
	g_info.mc_ins_num = 0;
	g_info.b_pointer = 0;
	g_info.mc_addr_ins_num = 0;
}
void  nfc_mcr_inst_add(u32 ins, u32 mode)
{
	unsigned int offset;
	unsigned int high_flag;
	unsigned int reg_value;
	offset = g_info.mc_ins_num >> 1;
	high_flag = g_info.mc_ins_num & 0x1;
	if(NF_MC_ADDR_ID == mode)
	{
		g_info.addr_array[g_info.mc_addr_ins_num ++] = ins;
	}
	if(high_flag)
	{
		reg_value = nfc_reg_read(NFC_START_ADDR0 + (offset << 2));
		reg_value &= 0x0000ffff;
		reg_value |= ins << 24;
		reg_value |= mode << 16;
	}
	else
	{
		reg_value = nfc_reg_read(NFC_START_ADDR0 + (offset << 2));
		reg_value &= 0xffff0000;
		reg_value |= ins << 8;
		reg_value |= mode;
	}
	nfc_reg_write(NFC_START_ADDR0 + (offset << 2), reg_value);
	g_info.mc_ins_num ++;
}
static unsigned int nfc_mcr_inst_exc(void)
{
	unsigned int value;
	value = nfc_reg_read(NFC_CFG0);
	if(g_info.chip->options & NAND_BUSWIDTH_16)
	{
		value |= NFC_BUS_WIDTH_16;
	}
	else
	{
		value &= ~NFC_BUS_WIDTH_16;
	}
	value |= (1 << NFC_CMD_SET_OFFSET);
	nfc_reg_write(NFC_CFG0, value);
	value = NFC_CMD_VALID | ((unsigned int)NF_MC_NOP_ID) | 	((g_info.mc_ins_num - 1) << 16);
	nfc_reg_write(NFC_CMD, value);
	return 0;
}

static unsigned int nfc_mcr_inst_exc_for_id(void)
{
	unsigned int value;

	value = nfc_reg_read(NFC_CFG0);
	value &= ~NFC_BUS_WIDTH_16;
	value |= (1 << NFC_CMD_SET_OFFSET);

	nfc_reg_write(NFC_CFG0, value);
	value = NFC_CMD_VALID | ((unsigned int)NF_MC_NOP_ID) |((g_info.mc_ins_num - 1) << 16);
	nfc_reg_write(NFC_CMD, value);
	return 0;
}

static void sc8810_nand_wp_en(int en)
{
	unsigned int value;
	if(en)
	{
		value = nfc_reg_read(NFC_CFG0);
		value &= ~ NFC_WPN;
		nfc_reg_write(NFC_CFG0, value);
	}
	else
	{
		value = nfc_reg_read(NFC_CFG0);
		value |= NFC_WPN;
		nfc_reg_write(NFC_CFG0, value);
	}
}
static int sc8810_nfc_wait_command_finish(unsigned int flag)
{
	unsigned int event = 0;
	unsigned int value;
	unsigned int counter = 0;
	while(((event & flag) != flag) && (counter < NFC_TIMEOUT_VAL/*time out*/))
	{
		value = nfc_reg_read(NFC_CLR_RAW);
		if(value & NFC_ECC_DONE_RAW)
		{
			event |= NFC_ECC_EVENT;
		}
		if(value & NFC_DONE_RAW)
		{
			event |= NFC_DONE_EVENT;
		}
		counter ++;
	}
	nfc_reg_write(NFC_CLR_RAW, 0xffff0000); //clear all interrupt status
	if(counter > NFC_TIMEOUT_VAL)
	{
		while (1);
		return -1;
	}
	return 0;
}
unsigned int ecc_mode_convert(u32 mode)
{
	u32 mode_m;
	switch(mode)
	{
	case 1:
		mode_m = 0;
		break;
	case 2:
		mode_m = 1;
		break;
	case 4:
		mode_m = 2;
		break;
	case 8:
		mode_m = 3;
		break;
	case 12:
		mode_m = 4;
		break;
	case 16:
		mode_m = 5;
		break;
	case 24:
		mode_m = 6;
		break;
	default:
		mode_m = 0;
		break;
	}
	return mode_m;
}
unsigned int sc8810_ecc_encode(struct sc8810_ecc_param *param)
{
	u32 reg;
	reg = (param->m_size - 1);
	memcpy((void *)NFC_MBUF_ADDR, param->p_mbuf, param->m_size);
	nfc_reg_write(NFC_ECC_CFG1, reg);
	reg = 0;
	reg = (ecc_mode_convert(param->mode)) << NFC_ECC_MODE_OFFSET;
	reg |= (param->ecc_pos << NFC_ECC_SP_POS_OFFSET) | ((param->sp_size - 1) << NFC_ECC_SP_SIZE_OFFSET) | ((param->ecc_num -1)<< NFC_ECC_NUM_OFFSET);
	reg |= NFC_ECC_ACTIVE;
	nfc_reg_write(NFC_ECC_CFG0, reg);
	sc8810_nfc_wait_command_finish(NFC_ECC_EVENT);
	memcpy(param->p_sbuf, (u8 *)NFC_SBUF_ADDR,param->sp_size);
	return 0;
}
static u32 sc8810_get_decode_sts(void)
{
	u32 err;
	err = nfc_reg_read(NFC_ECC_STS0);
	err &= 0x1f;
	if(err == 0x1f)
	{
		return -1;
	}
	return err;
}
static u32 sc8810_ecc_decode(struct sc8810_ecc_param *param)
{
	u32 reg;
	u32 ret = 0;
	s32 size = 0;
	memcpy((void *)NFC_MBUF_ADDR, param->p_mbuf, param->m_size);
	memcpy((void *)NFC_SBUF_ADDR, param->p_sbuf, param->sp_size);
	reg = (param->m_size - 1);
	nfc_reg_write(NFC_ECC_CFG1, reg);
	reg = 0;
	reg = (ecc_mode_convert(param->mode)) << NFC_ECC_MODE_OFFSET;
	reg |= (param->ecc_pos << NFC_ECC_SP_POS_OFFSET) | ((param->sp_size - 1) << NFC_ECC_SP_SIZE_OFFSET) | ((param->ecc_num -1)<< NFC_ECC_NUM_OFFSET);
	reg |= NFC_ECC_DECODE;
	reg |= NFC_ECC_ACTIVE;
	nfc_reg_write(NFC_ECC_CFG0, reg);
	sc8810_nfc_wait_command_finish(NFC_ECC_EVENT);
	ret = sc8810_get_decode_sts();
	if (ret != 0 && ret != -1) {
	//printk(KERN_INFO "sc8810_ecc_decode sts = %x\n",ret);
	}
	if (ret == -1) {
		size = param->sp_size;
		if (size > 0) {
			while (size--)
			{
				if (param->p_sbuf[size] != 0xff)
					break;
			}
			if (size < 0)
			{
				size = param->m_size;
				if (size > 0)
				{
					while (size--)
					{
						if (param->p_mbuf[size] != 0xff)
							break;
					}
					if (size < 0) {
						ret = 0;
					}
				}
			}
		}
	}
	if((ret != -1) && (ret != 0))
	{
		memcpy(param->p_mbuf, (void *)NFC_MBUF_ADDR, param->m_size);
		memcpy(param->p_sbuf, (void *)NFC_SBUF_ADDR, param->sp_size);
		ret = 0;
	}
	return  ret;
}

#if 0
static void set_nfc_param(unsigned long nfc_clk)
{
	u32 value = 0;
	u32 cycles;
	cycles = nand_timing.acs_time / (1000000000 / nfc_clk);
	value |= (cycles << NFC_ACS_OFFSET);

	cycles = nand_timing.rwh_time / (1000000000 / nfc_clk);
	value |= (cycles << NFC_RWH_OFFSET);

	cycles = nand_timing.rwl_time / (1000000000 / nfc_clk);
	value |= (cycles << NFC_RWL_OFFSET);

	cycles = nand_timing.acr_time / (1000000000 / nfc_clk);
	value |= (cycles << NFC_ACR_OFFSET);

	cycles = nand_timing.rr_time / (1000000000 / nfc_clk);
	value |= (cycles << NFC_RR_OFFSET);

	cycles = nand_timing.ceh_time / (1000000000 / nfc_clk);
	value |= (cycles << NFC_CEH_OFFSET);
	nfc_reg_write(NFC_TIMING, value);

//	local_irq_restore(flags);
}
#endif
#ifndef CONFIG_NAND_SPL
static struct nand_spec_str *get_nand_spec(u8 *nand_id)
{
	const struct nand_spec_str *p, *p_end;

	for (p = nand_spec_table, p_end = p + NELEM(nand_spec_table); p < p_end; p++) {
		if (nand_chip_id_match(p->id, nand_id)) {
			return p;
		}
	}

	return NULL;
}

#define DELAY_NFC_TO_PAD 9
#define DELAY_PAD_TO_NFC 6
#define DELAY_RWL (DELAY_NFC_TO_PAD + DELAY_PAD_TO_NFC)

static void set_nfc_timing(struct sc8810_nand_timing_param *nand_timing, u32 nfc_clk_MHz)
{
	u32 value = 0;
	u32 cycles;
	cycles = nand_timing->acs_time * nfc_clk_MHz / 1000 + 1;
	value |= ((cycles & 0x1F) << NFC_ACS_OFFSET);

	cycles = nand_timing->rwh_time * nfc_clk_MHz / 1000 + 2;
	value |= ((cycles & 0x1F) << NFC_RWH_OFFSET);

        cycles = (nand_timing->rwl_time+DELAY_RWL) * nfc_clk_MHz / 1000 + 1;
	value |= ((cycles & 0x3F) << NFC_RWL_OFFSET);

        cycles = nand_timing->acr_time * nfc_clk_MHz / 1000 + 1;
	value |= ((cycles & 0x1F) << NFC_ACR_OFFSET);

        cycles = nand_timing->rr_time * nfc_clk_MHz / 1000 + 1;
	value |= ((cycles & 0x1F) << NFC_RR_OFFSET);

        cycles = nand_timing->ceh_time * nfc_clk_MHz / 1000 + 1;
	value |= ((cycles & 0x3F) << NFC_CEH_OFFSET);

        nfc_reg_write(NFC_TIMING, value);

    debug("set_nfc_timing NFC_TIMING: %x ", nfc_reg_read(NFC_TIMING));
}
#endif
static void sc8810_nand_hw_init(void)
{
	int ik_cnt = 0;

	REG_AHB_CTL0 |= BIT_8;//no BIT_9
	REG_AHB_SOFT_RST |= BIT_5;
	for(ik_cnt = 0; ik_cnt < 0xffff; ik_cnt++);
	REG_AHB_SOFT_RST &= ~BIT_5;

	sc8810_nand_wp_en(0);
	//nfc_reg_write(NFC_TIMING, ((6 << 0) | (6 << 5) | (10 << 10) | (6 << 16) | (5 << 21) | (5 << 26)));
    nfc_reg_write(NFC_TIMING, ((12 << 0) | (7 << 5) | (10 << 10) | (6 << 16) | (5 << 21) | (7 << 26)));
	nfc_reg_write(NFC_TIMING+0X4, 0xffffffff);//TIMEOUT
	//set_nfc_param(0);//53MHz
}
static void sc8810_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	memcpy(buf, (void *)(g_info.b_pointer + NFC_MBUF_ADDR),len);
	g_info.b_pointer += len;
}
static void sc8810_nand_write_buf(struct mtd_info *mtd, const uint8_t *buf,
				   int len)
{
	struct nand_chip *chip = (struct nand_chip *)(mtd->priv);
	int eccsize = chip->ecc.size;
	memcpy((void *)(g_info.b_pointer + NFC_MBUF_ADDR), (unsigned char*)buf,len);
	if (g_info.b_pointer < eccsize)
			memcpy(io_wr_port, (unsigned char*)buf,len);
	g_info.b_pointer += len;
}
static u_char sc8810_nand_read_byte(struct mtd_info *mtd)
{
	u_char ch;
	ch = io_wr_port[g_info.b_pointer ++];
	return ch;
}
static u16 sc8810_nand_read_word(struct mtd_info *mtd)
{
	u16 ch = 0;
	unsigned char *port = (void *)NFC_MBUF_ADDR;

	ch = port[g_info.b_pointer ++];
	ch |= port[g_info.b_pointer ++] << 8;

	return ch;
}

static void sc8810_nand_data_add(unsigned int bytes, unsigned int bus_width, unsigned int read)
{
	unsigned int word;
	unsigned int blk;
	if(!bus_width)
	{
		blk = bytes >> 8;
		word = bytes & 0xff;
	}
	else
	{
		blk = bytes >> 9;
		word = (bytes & 0x1ff) >> 1;
	}
	if(read)
	{
		if(blk)
		{
			nfc_mcr_inst_add(blk - 1, NF_MC_RBLK_ID);
		}
		if(word)
		{
			nfc_mcr_inst_add(word - 1, NF_MC_RWORD_ID);
		}
	}
	else
	{
		if(blk)
		{
			nfc_mcr_inst_add(blk - 1, NF_MC_WBLK_ID);
		}
		if(word)
		{
			nfc_mcr_inst_add(word - 1, NF_MC_WWORD_ID);
		}
	}
}

static void correct_invalid_id(unsigned char *buf)
{
	int index;

	memcpy(buf, (void *)NFC_MBUF_ADDR, 5);

	for (index = 0; index < NELEM(nand_id_replace_table); index++)
	{
		if (nand_chip_id_match(nand_id_replace_table[index], buf)) {
			memcpy(buf, nand_id_replace_table[index] + 5, 5);
			break;
		}
	}
}

static void sc8810_nand_hwcontrol(struct mtd_info *mtd, int cmd,
				   unsigned int ctrl)
{
	struct nand_chip *chip = (struct nand_chip *)(mtd->priv);
	u32 eccsize, size = 0;
	if (ctrl & NAND_CLE) {
		switch (cmd) {
		case NAND_CMD_RESET:
			nfc_mcr_inst_init();
			nfc_mcr_inst_add(cmd, NF_MC_CMD_ID);
			nfc_mcr_inst_exc();
			sc8810_nfc_wait_command_finish(NFC_DONE_EVENT);
			break;
		case NAND_CMD_STATUS:

			nfc_mcr_inst_init();
			//nfc_reg_write(NFC_CMD, 0x80000070);
			//sc8810_nfc_wait_command_finish(NFC_DONE_EVENT);
			//memcpy(io_wr_port, (void *)NFC_ID_STS, 1);
			nfc_mcr_inst_add(0x70, NF_MC_CMD_ID);
			nfc_mcr_inst_add(0x10, NF_MC_NOP_ID);//add nop clk for twrh timing param
			nfc_mcr_inst_add(3, NF_MC_RWORD_ID);
			nfc_mcr_inst_exc_for_id();
			sc8810_nfc_wait_command_finish(NFC_DONE_EVENT);
			memcpy(io_wr_port, (void *)NFC_MBUF_ADDR, 1);
			//printf("read status3  0x%x, 0x%x\r\n", io_wr_port[0], nfc_reg_read(NFC_MBUF_ADDR));
			break;
		case NAND_CMD_READID:
			nfc_mcr_inst_init();
			nfc_mcr_inst_add(cmd, NF_MC_CMD_ID);
			nfc_mcr_inst_add(0x00, NF_MC_ADDR_ID);
			nfc_mcr_inst_add(0x10, NF_MC_NOP_ID);//add nop clk for twrh timing param
			nfc_mcr_inst_add(7, NF_MC_RWORD_ID);
			nfc_mcr_inst_exc_for_id();
			sc8810_nfc_wait_command_finish(NFC_DONE_EVENT);

			//memcpy(io_wr_port, (void *)NFC_MBUF_ADDR, 5);
			correct_invalid_id(io_wr_port);
			break;
		case NAND_CMD_ERASE1:
			nfc_mcr_inst_init();
			nfc_mcr_inst_add(cmd, NF_MC_CMD_ID);
			break;
		case NAND_CMD_ERASE2:
			nfc_mcr_inst_add(cmd, NF_MC_CMD_ID);
			nfc_mcr_inst_add(0, NF_MC_WAIT_ID);
			nfc_mcr_inst_exc();
			sc8810_nfc_wait_command_finish(NFC_DONE_EVENT);
			break;
		case NAND_CMD_READ0:
			nfc_mcr_inst_init();
			nfc_mcr_inst_add(cmd, NF_MC_CMD_ID);
			break;
		case NAND_CMD_READSTART:
			nfc_mcr_inst_add(cmd, NF_MC_CMD_ID);
			nfc_mcr_inst_add(0, NF_MC_WAIT_ID);
			if((!g_info.addr_array[0]) && (!g_info.addr_array[1]) )//main part
				size = mtd->writesize +mtd->oobsize;
			else
				size = mtd->oobsize;
			sc8810_nand_data_add(size, chip->options & NAND_BUSWIDTH_16, 1);
			nfc_mcr_inst_exc();
			sc8810_nfc_wait_command_finish(NFC_DONE_EVENT);
			break;
		case NAND_CMD_SEQIN:
			nfc_mcr_inst_init();
			nfc_mcr_inst_add(NAND_CMD_SEQIN, NF_MC_CMD_ID);
			break;
		case NAND_CMD_PAGEPROG:
			eccsize = chip->ecc.size;
			memcpy((void *)NFC_MBUF_ADDR, io_wr_port, eccsize);
			nfc_mcr_inst_add(0x10, NF_MC_NOP_ID);//add nop clk for twrh timing param
			sc8810_nand_data_add(g_info.b_pointer, chip->options & NAND_BUSWIDTH_16, 0);
			nfc_mcr_inst_add(cmd, NF_MC_CMD_ID);
			nfc_mcr_inst_add(0, NF_MC_WAIT_ID);
			nfc_mcr_inst_exc();
			sc8810_nfc_wait_command_finish(NFC_DONE_EVENT);
			break;
		default :
		break;
		}
	}
	else if(ctrl & NAND_ALE) {
		nfc_mcr_inst_add(cmd & 0xff, NF_MC_ADDR_ID);
	}
}
static int sc8810_nand_devready(struct mtd_info *mtd)
{
	unsigned long value = 0;

	value = nfc_reg_read(NFC_CMD);
	if ((value & NFC_CMD_VALID) != 0)
	{
		return 0;
	}else{
		return 1; /* ready */
	}
}

static void sc8810_nand_select_chip(struct mtd_info *mtd, int chip)
{
	//struct nand_chip *this = mtd->priv;
	//struct sprd_nand_info *info = this->priv;
#ifdef CONFIG_NAND_SPL
	mtd->erasesize = mtderasesize;
	mtd->writesize = mtdwritesize;
	mtd->oobsize = mtdoobsize;
#endif
}

static int sc8810_nand_calculate_ecc(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code)
{
	struct sc8810_ecc_param param;
	struct nand_chip *this = (struct nand_chip *)(mtd->priv);
	param.mode = g_info.ecc_mode;
	param.ecc_num = 1;
	param.sp_size = this->ecc.bytes;
	param.ecc_pos = 0;
	param.m_size = this->ecc.size;
	param.p_mbuf = (u8 *)dat;
	param.p_sbuf = ecc_code;
	if (sprd_ecc_mode == NAND_ECC_WRITE) {
		sc8810_ecc_encode(&param);
		sprd_ecc_mode = NAND_ECC_NONE;
	}
	return 0;
}
static void sc8810_nand_enable_hwecc(struct mtd_info *mtd, int mode)
{
	sprd_ecc_mode = mode;
}
static int sc8810_nand_correct_data(struct mtd_info *mtd, uint8_t *dat,
				     uint8_t *read_ecc, uint8_t *calc_ecc)
{
	struct sc8810_ecc_param param;
	struct nand_chip *this = (struct nand_chip *)(mtd->priv);
	int ret = 0;
	param.mode = g_info.ecc_mode;
	param.ecc_num = 1;
	param.sp_size = this->ecc.bytes;
	param.ecc_pos = 0;
	param.m_size = this->ecc.size;
	param.p_mbuf = dat;
	param.p_sbuf = read_ecc;
	ret = sc8810_ecc_decode(&param);
	return ret;
}

/* DDR para reset for para adapt*/
#ifdef CONFIG_DDR_PARA

static void set_emc_pad(u32 clk_drv, u32 ctl_drv, u32 dat_drv, u32 dqs_drv)
{
	unsigned int i = 0;
	REG32(PINMAP_REG_BASE + 0x27C) = clk_drv;
	REG32(PINMAP_REG_BASE + 0x280) = clk_drv;
	for(i = 0; i < 15; i++)
	{
		REG32(PINMAP_REG_BASE + 0x19c + i * 4) = ctl_drv;
	}
	REG32(PINMAP_REG_BASE + 0x1d8) = ctl_drv;

	for(i = 0; i < 10; i++)
	{
		REG32(PINMAP_REG_BASE + 0x284 + i * 4) = ctl_drv;
	}

	for(i = 0; i < 8; i++)
	{
		REG32(PINMAP_REG_BASE + 0x1DC + i * 4) = dat_drv;
		REG32(PINMAP_REG_BASE + 0x204 + i * 4) = dat_drv;
		REG32(PINMAP_REG_BASE + 0x22C + i * 4) = dat_drv;
		REG32(PINMAP_REG_BASE + 0x254 + i * 4) = dat_drv;
	}
	//dqs
	REG32(PINMAP_REG_BASE + 0x200) = dqs_drv;
	REG32(PINMAP_REG_BASE + 0x228) = dqs_drv;
	REG32(PINMAP_REG_BASE + 0x250) = dqs_drv;
	REG32(PINMAP_REG_BASE + 0x278) = dqs_drv;


	//dqm
	REG32(PINMAP_REG_BASE + 0x1FC) = dat_drv;
	REG32(PINMAP_REG_BASE + 0x224) = dat_drv;
	REG32(PINMAP_REG_BASE + 0x24C) = dat_drv;
	REG32(PINMAP_REG_BASE + 0x274) = dat_drv;

	// CKE OUTPUT in sleep
	REG32(PINMAP_REG_BASE + 0x1d8) |= 0x1;
	REG32(PINMAP_REG_BASE + 0x2a8) |= 0x1;
}


void ddr_para_reset(u8 id[5])
{
 // ddr_config_table
	int index;
	int array;
	u32 emc_para[7];
	u32 clkwr_dll;
   u32 i;


	array = ddr_reset_data.size;//sizeof(ddr_config_table) / sizeof(struct sc8810_ddr_reset_para);
	for (index = 0; index < array && nand_chip_id_match(ddr_reset_data.ddr_para[index].id, id) == 0; index ++);
    
	if (index < array) {
		emc_para[0] = (ddr_reset_data.ddr_para[index].dqs_drv << 8);
		emc_para[1] = (ddr_reset_data.ddr_para[index].dat_drv << 8);
		emc_para[2] = (ddr_reset_data.ddr_para[index].ctl_drv << 8);
		emc_para[3] = (ddr_reset_data.ddr_para[index].clk_drv << 8);
		emc_para[4] = (ddr_reset_data.ddr_para[index].clk_wr);
		emc_para[5] = (ddr_reset_data.ddr_para[index].read_value & 0xff);
		emc_para[6] = (ddr_reset_data.ddr_para[index].emc_clk/1000000/4);
		REG32(0x8b000018) |= (1 << 9);

		//set DPLL of EMC to 400MHz
		i = REG32(0x8b000040);
		i &= ~ 0x7ff;
		i |= emc_para[6]; //emc_clk;
		REG32(0x8b000040) = i;

		//set EMC dll
		clkwr_dll = (64*emc_para[4])/(emc_para[5]/2);
		REG32(0x2000010C) = (0x8000|clkwr_dll);
		REG32(0x8b000018) &= ~(1 << 9);

		set_emc_pad(emc_para[3], emc_para[2], emc_para[1], emc_para[0]);
	}
}
#endif

static int nand_relayout_check(void)
{
    printf("%s: re_oob_layout=0x%x\n", __FUNCTION__, re_oob_layout);
    return re_oob_layout;
}

void nand_relayout_set(int on)
{
    re_oob_layout=on;
}

const u8 nand_2bitconfig_4bitecc_table[][5] =
{
	{0x2c, 0xbc, 0x90, 0x55, 0x56},
	{0x2c, 0xb3, 0xd1, 0x55, 0x56},
	{0xc8, 0xbc, 0x90, 0x55, 0x54},
};

int nand_check_2bitconfig_4bitecc(void)
{
	int i;

	for (i = 0; i < NELEM(nand_2bitconfig_4bitecc_table); i++) {
		if (nand_chip_id_match(nand_2bitconfig_4bitecc_table[i], nand_id_table)) {
			return 1;
		}
	}

	return 0;
}

void nand_spl_hardware_config(struct nand_chip *this, u8 id[5])
{
	int index;
	int array;

	array = sizeof(nand_config_table) / sizeof(struct sc8810_nand_page_oob);
	for (index = 0; index < array && nand_chip_id_match(nand_config_table[index].id, id) == 0; index ++);

	if (index < array) {
		this->ecc.size = nand_config_table[index].eccsize;
		g_info.ecc_mode = nand_config_table[index].eccbit;
		this->eccbitmode = g_info.ecc_mode;

                if(nand_check_2bitconfig_4bitecc())
                {
                        this->ecc.bytes = 7;
                        this->ecc.layout = &_nand_oob_64_4bit;
                        g_info.ecc_mode = nand_config_table[index].eccbit=4;
                        this->eccbitmode = g_info.ecc_mode;
                }
                else
                {
                        /* 4 bit ecc, per 512 bytes can creat 13 * 4 = 52 bit , 52 / 8 = 7 bytes
		        8 bit ecc, per 512 bytes can creat 13 * 8 = 104 bit , 104 / 8 = 13 bytes, junqiang say use 14 bytes */
		        switch (g_info.ecc_mode) {
			        case 4:
				    /* 4 bit ecc, per 512 bytes can creat 13 * 4 = 52 bit , 52 / 8 = 7 bytes */
				    this->ecc.bytes = 7;
				    this->ecc.layout = &_nand_oob_128;
        		        break;
			        case 8:
				    /* 8 bit ecc, per 512 bytes can creat 13 * 8 = 104 bit , 104 / 8 = 13 bytes */
				    this->ecc.bytes = 14;
				    if (nand_config_table[index].oobsize == 224)
					this->ecc.layout = &_nand_oob_224;
				else
					this->ecc.layout = &_nand_oob_256;
				    mtdoobsize = nand_config_table[index].oobsize;
			        break;
		        }
                }
	}
}

#ifndef CONFIG_NAND_SPL
void nand_hardware_config(struct mtd_info *mtd, struct nand_chip *this, unsigned char id[8])
{
	int index;
	int array;

	/*for (index = 0; index < 5; index ++)
		printk(" %02x ", id[index]);
	printk("\n");*/
	for (index = 0; index < 5; index ++)
		this->nandid[index] = id[index];

	array = sizeof(nand_config_table) / sizeof(struct sc8810_nand_page_oob);
	for (index = 0; index < array && nand_chip_id_match(nand_config_table[index].id, id) == 0; index ++);

	if (index < array) {
		this->ecc.size = nand_config_table[index].eccsize;
		g_info.ecc_mode = nand_config_table[index].eccbit;
		this->eccbitmode = g_info.ecc_mode;

                printf("%s enter: eccsize=0x%x, eccmode=0x%x\n",__FUNCTION__, this->ecc.size, g_info.ecc_mode);
                if(nand_check_2bitconfig_4bitecc())
                {
            	    if(nand_relayout_check())
                    {
                        printf("%s: nand relayout 2bit ecc mode enter\n", __FUNCTION__);
                        this->ecc.bytes = 4;
                        this->ecc.layout = &_nand_oob_64;
                        g_info.ecc_mode = nand_config_table[index].eccbit=2;
	                this->eccbitmode = g_info.ecc_mode;
                    }
                    else
                    {
                        printf("%s: nand relayout 4bit ecc mode enter\n", __FUNCTION__);
                        this->ecc.bytes = 7;
                        this->ecc.layout = &_nand_oob_64_4bit;
                        g_info.ecc_mode = nand_config_table[index].eccbit=4;
                        this->eccbitmode = g_info.ecc_mode;
                    }
                }
                else
                {
                    /* 4 bit ecc, per 512 bytes can creat 13 * 4 = 52 bit , 52 / 8 = 7 bytes
		    8 bit ecc, per 512 bytes can creat 13 * 8 = 104 bit , 104 / 8 = 13 bytes */
		    switch (g_info.ecc_mode) {
			case 4:
				/* 4 bit ecc, per 512 bytes can creat 13 * 4 = 52 bit , 52 / 8 = 7 bytes */
				this->ecc.bytes = 7;
				this->ecc.layout = &_nand_oob_128;
			break;
			case 8:
				/* 8 bit ecc, per 512 bytes can creat 13 * 8 = 104 bit , 104 / 8 = 13 bytes */
				this->ecc.bytes = 14;
				if (nand_config_table[index].oobsize == 224)
					this->ecc.layout = &_nand_oob_224;
				else
					this->ecc.layout = &_nand_oob_256;
				mtd->oobsize = nand_config_table[index].oobsize;
			break;
		    }
                }
		mtdoobsize = nand_config_table[index].oobsize;
	} else
		printk("The type of nand flash is 2KB page, so use default configuration!\n");
}
#endif
int board_nand_init(struct nand_chip *this)
{
	int extid;

	g_info.chip = this;
	sc8810_nand_hw_init();
	//nfc_reset();
	this->IO_ADDR_R = this->IO_ADDR_W = (void __iomem	*)NFC_MBUF_ADDR;
	this->cmd_ctrl = sc8810_nand_hwcontrol;
	this->dev_ready = sc8810_nand_devready;
	this->select_chip = sc8810_nand_select_chip;

	this->ecc.calculate = sc8810_nand_calculate_ecc;
	this->ecc.correct = sc8810_nand_correct_data;
	this->ecc.hwctl = sc8810_nand_enable_hwecc;
	this->ecc.mode = NAND_ECC_HW;
	this->read_buf = sc8810_nand_read_buf;
	this->write_buf = sc8810_nand_write_buf;
	this->read_byte	= sc8810_nand_read_byte;
	this->read_word	= sc8810_nand_read_word;

	read_chip_id();
#ifndef CONFIG_NAND_SPL
	/* The 4th id byte is the important one */
	ptr_nand_spec = get_nand_spec(io_wr_port);

	if (ptr_nand_spec != NULL)
		set_nfc_timing(&ptr_nand_spec->timing_cfg, 153);
	else
		printf("ERROR! %s %d", __func__, __LINE__);
#endif
	extid = io_wr_port[3];
	/* Calc pagesize */
	mtdwritesize = 1024 << (extid & 0x3);
	extid >>= 2;
	/* Calc oobsize */
	mtdoobsize = (8 << (extid & 0x01)) * (mtdwritesize >> 9);
	extid >>= 2;
	/* Calc blocksize. Blocksize is multiples of 64KiB */
	mtderasesize = (64 * 1024) << (extid & 0x03);

	this->ecc.bytes = CONFIG_SYS_NAND_ECCBYTES;
	g_info.ecc_mode = CONFIG_SYS_NAND_ECC_MODE;
	this->eccbitmode = g_info.ecc_mode;
	this->ecc.size = CONFIG_SYS_NAND_ECCSIZE;
#ifdef CONFIG_NAND_SPL
	if (mtdoobsize == 64)
		this->ecc.layout = &_nand_oob_64;
	else if (mtdoobsize == 128)
		this->ecc.layout = &nand_oob_128;
	nand_spl_hardware_config(this, io_wr_port);

#ifdef CONFIG_DDR_PARA
/* DDR para adapt*/
	ddr_para_reset(io_wr_port);
#endif

#endif
	this->chip_delay = 20;
	this->priv = &g_info;
	this->options |= NAND_BUSWIDTH_16 | NAND_USE_FLASH_BBT;

	return 0;
}

#ifndef CONFIG_NAND_SPL
static unsigned long nfc_read_status(void)
{
	unsigned long status = 0;
	int i, cmd = NAND_CMD_STATUS;

	nfc_mcr_inst_init();
	nfc_reg_write(NFC_CMD, 0x80000070);
	sc8810_nfc_wait_command_finish(NFC_DONE_EVENT);
	memcpy(io_wr_port, (void *)NFC_ID_STS, 1);

	status = io_wr_port[0];
	return status;
}

static int sprd_scan_one_block(int blk, int erasesize, int writesize)
{
	int i, cmd;
	int status = 1, ii;
	u32 size = 0;
	//int oobsize = (writesize / 1024) * 32;
	int oobsize = mtdoobsize;
	int column, page_addr;

	page_addr = blk * (erasesize / writesize);
	for (ii = 0; ii < 2; ii ++) {
		nfc_mcr_inst_init();
		cmd = NAND_CMD_READ0;
		nfc_mcr_inst_add(cmd, NF_MC_CMD_ID);
		page_addr += ii;
		column = writesize;
		if (column != -1) {
			/* Adjust columns for 16 bit buswidth */
			column >>= 1;
			nfc_mcr_inst_add(column & 0xff, NF_MC_ADDR_ID);
			nfc_mcr_inst_add((column >> 8) & 0xff, NF_MC_ADDR_ID);
		}

		if (page_addr != -1) {
			nfc_mcr_inst_add(page_addr & 0xff, NF_MC_ADDR_ID);
			nfc_mcr_inst_add((page_addr >> 8) & 0xff, NF_MC_ADDR_ID);
			/* One more address cycle for devices > 128MiB */
			nfc_mcr_inst_add((page_addr >> 16) & 0xff, NF_MC_ADDR_ID);
		}

		cmd = NAND_CMD_READSTART;
		nfc_mcr_inst_add(cmd, NF_MC_CMD_ID);
		nfc_mcr_inst_add(0, NF_MC_WAIT_ID);

		if((!g_info.addr_array[0]) && (!g_info.addr_array[1]) )
			size = writesize + oobsize;//main part
		else
			size = oobsize;
		sc8810_nand_data_add(size, 1, 1);

		nfc_mcr_inst_exc();
		sc8810_nfc_wait_command_finish(NFC_DONE_EVENT);

		memcpy(io_wr_port, (void *)NFC_MBUF_ADDR, size);
#if 0
		for (i = 0; i < size; i++) {
			/*if ((i % 16) == 0)
				printf("\n");*/
        		printf("io_wr_port[%d] = 0x%02x\n", i, io_wr_port[i]);
		}
#endif
		if ((io_wr_port[0] != 0xff) || (io_wr_port[1] != 0xff))
			break;
	} //for (ii = 0; ii < 2; ii ++)

	if ((io_wr_port[0] == 0xff) && (io_wr_port[1] == 0xff))
		status = 0; //good block
	else
		status = 1; //bad block

	return status;
}

static unsigned long nand_ctl_erase_block(int blk, int erasesize, int writesize)
{
	int cmd, status;
	int page_addr;

	nfc_mcr_inst_init();
	cmd = NAND_CMD_ERASE1;
	nfc_mcr_inst_add(cmd, NF_MC_CMD_ID);
	page_addr = blk * (erasesize / writesize);
	if (page_addr != -1) {
		nfc_mcr_inst_add(page_addr & 0xff, NF_MC_ADDR_ID);
		nfc_mcr_inst_add((page_addr >> 8) & 0xff, NF_MC_ADDR_ID);
		/* One more address cycle for devices > 128MiB */
		nfc_mcr_inst_add((page_addr >> 16) & 0xff, NF_MC_ADDR_ID);
	}

	cmd = NAND_CMD_ERASE2;
	nfc_mcr_inst_add(cmd, NF_MC_CMD_ID);
	nfc_mcr_inst_add(0, NF_MC_WAIT_ID);
	nfc_mcr_inst_exc();
	sc8810_nfc_wait_command_finish(NFC_DONE_EVENT);

	status = nfc_read_status();
	return status;
}
#endif

void read_chip_id(void)
{
	int i, cmd = NAND_CMD_READID;

	nfc_mcr_inst_init();
	nfc_mcr_inst_add(cmd, NF_MC_CMD_ID);
	nfc_mcr_inst_add(0x00, NF_MC_ADDR_ID);
	nfc_mcr_inst_add(0x10, NF_MC_NOP_ID);//add nop clk for twrh timing param
	nfc_mcr_inst_add(7, NF_MC_RWORD_ID);
	nfc_mcr_inst_exc_for_id();
	sc8810_nfc_wait_command_finish(NFC_DONE_EVENT);
	//memcpy(io_wr_port, (void *)NFC_MBUF_ADDR, 5);
	correct_invalid_id(io_wr_port);
        memcpy(nand_id_table, io_wr_port, 5);
#if DEBUG
	printf("\r\n");
	for (i = 0; i < 5; i++)
        printf("read_chip_id NAND ID: %02x ", io_wr_port[i]);
	printf("\r\n");
#endif
}

#ifndef CONFIG_NAND_SPL
void McuReadNandType(unsigned char *array)
{
	int i, cmd = NAND_CMD_READID;

	nfc_mcr_inst_init();
	nfc_mcr_inst_add(cmd, NF_MC_CMD_ID);
	nfc_mcr_inst_add(0x00, NF_MC_ADDR_ID);
	nfc_mcr_inst_add(7, NF_MC_RWORD_ID);
	nfc_mcr_inst_exc_for_id();
	sc8810_nfc_wait_command_finish(NFC_DONE_EVENT);
	memcpy(array, (void *)NFC_MBUF_ADDR, 5);
}
#endif

#ifndef CONFIG_NAND_SPL
void nand_scan_patition(int blocks, int erasesize, int writesize)
{
	int blk;
	int ret;
	int status;

	//read_chip_id();
	for (blk = 0; blk < blocks; blk ++) {
		ret = sprd_scan_one_block(blk, erasesize, writesize);
		if (ret != 0) {
			printf("\n%d is bad, scrub to erase it, ", blk);
			ret = nand_ctl_erase_block(blk, erasesize, writesize);
			printf("0x%02x\n", ret);
		} else {
			ret = nand_ctl_erase_block(blk, erasesize, writesize);
			printf("erasing block : %d    %d % \r", blk, (blk * 100 ) / blocks);
		}
	}
}

int nand_scan_block(int block, int erasesize, int writesize){
	int ret = 0;
	ret = nand_ctl_erase_block(block, erasesize, writesize);
	ret = ret&1;

	return ret;
}
#endif
