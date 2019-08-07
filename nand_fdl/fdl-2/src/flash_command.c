#include "sci_types.h"
#include "dload_op.h"
#include "flash_command.h"
#include "fdl_nand.h"
#include "packet.h"
#include "fdl_conf.h"
#include "fdl_crc.h"
#include "fdl_stdio.h"
#include "parsemtdparts.h"
#include "fdl_yaffs2.h"

#include "asm/arch/sci_types.h"
#include "asm/arch/nand_controller.h"
#include <linux/mtd/mtd.h>
#include <nand.h>
#include <linux/mtd/nand.h>
#include <jffs2/jffs2.h>
#include <malloc.h>
#include "yaffs_format_data_translate.h"


extern void cmd_yaffs_mount(char *mp);
extern void cmd_yaffs_umount(char *mp);
extern int cmd_yaffs_ls_chk(const char *dirfilename);
extern void cmd_yaffs_mread_file(char *fn, unsigned char *addr);
extern void cmd_yaffs_mwrite_file(char *fn, char *addr, int size);
extern int nand_check_2bitconfig_4bitecc();
extern int nand_2bitconfig_4bitecc_layout_check(void);
extern void nand_relayout_set(int on);
extern int  yaffs_get_reserved_block_num(void);

#ifdef CONFIG_SP8810W
#define FIXNV_SIZE		(120 * 1024)
#else
#define FIXNV_SIZE		(64 * 1024)
#endif

#define PHASECHECK_SIZE		(3 * 1024)
#define TRANS_CODE_SIZE		(12 * 1024) /* dloadtools optimization value */

#define PAGE_SIZE_2K 2048
#define NAND_OOB_64 64

typedef struct _DL_FILE_STATUS
{
    unsigned long   total_size;
    unsigned long   recv_size;
} DL_FILE_STATUS, *PDL_FILE_STATUS;

typedef struct
{
	unsigned long chunkId;
	unsigned long dataSize;
	unsigned long objectId;
	__u8 p_data[PAGE_SIZE_2K];
	__u8 p_spare[NAND_OOB_64];
} chunk_data;

static unsigned long g_checksum;
static unsigned long g_sram_addr;
static int read_nv_flag = 0;
static int read_bkupnv_flag = 0;
static int read_dlstatus_flag = 0;
__align(4) unsigned char g_fixnv_buf[FIXNV_SIZE + 4];
__align(4) unsigned char g_fixnv_buf_yaffs[FIXNV_SIZE + 4];

#define CHECKSUM_OTHER_DATA       0x5555aaaa
static DL_FILE_STATUS g_status;
static int g_prevstatus;
static __inline void FDL2_SendRep (unsigned long err)
{
    FDL_SendAckPacket (convert_err (err));
}
unsigned long FDL2_GetRecvDataSize (void)
{
    return g_status.recv_size;
}

struct real_mtd_partition phy_partition;
struct real_mtd_partition phy_nv_partition;
static unsigned int is_nbl_write;
static unsigned int is_phasecheck_write;
static unsigned int is_phasecheck_write_secend = 0;
static unsigned int g_NBLFixBufDataSize = 0;
static unsigned char g_FixNBLBuf[0x8000];
static unsigned int g_PhasecheckBUFDataSize = 0;
static unsigned char g_PhasecheckBUF[0x2000];
static unsigned char temBuf[8192 + 1024];
static DL_OP_RECORD_S g_dl_op_table[DL_OP_MTD_COUNT];
static NAND_PAGE_OOB_STATUS nand_page_oob_info = {0};
static unsigned long g_dl_op_index = 0;
static unsigned long is_factorydownload_tools = 0;
static unsigned long is_check_dlstatus = 0; /* 1 : check  0 : don't check */
static DL_Address_CNT_S Dl_Data_Address = {NULL, 0};
static DL_Address_CNT_S Dl_Erase_Address = {NULL, 0};
static unsigned int check_flag = 0;

#ifdef  TRANS_CODE_SIZE
#define min(A,B)		(((A) < (B)) ? (A) : (B))
#define	DATA_BUFFER_SIZE	(TRANS_CODE_SIZE * 2)
static unsigned long  yaffs_buffer_size = 0;
static unsigned long g_BigSize = 0;
static unsigned long g_ReadBufLen = 0;
static unsigned long code_yaffs_buflen	= 0;
static unsigned long code_yaffs_onewrite = 0;
static unsigned char *g_BigBUF = NULL;
#endif
static void fill_partition(struct real_mtd_partition *mtd_part,unsigned int fill_from, unsigned int filler);

bootimg_hdr g_BootimgHDR;
unsigned g_BootimgCurAddr = 0;
unsigned g_BootimgWritesize = 0;
unsigned long totalchunk = 0;
unsigned long chunknumber = 0;
chunk_data chunk, chunk_odd, chunk_even;
static unsigned long outpos = 0;
static unsigned long check_image_type = 0;
int orginal_right, backupfile_right;
unsigned long orginal_index, backupfile_index;
unsigned char *orginal, *backupfile;

typedef struct _CUSTOM2LOG
{
    unsigned long   custom;
    unsigned long   log;
} CUSTOM2LOG;

static CUSTOM2LOG custom2log_table[] = {
	{0x90000001, 0x80000005},
	{0x90000003, 0x80000008},
	{0x90000002, 0x80000011},
	{0xffffffff, 0xffffffff}
};
#define ECC_NBL_SIZE 0x4000
//bootloader header flag offset from the beginning
#define BOOTLOADER_HEADER_OFFSET   32
#define NAND_PAGE_LEN              512
#define NAND_MAGIC_DATA            0xaa55a5a5
//define nand data bus len
#define NAND_BUS_SIZE_8              8
#define NAND_BUS_SIZE_16              16
#define NAND_BUS_SIZE_32              32

void set_dl_op_val(unsigned long addr, unsigned long size, DL_OP_TYPE_E type, DL_OP_STATUS_E status, unsigned long cnt)
{
	if (g_dl_op_index >= DL_OP_MTD_COUNT) {
		printf("\nmtd count is beyoned %d\n", DL_OP_MTD_COUNT);
		return;
	}

	switch (type) {
		case STARTDATA:
			if (status == FAIL) {
				memset(&(g_dl_op_table[g_dl_op_index]), 0, sizeof(DL_OP_RECORD_S));
				g_dl_op_table[g_dl_op_index].base = addr;
    				g_dl_op_table[g_dl_op_index].size = size;
    				g_dl_op_table[g_dl_op_index].type = type;
    				g_dl_op_table[g_dl_op_index].status = status;
    				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
			} else {
    				g_dl_op_table[g_dl_op_index].status = status;
    				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
				g_dl_op_index++;
				memset(&(g_dl_op_table[g_dl_op_index]), 0, sizeof(DL_OP_RECORD_S));
				g_dl_op_table[g_dl_op_index].base = addr;
    				g_dl_op_table[g_dl_op_index].size = size;
			}
		break;
		case MIDSTDATA:
			g_dl_op_table[g_dl_op_index].type = type;
			g_dl_op_table[g_dl_op_index].status = status;
			if (status == FAIL)
    				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
			else
				g_dl_op_table[g_dl_op_index].status_cnt++;
		break;
		case ENDDATA:
			if ((status == FAIL) && (cnt == 1)) {
				g_dl_op_index++;
				memset(&(g_dl_op_table[g_dl_op_index]), 0, sizeof(DL_OP_RECORD_S));
				g_dl_op_table[g_dl_op_index].base = g_dl_op_table[g_dl_op_index - 1].base;
				g_dl_op_table[g_dl_op_index].size = g_dl_op_table[g_dl_op_index - 1].size;
				g_dl_op_table[g_dl_op_index].type = type;
				g_dl_op_table[g_dl_op_index].status = status;
				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
			} else {
				g_dl_op_table[g_dl_op_index].status = status;
				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
				if (status == SUCCESS) {
					g_dl_op_index++;
					memset(&(g_dl_op_table[g_dl_op_index]), 0, sizeof(DL_OP_RECORD_S));
				}
			}
		break;
		case ERASEFLASH:
			if (status == FAIL) {
				memset(&(g_dl_op_table[g_dl_op_index]), 0, sizeof(DL_OP_RECORD_S));
				g_dl_op_table[g_dl_op_index].base = addr;
				g_dl_op_table[g_dl_op_index].size = size;
				g_dl_op_table[g_dl_op_index].type = type;
				g_dl_op_table[g_dl_op_index].status = status;
				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
			} else {
				g_dl_op_table[g_dl_op_index].status = status;
				g_dl_op_table[g_dl_op_index].status_cnt = cnt;
				g_dl_op_index++;
				memset(&(g_dl_op_table[g_dl_op_index]), 0, sizeof(DL_OP_RECORD_S));
			}
		break;
		case READFLASH:
		break;
	}
}

