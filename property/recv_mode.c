#include <config.h>
#include <common.h>
#include <linux/types.h>
#include <asm/arch/bits.h>
#include <image.h>
#include <linux/string.h>
#include <android_bootimg.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <nand.h>
#include <android_boot.h>
#include <environment.h>
#include <jffs2/jffs2.h>
#include <boot_mode.h>
#include <fat.h>
#include <asm/byteorder.h>
#include <part.h>
#include <mmc.h>

#ifdef dprintf
#undef dprintf
#endif
#define dprintf(fmt, args...) printf(fmt, ##args)


extern unsigned char raw_header[];

#include <android_recovery.h>

static const int MISC_PAGES = 3;			// number of pages to save
static const int MISC_COMMAND_PAGE = 1;		// bootloader command is this page
static char buf[8192];
unsigned boot_into_recovery = 0;


int get_recovery_message(struct recovery_message *out)
{
	loff_t offset = 0;
	unsigned pagesize;
	size_t size;

	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;

	ret = mtdparts_init();
	if(ret != 0){
		dprintf("mtdparts init error %d\n", ret);
		return -1;
	}

	ret = find_dev_and_part("misc", &dev, &pnum, &part);
	if(ret){
		dprintf("No partiton named %s found\n", "misc");
		return -1;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
        printf("Partition %s not a NAND device\n", "misc");
        return -1;
    } 

	nand = &nand_info[dev->id->num];	
	pagesize = nand->writesize;
	
	offset = pagesize * MISC_COMMAND_PAGE + part->offset; 
	size = pagesize;
	ret = nand_read_skip_bad(nand, offset, &size, (void *)buf);
	if(ret != 0){ 
		printf("function: %s nand read error %d\n", __FUNCTION__, ret);
		return -1;
	}

	memcpy(out, buf, sizeof(*out));
	return 0;
}

int set_recovery_message(const struct recovery_message *in)
{
	loff_t offset = 0;
	unsigned pagesize;
	size_t size;
	
	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;
	
	ret = mtdparts_init();
	if(ret != 0){
		dprintf("mtdparts init error %d\n", ret);
		return -1;
	}
	
	ret = find_dev_and_part("misc", &dev, &pnum, &part);
	if(ret){
		dprintf("No partiton named %s found\n", "misc");
		return -1;
	}else if(dev->id->type != MTD_DEV_TYPE_NAND){
		dprintf("Partition %s not a NAND device\n", "misc");
		return -1;
	} 
	
	offset = part->offset; 
	nand = &nand_info[dev->id->num];
	pagesize = nand->writesize;
	
	size = pagesize*(MISC_COMMAND_PAGE + 1);

	ret = nand_read_skip_bad(nand, offset, &size, (void *)SCRATCH_ADDR);
	if(ret != 0){
		dprintf("%s: nand read error %d\n", __FUNCTION__, ret);
		return -1;
	}

	

	offset += (pagesize * MISC_COMMAND_PAGE);
	offset += SCRATCH_ADDR;
	memcpy(offset, in, sizeof(*in));

	nand_erase_options_t opts;
	memset(&opts, 0, sizeof(opts));
	opts.offset = part->offset;
	opts.length = pagesize *(MISC_COMMAND_PAGE + 1);
	opts.jffs2 = 0;
	opts.scrub = 0;
	opts.quiet = 1;
	ret = nand_erase_opts(nand, &opts);
	if(ret != 0){
		dprintf("%s, nand erase error %d\n", __FUNCTION__, ret);
		return -1;
	}
	ret = nand_write_skip_bad(nand, part->offset, &size, (void *)SCRATCH_ADDR);
	if(ret != 0){
		dprintf("%s, nand erase error %d\n", __FUNCTION__, ret);
		return -1;
	}
	
}

int read_update_header_for_bootloader(struct update_header *header)
{
	return 0;
}

int update_firmware_image (struct update_header *header, char *name)
{

	return 0;
}

/* Bootloader / Recovery Flow
 *
 * On every boot, the bootloader will read the recovery_message
 * from flash and check the command field.  The bootloader should
 * deal with the command field not having a 0 terminator correctly
 * (so as to not crash if the block is invalid or corrupt).
 *
 * The bootloader will have to publish the partition that contains
 * the recovery_message to the linux kernel so it can update it.
 *
 * if command == "boot-recovery" -> boot recovery.img
 * else if command == "update-radio" -> update radio image (below)
 * else -> boot boot.img (normal boot)
 *
 * Radio Update Flow
 * 1. the bootloader will attempt to load and validate the header
 * 2. if the header is invalid, status="invalid-update", goto #8
 * 3. display the busy image on-screen
 * 4. if the update image is invalid, status="invalid-radio-image", goto #8
 * 5. attempt to update the firmware (depending on the command)
 * 6. if successful, status="okay", goto #8
 * 7. if failed, and the old image can still boot, status="failed-update"
 * 8. write the recovery_message, leaving the recovery field
 *    unchanged, updating status, and setting command to
 *    "boot-recovery"
 * 9. reboot
 *
 * The bootloader will not modify or erase the cache partition.
 * It is recovery's responsibility to clean up the mess afterwards.
 */

