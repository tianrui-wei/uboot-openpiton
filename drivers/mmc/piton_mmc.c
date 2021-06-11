// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2009 SAMSUNG Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
 * Jaehoon Chung <jh80.chung@samsung.com>
 * Portions Copyright 2011-2019 NVIDIA Corporation
 * Portions Copyright 2021 Tianrui Wei
 * This file is adapted from tegra_mmc.c
 * Tianrui Wei <tianrui-wei@outlook.com>
 */

#include <asm/gpio.h>
#include <asm/io.h>
#include <common.h>
#include <dm.h>
#include <errno.h>
#include <linux/bitops.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <log.h>
#include <div64.h>
#include <mmc.h>
#define CONFIG_MMC_TRACE
struct piton_mmc_plat {
	struct mmc_config cfg;
	struct mmc mmc;
};

struct piton_mmc_priv {
	u64 piton_mmc_base_addr; /* peripheral id */
};

/*
 * see mmc_read_blocks to see how it is used.
 * start block is hidden at cmd->arg
 * also, initialize the block size at init
 */
static int piton_mmc_send_cmd(struct udevice *dev, struct mmc_cmd *cmd,
							  struct mmc_data *data)
{
	/* check first if this is a pure command */
	if (!data)
		return 0;


	struct piton_mmc_priv *priv = dev_get_priv(dev);
    unsigned int *buff;
    u64 byte_cnt, start_block, start_addr;
	
    buff = (unsigned int *)data->dest;
    byte_cnt = data->blocks * data->blocksize;
	start_block = cmd->cmdarg;
    start_addr = priv->piton_mmc_base_addr + (start_block);

	/* if there is a read */
	if (data->flags & MMC_DATA_READ) {
		for (u64 i = 0; i < byte_cnt; i += 4) {
			*(buff) = readl((void *)(start_addr + i));
			buff++;
		}
	} else {
		/* else there is a write
		 * we don't handle write, so error right away
		 */
		return -ENODEV;
	}
    
	return 0;
}

static int piton_mmc_ofdata_to_platdata(struct udevice *dev)
{
	struct piton_mmc_priv *priv = dev_get_priv(dev);
	struct piton_mmc_plat *plat = dev_get_platdata(dev);
	struct mmc_config *cfg;
	struct mmc *mmc;
	/* fill in device description */
	struct blk_desc *bdesc;

	priv->piton_mmc_base_addr = dev_read_addr(dev);
	cfg = &plat->cfg;
	cfg->name = "PITON MMC";
	cfg->host_caps = MMC_MODE_8BIT;
	cfg->f_max = 100000;
	cfg->f_min = 400000;
	cfg->voltages = MMC_VDD_21_22;

	mmc = &plat->mmc;
	mmc->read_bl_len = MMC_MAX_BLOCK_LEN;
	mmc->capacity_user = 0x100000000;
	mmc->capacity_user *= mmc->read_bl_len;
	mmc->capacity_boot = 0;
	mmc->capacity_rpmb = 0;
	for (int i = 0; i < 4; i++)
		mmc->capacity_gp[i] = 0;
	mmc->capacity = 0x2000000000ULL;
	mmc->has_init = 1;

	bdesc = mmc_get_blk_desc(mmc);
	bdesc->lun = 0;
	bdesc->hwpart = 0;
	bdesc->type = 0;
	bdesc->blksz = mmc->read_bl_len;
	bdesc->log2blksz = LOG2(bdesc->blksz);
	bdesc->lba = lldiv(mmc->capacity, mmc->read_bl_len);

	return 0;
}

/* test if piton has the micro mmc card present
 * always return 1, which means present
 */
static int piton_mmc_getcd(struct udevice *dev)
{
	/*
	 * always return 1
	 */
	return 1;
}

/* dummy function, piton_mmc don't need initialization
 * in hw
 */
static const struct dm_mmc_ops piton_mmc_ops = {
	.send_cmd = piton_mmc_send_cmd,
	.get_cd = piton_mmc_getcd,
};

static int piton_mmc_probe(struct udevice *dev)
{
	struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
	struct piton_mmc_plat *plat = dev_get_platdata(dev);
	struct mmc_config *cfg = &plat->cfg;

	cfg->name = dev->name;
	upriv->mmc = &plat->mmc;
	upriv->mmc->has_init = 1;
	upriv->mmc->capacity = 0x2000000000ULL;
	upriv->mmc->read_bl_len = MMC_MAX_BLOCK_LEN;
	return 0;
}

static int piton_mmc_bind(struct udevice *dev)
{
	struct piton_mmc_plat *plat = dev_get_platdata(dev);
	struct mmc_config *cfg = &plat->cfg;

	cfg->name = dev->name;
	cfg->host_caps = MMC_MODE_HS_52MHz | MMC_MODE_HS | MMC_MODE_8BIT;
	cfg->voltages = MMC_VDD_165_195 | MMC_VDD_32_33 | MMC_VDD_33_34;
	cfg->f_min = 1000000;
	cfg->f_max = 52000000;
	cfg->b_max = U32_MAX;

	return mmc_bind(dev, &plat->mmc, cfg);
}

static const struct udevice_id piton_mmc_ids[] = {
		{.compatible = "openpiton,piton-mmc"},
		{ /* sentinel */ }
};

U_BOOT_DRIVER(piton_mmc_drv) = {
	.name = "piton_mmc",
	.id = UCLASS_MMC,
	.of_match = piton_mmc_ids,
	.ofdata_to_platdata = piton_mmc_ofdata_to_platdata,
	.bind = piton_mmc_bind,
	.probe = piton_mmc_probe,
	.ops = &piton_mmc_ops,
	.platdata_auto_alloc_size = sizeof(struct piton_mmc_plat),
	.priv_auto_alloc_size = sizeof(struct piton_mmc_priv),
};