DL_OP_STATUS_E check_dl_data_status(unsigned long addr)
{
	int cnt;
	DL_OP_STATUS_E status;

	status = FAIL;
	for (cnt = 0; cnt < g_dl_op_index; cnt ++)
		if ((g_dl_op_table[cnt].base == addr) && (g_dl_op_table[cnt].type == STARTDATA) && (g_dl_op_table[cnt].status == SUCCESS) && (g_dl_op_table[cnt].status_cnt == 1)) {
			if ((cnt + 2) < g_dl_op_index) {
				if ((g_dl_op_table[cnt + 1].base == addr) && (g_dl_op_table[cnt + 1].type == MIDSTDATA) && (g_dl_op_table[cnt + 1].status == SUCCESS) && (g_dl_op_table[cnt + 1].status_cnt >= 1)) {
					if ((g_dl_op_table[cnt + 2].base == addr) && (g_dl_op_table[cnt + 2].type == ENDDATA) && (g_dl_op_table[cnt + 2].status == SUCCESS) && (g_dl_op_table[cnt + 2].status_cnt == 1)) {
						status = SUCCESS;
						break;
					}
				}
			}
		}

	return status;
}

DL_OP_STATUS_E check_dl_erase_status(unsigned long addr)
{
	int cnt;
	DL_OP_STATUS_E status;

	status = FAIL;
	for (cnt = 0; cnt < g_dl_op_index; cnt ++)
		if ((g_dl_op_table[cnt].base == addr) && (g_dl_op_table[cnt].type == ERASEFLASH) && (g_dl_op_table[cnt].status == SUCCESS) && (g_dl_op_table[cnt].status_cnt == 1)) {
			status = SUCCESS;
			break;
		}

	return status;
}

unsigned long custom2log(unsigned long custom)
{
	unsigned long idx, log = 0xffffffff;

	if ((custom & 0xf0000000) == 0x80000000)
		return custom;
	for (idx = 0; custom2log_table[idx].custom != 0xffffffff; idx ++) {
		if (custom2log_table[idx].custom == custom) {
			log = custom2log_table[idx].log;
			break;
		}
	}

	return log;
}

void phy_partition_info(struct real_mtd_partition phy, int line)
{
	int i;

	if (phy.offset == 0xffffffff) {
		printf("\n\nInvaild partition address\n\n");
		return;
	}

	printf("line : %d, name : %20s, offset : 0x%08x, size : 0x%08x, yaffs : %d\n", line, phy.name, phy.offset, phy.size, phy.yaffs);

	return;
}


#define VLX_RAND_TO_U32( _addr ) \
	if( (_addr) & 0x3 ){_addr += 0x4 -((_addr) & 0x3); }
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
static int check_fixnv_struct(unsigned char *array,unsigned long size){

	int length = 0,keep_length=0, start_addr=(int)array;

	keep_length = *(unsigned int*)(start_addr+size-8);
	if(keep_length != 0xffffffff){
		length = Vlx_CalcFixnvLen(start_addr, start_addr+size);
		if(keep_length != length){
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
		0x5a5a5a5a == *((unsigned int*)&array[size]))||/*old flag*/
		((unsigned short*)crc16(0,array, size)== *((unsigned short*)&array[size])&&
		0x5a5a == *((unsigned short*)&array[size+2]))||/*new flag*/
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
	}

	if(try_backup_file&&ret==1){
		//recovery_sector(sector_path,sector_name,mem_addr,size);
		cmd_yaffs_mwrite_file(sector_name, mem_addr, size);
	}

	//unmout yaffs
	cmd_yaffs_umount(sector_path);

	return ret;
}

static int productinfo_file_found=0;
static int has_done_backup=0;
static int nand_product_read_write(int mode)
{
        char *productinfopoint = "/productinfo";
        char *productinfofilename = "/productinfo/productinfo.bin";
        char *bkproductinfofilename = "/productinfo/productinfobkup.bin";
        int  filesize=0x0;
        int  ret=0x0;

        if(mode==0x0)//read product partition in 2 bit ecc oob layout condition
        {
      	      printf("%s read mode enter\n", __FUNCTION__);
    	      cmd_yaffs_mount(productinfopoint);
	      ret = cmd_yaffs_ls_chk(productinfofilename);
	      if (ret == (PHASECHECK_SIZE + 4))
              {
		    filesize=ret;
                    memset(g_PhasecheckBUF, 0, 0x2000);
                    cmd_yaffs_mread_file(productinfofilename, g_PhasecheckBUF);
		    ret = nv_is_correct_endflag(g_PhasecheckBUF, filesize-4);
                    if(ret<0)
                    {
                        ret=-1;
                        printf("%s: no %s find \n", __FUNCTION__, productinfofilename);
                    }
                    else
                    {
                        productinfo_file_found=1;
                        ret=0;
                        printf("%s: %s file size=0x%x\n", __FUNCTION__, productinfofilename, filesize);
                    }
              }
              else
              {
                     ret=-1;
                     printf("%s: no %s exist\n", __FUNCTION__, productinfofilename);
              }

              if(productinfo_file_found==0)
              {
                    ret = cmd_yaffs_ls_chk(bkproductinfofilename);
	            if (ret == (PHASECHECK_SIZE + 4))
                    {
                        filesize=ret;
                        memset(g_PhasecheckBUF, 0, 0x2000);
                        cmd_yaffs_mread_file(bkproductinfofilename, g_PhasecheckBUF);
		        ret = nv_is_correct_endflag(g_PhasecheckBUF, filesize-4);
                        if(ret<0)
                        {
                            ret = -1;
                            printf("%s: no %s find\n", __FUNCTION__, bkproductinfofilename);
                        }
                        else
                        {
                            ret = 0;
                            productinfo_file_found=1;
                            printf("%s: %s file size=0x%x\n", __FUNCTION__, bkproductinfofilename, filesize);
                        }
                    }
                    else
                    {
                        ret =-1;
                        printf("%s: both %s and %s are not exist%x\n", __FUNCTION__, bkproductinfofilename, productinfofilename);
                    }
                }
	        cmd_yaffs_umount(productinfopoint);
        }
        else//write product partition in 4bit ecc oob layout condition
        {
                 printf("%s write mode  enter\n", __FUNCTION__);
                 memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
                 strcpy(phy_partition.name, "productinfo");
                 log2phy_table(&phy_partition);
                 phy_partition_info(phy_partition, __LINE__);
                 printf("erase productinfo start\n");
                 nand_start_write(&phy_partition, 0, &nand_page_oob_info);

                 if(productinfo_file_found == 0x0)
                 {
                    printf("%s no production file find, no need to write\n", __FUNCTION__);
                    return 0;
                 }

                 cmd_yaffs_mount(productinfopoint);
                 cmd_yaffs_mwrite_file(productinfofilename, g_PhasecheckBUF, (PHASECHECK_SIZE + 4));
                 ret = cmd_yaffs_ls_chk(productinfofilename);
                 if(ret<0)
                      printf("%s: productinfo.bin write file %s err=0x%x\n", __FUNCTION__, productinfofilename, ret);
                 printf("%s: productinfo.bin write file size=0x%x\n", __FUNCTION__, ret);
                 cmd_yaffs_mwrite_file(bkproductinfofilename, g_PhasecheckBUF, (PHASECHECK_SIZE + 4));
                 ret = cmd_yaffs_ls_chk(bkproductinfofilename);
                 if(ret<0)
                      printf("%s: productinfo.bin write file %s err=0x%x\n", __FUNCTION__, bkproductinfofilename, ret);
                 printf("%s: productinfobkup.bin write file size=0x%x\n", __FUNCTION__, ret);
                 cmd_yaffs_umount(productinfopoint);
                 productinfo_file_found=0;
                 ret=0;
        }

        printf("%s: exit ret=0x%x\n",__FUNCTION__, ret);
        return ret;
}


