#include <config.h>
#include <common.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#include <linux/string.h>
#include <android_bootimg.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <nand.h>
#include <android_boot.h>
#include <environment.h>
#include <jffs2/jffs2.h>
#include <boot_mode.h>
#include <malloc.h>

#include <asm/io.h>
#include <asm/arch/analog_reg_v3.h>
#include <asm/arch/adi_hal_internal.h>

unsigned char raw_header[8192];
static int flash_page_size = 0;

typedef enum{false, true} bool;
bool normal_shutdown = true;
bool lcd_resume = true;
#define LCD_RESUME 0xa5
#define REG32(x)   (*((volatile uint32 *)(x)))

#define VMJALUNA_PART "vmjaluna"
#define MODEM_PART "modem"
#define KERNEL_PART "kernel"
#define FIXNV_PART "fixnv"
#define BACKUPFIXNV_PART "backupfixnv"
#define RUNTIMEVN_PART "runtimenv"
#define DSP_PART "dsp"
#ifdef CONFIG_SP8810W
#define FIRMWARE_PART "firmware"
#endif
#define SIMLOCK_PART "simlock"

#define SIMLOCK_SIZE		1024
#define DSP_SIZE		(3968 * 1024)
#define VMJALUNA_SIZE		(300 * 1024)

#ifdef CONFIG_SP8810W
#define FIXNV_SIZE			(120 * 1024)
#define FIRMWARE_SIZE 		(9933 * 1024) /* 9.7M */
#else
#define FIXNV_SIZE		(64 * 1024)
#endif

#define PRODUCTINFO_SIZE	(3 * 1024)
#define RUNTIMENV_SIZE		(256 * 1024)
#ifdef MCP_F2R1
#define MODEM_SIZE		(3500 * 1024)  	/* 3.5MB */
#else
#define MODEM_SIZE		(8 * 1024 * 1024)
#endif

#define DSP_ADR			0x00020000
#define VMJALUNA_ADR		0x00400000
#define FIXNV_ADR		0x00480000
#define RUNTIMENV_ADR		0x004a0000
#define MODEM_ADR		0x00500000
#define RAMDISK_ADR 		0x04c00000
#ifdef CONFIG_SP8810W
#define PRODUCTINFO_ADR		0x0049E000
#define FIRMWARE_ADR		0x01600000
#else
#define PRODUCTINFO_ADR		0x00490000
#define SIMLOCK_ADR		0x4fe000
#endif

#if BOOT_NATIVE_LINUX
//pls make sure uboot running area
#define VLX_TAG_ADDR            (0x100)
#define KERNEL_ADR		(0x8000)

#else

#define KERNEL_ADR		0x04508000
#define VLX_TAG_ADDR            0x5100000 //after initrd

#endif

#define MAX_SN_LEN 			(24)
#define SP09_MAX_SN_LEN			MAX_SN_LEN
#define SP09_MAX_STATION_NUM		(15)
#define SP09_MAX_STATION_NAME_LEN	(10)
#define SP09_SPPH_MAGIC_NUMBER          (0X53503039)    // "SP09"
#define SP09_MAX_LAST_DESCRIPTION_LEN   (32)

//the following fixnv parameters's ids may need to be customized,please pay attention to this!!!
	//calibration   0x2
#define FIXNV_CALIBRATION_ID        0x2
	//IMEI 0x5,0x179,0x186,0x1e4,
#define FIXNV_IMEI1_ID        0x5
#define FIXNV_IMEI2_ID        0x179
#define FIXNV_IMEI3_ID        0x186
#define FIXNV_IMEI4_ID        0x1e4
	//TD_calibration 0x516
#define FIXNV_TD_CALIBRATION_ID        0x516
	//blue tooth 0x191
#define FIXNV_BT_ID        0x191
	//band select 0xd
#define FIXNV_BAND_SELECT_ID        0xd
	//WIFI 0x199
#define FIXNV_WIFI_ID        0x199
	//MMITEST 0x19a
#define FIXNV_MMITEST_ID        0x19a

typedef struct _tagSP09_PHASE_CHECK
{
	unsigned long 	Magic;                	// "SP09"
	char    	SN1[SP09_MAX_SN_LEN]; 	// SN , SN_LEN=24
	char    	SN2[SP09_MAX_SN_LEN];    // add for Mobile
	int     	StationNum;                 	// the test station number of the testing
	char    	StationName[SP09_MAX_STATION_NUM][SP09_MAX_STATION_NAME_LEN];
	unsigned char 	Reserved[13];               	//
	unsigned char 	SignFlag;
	char    	szLastFailDescription[SP09_MAX_LAST_DESCRIPTION_LEN];
	unsigned short  iTestSign;				// Bit0~Bit14 ---> station0~station 14
	                 		  			  //if tested. 0: tested, 1: not tested
	unsigned short  iItem;    // part1: Bit0~ Bit_14 indicate test Station,1 : Pass,

}SP09_PHASE_CHECK_T, *LPSP09_PHASE_CHECK_T;
const static int SP09_MAX_PHASE_BUFF_SIZE = sizeof(SP09_PHASE_CHECK_T);
static int check_compatibilty =  0;
char u_boot_version[] __attribute__ ((section("u-boot-version"))) = {'s','p','r','d','-','u','-','b','o','o','t','-','V','1','.','0','.','1'};

///////////////////////////////////////////////////////////////////
//CRC Table
///////////////////////////////////////////////////////////////////
/** CRC table for the CRC-16. The poly is 0x8005 (x^16 + x^15 + x^2 + 1) */
static const unsigned short  crc16_table[256] =
{
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};
/*********************************************************************/
unsigned short crc16 (unsigned short crc, unsigned char const *buffer, unsigned int len)
{
    while (len--)
    {
        crc = (crc >> 8) ^ (crc16_table[ (crc^ (*buffer++)) &0xff]);
    }

    return crc;
}
uint16_t adc_voltage_tab[2][2] =
{
    {947, 4200},
    {811, 3600},
};

