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
#include <asm/arch/adc_drvapi.h>
#include <asm/arch/regs_ahb.h>

#define CALIBRATE_TO	(60 * 1)	/*one minute */
#define MEASURE_TIMES	(128)

#define REG32(x)   (*((volatile uint32 *)(x)))

//#define 	SPRD_ANA_BASE 	(SPRD_MISC_BASE + 0x600)
#define   ANA_REG_BASE        		  0x82000600
#define   ANA_DCDC_CTRL             (ANA_REG_BASE + 0x38)
#define   ANA_DCDC_CTRL_DS           (ANA_REG_BASE + 0x3C)
#define   ANA_DCDC_CTRL_CAL           (ANA_REG_BASE + 0x40)
#define   ANA_DCDCARM_CTRL            (ANA_REG_BASE + 0x44)
#define   ANA_DCDCARM_CTRL_CAL      (ANA_REG_BASE + 0x48)

#define abs(_x) (_x>=0? _x: -_x)

const int dcdc_ctl_vol[] = {
	650, 700, 800, 900, 1000, 1100, 1200, 1300, 1400,
};

extern uint16_t dcdc_AdcvalueToVoltage(uint16_t adcvalue);

static int dcdc_calibrate(int adc_chan, int def_vol, int to_vol)
{
	int i;
	u32 val[MEASURE_TIMES], sum = 0;
	u32 adc_vol, ctl_vol, cal_vol;
	for (i = 0; i < ARRAY_SIZE(val); i++) {
		sum += val[i] = ADC_GetValue(adc_chan, false);
	}
	sum /= ARRAY_SIZE(val);	//get average value
	printf("adc chan %d, value %d\n", adc_chan, sum);
	adc_vol = dcdc_AdcvalueToVoltage(sum) * (8 * 5) / (30 * 4);

	if (!def_vol) {
		switch (adc_chan) {
		case ADC_CHANNEL_DCDC:
			def_vol = 1100;
			cal_vol = ANA_REG_GET(ANA_DCDC_CTRL_CAL) & 0x1f;
			i = ANA_REG_GET(ANA_DCDC_CTRL) & 0x07;
			break;
		case ADC_CHANNEL_DCDCARM:
			def_vol = 1200;
			cal_vol = ANA_REG_GET(ANA_DCDCARM_CTRL_CAL) & 0x1f;
			i = ANA_REG_GET(ANA_DCDCARM_CTRL) & 0x07;
			break;
		default:
			goto exit;
		}
		if (0 != i /* + cal_vol */ )
			def_vol = dcdc_ctl_vol[i];
		def_vol += cal_vol * 100 / 32;
	}

	printf("%s default %dmv, from %dmv to %dmv\n", __FUNCTION__, def_vol,
	     adc_vol, to_vol);

	cal_vol = abs(adc_vol - to_vol);
	if (cal_vol > 200 /* mv */ )
		goto exit;
	else if (cal_vol < to_vol / 100) {
		printf("%s is ok\n", __FUNCTION__);
		return 0;
	}

	ctl_vol = def_vol * to_vol / adc_vol;
	for (i = 0; i < ARRAY_SIZE(dcdc_ctl_vol) - 1; i++) {
		if (ctl_vol < dcdc_ctl_vol[i + 1])
			break;
	}
	if (i >= ARRAY_SIZE(dcdc_ctl_vol) - 1)
		goto exit;

	cal_vol = ((ctl_vol - dcdc_ctl_vol[i]) * 32 / 100) % 32;
	printf("%s cal_vol %dmv: %d, 0x%02x\n", __FUNCTION__,
	      dcdc_ctl_vol[i] + cal_vol * 100 / 32, i, cal_vol);
	switch (adc_chan) {
	case ADC_CHANNEL_DCDC:
		ANA_REG_SET(ANA_DCDC_CTRL_CAL, cal_vol | (0x1f - cal_vol) << 8);
		ANA_REG_SET(ANA_DCDC_CTRL, i | (0x07 - i) << 4);
		break;
	case ADC_CHANNEL_DCDCARM:
		ANA_REG_SET(ANA_DCDCARM_CTRL_CAL,
			    cal_vol | (0x1f - cal_vol) << 8);
		ANA_REG_SET(ANA_DCDCARM_CTRL, i | (0x07 - i) << 4);
		break;
	default:
		break;
	}

	return dcdc_ctl_vol[i] + cal_vol * 100 / 32;
exit:
	printf("%s failure\n", __FUNCTION__);
	return -1;
}


void do_dcdc_work()
{
	int ret, cnt = CALIBRATE_TO;
	int dcdcarm_to_vol = 1230;	/* vddarm */
//	int cpu_freq = 1000;	/* Mega */
//	int dcdc_to_vol = 1100;	/* vddcore */
	u32 val = 0;
	int i ;

	printf("%s %d\n", __func__, cnt);

	if (REG32(0x209003fc) == CHIP_ID_8810S) {
		dcdcarm_to_vol = 1300;//vddarm 1.30v
	}

	ret = dcdc_calibrate(ADC_CHANNEL_DCDCARM, 0, dcdcarm_to_vol);
	if (ret > 0)
		dcdc_calibrate(ADC_CHANNEL_DCDCARM, ret, dcdcarm_to_vol);
	for(i = 0; i < 0x1000; ++i);

/*
	ret = dcdc_calibrate(ADC_CHANNEL_DCDC, 0, dcdc_to_vol);
	if (ret > 0)
		dcdc_calibrate(ADC_CHANNEL_DCDC, ret, dcdc_to_vol);
*/

	return;
}

