// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2019 SiFive, Inc
 *
 * Authors:
 *   Pragnesh Patel <pragnesh.patel@sifive.com>
 *   Tianrui Wei <tianrui-wei@outlook.com>
 */
#include <hang.h>
#include <common.h>
#include <init.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <log.h>
#include <configs/openpiton-ariane.h>
#include <mmc.h>
#include <dm.h>
#include <spl.h>

void board_boot_order(u32 *spl_boot_list) {
  u8 i;
  u32 boot_devices[] = {
    BOOT_DEVICE_MMC1,
  };

  for (i = 0; i < ARRAY_SIZE(boot_devices); i++)
    spl_boot_list[i] = boot_devices[i];
}


#ifdef CONFIG_SPL_LOAD_FIT
int board_fit_config_name_match(const char *name) {
  /* boot using first FIT config */
  return 0;
}
#endif