void set_adc_vol_table(uint16_t *table)
{
	adc_voltage_tab[0][0] = (table[0]>>16)&0xffff;
	adc_voltage_tab[0][1] = table[0]&0xffff;
	adc_voltage_tab[1][0] = (table[1]>>16)&0xffff;
	adc_voltage_tab[1][1] = table[1]&0xffff;
	
}

uint16_t dcdc_AdcvalueToVoltage(uint16_t adcvalue)
{
	 int32_t temp;
    temp = adc_voltage_tab[0][1] - adc_voltage_tab[1][1];
    temp = temp * (adcvalue - adc_voltage_tab[0][0]);
    temp = temp / (adc_voltage_tab[0][0] - adc_voltage_tab[1][0]);
    return temp + adc_voltage_tab[0][1];
}

#define VLX_ADC_ID   2
#define VLX_RAND_TO_U32( _addr ) \
	if( (_addr) & 0x3 ){_addr += 0x4 -((_addr) & 0x3); }


u32 Vlx_GetFixedBvitemAddr(u16 identifier, u32 search_start, u32 search_end)
{
	u32 start_addr, end_addr;
	u16 id, len;
	volatile u16 *flash_ptr;

	start_addr = search_start;
	end_addr   = search_end;
	start_addr += sizeof(u32); //skip update flag
	
	while(start_addr < end_addr)
	{
		flash_ptr = (volatile u16 *)(start_addr);
		id  = *flash_ptr++;
		len = *flash_ptr;
		if(0xFFFF == id)
		{
			return 0xffffffff;
		}
		if(identifier == id)
		{
			return (start_addr + 4);	
		}
		else
		{
			start_addr += 4 + len +(len & 0x1);
			VLX_RAND_TO_U32( start_addr )
		}
	}
	return 0xffffffff;
}

int eng_getphasecheck(SP09_PHASE_CHECK_T* phase_check)
{
	int aaa;
	unsigned long tested;

	if (phase_check->Magic == SP09_SPPH_MAGIC_NUMBER) {
		//printf("Magic = 0x%08x\n",phase_check->Magic);
		printf("SN1 = %s   SN2 = %s\n",phase_check->SN1, phase_check->SN2);
		/*printf("StationNum = %d\n",phase_check->StationNum);
		printf("Reserved = %s\n",phase_check->Reserved);
		printf("SignFlag = 0x%02x\n",phase_check->SignFlag);
		printf("iTestSign = 0x%04x\n",phase_check->iTestSign);
		printf("iItem = 0x%04x\n",phase_check->iItem);*/
		if (phase_check->SignFlag == 1) {
			for (aaa = 0; aaa < phase_check->StationNum/*SP09_MAX_STATION_NUM*/; aaa ++) {
				printf("%s : ", phase_check->StationName[aaa]);
				tested = 1 << aaa;
				if ((tested & phase_check->iTestSign) == 0) {
					if ((tested & phase_check->iItem) == 0)
						printf("Pass; ");
					else
						printf("Fail; ");
				} else
					printf("UnTested; ");
			}
		} else {
			printf("station status are all invalid!\n");
			for (aaa = 0; aaa < phase_check->StationNum/*SP09_MAX_STATION_NUM*/; aaa ++)
				printf("%s  ", phase_check->StationName[aaa]);
		}
		printf("\nLast error: %s\n",phase_check->szLastFailDescription);
	} else
		printf("no production information / phase check!\n");

	return 0;
}

int eng_phasechecktest(unsigned char *array, int len)
{
	SP09_PHASE_CHECK_T phase;

	memset(&phase, 0, sizeof(SP09_PHASE_CHECK_T));
	memcpy(&phase, array, len);

	return eng_getphasecheck(&phase);
}

extern void cmd_yaffs_mount(char *mp);
extern void cmd_yaffs_umount(char *mp);
extern int cmd_yaffs_ls_chk(const char *dirfilename);
extern void cmd_yaffs_mread_file(char *fn, unsigned char *addr);
extern void cmd_yaffs_mwrite_file(char *fn, char *addr, int size);
extern void cmd_yaffs_rm(const char *path);
void set_vibrator(int on);
void vibrator_hw_init(void);
void MMU_InvalideICACHEALL(void);

void nand_block_info(struct mtd_info *nand, int *good, int *bad)
{
	loff_t off;
	int goodblk, badblk;

	goodblk = badblk = 0;

	for (off = 0; off < nand->size; off += nand->erasesize)
		if (nand_block_isbad(nand, off)) {
			//printf("bad block :  %08llx\n", (unsigned long long)off);
			badblk ++;
		} else {
			//printf("good block : %08llx\n", (unsigned long long)off);
			goodblk ++;
		}
	*good = goodblk;
	*bad = badblk;
}

static unsigned int Vlx_CalcFixnvLen(unsigned int search_start, unsigned int search_end)
{
	unsigned int start_addr, end_addr;
	unsigned short id, len;
	volatile unsigned short *flash_ptr;

	start_addr = search_start;
	end_addr   = search_end;
	start_addr += sizeof(unsigned int); //skip update flag
	
	while(start_addr < end_addr)
	{
		flash_ptr = (volatile unsigned short *)(start_addr);
		id  = *flash_ptr++;
		len = *flash_ptr;
		if(0xFFFF == id)
		{
			return (start_addr-search_start);
		}
		else
		{
			start_addr += 4 + len +(len & 0x1);
			VLX_RAND_TO_U32( start_addr );
		}
	}
	return 0xffffffff;
}