int nand_fixnv_erase(void)
{
        memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
        strcpy(phy_partition.name, "fixnv");
        log2phy_table(&phy_partition);
        phy_partition_info(phy_partition, __LINE__);
        printf("erase fixnv start\n");
        nand_start_write(&phy_partition, 0, &nand_page_oob_info);

        memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
        strcpy(phy_partition.name, "backupfixnv");
        log2phy_table(&phy_partition);
        phy_partition_info(phy_partition, __LINE__);
        printf("erase backupfixnv start\n");
        nand_start_write(&phy_partition, 0, &nand_page_oob_info);
}

int nand_read_fdl_yaffs(struct real_mtd_partition *phypart, unsigned int off, unsigned int size, unsigned char *buf)
{
	int ret = 0;
	int pos;
        int err=0;
	int relayout = 0;
        unsigned long addr = phypart->offset;

	printf("%s: phypart = %s, off = %d,size = %d\n",__FUNCTION__,phypart->name,off,size);

	if (strcmp(phypart->name, "fixnv") == 0) {
		/* for fixnv, read total 64KB */
		char *fixnvpoint = "/fixnv";
		char *fixnvfilename = "/fixnv/fixnv.bin";
		char *backupfixnvpoint = "/backupfixnv";
		char *backupfixnvfilename = "/backupfixnv/fixnv.bin";

                //check 2bit ecc config but flash true 4bit ecc condition
                if(nand_check_2bitconfig_4bitecc())
                {
                    printf("%s: 2bit ecc config, 4bit ecc flash verify enter\n", __FUNCTION__);
                    //check whether current fld2 configuration ecc layout match oob ecc layout in flash
                    ret = nand_2bitconfig_4bitecc_layout_check();
                    if(ret<0)
                    {
                        //if not match, in order to read fixnv partition correctly relayout the oob
                        nand_relayout_set(1);
                        err = nand_flash_init();
		        if ((NAND_SUCCESS != err) && (NAND_INCOMPATIBLE_PART != err)) {
		            printf("%s: 2bit ecc config, 4bit ecc flash reinit error=0x%x\n", __FUNCTION__, err);
                            FDL_SendAckPacket (convert_err (err));
			    return NAND_SYSTEM_ERROR;
		        }
                        //check whether after relocat oob read is ok
                        ret = nand_2bitconfig_4bitecc_layout_check();
                        if(ret <0)
                        {
                            //test read failed so just restore original configuration
                            printf("%s: read after relocation failed\n", __FUNCTION__);
                            nand_relayout_set(0);
                            err = nand_flash_init();
                            if ((NAND_SUCCESS != err) && (NAND_INCOMPATIBLE_PART != err)) {
		                printf("%s: 2bit ecc config, 4bit ecc flash reinit error=0x%x\n", __FUNCTION__, err);
                                FDL_SendAckPacket (convert_err (err));
			        return NAND_SYSTEM_ERROR;
		            }
                        }
                        else
                        {
                            printf("%s: read after relocation success\n!", __FUNCTION__);
                            if(!has_done_backup)
                            {
                                //read out 2bit ecc layout product partition data correctly
                                nand_product_read_write(0);
                                relayout=1;
                            }
                        }
                    }
                    else
                    {
                        printf("%s: correct config, no need retry\n", __FUNCTION__);
                    }
                }

		if ((read_nv_flag == 0) && (read_bkupnv_flag == 0)) {
			read_nv_flag = 1;//wrong
			memset(g_fixnv_buf_yaffs, 0xff, FIXNV_SIZE + 4);
			/* read fixnv */
    			cmd_yaffs_mount(fixnvpoint);
			ret = cmd_yaffs_ls_chk(fixnvfilename);
			if (ret == (FIXNV_SIZE + 4)) {
				cmd_yaffs_mread_file(fixnvfilename, g_fixnv_buf_yaffs);
				if (1 == nv_is_correct_endflag(g_fixnv_buf_yaffs, FIXNV_SIZE)&&
					1 == check_fixnv_struct(g_fixnv_buf_yaffs, FIXNV_SIZE))
					read_nv_flag = 2;//right
			}
			cmd_yaffs_umount(fixnvpoint);

                        if(read_nv_flag!=2)
                        {
			    read_bkupnv_flag = 1;//wrong
			    memset(g_fixnv_buf, 0xff, FIXNV_SIZE + 4);
			    cmd_yaffs_mount(backupfixnvpoint);
			    ret = cmd_yaffs_ls_chk(backupfixnvfilename);
			    if (ret == (FIXNV_SIZE + 4)) {
				cmd_yaffs_mread_file(backupfixnvfilename, g_fixnv_buf);
				if (1 == nv_is_correct_endflag(g_fixnv_buf, FIXNV_SIZE)&&
					1 == check_fixnv_struct(g_fixnv_buf, FIXNV_SIZE))
					read_bkupnv_flag = 2;//right
			    }
			    cmd_yaffs_umount(backupfixnvpoint);
                        }
                        printf("%s: nv_flag=0x%x, back_nv_flag=0x%x\n", __FUNCTION__, read_nv_flag, read_bkupnv_flag);

#if 0
			if ((read_nv_flag == 2) && (read_bkupnv_flag == 1)) {
				printf("fixnv is right, but backupfixnv is wrong, so erase and recovery backupfixnv\n");
				memset(&phy_nv_partition, 0, sizeof(struct real_mtd_partition));
				strcpy(phy_nv_partition.name, "backupfixnv");
				log2phy_table(&phy_nv_partition);
				phy_partition_info(phy_nv_partition, __LINE__);
				printf("erase backupfixnv start\n");
				nand_start_write(&phy_nv_partition, 0, &nand_page_oob_info);
				printf("\nerase backupfixnv end\n");
				printf("write backupfixnv start\n");
				cmd_yaffs_mount(backupfixnvpoint);
    				cmd_yaffs_mwrite_file(backupfixnvfilename, (char *)g_fixnv_buf_yaffs, (FIXNV_SIZE + 4));
				cmd_yaffs_ls_chk(backupfixnvfilename);
				cmd_yaffs_umount(backupfixnvpoint);
				printf("write backupfixnv end\n");
			} else if ((read_nv_flag == 1) && (read_bkupnv_flag == 2)) {
				printf("backupfixnv is right, but fixnv is wrong, so erase and recovery fixnv\n");
				memset(&phy_nv_partition, 0, sizeof(struct real_mtd_partition));
				strcpy(phy_nv_partition.name, "fixnv");
				log2phy_table(&phy_nv_partition);
				phy_partition_info(phy_nv_partition, __LINE__);
				/* erase fixnv partition */
				printf("erase fixnv start\n");
				nand_start_write(&phy_nv_partition, 0, &nand_page_oob_info);
				printf("\nerase fixnv end\n");
				printf("write fixnv start\n");
				cmd_yaffs_mount(fixnvpoint);
    				cmd_yaffs_mwrite_file(fixnvfilename, (char *)g_fixnv_buf, (FIXNV_SIZE + 4));
				cmd_yaffs_ls_chk(fixnvfilename);
				cmd_yaffs_umount(fixnvpoint);
				printf("write fixnv end\n");
				memcpy((unsigned char *)g_fixnv_buf_yaffs, (unsigned char *)g_fixnv_buf, (FIXNV_SIZE + 4));
			} else if ((read_nv_flag == 1) && (read_bkupnv_flag == 1)) {
				printf("\n\nfixnv and backupfixnv are all wrong.\n\n");
				memset(g_fixnv_buf_yaffs, 0xff, FIXNV_SIZE + 4);
			} else if ((read_nv_flag == 2) && (read_bkupnv_flag == 2))
				printf("fixnv and backupfixnv are all right.\n");
                        nv_is_correct(g_fixnv_buf_yaffs, FIXNV_SIZE);
                        memset(g_fixnv_buf, 0xff, FIXNV_SIZE + 4);
#endif
		} //if ((read_nv_flag == 0) && (read_bkupnv_flag == 0))

                if(read_nv_flag == 2)
                {
		    memcpy(buf, (unsigned char *)(g_fixnv_buf_yaffs + off), size);
                    ret = NAND_SUCCESS;
                }
                else if(read_bkupnv_flag == 2)
                {
		    memcpy(buf, (unsigned char *)(g_fixnv_buf + off), size);
                    ret = NAND_SUCCESS;
                }
                else
                {
                    ret = NAND_SYSTEM_ERROR;
                }
	}//if (strcmp(phypart->name, "fixnv") == 0)

	if (strcmp(phypart->name, "productinfo") == 0) {
		/* for dlstatus, read real length */
		char *productinfopoint = "/productinfo";
		//char *productinfofilename = "/productinfo/dlstatus.txt";
		char *productinfofilename = "/productinfo/productinfo.bin";

		if (read_dlstatus_flag == 0) {
			memset(g_PhasecheckBUF, 0, 0x2000);
			/* read dlstatus */
    			cmd_yaffs_mount(productinfopoint);
			ret = cmd_yaffs_ls_chk(productinfofilename);
			if (ret > 0) {
				cmd_yaffs_mread_file(productinfofilename, g_PhasecheckBUF);
				read_dlstatus_flag = 1;//success
			}
			cmd_yaffs_umount(productinfopoint);
		}

		memcpy(buf, (unsigned char *)(g_PhasecheckBUF + off), size);

		if (read_dlstatus_flag == 1)
                {
                    ret = NAND_SUCCESS;
                }
                else
                {
		    ret = NAND_SYSTEM_ERROR;
                }
	}//if (strcmp(phypart->name, "dlstatus") == 0)


        if(relayout == 1)
        {
                printf("%s: relayout recover\n", __FUNCTION__);
                nand_relayout_set(0);
                err = nand_flash_init();
		if ((NAND_SUCCESS != err) && (NAND_INCOMPATIBLE_PART != err)) {
		        printf("%s: 2bit ecc config, 4bit ecc flash reinit err=0x%x\n", __FUNCTION__, err);
                        FDL_SendAckPacket (convert_err (err));
			return NAND_SYSTEM_ERROR;
		}
                if(!has_done_backup)
                {
                    has_done_backup=0x1;
                    //write product info in 4bit ecc oob layout condition
                    nand_product_read_write(1);
                    //erase nand fixnv partition
                    nand_fixnv_erase();
                }
        }
        return ret;
}

