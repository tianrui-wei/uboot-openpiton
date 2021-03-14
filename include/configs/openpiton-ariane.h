/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2019 Western Digital Corporation or its affiliates.
 *
 * Authors:
 *   Anup Patel <anup.patel@wdc.com>
 */

#ifndef __OPENPITON_ARIANE_CONFIG_H
#define __OPENPITON_ARIANE_CONFIG_H

#include <linux/sizes.h>
#define DEBUG
//#define CONFIG_SPL_MMC_BOOT
//#define CONFIG_SPL

//#ifdef CONFIG_SPL
//#define CONFIG_SYS_UBOOT_BASE 0x80000000
//#define CONFIG_STANDALONE_LOAD_ADDR	0x80000000

#define CONFIG_SPL_MAX_SIZE     0x00100000
#define CONFIG_SPL_BSS_START_ADDR   0x82000000
#define CONFIG_SPL_BSS_MAX_SIZE     0x00100000
#define CONFIG_SYS_SPL_MALLOC_START (CONFIG_SPL_BSS_START_ADDR + \
        CONFIG_SPL_BSS_MAX_SIZE)
#define CONFIG_SYS_SPL_MALLOC_SIZE  0x0100000
#define CONFIG_SPL_STACK    (0x80000000 + 0x04000000 - \
        GENERATED_GBL_DATA_SIZE)

#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME "fw_payload.bin"
#define CONFIG_SPL_GD_ADDR 0x85000000
//#endif

// #define SYS_TEXT_BASE 0x80000000
/* Environment options */
#define CONFIG_SYS_SDRAM_BASE 0x80000000
#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_SDRAM_BASE + SZ_2M)
#define CONFIG_SYS_LOAD_ADDR        (CONFIG_SYS_SDRAM_BASE + SZ_2M)
#define CONFIG_SYS_MALLOC_LEN       SZ_8M
#define CONFIG_SYS_BOOTM_LEN        SZ_64M

/* -------------------------------------------------
 * Environment
 */
//Disable persistent environment variable storage
#define CONFIG_ENV_IS_NOWHERE   1


//#define CONFIG_ENV_SIZE		0x4000

#define CONFIG_EXTRA_ENV_SETTINGS "\0"

#define CONFIG_USE_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
    "fdt addr ${fdtcontroladdr}; " \
    "fdt move ${fdtcontroladdr} 0x86000000; " \
    "ext4load mmc 0:1 0x80200000 Image; " \
    "booti 0x80200000 - 0x86000000; "

/* ---------------------------------------------------------------------
 * Board boot configuration
 */

#define CONFIG_TIMESTAMP	/* Print image info with timestamp */

#endif/* __CONFIG_H */