static int check_fixnv_struct(unsigned int addr,unsigned int size){
	unsigned int length = 0,keep_length=0;
	volatile unsigned int *flash_ptr;
	
	flash_ptr = (volatile unsigned int *)(addr+size-8);
	keep_length = *flash_ptr;
	//printf("keep_length=%d  line=%d\r\n",keep_length ,__LINE__);
	if(keep_length != 0xffffffff){
		length = Vlx_CalcFixnvLen(addr, addr+size);
		if(keep_length != length){
			printf("keep_length=%d  length=%d line=%d\r\n",keep_length ,length,__LINE__);
			return -1;
		}
	}
	return 1;
}

/*
* retval : -1 is wrong  ;  1 is correct
*/
int nv_is_correct(unsigned char *array, unsigned long size)
{
	//if ((array[size] == 0x5a) && (array[size + 1] == 0x5a) && (array[size + 2] == 0x5a) && (array[size + 3] == 0x5a)) {
	//	array[size] = 0xff; array[size + 1] = 0xff;
	//	array[size + 2] = 0xff; array[size + 3] = 0xff;
	//	return 1;
	//} else
	//	return -1;
	return nv_is_correct_endflag(array,size);
}

int nv_is_correct_endflag(unsigned char *array, unsigned long size)
{
	//if ((array[size] == 0x5a) && (array[size + 1] == 0x5a) && (array[size + 2] == 0x5a) && (array[size + 3] == 0x5a))
		//return 1;
	//else
		//return -1;
	//check both crc16 and fixed flag.	
	if( (0xffff == *((unsigned short*)&array[size-2])&&
		0x5a5a5a5a == *((unsigned int*)&array[size])&&
		check_compatibilty)||/*old flag*/
		((unsigned short*)crc16(0,array, size)== *((unsigned short*)&array[size])&&
		0x5a5a == *((unsigned short*)&array[size+2])&&
		check_compatibilty)||/*new flag*/
		((unsigned short*)crc16(0,array, size-2)== *((unsigned short*)&array[size-2])&&
		0x5a5a5a5a == *((unsigned int*)&array[size])&&
		*((unsigned short*)&array[size-2]) != 0)/*latest flag*//*if crc flag is zero then there may be an fatal error*/		
	){
		//*((unsigned int *)&array[size])= 0xffffffff;
		return 1; 
	}else{
		return -1;
	}

}
void array_value_range(unsigned char * array, int start, int end)
{
	int aaa;

	printf("\n\n");

	for (aaa = start; aaa <= end; aaa ++) {
		printf("arr[%d] = %02x\n", aaa, array[aaa]);
	}

	printf("\n\n");
}


void array_value(unsigned char * array, int len)
{
	int aaa;

	printf("\n\n");

	for (aaa = 0; aaa < len; aaa ++) {
		if ((aaa % 16) == 0)
			printf("\n");
		printf(" %02x", array[aaa]);
	}

	printf("\n\n");
}

void array_diff(unsigned char * array1, unsigned char * array2, int len)
{
	int ii;

	printf("arrar diff is starting   array1 = 0x%08x  array2 = 0x%08x  len = %d\n", (unsigned int)array1, (unsigned int)array2, len);
	for (ii = 0; ii < len; ii ++) {
		if (array1[ii] != array2[ii]) {
			printf("\narray1[%d] = 0x%02x  array2[%d] = 0x%02x\n", ii, array1[ii], ii, array2[ii]);
		}
	}
	printf("arrar diff is finished\n");
}
static int start_linux()
{
	void (*theKernel)(int zero, int arch, u32 params);
	u32 exec_at = (u32)-1;
	u32 parm_at = (u32)-1;
	u32 machine_type;

	machine_type = 0x7dd;         /* get machine type */

	theKernel = (void (*)(int, int, u32))KERNEL_ADR; /* set the kernel address */

	*(volatile u32*)0x84001000 = 'j';
	*(volatile u32*)0x84001000 = 'm';
	*(volatile u32*)0x84001000 = 'p';

	*(volatile u32*)(0x20900000 + 0x218) |= (0x1);//internal ram using 0xffff0000
	theKernel(0, machine_type, VLX_TAG_ADDR);    /* jump to kernel with register set */
	while(1);
	return 0;
}

static void recovery_sector(const char* dst_sector_path,
			const char* dst_sector_name,
			const char* backup_dst_sector_name,
			unsigned char * mem_addr,
			unsigned int size)
{
	cmd_yaffs_mount(dst_sector_path);
	cmd_yaffs_mwrite_file(dst_sector_name, mem_addr, size);
	if(backup_dst_sector_name)
		cmd_yaffs_mwrite_file(backup_dst_sector_name, mem_addr, size);
	cmd_yaffs_umount(dst_sector_path);	
}

static int load_sector_to_memory(const char* sector_path,
				const char* sector_name,
				const char* backup_sector_name,
				unsigned char * mem_addr,
				unsigned int size)
{
	int ret = -1;
	int try_backup_file = 0;
	const char * curr_file = sector_name;

	//clear memory
	memset(mem_addr, 0xff,size);
	//mount yaffs
	cmd_yaffs_mount(sector_path);
	
TRY_BACKUP_FILE:
	//is file exist and has a right size?
	ret = cmd_yaffs_ls_chk(curr_file);
	if (ret == size) {
		//read file to mem
		cmd_yaffs_mread_file(curr_file, mem_addr);
		ret = nv_is_correct_endflag(mem_addr, size-4);
	}
	else{
		ret = -1;
	}
	
	//try backup files.
	if(backup_sector_name&&ret == -1&&!try_backup_file){
		curr_file = backup_sector_name;
		try_backup_file = 1;
		goto TRY_BACKUP_FILE;
	}else if(backup_sector_name&&ret == 1&&!try_backup_file){
		printf("[load_sector_to_memory]sync the latest file......\n");
		cmd_yaffs_mwrite_file(backup_sector_name, mem_addr, size);
	}

	if(try_backup_file&&ret==1){
		//recovery_sector(sector_path,sector_name,mem_addr,size);
		printf("[load_sector_to_memory]recovery the latest file......\n");
		cmd_yaffs_mwrite_file(sector_name, mem_addr, size);
	}else if(try_backup_file&&ret==-1){
		printf("[load_sector_to_memory]both of the files are not correct......\n");
	}

	//unmout yaffs
	cmd_yaffs_umount(sector_path);
	
	return ret;
}