int yaffs2_is4kImg(void)
{
	//return (((!strcmp(phy_partition.name, "system")) || (!strcmp(phy_partition.name, "userdata"))) && (nand_page_oob_info.writesize == 4096));
	//return (((!strcmp(phy_partition.name, "system")) || (!strcmp(phy_partition.name, "userdata"))) && (nand_page_oob_info.writesize != 2048));
	return (((!strcmp(phy_partition.name, "system")) || (!strcmp(phy_partition.name, "userdata"))));
}
int yaffs2_convertAndWrite(int last_flag)
{
	yaffs_page src = {0};
	yaffs_page dst = {0};
	int count = 0;
	unsigned int size_threshold = 0;
	int ret = NAND_SUCCESS;
	unsigned int remaindata = 0;

	src.oobsize = NAND_OOB_64;
	src.pagesize = PAGE_SIZE_2K;
	src.p_pagedata = g_BigBUF;
	dst.oobsize = nand_page_oob_info.oobsize;
	dst.pagesize = nand_page_oob_info.writesize;
	dst.p_pagedata = temBuf;
	count = dst.pagesize/src.pagesize;
	size_threshold = (src.oobsize + src.pagesize)*count;
	if(last_flag){
		size_threshold = src.oobsize + src.pagesize;
	}
	while ((g_BigSize - g_ReadBufLen) >= size_threshold){
		//we have received more than one page data.
		count = yaffs_page_translate(&src, &dst);
		if(count > 0){
			//update global buffer
			g_ReadBufLen += count*(src.oobsize + src.pagesize);
			//update src data stream pointer
			src.p_pagedata = (unsigned char*)&src.p_pagedata[count*(src.oobsize + src.pagesize)];
			//restore one page data
		    ret = nand_write_fdl(dst.pagesize+ dst.oobsize, dst.p_pagedata);
			if(ret != NAND_SUCCESS){
				return NAND_SYSTEM_ERROR;
			}
		}
	}

	remaindata = g_BigSize - g_ReadBufLen;
	memcpy(g_BigBUF, g_BigBUF + g_ReadBufLen, remaindata);
	memset(g_BigBUF + remaindata, 0xff, yaffs_buffer_size - remaindata);
	g_BigSize = remaindata;
	g_ReadBufLen = 0;

	return ret;
}

int nand_bbt_init(void)
{
        struct mtd_info *nand;
        int ret=0, i=0x0;
        int max_search_blocks = 0x4;//according to current bbt configuration
	unsigned long addr;
		int startblock;
        struct nand_chip *chip = NULL;

        //only for 4bit ecc but 2bit configuratrion
        if(!nand_check_2bitconfig_4bitecc())
                return 0;

        if(nand_2bitconfig_4bitecc_layout_check()<0)
        {
            if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
	  			return -1;
            nand = &nand_info[nand_curr_device];
            chip = nand->priv;
            addr=nand->size-max_search_blocks*nand->erasesize;
			startblock = (nand->size) / (nand->erasesize) - 1;
           for(i=0x0; i<max_search_blocks; i++)
            {
                printf("%s: erase address=0x%x\n", __FUNCTION__, addr+i*nand->erasesize);
				printf("%s: blocnum=0x%x\n", __FUNCTION__, startblock );
				nand_scan_block(startblock, nand->erasesize, nand->writesize);
				startblock -= 1;
            }
            chip->options = chip->options & ~NAND_BBT_SCANNED;
            printf("%s: ret=0x%x\n", __FUNCTION__, ret);
        }
        return ret;
}

int FDL2_DataStart (PACKET_T *packet, void *arg)
{
    unsigned long *data = (unsigned long *) (packet->packet_body.content);
    unsigned long start_addr = *data;
    unsigned long size = * (data + 1);
    int           ret;
	#if defined(CHIP_ENDIAN_LITTLE)
    start_addr = EndianConv_32 (start_addr);
    size = EndianConv_32 (size);
	#endif

    set_dl_op_val(start_addr, size, STARTDATA, FAIL, 1);

    if (packet->packet_body.size == 12){
		memset(g_fixnv_buf, 0xff, FIXNV_SIZE + 4);
		g_checksum = * (data+2);
		g_sram_addr = (unsigned long) g_fixnv_buf;
    } else {
	    g_checksum = CHECKSUM_OTHER_DATA;
    }

    if (0 == (g_checksum & 0xffffff)){
        //The fixnv checksum is error.
        SEND_ERROR_RSP (BSL_EEROR_CHECKSUM); /*lint !e527*/
    }

    do
    {
		memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
		phy_partition.offset = custom2log(start_addr);
		ret = log2phy_table(&phy_partition);
		phy_partition_info(phy_partition, __LINE__);

		if (NAND_SUCCESS != ret)
        	break;
		if (size >= phy_partition.size) {
			printf("\n\nimage file size : 0x%08x is bigger than partition size : 0x%08x\n", size, phy_partition.size);
			ret = NAND_INVALID_SIZE;
			break;
		}

		//erase flash,canculate write pos,...
		if (strcmp(phy_partition.name, "fixnv") == 0){
			ret = get_nand_pageoob(&nand_page_oob_info);
		}else{
	                ret = nand_start_write(&phy_partition, phy_partition.size, &nand_page_oob_info);
	                if (NAND_SUCCESS != ret){
	                break;
	                }
                }

		is_nbl_write = 0;
		is_phasecheck_write = 0;

		if (strcmp(phy_partition.name, "spl") == 0) {
			is_nbl_write = 1;
			g_NBLFixBufDataSize = 0;
		} else if (strcmp(phy_partition.name, "productinfo") == 0) {
			if (is_phasecheck_write_secend == 0) {
				is_phasecheck_write = 1;
				is_phasecheck_write_secend = 1;
				memset(g_PhasecheckBUF, 0xff, 0x2000);
			} else {
				printf("Error, write phase check repeatedly.");
				ret = NAND_SYSTEM_ERROR;
				break;
			}
		}

		#ifdef TRANS_CODE_SIZE
		yaffs_buffer_size = (DATA_BUFFER_SIZE + (DATA_BUFFER_SIZE / nand_page_oob_info.writesize) * nand_page_oob_info.oobsize);

		if (phy_partition.yaffs == 0) {
			code_yaffs_buflen = DATA_BUFFER_SIZE;
			code_yaffs_onewrite = nand_page_oob_info.writesize;
		} else if (phy_partition.yaffs == 1) {
			code_yaffs_buflen = yaffs_buffer_size;
			code_yaffs_onewrite = nand_page_oob_info.writesize + nand_page_oob_info.oobsize;
		}

		g_BigSize = 0;
		if (g_BigBUF == NULL)
			g_BigBUF = (unsigned char *)malloc(yaffs_buffer_size);

		if (g_BigBUF == NULL) {
			printf("malloc is wrong : %d\n", yaffs_buffer_size);
			ret = NAND_SYSTEM_ERROR;
			break;
		}
		memset(g_BigBUF, 0xff, yaffs_buffer_size);
		//printf("code_yaffs_onewrite = %d  code_yaffs_buflen = %d  yaffs_buffer_size = %d\n", code_yaffs_onewrite, code_yaffs_buflen, yaffs_buffer_size);
		#endif

        g_status.total_size  = size;
        g_status.recv_size   = 0;
        g_prevstatus = NAND_SUCCESS;
		g_ReadBufLen = 0;
		g_BootimgCurAddr = 0;
		g_BootimgWritesize = 0;
		check_image_type = 0;
		check_flag =0;
		set_dl_op_val(start_addr, size, STARTDATA, SUCCESS, 1);
        FDL_SendAckPacket (BSL_REP_ACK);
        return 1;
    }while (0);

    FDL2_SendRep (ret);
    return 0;
}

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

