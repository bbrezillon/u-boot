// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2012, Google Inc. All rights reserved.
 */

#include <common.h>

static int do_bootstage_report(cmd_tbl_t *cmdtp, int flag, int argc,
			       char * const argv[])
{
	bootstage_report();

	return 0;
}

static int get_base_size(int argc, char * const argv[], ulong *basep,
			 ulong *sizep)
{
	char *endp;

	*basep = CONFIG_BOOTSTAGE_STASH_ADDR;
	*sizep = CONFIG_BOOTSTAGE_STASH_SIZE;
	if (argc < 2)
		return 0;
	*basep = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;
	if (argc == 2)
		return 0;
	*sizep = simple_strtoul(argv[2], &endp, 16);
	if (*argv[2] == 0 || *endp != 0)
		return -1;

	return 0;
}

static int do_bootstage_stash(cmd_tbl_t *cmdtp, int flag, int argc,
			      char * const argv[])
{
	ulong base, size;
	int ret;

	if (get_base_size(argc, argv, &base, &size))
		return CMD_RET_USAGE;
	if (base == -1UL) {
		printf("No bootstage stash area defined\n");
		return 1;
	}

	if (0 == strcmp(argv[0], "stash"))
		ret = bootstage_stash((void *)base, size);
	else
		ret = bootstage_unstash((void *)base, size);
	if (ret)
		return 1;

	return 0;
}

U_BOOT_CMD_WITH_SUBCMDS(bootstage,
	"Boot stage command",
	" - check boot progress and timing\n"
	"report                      - Print a report\n"
	"stash [<start> [<size>]]    - Stash data into memory\n"
	"unstash [<start> [<size>]]  - Unstash data from memory",
	U_BOOT_SUBCMD_MKENT(report, 2, 1, do_bootstage_report),
	U_BOOT_SUBCMD_MKENT(stash, 4, 0, do_bootstage_stash),
	U_BOOT_SUBCMD_MKENT(unstash, 4, 0, do_bootstage_stash));
