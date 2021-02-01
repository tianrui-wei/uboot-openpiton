// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2009 SAMSUNG Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
 * Jaehoon Chung <jh80.chung@samsung.com>
 * Portions Copyright 2011-2019 NVIDIA Corporation
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <log.h>
#include <mmc.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/arch-piton/piton_mmc.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/err.h>
//FIXME: fix the includes. Includes below are for the benefits of clangd, which doesn't really work...
#include "../../include/log.h"
#include "../../include/mmc.h"
#include "../../include/dm/device.h"
#include "../../include/linux/bitops.h"
#include "../../include/linux/types.h"
#include "../../arch/riscv/include/asm/io.h"

struct piton_mmc_plat {
	struct mmc_config cfg;
	struct mmc mmc;
};

struct piton_mmc_priv {
	struct piton_mmc *reg;
	uint64_t piton_sd_base_addr;		/* peripheral id */
};

#define PITON_SD_READ 0
#define PITON_SD_WRITE 1

/* transimit data by cpu
 */
static int mmc_trans_data_by_cpu(struct piton_mmc_priv *priv, struct mmc *mmc,
                                 struct mmc_data *data) {
  const int reading = !!(data->flags & MMC_DATA_READ);
  unsigned i;
  unsigned *buff = (unsigned int *)(reading ? data->dest : data->src);
  unsigned byte_cnt = data->blocksize * data->blocks;
  unsigned timeout_msecs = byte_cnt >> 8;
  unsigned long start;

  if (timeout_msecs < 2000)
    timeout_msecs = 2000;

  start = get_timer(0);

  for (i = 0; i < (byte_cnt >> 2); i++) {
    if (get_timer(start) > timeout_msecs)
      return -1;

    if (reading)
      buff[i] = readl(&priv->reg->fifo);
    else
      writel(buff[i], &priv->reg->fifo);
  }

  return 0;
}

// see mmc_read_blocks to see how it is used.
// start block is hidden at cmd->arg
// also, initialize the block size at init
static int piton_mmc_send_cmd(struct udevice *dev, struct mmc_cmd *cmd,
			      struct mmc_data *data)
{
  uint8_t is_reading = data->flags & MMC_DATA_READ ;
	uint64_t byte_cnt = data->blocks * data->blocksize;
  unsigned *buff = 
	int ret;

  /* if data is not empty*/
	if (data) {
    /* if there is a read */
		if (data->flags & MMC_DATA_READ) {
			buf = data->dest;
      rw_mode = PITON_SD_READ;
		} else {
      /* else there is a write*/
			buf = (void *)data->src;
      rw_mode = PITON_SD_WRITE;
		}
		len = data->blocks * data->blocksize;
	} else {
    debug("data is empty\n");
  }

	if (data) {
  }

	return ret;
}



/*
 * currently, this is ignored. we only use fixed speed
 */
static int piton_mmc_set_ios(struct udevice *dev)
{
	return 0;
}

/* test if piton has the micro sd card present
 * always return 1, which means present
 */
static int piton_mmc_getcd(struct udevice *dev)
{
	debug("piton_mmc_getcd called\n");

	return 1;
}

//TODO: add more operation here
static const struct dm_mmc_ops piton_mmc_ops = {
  // send a command to mmc device
	.send_cmd	= piton_mmc_send_cmd,
  // set iospeed
	.set_ios	= piton_mmc_set_ios,
  // detect if card is present
	.get_cd		= piton_mmc_getcd,
};

//TODO: bind block size here
static int piton_mmc_probe(struct udevice *dev)
{
	struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
	struct piton_mmc_plat *plat = dev_get_platdata(dev);
	struct piton_mmc_priv *priv = dev_get_priv(dev);
	struct mmc_config *cfg = &plat->cfg;
	int bus_width, ret;

	cfg->name = dev->name;




	return piton_mmc_init(dev);
}

static int piton_mmc_bind(struct udevice *dev)
{
	struct piton_mmc_plat *plat = dev_get_platdata(dev);

	return mmc_bind(dev, &plat->mmc, &plat->cfg);
}

static const struct udevice_id piton_mmc_ids[] = {
	{ .compatible = "ariane,piton-mmc" },
	{ }
};

U_BOOT_DRIVER(piton_mmc_drv) = {
	.name		= "piton_mmc",
	.id		= UCLASS_MMC,
	.of_match	= piton_mmc_ids,
	.bind		= piton_mmc_bind,
	.probe		= piton_mmc_probe,
	.ops		= &piton_mmc_ops,
	.platdata_auto_alloc_size = sizeof(struct piton_mmc_plat),
	.priv_auto_alloc_size = sizeof(struct piton_mmc_priv),
};