static int load_kernel_and_layout(struct mtd_info *nand,
							unsigned int phystart,
							char *header,
							char *kernel,
							char *ramdisk,
							unsigned int virtual_page_size,
							unsigned int real_page_size
							){
	int ret = -1; 
	boot_img_hdr *hdr = (boot_img_hdr*)header;
	unsigned int off = phystart;
	int size = real_page_size;
	
	printf("virtual_page_size : %x\n",virtual_page_size);
	printf("real_page_size : %x\n",real_page_size);
	//read boot image header
	ret = nand_read_offset_ret(nand, off, &size, (void *)hdr, &off);
	if(ret != 0){
		printf("function: %s nand read error %d\n", __FUNCTION__, ret);
        return -1;
	}
	if(memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)){
		printf("bad boot image header, give up read!!!!\n");
        return -1;
	}
	else
	{

		char* prev_page_addr = header;
		//we asume that the header takes only one page.
		//read kernel image prepare
		unsigned int used_size = 1*virtual_page_size;
		unsigned int spare_size = 0;
		unsigned int next_file_size = hdr->kernel_size;

		if(used_size > 0){
			spare_size = real_page_size - used_size;
		}else{
			spare_size = 0;
		}
		//read kernel image
		printf("file size: %x\n",hdr->kernel_size);
		printf("use size: %x\n",used_size);
		printf("spare size: %x\n",spare_size);	
		
		if(spare_size){
			memcpy(kernel,&prev_page_addr[used_size],spare_size);
			next_file_size -= spare_size;
		}		
		size = (next_file_size+(real_page_size - 1)) & (~(real_page_size - 1));
		ret = nand_read_offset_ret(nand, off, &size, (void *)(kernel+spare_size), &off);
		if(ret != 0){
			printf("reading kernel error!\n");
			printf("try reading to %x\n",kernel+spare_size);
		}
		//read ramdisk image prepare
		prev_page_addr =  (char*)(kernel+spare_size+size-real_page_size);
		used_size = (next_file_size%real_page_size+virtual_page_size-1)&(~(virtual_page_size-1));
		if(used_size > 0){
			spare_size = real_page_size - used_size;
		}else{
			spare_size = 0;
		}
		next_file_size = hdr->ramdisk_size;
		printf("file size: %x\n",hdr->ramdisk_size);
		printf("use size: %x\n",used_size);
		printf("spare size: %x\n",spare_size);
		//read ramdisk image
		if(spare_size){
			memcpy(ramdisk,&prev_page_addr[used_size],spare_size);
			next_file_size -= spare_size;
		}		
		size = (next_file_size+(real_page_size - 1)) & (~(real_page_size - 1));
		ret = nand_read_offset_ret(nand, off, &size, (void *)(ramdisk+spare_size), &off);
		if(ret != 0){
			printf("reading ramdisk error!\n");
			printf("try reading to %x\n",ramdisk+spare_size);
		}
	}

	return ret;
}

void update_fixnv(unsigned char *old_addr,unsigned char*new_addr){
	unsigned short item_id = 0,i = 0,item_array[] = {FIXNV_CALIBRATION_ID,
                                                 FIXNV_IMEI1_ID,
                                                 FIXNV_IMEI2_ID,
                                                 FIXNV_IMEI3_ID,
                                                 FIXNV_IMEI4_ID,
                                                 FIXNV_TD_CALIBRATION_ID,
                                                 FIXNV_BT_ID,
                                                 FIXNV_BAND_SELECT_ID,
                                                 FIXNV_WIFI_ID,
                                                 FIXNV_MMITEST_ID,
                                                 0x0};
	unsigned int old_item_base = 0,new_item_base = 0,length = 0;
	unsigned short old_item_len=0, new_item_len=0;
	
	while(item_id = item_array[i++]){
		old_item_base = Vlx_GetFixedBvitemAddr(item_id, old_addr, (old_addr+FIXNV_SIZE));
		new_item_base = Vlx_GetFixedBvitemAddr(item_id, new_addr, (new_addr+FIXNV_SIZE));
		if(old_item_base == 0xffffffff || new_item_base == 0xffffffff){
			continue;
		}
		old_item_len =*(unsigned short*)(old_item_base-2);
		new_item_len =*(unsigned short*)(new_item_base-2);
		printf("item_id = 0x%x,old_item_len = %d,new_item_len = %d\n",item_id,new_item_len,old_item_len);
		if(old_item_len == new_item_len&&old_item_len != 0xffffffff){
			printf("copy from 0x%x to 0x%x and size = %d\n",old_item_base,new_item_base,old_item_len);
			memcpy(new_item_base,old_item_base,old_item_len);
		}
	}

	length = Vlx_CalcFixnvLen(new_addr, new_addr+FIXNV_SIZE);	
	*((unsigned int*)&new_addr[FIXNV_SIZE-8])= length;//keep the real  fixnv file size.	
	*(unsigned short*)&new_addr[FIXNV_SIZE-2] = crc16(0,(unsigned const char*)new_addr,FIXNV_SIZE-2);
	*(unsigned int*)&new_addr[FIXNV_SIZE] = 0x5a5a5a5a;
	memcpy(old_addr,new_addr,FIXNV_SIZE+4);
}

