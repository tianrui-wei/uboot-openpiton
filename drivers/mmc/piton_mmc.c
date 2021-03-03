// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2009 SAMSUNG Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
 * Jaehoon Chung <jh80.chung@samsung.com>
 * Portions Copyright 2011-2019 NVIDIA Corporation
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

struct piton_mmc_plat {
  struct mmc_config cfg;
  struct mmc mmc;
};

struct piton_mmc_priv {
  uint64_t piton_sd_base_addr; /* piton base address */
};

// see mmc_read_blocks to see how it is used.
// start block is hidden at cmd->arg
// also, initialize the block size at init
static int piton_mmc_send_cmd(struct udevice *dev, struct mmc_cmd *cmd,
                              struct mmc_data *data) {
  // check first if this is a pure command
  if (data == NULL) {
    return 0;
  }

  // byte count counts all the bytes required for this command
  uint64_t byte_cnt = data->blocks * data->blocksize;
  // get which block in sd card to start from
  uint64_t start_block = cmd->cmdarg;
  // buff points to the address we store the data stored at sd card
  unsigned *buff = (unsigned int *) data->dest;

  struct piton_mmc_priv *priv = dev_get_priv(dev);
  // start address denotes the absolute address where the transmission start
  uint64_t start_addr = priv->piton_sd_base_addr + (start_block);

  /* if data is not empty*/
  if (data) {

    /* if there is a read */
    if (data->flags & MMC_DATA_READ) {
      for (uint64_t i = 0; i < byte_cnt; i += 4) {
        *(buff) = readl((void *)(start_addr + i));
        buff++;
      }
    } else {
      /* else there is a write
       * we don't handle write, so error right away
       */
      return -ENODEV;
    }
  }
  return 0;
}

static int piton_mmc_ofdata_to_platdata(struct udevice *dev)
{
  struct piton_mmc_plat *plat = dev_get_platdata(dev);
  struct mmc_config *cfg;
  struct mmc *mmc;

  cfg = &plat->cfg;
  cfg->name = "PITON MMC";
  cfg->host_caps = MMC_MODE_8BIT;
  cfg->f_max = 100000;
  cfg->f_min = 400000;
  cfg->voltages = MMC_VDD_21_22;

  mmc = &plat->mmc;
  mmc->read_bl_len = MMC_MAX_BLOCK_LEN;
  mmc->capacity_user = 0x100000000ULL;
  mmc->capacity_user *= mmc->read_bl_len;
  mmc->capacity_boot = 0;
  mmc->capacity_rpmb = 0;
  for (int i = 0; i < 4; i++)
    mmc->capacity_gp[i] = 0;
  mmc->capacity = 0x2000000000ULL;
  mmc->has_init = 1;

  /* fill in device description */
  struct blk_desc *bdesc;

  bdesc = mmc_get_blk_desc(mmc);
  bdesc->lun = 0;
  bdesc->hwpart = 0;
  bdesc->type = 0;
  bdesc->blksz = mmc->read_bl_len;
  bdesc->log2blksz = LOG2(bdesc->blksz);
  bdesc->lba = lldiv(mmc->capacity, mmc->read_bl_len);

  return 0;
}
/*
 * currently, this is ignored. we only use fixed speed
 */
static int piton_mmc_set_ios(struct udevice *dev) { return 0; }

/* test if piton has the micro sd card present
 * always return 1, which means present
 */
static int piton_mmc_getcd(struct udevice *dev) {
  return 1;
}

/* dummy function, piton_sd don't need initialization in hw*/
static int piton_mmc_init(struct udevice *dev) {
  return 0;
}

// TODO: add more operation here
static const struct dm_mmc_ops piton_mmc_ops = {
    // send a command to mmc device
    .send_cmd = piton_mmc_send_cmd,
    // set iospeed
    .set_ios = piton_mmc_set_ios,
    // detect if card is present
    .get_cd = piton_mmc_getcd,
};

static int piton_mmc_probe(struct udevice *dev) {
  struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
  struct piton_mmc_plat *plat = dev_get_platdata(dev);
  struct piton_mmc_priv *priv = dev_get_uclass_priv(dev);
  struct mmc_config *cfg = &plat->cfg;

  priv->piton_sd_base_addr = dev_read_addr(dev);

  cfg->name = dev->name;
  upriv->mmc = &plat->mmc;
  upriv->mmc->has_init = 1;
  upriv->mmc->capacity = 0x2000000000ULL;
  upriv->mmc->read_bl_len = MMC_MAX_BLOCK_LEN;

  return piton_mmc_init(dev);
}

static int piton_mmc_bind(struct udevice *dev) {
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
    {.compatible = "ariane,piton-mmc"}, {}};

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
