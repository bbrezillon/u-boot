// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2013 Xilinx, Inc.
 */
#include <common.h>
#include <command.h>
#include <clk.h>
#if defined(CONFIG_DM) && defined(CONFIG_CLK)
#include <dm.h>
#include <dm/device-internal.h>
#endif

int __weak soc_clk_dump(void)
{
#if defined(CONFIG_DM) && defined(CONFIG_CLK)
	struct udevice *dev;
	struct uclass *uc;
	struct clk clk;
	int ret;

	/* Device addresses start at 1 */
	ret = uclass_get(UCLASS_CLK, &uc);
	if (ret)
		return ret;

	uclass_foreach_dev(dev, uc) {
		memset(&clk, 0, sizeof(clk));
		ret = device_probe(dev);
		if (ret) {
			printf("%-30.30s : ? Hz\n", dev->name);
			continue;
		}

		ret = clk_request(dev, &clk);
		if (ret) {
			printf("%-30.30s : ? Hz\n", dev->name);
			continue;
		}

		printf("%-30.30s : %lu Hz\n", dev->name, clk_get_rate(&clk));

		clk_free(&clk);
	}

	return 0;
#else
	puts("Not implemented\n");
	return 1;
#endif
}

static int do_clk_dump(cmd_tbl_t *cmdtp, int flag, int argc,
		       char *const argv[])
{
	int ret;

	ret = soc_clk_dump();
	if (ret < 0) {
		printf("Clock dump error %d\n", ret);
		ret = CMD_RET_FAILURE;
	}

	return ret;
}

#ifdef CONFIG_SYS_LONGHELP
static char clk_help_text[] =
	"dump - Print clock frequencies";
#endif

U_BOOT_CMD_WITH_SUBCMDS(clk, 1, "CLK sub-system", clk_help_text,
			U_BOOT_SUBCMD_MKENT(dump, 1, do_clk_dump));