int recovery_init (void)
{
	struct recovery_message msg;
	struct update_header header;
	char partition_name[32];
	unsigned valid_command = 0;

	// get recovery message
	if(get_recovery_message(&msg))
		return -1;
	if (msg.command[0] != 0 && msg.command[0] != 255) {
		dprintf("Recovery command: %.*s\n", sizeof(msg.command), msg.command);
	}
	msg.command[sizeof(msg.command)-1] = '\0'; //Ensure termination

	if (!strcmp("boot-recovery",msg.command)) {
		valid_command = 1;
		strcpy(msg.command, "");	// to safe against multiple reboot into recovery
		strcpy(msg.status, "OKAY");
		set_recovery_message(&msg);	// send recovery message
		// Boot in recovery mode
		return 1;
	}

#if 0 //in the latest 'recovery' we have done most of the update works,so,we needn't to do it anymore!!!
	if (!strcmp("boot-update",msg.command)) {
		valid_command = 1;
		strcpy(msg.command, "");	// to safe against multiple reboot into recovery
		strcpy(msg.status, "OKAY");
		set_recovery_message(&msg);	// send recovery message
		// Boot in update mode
		return 2;
	}
#endif
	if (!strcmp("update-radio",msg.command)) {
		valid_command = 1;
		strcpy(partition_name, "FOTA");
	}

	//Todo: Add support for bootloader update too.

	if(!valid_command) {
		//We need not to do anything
		return 0; // Boot in normal mode
	}

	if (read_update_header_for_bootloader(&header)) {
		strcpy(msg.status, "invalid-update");
		goto SEND_RECOVERY_MSG;
	}

	if (update_firmware_image (&header, partition_name)) {
		strcpy(msg.status, "failed-update");
		goto SEND_RECOVERY_MSG;
	}
	strcpy(msg.status, "OKAY");

SEND_RECOVERY_MSG:
	strcpy(msg.command, "boot-recovery");
	set_recovery_message(&msg);	// send recovery message
	reboot_devices(0);
	return 0;
}
#ifdef CONFIG_GENERIC_MMC
#define FIX_SIZE (64*1024)
void nv_patch(char * addr, int size)
{
	int i = 0;
	for(i=0; i<FIX_SIZE-size; i++){
		addr[size + i] = 0xff;
	}
	for(i=0; i<4; i++){
		addr[FIX_SIZE + i] = 0x5a;
	}
	//generate crc16 flag and restore it.
	*(unsigned short*)&addr[FIX_SIZE-2] = crc16(0,(unsigned const char*)addr,FIX_SIZE-2);	
	*((unsigned int*)&addr[FIX_SIZE-8])= size;//keep the real  fixnv file size.
	return;
}

#define BUF_ADDR 0x1000000
#define SD_NV_NAME "nvitem.bin"
#define NAND_NV_NAME " "
#define MODEM_PART "modem"
#define SD_MODEM_NAME "modem.bin"
#define DSP_PART "dsp"
#define SD_DSP_NAME "dsp.bin"
#define VM_PART "vmjaluna"
#define SD_VM_NAME "vmjaluna.bin"

