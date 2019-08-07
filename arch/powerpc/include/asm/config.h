/*
 * Copyright 2009-2010 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

#ifndef _ASM_CONFIG_H_
#define _ASM_CONFIG_H_

#define CONFIG_LMB
#define CONFIG_SYS_BOOT_RAMDISK_HIGH
#define CONFIG_SYS_BOOT_GET_CMDLINE
#define CONFIG_SYS_BOOT_GET_KBD

#ifndef CONFIG_MAX_MEM_MAPPED
#if defined(CONFIG_4xx) || defined(CONFIG_E500) || defined(CONFIG_MPC86xx)
#define CONFIG_MAX_MEM_MAPPED	((phys_size_t)2 << 30)
#else
#define CONFIG_MAX_MEM_MAPPED	(256 << 20)
#endif
#endif

/* Check if boards need to enable FSL DMA engine for SDRAM init */
#if !defined(CONFIG_FSL_DMA) && defined(CONFIG_DDR_ECC)
#if (defined(CONFIG_MPC83xx) && defined(CONFIG_DDR_ECC_INIT_VIA_DMA)) || \
	((defined(CONFIG_MPC85xx) || defined(CONFIG_MPC86xx)) && \
	!defined(CONFIG_ECC_INIT_VIA_DDRCONTROLLER))
#define CONFIG_FSL_DMA
#endif
#endif

#if defined(CONFIG_MPC8572) || defined(CONFIG_P1020) || \
	defined(CONFIG_P1021) || defined(CONFIG_P1022) || \
	defined(CONFIG_P2020) || defined(CONFIG_MPC8641)
#define CONFIG_MAX_CPUS		2
#elif defined(CONFIG_PPC_P3041)
#define CONFIG_MAX_CPUS		4
#elif defined(CONFIG_PPC_P4080)
#define CONFIG_MAX_CPUS		8
#elif defined(CONFIG_PPC_P5020)
#define CONFIG_MAX_CPUS		2
#else
#define CONFIG_MAX_CPUS		1
#endif

/*
 * Provide a default boot page translation virtual address that lines up with
 * Freescale's default e500 reset page.
 */
#if (defined(CONFIG_E500) && defined(CONFIG_MP))
#ifndef CONFIG_BPTR_VIRT_ADDR
#define CONFIG_BPTR_VIRT_ADDR	0xfffff000
#endif
#endif

/* Enable TSEC2.0 for the platforms that have it if we are using TSEC */
#if defined(CONFIG_TSEC_ENET) && \
    (defined(CONFIG_P1020) || defined(CONFIG_P1011))
#define CONFIG_TSECV2
#endif

/*
 * SEC (crypto unit) major compatible version determination
 */
#if defined(CONFIG_FSL_CORENET)
#define CONFIG_SYS_FSL_SEC_COMPAT	4
#elif defined(CONFIG_MPC85xx) || defined(CONFIG_MPC83xx)
#define CONFIG_SYS_FSL_SEC_COMPAT	2
#endif

/* Number of TLB CAM entries we have on FSL Book-E chips */
#if defined(CONFIG_E500MC)
#define CONFIG_SYS_NUM_TLBCAMS	64
#elif defined(CONFIG_E500)
#define CONFIG_SYS_NUM_TLBCAMS	16
#endif

/* Since so many PPC SOCs have a semi-common LBC, define this here */
#if defined(CONFIG_MPC85xx) || defined(CONFIG_MPC86xx) || \
	defined(CONFIG_MPC83xx)
#define CONFIG_FSL_LBC
#endif

/* All PPC boards must swap IDE bytes */
#define CONFIG_IDE_SWAP_IO

#endif /* _ASM_CONFIG_H_ */
