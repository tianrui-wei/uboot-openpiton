// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2019 Western Digital Corporation or its affiliates.
 *
 * Authors:
 *   Anup Patel <anup.patel@wdc.com>
 */

#include <dm.h>
#include <env.h>
#include <init.h>
#include <log.h>
#include <linux/bitops.h>
#include <linux/bug.h>
#include <linux/delay.h>
#include <misc.h>
#include <spl.h>

/*
 * This define is a value used for error/unknown serial.
 * If we really care about distinguishing errors and 0 is
 * valid, we'll need a different one.
 */
#define ERROR_READING_SERIAL_NUMBER       0


int board_init(void)
{

	return 0;
}
