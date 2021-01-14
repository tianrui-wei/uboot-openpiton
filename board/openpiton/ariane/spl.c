// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2019 SiFive, Inc
 *
 * Authors:
 *   Pragnesh Patel <pragnesh.patel@sifive.com>
 *   Tianrui Wei <tianrui-wei@outlook.com>
 */

#include <init.h>
#include <spl.h>
#include <misc.h>
#include <log.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <asm/gpio.h>
#include <asm/arch/gpio.h>
#include <asm/arch/spl.h>

#define GEM_PHY_RESET		SIFIVE_GENERIC_GPIO_NR(0, 12)

#define MODE_SELECT_REG		0x1000
#define MODE_SELECT_QSPI	0x6
#define MODE_SELECT_SD		0xb
#define MODE_SELECT_MASK	GENMASK(3, 0)

int spl_board_init_f(void)
{
	int ret;

	ret = spl_soc_init();
	if (ret) {
		debug("Openpiton Ariane SPL init failed: %d\n", ret);
		return ret;
	}

	return 0;
}

u32 spl_boot_device(void)
{
	u32 mode_select = readl((void *)MODE_SELECT_REG);
	u32 boot_device = mode_select & MODE_SELECT_MASK;

    //TODO: boot directly from sdcard exposed as rom
	switch (boot_device) {
	case MODE_SELECT_QSPI:
		return BOOT_DEVICE_SPI;
	case MODE_SELECT_SD:
		return BOOT_DEVICE_MMC1;
	default:
		debug("Unsupported boot device 0x%x but trying MMC1\n",
		      boot_device);
		return BOOT_DEVICE_MMC1;
	}
}

#ifdef CONFIG_SPL_LOAD_FIT
int board_fit_config_name_match(const char *name)
{
	/* boot using first FIT config */
	return 0;
}
#endif
