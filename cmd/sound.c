// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2012 Samsung Electronics
 * Rajeshwari Shinde <rajeshwari.s@samsung.com>
 */

#include <common.h>
#include <command.h>
#include <fdtdec.h>
#include <sound.h>

DECLARE_GLOBAL_DATA_PTR;

/* Initilaise sound subsystem */
static int do_init(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret;

	ret = sound_init(gd->fdt_blob);
	if (ret) {
		printf("Initialise Audio driver failed\n");
		return CMD_RET_FAILURE;
	}

	return 0;
}

/* play sound from buffer */
static int do_play(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret = 0;
	int msec = 1000;
	int freq = 400;

	if (argc > 1)
		msec = simple_strtoul(argv[1], NULL, 10);
	if (argc > 2)
		freq = simple_strtoul(argv[2], NULL, 10);

	ret = sound_play(msec, freq);
	if (ret) {
		printf("play failed");
		return CMD_RET_FAILURE;
	}

	return 0;
}

U_BOOT_CMD_WITH_SUBCMDS(sound,
	"sound sub-system",
	"init - initialise the sound driver\n"
	"sound play [len] [freq] - play a sound for len ms at freq hz\n",
	U_BOOT_SUBCMD_MKENT(init, 1, 1, do_init),
	U_BOOT_SUBCMD_MKENT(play, 3, 1, do_play));