/******************************************************************************
 * change the header of first bootloader page
 ******************************************************************************/
int NandChangeBootloaderHeader(unsigned int *bl_start_addr)
{
    unsigned int       *start_addr = bl_start_addr;
    unsigned short     check_sum = 0;

    unsigned short 	g_PageAttr = 0x1;
    unsigned short      nCycleDev = 0x5;
    unsigned int        nAdvance = 1;


    //set pointer to nand parameter config start address
    start_addr += BOOTLOADER_HEADER_OFFSET / 4;

    //set nand page attribute
    * (start_addr + 1) = g_PageAttr;
    //set nand address cycle
    * (start_addr+2) = nCycleDev;

    //set nand data bus len
    //* (start_addr + 3) = NAND_BUS_SIZE_8;
    * (start_addr + 3) = NAND_BUS_SIZE_16;
    //* (start_addr + 3) = NAND_BUS_SIZE_32;

    if (0) // for 6800h
    {
        //set magic data
        * (start_addr+4) = NAND_MAGIC_DATA;
        //make checksum of first 504 bytes
        check_sum = CheckSum ((unsigned int *) (start_addr + 1), (NAND_PAGE_LEN - BOOTLOADER_HEADER_OFFSET - 4));
    }
    else
    {
        if (nAdvance)
            * (start_addr + 4) = 1;
        else
            * (start_addr + 4) = 0;

        //set magic data
        * (start_addr + 5) = NAND_MAGIC_DATA;

        //make checksum of first 504 bytes
        check_sum = CheckSum((unsigned int *)(start_addr + 1), (NAND_PAGE_LEN - BOOTLOADER_HEADER_OFFSET - 4));
    }

    //set checksum
    * (start_addr) = (unsigned int) check_sum;

    return 1;

}

int NandWriteAndCheck(unsigned int size, unsigned char *buf)
{

    memcpy (g_FixNBLBuf + g_NBLFixBufDataSize, buf, size); /* copy the data to the temp buffer */
    g_NBLFixBufDataSize += size;

    if ((g_NBLFixBufDataSize) <= ECC_NBL_SIZE)
    {
        return NAND_SUCCESS;
    }
#ifndef CONFIG_NAND_SC8810
    NandChangeBootloaderHeader((unsigned int *) g_FixNBLBuf);
#endif
    return NAND_SUCCESS;
}

int FDL2_DataMidst (PACKET_T *packet, void *arg)
{
    	unsigned long size;
	unsigned long ii;

    /* The previous download step failed. */
    if (NAND_SUCCESS != g_prevstatus)
    {
	set_dl_op_val(0, 0, MIDSTDATA, FAIL, 1);
        FDL2_SendRep (g_prevstatus);
        return 0;
    }

    size = packet->packet_body.size;
	//printf("size = %d  recv_size = %d   total_size = %d\n", size, g_status.recv_size, g_status.total_size);
    if ( (g_status.recv_size + size) > g_status.total_size)
    {
        g_prevstatus = NAND_INVALID_SIZE;
	set_dl_op_val(0, 0, MIDSTDATA, FAIL, 2);
        FDL2_SendRep (g_prevstatus);
        return 0;
    }

    if (CHECKSUM_OTHER_DATA == g_checksum)
    {
        if (is_nbl_write == 1) {
		g_prevstatus = NandWriteAndCheck( (unsigned int) size, (unsigned char *) (packet->packet_body.content));
	} else if (is_phasecheck_write == 1) {
		//printf("g_PhasecheckBUFDataSize = %d\n", g_PhasecheckBUFDataSize);
        	memcpy((g_PhasecheckBUF + g_PhasecheckBUFDataSize), (char *)(packet->packet_body.content), size);
        	g_PhasecheckBUFDataSize += size;
		g_prevstatus = NAND_SUCCESS;
	} else {
#ifdef TRANS_CODE_SIZE
		//printf("g_BigSize = %d  buflen = %d, onewrite = %d  size = %d\n", g_BigSize, code_yaffs_buflen, code_yaffs_onewrite, size);
		memcpy((g_BigBUF + g_BigSize), (char *)(packet->packet_body.content), size);
		g_BigSize += size;

		if (g_BigSize < (code_yaffs_buflen / 2)) {
			//printf("continue to big buffer\n");
			g_prevstatus = NAND_SUCCESS;
		} else {
			check_flag = 1;
			if (yaffs2_is4kImg())
				g_prevstatus = yaffs2_convertAndWrite(0);
			else {
				//printf("big buffer is full. g_BigSize = %d\n", g_BigSize);
				for (ii = 0; ii < g_BigSize; ii += code_yaffs_onewrite) {
					//printf(".");
					if (!strcmp(phy_partition.name, "cache") )
						g_prevstatus = NAND_SUCCESS;
					else
		g_prevstatus = nand_write_fdl((unsigned int)code_yaffs_onewrite, (unsigned char *) (g_BigBUF + ii));
					if (NAND_SUCCESS != g_prevstatus) {
						//printf("\n");
						printf("big buffer write error.\n");
                        break;
					}
				}
			//printf("\n");
			g_BigSize = 0;
			memset(g_BigBUF, 0xff, yaffs_buffer_size);
			}
		}
#else
        check_flag = 1;
        g_prevstatus = nand_write_fdl( (unsigned int) size, (unsigned char *) (packet->packet_body.content));
#endif
	}

        if (NAND_SUCCESS == g_prevstatus)
        {
            g_status.recv_size += size;

            if (!packet->ack_flag)
            {
                packet->ack_flag = 1;
		set_dl_op_val(0, 0, MIDSTDATA, SUCCESS, 8);
		FDL_SendAckPacket (BSL_REP_ACK);
                return NAND_SUCCESS == g_prevstatus;
            }
        }

	set_dl_op_val(0, 0, MIDSTDATA, FAIL, 4);
        FDL2_SendRep (g_prevstatus);
        return NAND_SUCCESS == g_prevstatus;
    }
    else //It's fixnv data. We should backup it.
    {
        memcpy ( (unsigned char *) g_sram_addr, (char *) (packet->packet_body.content), size);
        g_sram_addr += size;
	g_status.recv_size += size;
	set_dl_op_val(0, 0, MIDSTDATA, SUCCESS, 8);
        FDL_SendAckPacket (BSL_REP_ACK);
        return 1;
    }
}

