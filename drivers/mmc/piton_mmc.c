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
#include <mmc.h>
// FIXME: fix the includes. Includes below are for the benefits of clangd, which
// doesn't really work...
//#include <stdint.h>
//#include "../../arch/riscv/include/asm/io.h"
//#include "../../include/dm/device.h"
//#include "../../include/linux/bitops.h"
//#include "../../include/log.h"
//#include "../../include/mmc.h"

struct piton_mmc_plat {
  struct mmc_config cfg;
  struct mmc mmc;
};

struct piton_mmc_priv {
  uint64_t piton_sd_base_addr; /* peripheral id */
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

  if (cmd->cmdidx == 12)
    return 0; 
  // byte count counts all the bytes required for this command
  uint64_t byte_cnt = data->blocks * data->blocksize;
  // get which block in sd card to start from
  uint64_t start_block = cmd->cmdarg;
  // buff points to the address we store the data stored at sd card
  unsigned *buff = (unsigned int *) data->dest;

  struct piton_mmc_priv *priv = dev_get_priv(dev);
  // start address denotes the absolute address where the transmission start
  uint64_t start_addr = priv->piton_sd_base_addr + (start_block);

#ifdef DEBUGFF
  printf("sd card debug: command index is %d\n", cmd->cmdidx);
  printf("sd card debug: command argument is %d\n", cmd->cmdarg);
  printf("sd card debug: command response type is %d\n", cmd->resp_type);
#endif
  // TODO: handle command response

  /* if data is not empty*/
  if (data) {
#ifdef DEBUGFF
    printf("sd card debug: data source is %lld\n", start_addr);
    printf("sd card debug: data destination is %p\n", data->dest);
    printf("sd card debug: data number of blocks is is %d\n", data->blocks);
    printf("sd card debug: data blocksize is %d\n", data->blocksize);
    printf("sd card debug: data flag is %d\n", data->flags);
#endif
    // FIXME: pin the sd card to 512 sector

    /* if there is a read */
    if (data->flags & MMC_DATA_READ) {
      for (uint64_t i = 0; i < byte_cnt; i += 4) {
        *(buff) = readl((void *)(start_addr + i));
        //printf("sd card debug: read data is 0x%08d\n", readl((void *)(start_addr + i)));
        buff++;
      }
    } else {
      //printf("wrong command! Only read is supported\n");
      /* else there is a write
       * we don't handle write, so error right away
       */
      return -ENODEV;
    }
  } else {
    //printf("data is empty\n");
  }

  return 0;
}

static int piton_mmc_ofdata_to_platdata(struct udevice *dev)
{
  struct piton_mmc_priv *priv = dev_get_priv(dev);
  struct piton_mmc_plat *plat = dev_get_platdata(dev);
  struct mmc_config *cfg;

  //FIXME: wrong base addrss
  priv->piton_sd_base_addr = 0xf000000000L;
  cfg = &plat->cfg;
  cfg->name = "PITON MMC";
  cfg->host_caps = MMC_MODE_8BIT;
  cfg->f_max = 100000;
  cfg->f_min = 400000;
  cfg->voltages = MMC_VDD_21_22;
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

// TODO: bind block size here
static int piton_mmc_probe(struct udevice *dev) {
  struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
  struct piton_mmc_plat *plat = dev_get_platdata(dev);
  struct piton_mmc_priv *priv = dev_get_uclass_priv(dev);
  struct mmc_config *cfg = &plat->cfg;

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