void try_update_modem(void)
{
	struct mmc *mmc;
	block_dev_desc_t *dev_desc=NULL;
	loff_t off, size;
	nand_info_t *nand;
	struct mtd_device *dev;
	u8 pnum;
	struct part_info *part;
	int ret;
	char *fixnvpoint = "/fixnv";
	//char *fixnvfilename = "/fixnv/fixnv.bin";
	char *fixnvfilename = "/fixnv/fixnvchange.bin";
	nand_erase_options_t opts;

	mmc = find_mmc_device(0);
	if(mmc){
		ret = mmc_init(mmc);
		if(ret < 0){
			printf("mmc init failed %d\n", ret);
			return;
		}
	}else{
		printf("no mmc card found\n");
		return;
	}
	//add by caofuang.
	ret = mtdparts_init();
	if(ret != 0){
		dprintf("mtdparts init error %d\n", ret);
		return;
	}
	//add end.

	dev_desc = &mmc->block_dev;
	if(dev_desc==NULL){
		printf("no mmc block device found\n");
		return;
	}
	ret = fat_register_device(dev_desc, 1);
	if(ret < 0){
		printf("fat regist fail %d\n", ret);
		return;
	}
	ret = file_fat_detectfs();
	if(ret){
		printf("detect fs failed\n");
		return;
	}
	do{
		printf("reading %s\n", SD_NV_NAME);
		ret = do_fat_read(SD_NV_NAME, BUF_ADDR, 0, LS_NO);
		if(ret <= 0){
			printf("sd file read error %d\n", ret);
			break;
		}
		size = FIX_SIZE + 4;
		nv_patch(BUF_ADDR, ret);
		cmd_yaffs_mount(fixnvpoint);
		cmd_yaffs_mwrite_file(fixnvfilename, BUF_ADDR, size);
		cmd_yaffs_umount(fixnvpoint);

		file_fat_rm(SD_NV_NAME);
	}while(0);

	do{
		printf("reading %s\n", SD_MODEM_NAME);
		ret = do_fat_read(SD_MODEM_NAME, BUF_ADDR, 0, LS_NO);
		if(ret <= 0){
			printf("sd file read error %d\n", ret);
			break;
		}
		size = ret;
		ret = find_dev_and_part(MODEM_PART, &dev, &pnum, &part);
		if (ret) {
			printf("No partition named %s\n", MODEM_PART);
			break;
		} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
			printf("Partition %s not a NAND device\n", MODEM_PART);
			break;
		}
		off = part->offset;
		nand = &nand_info[dev->id->num];
		memset(&opts, 0, sizeof(opts));
		opts.offset = off;
		opts.length = part->size;
		opts.quiet  = 1;
		ret = nand_erase_opts(nand, &opts);
		if(ret){
			printf("nand erase bad %d\n", ret);
			break;
		}
		ret = nand_write_skip_bad(nand, off, &size, BUF_ADDR);
		if(ret){
			printf("nand write bad %d\n", ret);
			break;
		}

		file_fat_rm(SD_MODEM_NAME);
	}while(0);

	do{
		printf("reading %s\n", SD_DSP_NAME);
		ret = do_fat_read(SD_DSP_NAME, BUF_ADDR, 0, LS_NO);
		if(ret <= 0){
			printf("sd file read error %d\n", ret);
			break;
		}
		size = ret;
		ret = find_dev_and_part(DSP_PART, &dev, &pnum, &part);
		if (ret) {
			printf("No partition named %s\n", DSP_PART);
			break;
		} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
			printf("Partition %s not a NAND device\n", DSP_PART);
			break;
		}
		off = part->offset;
		nand = &nand_info[dev->id->num];
		memset(&opts, 0, sizeof(opts));
		opts.offset = off;
		opts.length = part->size;
		opts.quiet  = 1;
		ret = nand_erase_opts(nand, &opts);
		if(ret){
			printf("nand erase bad %d\n", ret);
			break;
		}
		ret = nand_write_skip_bad(nand, off, &size, BUF_ADDR);
		if(ret < 0){
			printf("nand write bad %d\n", ret);
			break;
		}

		file_fat_rm(SD_DSP_NAME);
	}while(0);

	do{
		printf("reading %s\n", SD_VM_NAME);
		ret = do_fat_read(SD_VM_NAME, BUF_ADDR, 0, LS_NO);
		if(ret <= 0){
			printf("sd file read error %d\n", ret);
			break;
		}
		size = ret;
		ret = find_dev_and_part(VM_PART, &dev, &pnum, &part);
		if (ret) {
			printf("No partition named %s\n", VM_PART);
			break;
		} else if (dev->id->type != MTD_DEV_TYPE_NAND) {
			printf("Partition %s not a NAND device\n", VM_PART);
			break;
		}
		off = part->offset;
		nand = &nand_info[dev->id->num];
		memset(&opts, 0, sizeof(opts));
		opts.offset = off;
		opts.length = part->size;
		opts.quiet  = 1;
		ret = nand_erase_opts(nand, &opts);
		if(ret){
			printf("nand erase bad %d\n", ret);
			break;
		}
		ret = nand_write_skip_bad(nand, off, &size, BUF_ADDR);
		if(ret < 0){
			printf("nand write bad %d\n", ret);
			break;
		}

		file_fat_rm(SD_VM_NAME);
	}while(0);

	printf("update done\n");
	return;
}
#else
#define try_update_modem()
#endif

void recovery_mode(void)
{
    printf("%s\n", __func__);
    //there is no need to do it  anymore!!!
    //try_update_modem(); //update img from mmc
    vlx_nand_boot(RECOVERY_PART, NULL, BACKLIGHT_ON);
}

void recovery_mode_without_update(void)
{
    printf("%s\n", __func__);
   //try_update_modem();//do not update img from mmc
    vlx_nand_boot(RECOVERY_PART, NULL, BACKLIGHT_ON);
}
void update_mode(void)
{
    printf("%s\n", __func__);
    vlx_nand_boot(RECOVERY_PART, "androidboot.mode=update", BACKLIGHT_ON);
}
