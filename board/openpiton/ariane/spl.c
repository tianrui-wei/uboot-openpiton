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
#include <asm/arch/spl.h>
#include <init.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <log.h>
#include <configs/openpiton-ariane.h>
#include <mmc.h>
#include <dm.h>
//#include <misc.h>
#include <spl.h>
#include <config.h>
// includes to make lsp happy
//#include "../../../arch/riscv/include/asm/arch-ariane/spl.h"
//#include "../../../arch/riscv/include/asm/spl.h"
//#include "../../../include/spl.h"

DECLARE_GLOBAL_DATA_PTR;

int spl_board_init_f(void) {
  int ret;

  ret = spl_soc_init();
  if (ret) {
    debug("Openpiton Ariane SPL init failed: %d\n", ret);
    return ret;
  }

  return 0;
}

void spl_board_init()
{
  //  struct mmc *mmc;
  //  mmc = find_mmc_device(BOOT_DEVICE_MMC1);
  //  if (!mmc)
  //    hang();
  //
  //  ret = mmc_init(mmc);
  //  if (ret)
  //    hang();
}

void board_init_f(ulong dummy) {
  debug("entered custom board init r function");
  int ret;

  ret = spl_early_init();
  if (ret)
    panic("spl_early_init() failed: %d\n", ret);

  arch_cpu_init_dm();

  preloader_console_init();

  ret = spl_board_init_f();
  if (ret)
    panic("spl_board_init_f() failed: %d\n", ret);
  u64 current_pc;
  asm volatile("auipc %0, 0x0":"=r"(current_pc):);
  debug("current pc is %llx\n", current_pc);
  //TODO: make this into a macro
  if (current_pc < 0x84000000ULL) {
    debug("Relocation executing");
    //    spl_reloc(0x80000000ULL, 0x85000000ULL, (0x10000ULL));
    relocate_code(0x87000000ULL, (gd_t *)CONFIG_SPL_GD_ADDR, 0x85000000ULL);
  } else {
    debug("Not relocating");
  }
}

void board_boot_order(u32 *spl_boot_list) {
  u8 i;
  u32 boot_devices[] = {
    BOOT_DEVICE_MMC2_2,
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
