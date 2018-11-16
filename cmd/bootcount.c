// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <command.h>
#include <bootcount.h>

static int do_bootcount_print(cmd_tbl_t *cmdtp, int flag, int argc,
			      char * const argv[])
{
	printf("%lu\n", bootcount_load());
	return CMD_RET_SUCCESS;
}

static int do_bootcount_reset(cmd_tbl_t *cmdtp, int flag, int argc,
			      char * const argv[])
{
	/*
	 * note that we're explicitly not resetting the environment
	 * variable, so you still have the old bootcounter available
	 */
	bootcount_store(0);
	return CMD_RET_SUCCESS;
}

#if CONFIG_IS_ENABLED(SYS_LONGHELP)
static char bootcount_help_text[] =
	"print - print current bootcounter\n"
	"reset - reset the bootcounter"
	;
#endif

U_BOOT_CMD_WITH_SUBCMDS(bootcount,
	"bootcount",
#if CONFIG_IS_ENABLED(SYS_LONGHELP)
	bootcount_help_text,
#else
	"",
#endif
	U_BOOT_SUBCMD_MKENT(print, 1, 1, do_bootcount_print),
	U_BOOT_SUBCMD_MKENT(reset, 1, 1, do_bootcount_reset));
