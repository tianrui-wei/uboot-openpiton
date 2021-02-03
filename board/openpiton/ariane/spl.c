// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2019 SiFive, Inc
 *
 * Authors:
 *   Pragnesh Patel <pragnesh.patel@sifive.com>
 *   Tianrui Wei <tianrui-wei@outlook.com>
 */

#include <asm/arch/spl.h>
#include <init.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <log.h>
//#include <misc.h>
#include <spl.h>
// includes to make lsp happy
//#include "../../../arch/riscv/include/asm/arch-ariane/spl.h"
//#include "../../../arch/riscv/include/asm/spl.h"
//#include "../../../include/spl.h"

int spl_board_init_f(void) {
  int ret;

  ret = spl_soc_init();
  if (ret) {
    debug("Openpiton Ariane SPL init failed: %d\n", ret);
    return ret;
  }

  return 0;
}
void board_boot_order(u32 *spl_boot_list) {
  u8 i;
  u32 boot_devices[] = {
      BOOT_DEVICE_MMC1,
  };

  for (i = 0; i < ARRAY_SIZE(boot_devices); i++)
    spl_boot_list[i] = boot_devices[i];
}

u32 spl_boot_device(void) { return BOOT_DEVICE_MMC1; }

#ifdef CONFIG_SPL_LOAD_FIT
int board_fit_config_name_match(const char *name) {
  /* boot using first FIT config */
  return 0;
}
#endif