int FDL2_DataEnd (PACKET_T *packet, void *arg)
{
		unsigned long pos, size, ret, crc;
    	unsigned long i, fix_nv_size, fix_nv_checksum, ii, realii;
		int imgsize = 0x0;
		int blksize = 0x0;
		int first_nv_ok = 0;
		int change_nv_ok = 0;

		set_dl_op_val(0, 0, ENDDATA, FAIL, 1);
	//step1: check if dwonload size exceed partition size limit.
	if((check_flag==1) && (phy_partition.size < (get_end_write_pos()-phy_partition.offset)))
    {
	    check_flag = 0;
            printf("[%s][%s]: partition:%s write pos exceed,please increase partition page num\n",\
                            __func__, __LINE__, phy_partition.name);
            printf("[%s][%s]: partition size=0x%x, partition offset=0x%x, end_pos=0x%x\n", \
	              __func__, __LINE__, phy_partition.size,  phy_partition.offset, get_end_write_pos());
            return 0;
    }
    	if (CHECKSUM_OTHER_DATA != g_checksum) {
		/* It's fixnv data */
        	fix_nv_size = g_sram_addr - (unsigned long) g_fixnv_buf;
        	fix_nv_checksum = Get_CheckSum ( (unsigned char *) g_fixnv_buf, fix_nv_size);
        	fix_nv_checksum = EndianConv_32 (fix_nv_checksum);
        	if (fix_nv_checksum != g_checksum)
            		SEND_ERROR_RSP(BSL_CHECKSUM_DIFF);

			/* write fixnv to yaffs2 format : orginal */
			char *fixnvpoint = "/fixnv";
			char *fixnvfilename = "/fixnv/fixnv.bin";
			char *fixnvfilename2 = "/fixnv/fixnvchange.bin";
			char *backupfixnvpoint = "/backupfixnv";
			char *backupfixnvfilename = "/backupfixnv/fixnv.bin";

			/* g_fixnv_buf : (FIXNV_SIZE + 4) instead of fix_nv_size */
			//g_fixnv_buf[FIXNV_SIZE + 0] = g_fixnv_buf[FIXNV_SIZE + 1] = 0x5a;
			*((unsigned int*)&g_fixnv_buf[FIXNV_SIZE-8])= fix_nv_size;//keep the real  fixnv file size.
			*((unsigned short*)&g_fixnv_buf[FIXNV_SIZE-2])= crc16(0,g_fixnv_buf, FIXNV_SIZE-2);
			g_fixnv_buf[FIXNV_SIZE + 0] = g_fixnv_buf[FIXNV_SIZE + 1] = 0x5a;
			g_fixnv_buf[FIXNV_SIZE + 2] = g_fixnv_buf[FIXNV_SIZE + 3] = 0x5a;
			/* erase fixnv partition */

			memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
			strcpy(phy_partition.name, "fixnv");
			ret = log2phy_table(&phy_partition);
			phy_partition_info(phy_partition, __LINE__);
			g_prevstatus = ret;
			if (ret == NAND_SUCCESS){
					cmd_yaffs_mount(fixnvpoint);
					cmd_yaffs_mread_file(fixnvfilename, g_fixnv_buf_yaffs);
					first_nv_ok = nv_is_correct_endflag(g_fixnv_buf_yaffs, FIXNV_SIZE);

				if(first_nv_ok == 0){
					printf("/fixnv/fixnv.bin is error !\r\n");
					cmd_yaffs_mread_file(fixnvfilename2, g_fixnv_buf_yaffs);
					change_nv_ok = nv_is_correct_endflag(g_fixnv_buf_yaffs, FIXNV_SIZE);

				}
					cmd_yaffs_umount(fixnvpoint);

			}

			if(first_nv_ok == 1 || change_nv_ok == 1){
				//write backup fixnv first
				/* erase backup fixnv partition */
				memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
				strcpy(phy_partition.name, "backupfixnv");
				ret = log2phy_table(&phy_partition);
				phy_partition_info(phy_partition, __LINE__);
				g_prevstatus = ret;
				if (ret == NAND_SUCCESS) {

						{//download size check
							imgsize = fix_nv_size;
							blksize = nand_page_oob_info.erasesize;

							if(((yaffs_get_reserved_block_num()+2)*blksize+imgsize) > phy_partition.size)
							{
						     		printf("[%s][%s]: partition:%s write pos exceed,\
										please increase partition size\n",\
					    						__func__, __LINE__, phy_partition.name);
							}
						}

						if(1 == check_fixnv_struct((char *)g_fixnv_buf, FIXNV_SIZE)){
							//printf("check_backupfixnv_struct success !\r\n");
							printf("erase backupfixnv start\n");
							ret = nand_start_write (&phy_partition, fix_nv_size, &nand_page_oob_info);
							printf("\nerase backupfixnv end\n");
							g_prevstatus = ret;
							if (ret == NAND_SUCCESS){
									printf("write backupfixnv start\n");
									cmd_yaffs_mount(backupfixnvpoint);
									cmd_yaffs_mwrite_file(backupfixnvfilename, (char *)g_fixnv_buf, (FIXNV_SIZE + 4));
									ret = cmd_yaffs_ls_chk(backupfixnvfilename);
									cmd_yaffs_umount(backupfixnvpoint);
									printf("write backupfixnv end\n");
									g_prevstatus = NAND_SUCCESS;
							}else{
								set_dl_op_val(0, 0, ENDDATA, FAIL, 2);
								FDL2_SendRep (g_prevstatus);
								return 0;
							}

						}else{
							g_prevstatus = NAND_INVALID_SIZE;
							cmd_yaffs_umount(backupfixnvpoint);
							set_dl_op_val(0, 0, ENDDATA, FAIL, 2);
	        					FDL2_SendRep (g_prevstatus);
	        					return 0;
						}
				}
				//write change fixnv first
				/* erase change fixnv partition */
				memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
				strcpy(phy_partition.name, "fixnv");
				ret = log2phy_table(&phy_partition);
				phy_partition_info(phy_partition, __LINE__);
				g_prevstatus = ret;
				if (ret == NAND_SUCCESS) {

						{//download size check
							imgsize = fix_nv_size;
							blksize = nand_page_oob_info.erasesize;

							if(((yaffs_get_reserved_block_num()+2)*blksize+imgsize) > phy_partition.size)
							{
						     		printf("[%s][%s]: partition:%s write pos exceed,\
										please increase partition size\n",\
					    						__func__, __LINE__, phy_partition.name);
							}
						}

						if(1 == check_fixnv_struct((char *)g_fixnv_buf, FIXNV_SIZE)){
							//printf("check_fixnv_struct success !\r\n");
							printf("erase fixnv start\n");
							ret = nand_start_write (&phy_partition, fix_nv_size, &nand_page_oob_info);
							printf("\nerase fixnv end\n");
							g_prevstatus = ret;

							if(ret == NAND_SUCCESS){
								printf("write change fixnv start\n");
								cmd_yaffs_mount(fixnvpoint);
								cmd_yaffs_mwrite_file(fixnvfilename2, (char *)g_fixnv_buf, (FIXNV_SIZE + 4));
								ret = cmd_yaffs_ls_chk(fixnvfilename2);

								cmd_yaffs_mwrite_file(fixnvfilename, (char *)g_fixnv_buf, (FIXNV_SIZE + 4));
								ret = cmd_yaffs_ls_chk(fixnvfilename);

								cmd_yaffs_umount(fixnvpoint);
								printf("write changefixnv end\n");
								g_prevstatus = NAND_SUCCESS;
							}else{
								set_dl_op_val(0, 0, ENDDATA, FAIL, 2);
								FDL2_SendRep (g_prevstatus);
								return 0;
							}
						}else{
							g_prevstatus = NAND_INVALID_SIZE;
							cmd_yaffs_umount(fixnvpoint);
							set_dl_op_val(0, 0, ENDDATA, FAIL, 2);
	        					FDL2_SendRep (g_prevstatus);
	        					return 0;
						}
				}

			}else{
				//write fixnv first
				if (ret == NAND_SUCCESS) {
						{//download size check
							imgsize = fix_nv_size;
							blksize = nand_page_oob_info.erasesize;

							if(((yaffs_get_reserved_block_num()+2)*blksize+imgsize) > phy_partition.size)
							{
						     		printf("[%s][%s]: partition:%s write pos exceed,\
										please increase partition size\n",\
					    						__func__, __LINE__, phy_partition.name);
							}
						}

						if(1 == check_fixnv_struct((char *)g_fixnv_buf, FIXNV_SIZE)){
							//printf("check_fixnv_struct success !\r\n");

							printf("erase fixnv start ; line=%d\n",__LINE__);
							ret = nand_start_write (&phy_partition, fix_nv_size, &nand_page_oob_info);
							printf("\nerase fixnv end ; line=%d\n",__LINE__);
							g_prevstatus = ret;

							if(ret == NAND_SUCCESS){
								printf("write fixnv start\n");
								cmd_yaffs_mount(fixnvpoint);

								cmd_yaffs_mwrite_file(fixnvfilename, (char *)g_fixnv_buf, (FIXNV_SIZE + 4));
								ret = cmd_yaffs_ls_chk(fixnvfilename);

								cmd_yaffs_mwrite_file(fixnvfilename2, (char *)g_fixnv_buf, (FIXNV_SIZE + 4));
								ret = cmd_yaffs_ls_chk(fixnvfilename2);

								cmd_yaffs_umount(fixnvpoint);
								printf("write fixnv end\n");
								g_prevstatus = NAND_SUCCESS;
							}else{
								set_dl_op_val(0, 0, ENDDATA, FAIL, 2);
								FDL2_SendRep (g_prevstatus);
								return 0;
							}
						}else{
							//fixnv structure check fail.
						g_prevstatus = NAND_INVALID_SIZE;
						cmd_yaffs_umount(fixnvpoint);
						set_dl_op_val(0, 0, ENDDATA, FAIL, 2);
        					FDL2_SendRep (g_prevstatus);
        					return 0;
						}
				}

				/* erase backup fixnv partition */
				memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
				strcpy(phy_partition.name, "backupfixnv");
				ret = log2phy_table(&phy_partition);
				phy_partition_info(phy_partition, __LINE__);
				g_prevstatus = ret;
				if (ret == NAND_SUCCESS) {
						{//download size check
							imgsize = fix_nv_size;
							blksize = nand_page_oob_info.erasesize;

							if(((yaffs_get_reserved_block_num()+2)*blksize+imgsize) > phy_partition.size)
							{
						     		printf("[%s][%s]: partition:%s write pos exceed,\
										please increase partition size\n",\
					    						__func__, __LINE__, phy_partition.name);
							}
						}
						if(1 == check_fixnv_struct((char *)g_fixnv_buf, FIXNV_SIZE)){
							//printf("check_backupfixnv_struct success !\r\n");
							printf("erase backupfixnv start\n");
							ret = nand_start_write (&phy_partition, fix_nv_size, &nand_page_oob_info);
							printf("\nerase backupfixnv end\n");
							g_prevstatus = ret;

							if (ret == NAND_SUCCESS) {
								printf("write backupfixnv start\n");
								cmd_yaffs_mount(backupfixnvpoint);

								cmd_yaffs_mwrite_file(backupfixnvfilename, (char *)g_fixnv_buf, (FIXNV_SIZE + 4));
								ret = cmd_yaffs_ls_chk(backupfixnvfilename);
								cmd_yaffs_umount(backupfixnvpoint);
								printf("write backupfixnv end\n");
								g_prevstatus = NAND_SUCCESS;
							}else{
								set_dl_op_val(0, 0, ENDDATA, FAIL, 2);
								FDL2_SendRep (g_prevstatus);
								return 0;
							}
						}else{
							g_prevstatus = NAND_INVALID_SIZE ;
							cmd_yaffs_umount(backupfixnvpoint);
							set_dl_op_val(0, 0, ENDDATA, FAIL, 2);
	        				FDL2_SendRep (g_prevstatus);
	        				return 0;
						}
				}

			}

    	} else if (is_nbl_write == 1) {
#ifdef CONFIG_NAND_SC8810	//only for sc8810 to write spl
		g_prevstatus = nand_write_fdl(0x0, g_FixNBLBuf);
#else
	   	/* write the spl loader image to the nand*/
		for (i = 0; i < 3; i++) {
			pos = 0;
			while (pos < g_NBLFixBufDataSize) {
				if ((g_NBLFixBufDataSize - pos) >= 2048)
					size = 2048;
				else
					size = g_NBLFixBufDataSize - pos;
				//printf("pos = %d  size = %d\n", pos, size);
				if (size == 0)
					break;
				g_prevstatus = nand_write_fdl (size, g_FixNBLBuf + pos);
				pos += size;
			}
        	}//for (i = 0; i < 3; i++)
#endif
		is_nbl_write = 0;
   	} else if (is_phasecheck_write == 1) {
		/* write phasecheck to yaffs2 format */
		char *productinfopoint = "/productinfo";
		char *productinfofilename = "/productinfo/productinfo.bin";
		char *bkproductinfofilename = "/productinfo/productinfobkup.bin";


		*(unsigned short*)&g_PhasecheckBUF[PHASECHECK_SIZE-2] = crc16(0,g_PhasecheckBUF, PHASECHECK_SIZE-2);
		*(unsigned int*)&g_PhasecheckBUF[PHASECHECK_SIZE] = 0x5a5a5a5a;

		cmd_yaffs_mount(productinfopoint);
    	cmd_yaffs_mwrite_file(productinfofilename, g_PhasecheckBUF, (PHASECHECK_SIZE + 4));
		ret = cmd_yaffs_ls_chk(productinfofilename);
		cmd_yaffs_mwrite_file(bkproductinfofilename, g_PhasecheckBUF, (PHASECHECK_SIZE + 4));
		ret = cmd_yaffs_ls_chk(bkproductinfofilename);
        cmd_yaffs_umount(productinfopoint);


		g_prevstatus = NAND_SUCCESS;
		/* factorydownload tools */
		is_factorydownload_tools = 1;
		is_check_dlstatus = get_DL_Status();
		if (is_check_dlstatus == 1) {
			get_Dl_Erase_Address_Table(&Dl_Erase_Address);
			get_Dl_Data_Address_Table(&Dl_Data_Address);
		}
    }
#ifdef	TRANS_CODE_SIZE
	else {
		//printf("data end, g_BigSize = %d\n", g_BigSize);
		if (yaffs2_is4kImg())
			g_prevstatus = yaffs2_convertAndWrite(1);
		else {
			ii = 0;
			while (ii < g_BigSize) {
				realii = min(g_BigSize - ii, code_yaffs_onewrite);
				//printf(".");
				if (!strcmp(phy_partition.name, "cache") )
					g_prevstatus = NAND_SUCCESS;
				else
				g_prevstatus = nand_write_fdl((unsigned int)realii, (unsigned char *)(g_BigBUF + ii));
				if (NAND_SUCCESS != g_prevstatus) {
					//printf("\n");
					printf("1big buffer write error.\n");
                    break;
				}
				ii += realii;
			}
			//printf("\n");
		}
		g_BigSize = 0;
	}
#endif

    	if (NAND_SUCCESS != g_prevstatus) {
		set_dl_op_val(0, 0, ENDDATA, FAIL, 2);
        	FDL2_SendRep (g_prevstatus);
        	return 0;
    	}

	//step2: check if last file dwonload size exceed partition size limit.
	if((check_flag==1) && (phy_partition.size <= (get_end_write_pos()-phy_partition.offset)))
    {
	    check_flag = 0;
            printf("[%s][%s]: partition:%s write pos exceed,please increase partition page num\n",\
                            __func__, __LINE__, phy_partition.name);
            printf("[%s][%s]: partition size=0x%x, partition offset=0x%x, end_pos=0x%x\n", \
	              __func__, __LINE__, phy_partition.size,  phy_partition.offset, get_end_write_pos());
            return 0;
    }
        fill_partition(&phy_partition, (get_end_write_pos()-phy_partition.offset), 0xffffffff);
        
    	g_prevstatus = nand_end_write();
	set_dl_op_val(0, 0, ENDDATA, SUCCESS, 1);
    	FDL2_SendRep (g_prevstatus);
    	return (NAND_SUCCESS == g_prevstatus);
}

