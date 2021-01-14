// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2020 SiFive, Inc
 * Pragnesh Patel <pragnesh.patel@sifive.com>
 */

#include <dm.h>
#include <log.h>

int spl_soc_init(void)
{
    debug("hello world from uboot spl\n");
	return 0;
}