//if not to boot native linux, cmdline=NULL, kerne_pname=boot, backlight_set=on.
void vlx_nand_boot(char * kernel_pname, char * cmdline, int backlight_set)
{
    boot_img_hdr *hdr = (void *)raw_header;
	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	struct nand_chip *chip;
	u8 pnum;
	int ret;
	size_t size;
	loff_t off = 0;
	char *fixnvpoint = "/fixnv";
	char *fixnvfilename = "/fixnv/fixnv.bin";
	char *fixnvfilename2 = "/fixnv/fixnvchange.bin";
	char *backupfixnvpoint = "/backupfixnv";
	char *backupfixnvfilename = "/backupfixnv/fixnv.bin";
	char *runtimenvpoint = "/runtimenv";
	char *runtimenvfilename = "/runtimenv/runtimenv.bin";
	char *runtimenvfilename2 = "/runtimenv/runtimenvbkup.bin";
	char *productinfopoint = "/productinfo";
	char *productinfofilename = "/productinfo/productinfo.bin";
	char *productinfofilename2 = "/productinfo/productinfobkup.bin";
	int orginal_right, backupfile_right;
	unsigned long orginal_index, backupfile_index;
	nand_erase_options_t opts;
    	char * mtdpart_def = NULL;
        #ifdef CONFIG_SC8810
     	MMU_Init(CONFIG_MMU_TABLE_ADDR);
	#endif
	ret = mtdparts_init();
	if (ret != 0){
		printf("mtdparts init error %d\n", ret);
		return;
	}


#if !(BOOT_NATIVE_LINUX)
	/*int good_blknum, bad_blknum;
	nand_block_info(nand, &good_blknum, &bad_blknum);
	printf("good is %d  bad is %d\n", good_blknum, bad_blknum);*/

	///////////////////////////////////////////////////////////////////////
	printf("test if there is a need to update spl......\n");
	{
		char *spl_mount_point = "/spl";
		char *tmp_mount_point = "/backupfixnv";
		char *tmp_file_name = "/backupfixnv/spl.bin";
		int length = 0;

		ret = find_dev_and_part("spl", &dev, &pnum, &part);
                 if(!ret)
                         nand = &nand_info[dev->id->num];
		cmd_yaffs_mount(tmp_mount_point);
		length = cmd_yaffs_ls_chk(tmp_file_name);
		if(length > 0){
			//try to update spl.
			int ret = 0;
			printf("trying to update spl ......\n");
			memset((unsigned char*)RUNTIMENV_ADR, 0xff,16*1024);//spl.bin's valid size is 16K
			cmd_yaffs_mread_file(tmp_file_name, RUNTIMENV_ADR);
			extern int nand_write_spl(u8 *buf, struct mtd_info *mtd);
			ret = nand_write_spl((u8*)RUNTIMENV_ADR, nand);
			if(ret != 0){
				return -1;
			}
			printf("spl is updated remove the tmp bin file in backupfixnv partition!\n");
			cmd_yaffs_rm(tmp_file_name);
		}
		cmd_yaffs_umount(tmp_mount_point);	
		
	}

	printf("test if there is a need to update fixnv......\n");
	{
		int length = 0,need_update = 0;
		char *nvitem_point = "/backupfixnv";
		char *nvitem_name = "/backupfixnv/nvitem.bin";
		cmd_yaffs_mount(nvitem_point);
		length = cmd_yaffs_ls_chk(nvitem_name);
		if(length > 0&&length < FIXNV_SIZE){
			memset((unsigned char*)RUNTIMENV_ADR, 0xff,FIXNV_SIZE + 4);
			cmd_yaffs_mread_file(nvitem_name, RUNTIMENV_ADR);
			need_update = 1;
			check_compatibilty = 1;//some times the fixnv files in the phone is old style and use OTA!!
		}
		cmd_yaffs_umount(nvitem_point);
	}


	/* FIXNV_PART */
	printf("Reading fixnv to 0x%08x\n", FIXNV_ADR);
	//try "/fixnv/fixnvchange.bin" first,if fail,
	//try /fixnv/fixnv.bin instead
	ret = load_sector_to_memory(fixnvpoint,
							fixnvfilename2,//fixnvfilename,
							fixnvfilename,//fixnvfilename2,
							(unsigned char *)FIXNV_ADR,
							FIXNV_SIZE + 4);
	if(ret == -1){
		//fixnvpoint's files are not correct
		//the "/backupfixnv/fixnv.bin" must be correct!
		ret = load_sector_to_memory(backupfixnvpoint,
						backupfixnvfilename,
						0,//backupfixnvfilename2,
						(unsigned char *)FIXNV_ADR,
						FIXNV_SIZE + 4);
		if(ret ==1){
			//we got a right file in backupfixnvpoint,
			//use it to recovery fixnvpoint's files.
			recovery_sector(fixnvpoint, 
				fixnvfilename, 
				fixnvfilename2, 
				(unsigned char *)FIXNV_ADR,
				FIXNV_SIZE + 4);
		}else{
			//backupfixnvpoint's files are still uncorrect.
			//then we can do nothing to get it right!!!!
			//there is an fatal error has occured.
			printf("\n\nfixnv and backupfixnv are all wrong!\n\n");
			//clear memory
			//memset(FIXNV_ADR, 0xff,FIXNV_SIZE + 4);
		}
	}else{
		//everything is right!!
        //there's nothing to do here ......
	}	

	
	
	if(check_compatibilty){		
		char *nvitem_point = "/backupfixnv";
		char *nvitem_name = "/backupfixnv/nvitem.bin";
		cmd_yaffs_mount(nvitem_point);		
		
		printf("update fixnv's items\n");	
		update_fixnv(FIXNV_ADR,RUNTIMENV_ADR);
		printf("update fixnv partition");
		recovery_sector(fixnvpoint, 
			fixnvfilename, 
			fixnvfilename2, 
			(unsigned char *)FIXNV_ADR,
			FIXNV_SIZE + 4);
		printf("remove /fixnv/nvitem.bin");
		cmd_yaffs_rm(nvitem_name);
		cmd_yaffs_umount(nvitem_point);	
		check_compatibilty = 0;//ensure the value is 0!
	}


	//finally we check the fixnv structure,if fail,then u-boot will hung up!!!		
	if(check_fixnv_struct(FIXNV_ADR,FIXNV_SIZE) == -1){

		return -1;
	}

	///////////////////////////////////////////////////////////////////////
	/* PRODUCTINFO_PART */
	check_compatibilty = 1;
	ret = load_sector_to_memory(productinfopoint,
							productinfofilename2,
							productinfofilename,
							(unsigned char *)PRODUCTINFO_ADR,
							PRODUCTINFO_SIZE + 4);
	check_compatibilty = 0;
	if(ret == -1){
		printf("Reading productinfo to 0x%08x error!\n", PRODUCTINFO_ADR);
	}
	eng_phasechecktest((unsigned char *)PRODUCTINFO_ADR, SP09_MAX_PHASE_BUFF_SIZE);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	/* RUNTIMEVN_PART */
	ret = load_sector_to_memory(runtimenvpoint,
							runtimenvfilename2,
							runtimenvfilename,
							(unsigned char *)RUNTIMENV_ADR,
							RUNTIMENV_SIZE + 4);
	if(ret == -1){
		//clear memory
		memset(RUNTIMENV_ADR, 0xff,RUNTIMENV_SIZE + 4);
	}
	////////////////////////////////////////////////////////////////


	{
		u32 item_base;
		int adc_offset = 0x175c;
		int value[4];
		unsigned int battery;
		int i;
		u32 battery_id[8]={0};
		item_base = Vlx_GetFixedBvitemAddr(VLX_ADC_ID, FIXNV_ADR, (FIXNV_ADR+FIXNV_SIZE));
		if(item_base != 0xffffffff)
		{
			item_base = item_base+adc_offset;
			for(i = 0; i < 4 ; i++){
				value[i] = *(volatile u8*)(item_base + i);
			}
			battery = (value[1]<<8) + value[0];
			battery &= 0xffff;
			adc_voltage_tab[0][1] = battery;

			battery = (value[3]<<8) + value[2];
			battery &= 0xffff;
			adc_voltage_tab[0][0] = battery;

			for(i = 0  ; i < 4 ; i++){
				value[i] = *(volatile u8*)(item_base + i + 4);
			}
			battery = (value[1]<<8) + value[0];
			battery &= 0xffff;
			adc_voltage_tab[1][1] = battery;

			battery = (value[3]<<8) + value[2];
			battery &= 0xffff;
			adc_voltage_tab[1][0] = battery;

			//printf("adc_voltage_tab 0x%x   0x%x  0x%x   0x%x \n",adc_voltage_tab[0][0],adc_voltage_tab[0][1],adc_voltage_tab[1][0],adc_voltage_tab[1][1]);
			
			for(i = 0; i < 4 ; i++){
				value[i] = *(volatile u8*)(item_base + i + 36);
				//printf("addr:item_base = 0x%x---value[i] = 0x%x \n",item_base + i,value[i]);
			}
			battery = (value[1]<<8) + value[0];
			battery &= 0xffff;
			//printf("battery = (value[1]<<8) + value[0]---0x%x \n",battery);

			if((battery &(BIT_9))){
				do_dcdc_work();
			}
		}
	
	}
	//add by chenmingsen.
    if(normal_shutdown){
		set_vibrator(0);
    }
	//add end.
#ifdef CONFIG_LCD
	extern int drv_lcd_init(void);
	if(normal_shutdown == true){
		drv_lcd_init();
	}else if(lcd_resume == true){
      set_backlight(100);
	//add by chenmingsen.
	}else{
		printf("%s normal_shutdown=%s\n", __func__, normal_shutdown?"true":"false");
		drv_lcd_init();
	//add end.
	}
#endif

#ifdef CONFIG_SPLASH_SCREEN
#define SPLASH_PART "boot_logo"
	ret = find_dev_and_part(SPLASH_PART, &dev, &pnum, &part);
	if(ret){
		printf("No partition named %s\n", SPLASH_PART);
		return;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		printf("Partition %s not a NAND device\n", SPLASH_PART);
		return;
	}

	off=part->offset;
	nand = &nand_info[dev->id->num];
	//read boot image header
	size = 1<<19;//where the size come from????
	char * bmp_img = malloc(size);
	if(!bmp_img){
	    printf("not enough memory for splash image\n");
	    return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void *)bmp_img, &off);
	if(ret != 0){
		printf("function: %s nand read error %d\n", __FUNCTION__, ret);
		return;
	}
    extern int lcd_display_bitmap(ulong bmp_image, int x, int y);
    extern void lcd_display(void);
    extern void set_backlight(uint32_t value);
    if(backlight_set == BACKLIGHT_ON){
	    extern void *lcd_base;
	    extern void Dcache_CleanRegion(unsigned int addr, unsigned int length);

	    lcd_display_bitmap((ulong)bmp_img, 0, 0);
#ifdef CONFIG_SC8810
	    Dcache_CleanRegion((unsigned int)(lcd_base), size);//Size is to large.
#endif
        if(normal_shutdown == true){
	    	lcd_display();
			//modify by chenmingsen.
			udelay(100000);
	    	set_backlight(255);
       	    //printf("aftersetbacklight\n");
	    	udelay(1);	//zhuwenjian need a delay to effect register
    	}
		else {
			lcd_display();
			printf("not set backlight if abnormal\n");
			//modify end.
	    	udelay(1);	//zhuwenjian need a delay to effect register
    	}
    }