int FDL2_ReadFlash (PACKET_T *packet, void *arg)
{
    	unsigned long *data = (unsigned long *) (packet->packet_body.content);
    	unsigned long addr = *data;
    	unsigned long size = * (data + 1);
    	unsigned long off = 0;
    	int           ret;

#if defined(CHIP_ENDIAN_LITTLE)
    	addr = EndianConv_32 (addr);
    	size = EndianConv_32 (size);
#endif
	memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
	phy_partition.offset = custom2log(addr);
	ret = log2phy_table(&phy_partition);
	//phy_partition_info(phy_partition, __LINE__);

	if (size > MAX_PKT_SIZE) {
    	FDL_SendAckPacket (BSL_REP_DOWN_SIZE_ERROR);
    	return 0;
    }

    if (packet->packet_body.size > 8)
        off = EndianConv_32 (* (data + 2));
	//printf("addr = 0x%08x  size = 0x%08x  off = 0x%08x  name = %s\n", addr, size, off, phy_partition.name);

	if ((strcmp(phy_partition.name, "fixnv") == 0) || (strcmp(phy_partition.name, "productinfo") == 0))
		ret = nand_read_fdl_yaffs(&phy_partition, off, size, (unsigned char *)(packet->packet_body.content));
    else if ((strcmp(phy_partition.name, "spl") == 0) || (strcmp(phy_partition.name, "2ndbl") == 0))
    	ret = nand_read_fdl(&phy_partition, off, size, (unsigned char *)(packet->packet_body.content));
	else
		ret = NAND_INVALID_ADDR;

    	if (NAND_SUCCESS == ret) {
        	packet->packet_body.type = BSL_REP_READ_FLASH;
        	packet->packet_body.size = size;
        	FDL_SendPacket (packet);
        	return 1;
    	} else {
        	FDL2_SendRep (ret);
        	return 0;
    }
}

