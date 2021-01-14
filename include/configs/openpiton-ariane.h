/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2019 Western Digital Corporation or its affiliates.
 *
 * Authors:
 *   Anup Patel <anup.patel@wdc.com>
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>

//#define CONFIG_SPL

#define CONFIG_STANDALONE_LOAD_ADDR	0x80200000

#define SYS_TEXT_BASE 0x80000000
/* Environment options */
#define CONFIG_SYS_SDRAM_BASE       0x80000000                                                           
#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_SDRAM_BASE + SZ_2M)                                                                                                                                                          
#define CONFIG_SYS_LOAD_ADDR        (CONFIG_SYS_SDRAM_BASE + SZ_2M)    
#define CONFIG_SYS_MALLOC_LEN       SZ_8M
#define CONFIG_SYS_BOOTM_LEN        SZ_64M
#define CONFIG_STANDALONE_LOAD_ADDR 0x80200000
#endif/* __CONFIG_H */