#endif
//    set_vibrator(0);
	
	/* DSP_PART */
	printf("Reading dsp to 0x%08x\n", DSP_ADR);
	ret = find_dev_and_part(DSP_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", DSP_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", DSP_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];
	flash_page_size = nand->writesize;
	size = (DSP_SIZE + (flash_page_size - 1)) & (~(flash_page_size - 1));
	if(size <= 0) {
		printf("dsp image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)DSP_ADR, &off);
	if(ret != 0) {
		printf("dsp nand read error %d\n", ret);
		return;
	}
#endif
	////////////////////////////////////////////////////////////////
		/* KERNEL_PART */
	printf("Reading kernel to 0x%08x\n", KERNEL_ADR);
	ret = find_dev_and_part(kernel_pname, &dev, &pnum, &part);
	if(ret){
		printf("No partition named %s\n", kernel_pname);
        return;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		printf("Partition %s not a NAND device\n", kernel_pname);
        return;
	}
	off=part->offset;
	nand = &nand_info[dev->id->num];
	ret = load_kernel_and_layout(nand, 
							(unsigned int)off, 
							(char *)raw_header, 
							(char *) KERNEL_ADR,
							(char *) RAMDISK_ADR, 
							2048, 
							nand->writesize);
	if(ret != 0){
		printf("ramdisk nand read error %d\n", ret);
		return;
	}

	
#if !(BOOT_NATIVE_LINUX)
	////////////////////////////////////////////////////////////////
	/* MODEM_PART */
	printf("Reading modem to 0x%08x\n", MODEM_ADR);
	ret = find_dev_and_part(MODEM_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", MODEM_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", MODEM_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];
	size = (MODEM_SIZE +(flash_page_size - 1)) & (~(flash_page_size - 1));
	if(size <= 0) {
		printf("modem image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)MODEM_ADR, &off);
	if(ret != 0) {
		printf("modem nand read error %d\n", ret);
		return;
	}

	//array_value((unsigned char *)MODEM_ADR, MODEM_SIZE);
#ifdef CONFIG_SP8810W
	/* FIRMWARE_PART */
	printf("Reading firmware to 0x%08x\n", FIRMWARE_ADR);
	ret = find_dev_and_part(FIRMWARE_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", FIRMWARE_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", FIRMWARE_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];
	size = (FIRMWARE_SIZE +(flash_page_size - 1)) & (~(flash_page_size - 1));
	if(size <= 0) {
		printf("firmware image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)FIRMWARE_ADR, &off);
	if(ret != 0) {
		printf("firmware nand read error %d\n", ret);
		return;
	}
#else
   /* SIMLOCK_PART */
	printf("Reading simlock to 0x%08x\n", SIMLOCK_ADR);
	ret = find_dev_and_part(SIMLOCK_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", SIMLOCK_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", SIMLOCK_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];
	size = (SIMLOCK_SIZE +(flash_page_size - 1)) & (~(flash_page_size - 1));
	if(size <= 0) {
		printf("simlock image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)SIMLOCK_ADR, &off);
	if(ret != 0) {
		printf("simlock nand read error %d\n", ret);
		return;
	}
#endif
	////////////////////////////////////////////////////////////////
	/* VMJALUNA_PART */
	printf("Reading vmjaluna to 0x%08x\n", VMJALUNA_ADR);
	ret = find_dev_and_part(VMJALUNA_PART, &dev, &pnum, &part);
	if (ret) {
		printf("No partition named %s\n", VMJALUNA_PART);
		return;
	} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
		printf("Partition %s not a NAND device\n", VMJALUNA_PART);
		return;
	}

	off = part->offset;
	nand = &nand_info[dev->id->num];
	size = (VMJALUNA_SIZE +(flash_page_size - 1)) & (~(flash_page_size - 1));
	if(size <= 0) {
		printf("modem image should not be zero\n");
		return;
	}
	ret = nand_read_offset_ret(nand, off, &size, (void*)VMJALUNA_ADR, &off);
	if(ret != 0) {
		printf("modem nand read error %d\n", ret);
		return;
	}
#endif

	//array_value((unsigned char *)VMJALUNA_ADR, 16 * 10);
    //check caliberation mode
    int str_len;
    char * buf;
    buf = malloc(1024);
    memset (buf, 0, 1024);

    sprintf(buf, "initrd=0x%x,0x%x", RAMDISK_ADR, hdr->ramdisk_size);

    /* preset loop_per_jiffy */
    str_len = strlen(buf);
#ifdef CONFIG_LOOP_PER_JIFFY
    sprintf(&buf[str_len], " lpj=%d", CONFIG_LOOP_PER_JIFFY);
#else
    sprintf(&buf[str_len], " lpj=%d", 3350528); /* SC8810 1GHz */
#endif

    str_len = strlen(buf);
    mtdpart_def = get_mtdparts();
    sprintf(&buf[str_len], " %s", mtdpart_def);
    if(cmdline && cmdline[0]){
            str_len = strlen(buf);
            sprintf(&buf[str_len], " %s", cmdline);
    }

	char *factorymodepoint = "/productinfo";
	{
		//add lcd id
		extern uint32_t load_lcd_id_to_kernel();
		
		char *file_lcdid = "/productinfo/lcdid.file";
		uint32_t lcd_id = 0;
		char lcdid_buf[8]={0};
		char lcdid_buf1[8]={0};


		cmd_yaffs_mount(factorymodepoint);
		if (normal_shutdown == true){
			lcd_id = load_lcd_id_to_kernel();
			sprintf(lcdid_buf,"%x",lcd_id);
			printf("lcdid_buf = 0x%s\n",lcdid_buf);
			cmd_yaffs_mwrite_file(file_lcdid,(char *)lcdid_buf,strlen(lcdid_buf)+4);
			ret = cmd_yaffs_ls_chk(file_lcdid);
			if(ret == -1)
				printf("can't find lcdid.file\n");
			cmd_yaffs_mread_file(file_lcdid, (unsigned char *)lcdid_buf1);
			printf("lcdid_buf1 = 0x%s\n",lcdid_buf1);
		}else{
			cmd_yaffs_mread_file(file_lcdid, (unsigned char *)lcdid_buf);
			printf("lcdid_buf--abnormal = 0x%s\n",lcdid_buf);
			lcd_id = simple_strtol(lcdid_buf, NULL, 16);
		}
		cmd_yaffs_umount(factorymodepoint);

		if(normal_shutdown != true){
			str_len = strlen(buf);
			sprintf(&buf[str_len], " abnormal");
		}

		
		printf("lcd_id = 0x%x\n",lcd_id);
		printf("buf = %s\n",buf);

		str_len = strlen(buf);
		sprintf(&buf[str_len], " video=sprdfb:fb0_id=0x%x,fb1_id=0x%x",
				 lcd_id, 0);
		str_len = strlen(buf);

	}
	{
		//char *factorymodepoint = "/productinfo";
		char *factorymodefilename = "/productinfo/factorymode.file";
		char *usb_serialfilename = "/productinfo/usb_s.ini";
		char *tpcalcoefilename = "/productinfo/tpcalcoef";  
		cmd_yaffs_mount(factorymodepoint);
		ret = cmd_yaffs_ls_chk(usb_serialfilename );
		if (ret == -1) {
			/* no usb file */
		} else {
			str_len = strlen(buf);
			sprintf(&buf[str_len], " usb_s=");
			str_len = strlen(buf);
			cmd_yaffs_mread_file(usb_serialfilename, &buf[str_len]);
			while((buf[str_len] != '\n') && (buf[str_len] != '\r'))//\r is mac file; \n is unix/linux file; \n\r is windows file;
			{
				str_len++;
			}
			buf[str_len] = '\0';
			buf[str_len+1] = '\0';
		}
		ret = cmd_yaffs_ls_chk(factorymodefilename );
		if (ret == -1) {
			/* no factorymode.file found, nothing to do */
		} else {
			str_len = strlen(buf);
			sprintf(&buf[str_len], " factory");
		}
		ret = cmd_yaffs_ls_chk(tpcalcoefilename );
		if (ret == -1) {
			/* no tpcalcoef file */
		} else {
			str_len = strlen(buf);
			sprintf(&buf[str_len], " cal_coef=");
			str_len = strlen(buf);
			cmd_yaffs_mread_file(tpcalcoefilename, &buf[str_len]);
			while((buf[str_len] != '\n') && (buf[str_len] != '\r'))//\r is mac file; \n is unix/linux file; \n\r is windows file;
			{
				str_len++;
			}
			buf[str_len] = '\0';
			buf[str_len+1] = '\0';
		}
		cmd_yaffs_umount(factorymodepoint);
	}
	str_len = strlen(buf);
#ifdef RAM512M
    sprintf(&buf[str_len], " ram=512M");
#else
    sprintf(&buf[str_len], " ram=256M");
#endif

	chip = (struct nand_chip *)(nand->priv);
	str_len = strlen(buf);
	sprintf(&buf[str_len], " nandflash=nandid(0x%02x,", chip->nandid[0]);
	str_len = strlen(buf);
	sprintf(&buf[str_len], "0x%02x,", chip->nandid[1]);
	str_len = strlen(buf);
	sprintf(&buf[str_len], "0x%02x,", chip->nandid[2]);
	str_len = strlen(buf);
	sprintf(&buf[str_len], "0x%02x,", chip->nandid[3]);
	str_len = strlen(buf);
	sprintf(&buf[str_len], "0x%02x),", chip->nandid[4]);
	str_len = strlen(buf);
	sprintf(&buf[str_len], "pagesize(%d),oobsize(%d),eccsize(%d),eccbit(%d)", nand->writesize, nand->oobsize, chip->ecc.size, chip->eccbitmode);

    printf("pass cmdline: %s\n", buf);
    //lcd_printf(" pass cmdline : %s\n",buf);
    //lcd_display();
    creat_atags(VLX_TAG_ADDR, buf, NULL, 0);
    void (*entry)(void) = (void*) VMJALUNA_ADR;
#ifndef CONFIG_SC8810
    MMU_InvalideICACHEALL();
#endif
#ifdef CONFIG_SC8810
    MMU_DisableIDCM();
#endif

#if BOOT_NATIVE_LINUX
	start_linux();
#else
	entry();
#endif
}
void normal_mode(void)
{
#ifdef CONFIG_SC8810
    //MMU_Init(CONFIG_MMU_TABLE_ADDR);
	vibrator_hw_init();
#endif

	unsigned rst_mode= 0;
   rst_mode = ANA_REG_GET(ANA_HWRST_STATUS);
//	printf("ANA_REG_GET(ANA_HWRST_STATUS) = 0x%16x \n",ANA_REG_GET(ANA_HWRST_STATUS));
	if(!(rst_mode &(BIT_9))){
		normal_shutdown = true;
	}else {
		normal_shutdown = false;
	}

	ANA_REG_SET(ANA_HWRST_STATUS, 0); //clear flag

//	printf("__raw_readl(ANA_HWRST_RTC) = 0x%16x \n",ANA_REG_GET(ANA_HWRST_RTC));
	if(!(normal_shutdown)&&!(ANA_REG_GET(ANA_HWRST_RTC) & (0x0100))){
		normal_shutdown = true;
		printf("cab plug out--\n");
	}

#ifdef CONFIG_ENG
	normal_shutdown = true;
#endif

	if(REG32(0x40007ff0) != LCD_RESUME){
		lcd_resume = false;
	}

	if(normal_shutdown){
		set_vibrator(1);
	}
    
		
#if BOOT_NATIVE_LINUX
    vlx_nand_boot(BOOT_PART, CONFIG_BOOTARGS, BACKLIGHT_ON);
#else
    vlx_nand_boot(BOOT_PART, NULL, BACKLIGHT_ON);
#endif

}