static void fill_partition(struct real_mtd_partition *mtd_part,unsigned int fill_from, unsigned int filler){
	unsigned long  offset,size,start,end;
        unsigned int   write_size=nand_page_oob_info.writesize;
        unsigned int   erase_size=nand_page_oob_info.erasesize;
        unsigned int   oob_size=nand_page_oob_info.oobsize;
        int            ret;
        struct mtd_info *nand;
        
        if ((nand_curr_device < 0) || (nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE))
		return NAND_SYSTEM_ERROR;
	nand = &nand_info[nand_curr_device];

	printf("\n\nfill_partition: yaffs flag=0x%x, fill_from=0x%x, write_size=0x%x, erase_size=0x%x, oob_size=0x%x\n", \
                    mtd_part->yaffs, fill_from, write_size, erase_size, oob_size);
        if(mtd_part->yaffs == 1)
	    return;//if the data format is not raw,then just return
        
        if ((strcmp(mtd_part->name, "vmjaluna")==0x0) ||
                    (strcmp(mtd_part->name, "modem")==0x0)||
                        (strcmp(mtd_part->name, "dsp")==0x0)||
                            (strcmp(mtd_part->name, "2ndbl")==0x0)||
                                 (strcmp(mtd_part->name, "boot_logo")==0x0)||
                                     (strcmp(mtd_part->name, "fastboot_logo")==0x0))
        {
	    offset = mtd_part->offset;//the begin of the partition
	    size = mtd_part->size;//the total size of the partition
	    start = (offset + fill_from + write_size - 1)&(~(write_size - 1));//fill partition begin
	    end = (offset + size + write_size - 1)&(~(write_size - 1));//fill partition end
	    memset(temBuf,filler,write_size);//generate a filler
	    printf("fill_partition: offset=0x%x, size=0x%x, start=0x%x, end=0x%x\n", offset, size, start, end);
            {
		struct nand_chip *chip = nand->priv;
		chip->ops.mode = MTD_OOB_AUTO;
		chip->ops.len = write_size;
		chip->ops.datbuf = (uint8_t *)temBuf;
		chip->ops.oobbuf = NULL;
		chip->ops.ooblen = oob_size;
		chip->ops.ooboffs = 0;
	        while(start < end)
                {
                    while(!(start & (erase_size-1)))
                    {
                        if(nand_block_isbad(nand, start&(~(erase_size-1))))
                        {
                            start = (start + erase_size) & (~(erase_size-1));
                            if(start >= end)
                            { 
	                        printf("fill_partition: exit start=0x%x, end=0x%x\n", start, end);
                                return;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    ret = nand_do_write_ops(nand, (unsigned long long)start, &(chip->ops));
                    if(ret < 0)
                    {
                        printf("fill_partition: write error\n");
                    }
                    start += write_size;
                }
            }
        }
}
int FDL2_EraseFlash (PACKET_T *packet, void *arg)
{
    unsigned long *data = (unsigned long *) (packet->packet_body.content);
    unsigned long addr = *data;
    unsigned long size = * (data + 1);
    int           ret;
    int           cnt;
    int           dl_op_buf_len = 0;
    int 	  dl_item_cnt;
    DL_OP_STATUS_E	dl_data_status, dl_erase_status;

    addr = EndianConv_32 (addr);
    size = EndianConv_32 (size);

	set_dl_op_val(addr, size, ERASEFLASH, FAIL, 1);
	if ((addr == 0) && (size = 0xffffffff)) {
		printf("Scrub to erase all of flash\n");
		//nand_erase_allflash();
		nand_erase_partition(0,0xffffffff,0);
		ret = NAND_SUCCESS;
		set_dl_op_val(addr, size, ERASEFLASH, SUCCESS, 1);
	} else {
		memset(&phy_partition, 0, sizeof(struct real_mtd_partition));
		phy_partition.offset = custom2log(addr);
		ret = log2phy_table(&phy_partition);
		phy_partition_info(phy_partition, __LINE__);
		if (NAND_SUCCESS == ret){
			ret = nand_erase_partition(phy_partition.offset, phy_partition.size,1);
		}

		if (NAND_SUCCESS == ret)
			set_dl_op_val(addr, size, ERASEFLASH, SUCCESS, 1);
	}

	/*printf("Dl_Erase_Address.cnt = 0x%08x  Dl_Data_Address.cnt = 0x%08x\n", Dl_Erase_Address.cnt, Dl_Data_Address.cnt);
	for (cnt = 0; cnt < Dl_Erase_Address.cnt; cnt ++)
		printf("Dl_Erase_Address_Table[%d] = 0x%08x\n", cnt, Dl_Erase_Address.base[cnt]);
	for (cnt = 0; cnt < Dl_Data_Address.cnt; cnt ++)
		printf("Dl_Data_Address_Table[%d] = 0x%08x\n", cnt, Dl_Data_Address.base[cnt]);*/

	if ((is_factorydownload_tools == 1) && (is_check_dlstatus == 1) && (Dl_Erase_Address.cnt > 0)) {
		if (addr == Dl_Erase_Address.base[Dl_Erase_Address.cnt - 1]) {
			printf("\nSave dload status into dlstatus.txt\n");
			dl_op_buf_len = DL_OP_RECORD_LEN * (g_dl_op_index + 1);
			if (dl_op_buf_len > 0x2000) {
				printf("dload status is too long and does not save it.\n");
			} else {
				memset(g_PhasecheckBUF, 0, 0x2000);
				for (cnt = 0; cnt <= g_dl_op_index; cnt++)
					sprintf((g_PhasecheckBUF + cnt * DL_OP_RECORD_LEN),
					"{%02d Base:0x%08x Size:0x%08x Op:%s Status:%s Scnt:0x%08x}",
					cnt,
					g_dl_op_table[cnt].base,
					g_dl_op_table[cnt].size,
					Dl_Op_Type_Name[g_dl_op_table[cnt].type],
					Dl_Op_Status_Name[g_dl_op_table[cnt].status],
					g_dl_op_table[cnt].status_cnt);
				/* printf("%s\n", g_PhasecheckBUF); the line will result in dead here, so mask it */
				/* write dload_status to yaffs2 format */
				char *productinfopoint = "/productinfo";
				char *productinfofilename = "/productinfo/dlstatus.txt";
				cmd_yaffs_mount(productinfopoint);
 				cmd_yaffs_mwrite_file(productinfofilename, g_PhasecheckBUF, dl_op_buf_len);
				cmd_yaffs_ls_chk(productinfofilename);
				cmd_yaffs_umount(productinfopoint);
			}

			/* check factorydownload status */
			printf("\nCheck dload status\n");
			for (cnt = 0; cnt <= g_dl_op_index; cnt++)
				printf("%02d Base:0x%08x Size:0x%08x Op:%d Status:%d Scnt:0x%08x\n",
				cnt,
				g_dl_op_table[cnt].base,
				g_dl_op_table[cnt].size,
				g_dl_op_table[cnt].type,
				g_dl_op_table[cnt].status,
				g_dl_op_table[cnt].status_cnt);

			dl_data_status = FAIL;
			dl_erase_status = FAIL;
			for (cnt = 0; cnt < Dl_Data_Address.cnt; cnt++) {
				dl_data_status = check_dl_data_status(Dl_Data_Address.base[cnt]);
				if (dl_data_status == FAIL) {
					printf("check address:0x%08x download status error\n", Dl_Data_Address.base[cnt]);
					break;
				}
			}

			for (cnt = 0; cnt < Dl_Erase_Address.cnt; cnt++) {
				dl_erase_status = check_dl_erase_status(Dl_Erase_Address.base[cnt]);
				if (dl_erase_status == FAIL) {
			   		printf("check address:0x%08x erase status error\n", Dl_Erase_Address.base[cnt]);
					break;
				}
			}

			if ((dl_data_status == SUCCESS) && (dl_erase_status == SUCCESS))
				ret = NAND_SUCCESS;
			else
				ret = NAND_SYSTEM_ERROR;
		} //if (addr == Dl_Erase_Address.base[Dl_Erase_Address.cnt - 1])
	} //if ((is_factorydownload_tools == 1) && (Dl_Erase_Address.cnt > 0))

    FDL2_SendRep (ret);
    return (NAND_SUCCESS == ret);
}

int FDL2_FormatFlash (PACKET_T *pakcet, void *arg)
{
    int ret = nand_format();
    FDL2_SendRep (ret);
    return (NAND_SUCCESS == ret);
}
