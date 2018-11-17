// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) Nelson Integration, LLC 2016
 * Author: Eric Nelson<eric@nelint.com>
 *
 */
#include <config.h>
#include <common.h>
#include <malloc.h>
#include <part.h>

static int blkc_show(cmd_tbl_t *cmdtp, int flag,
		     int argc, char * const argv[])
{
	struct block_cache_stats stats;
	blkcache_stats(&stats);

	printf("hits: %u\n"
	       "misses: %u\n"
	       "entries: %u\n"
	       "max blocks/entry: %u\n"
	       "max cache entries: %u\n",
	       stats.hits, stats.misses, stats.entries,
	       stats.max_blocks_per_entry, stats.max_entries);
	return 0;
}

static int blkc_configure(cmd_tbl_t *cmdtp, int flag,
			  int argc, char * const argv[])
{
	unsigned blocks_per_entry, max_entries;
	if (argc != 3)
		return CMD_RET_USAGE;

	blocks_per_entry = simple_strtoul(argv[1], 0, 0);
	max_entries = simple_strtoul(argv[2], 0, 0);
	blkcache_configure(blocks_per_entry, max_entries);
	printf("changed to max of %u entries of %u blocks each\n",
	       max_entries, blocks_per_entry);
	return 0;
}

U_BOOT_CMD_WITH_SUBCMDS(blkcache,
	"block cache diagnostics and control",
	"show - show and reset statistics\n"
	"blkcache configure blocks entries\n",
	U_BOOT_SUBCMD_MKENT(show, 0, 0, blkc_show),
	U_BOOT_SUBCMD_MKENT(configure, 3, 0, blkc_configure),
);
