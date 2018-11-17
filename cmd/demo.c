// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2013 Google, Inc
 *
 * (C) Copyright 2012
 * Pavel Herrmann <morpheus.ibis@gmail.com>
 */

#include <common.h>
#include <dm-demo.h>
#include <mapmem.h>
#include <asm/io.h>

struct udevice *demo_dev;

static int do_demo_hello(cmd_tbl_t *cmdtp, int flag, int argc,
			 char * const argv[])
{
	int devnum = 0;
	int ch = 0;
	int ret;

	if (argc < 2)
		return CMD_RET_USAGE;

	devnum = simple_strtoul(argv[1], NULL, 10);
	ret = uclass_get_device(UCLASS_DEMO, devnum, &demo_dev);
	if (ret)
		return cmd_process_error(cmdtp, ret);

	if (argc > 2)
		ch = *argv[0];

	ret = demo_hello(demo_dev, ch);
	return cmd_process_error(cmdtp, ret);
}

static int do_demo_status(cmd_tbl_t *cmdtp, int flag, int argc,
			  char * const argv[])
{
	int devnum = 0;
	int status;
	int ret;

	if (argc != 2)
		return CMD_RET_USAGE;

	devnum = simple_strtoul(argv[1], NULL, 10);
	ret = uclass_get_device(UCLASS_DEMO, devnum, &demo_dev);
	if (ret)
		return cmd_process_error(cmdtp, ret);

	ret = demo_status(demo_dev, &status);
	if (!ret)
		printf("Status: %d\n", status);

	return cmd_process_error(cmdtp, ret);
}

static int do_demo_light(cmd_tbl_t *cmdtp, int flag, int argc,
			 char * const argv[])
{
	int devnum = 0;
	int light;
	int ret;

	if (argc < 2)
		return CMD_RET_USAGE;

	devnum = simple_strtoul(argv[1], NULL, 10);
	ret = uclass_get_device(UCLASS_DEMO, devnum, &demo_dev);
	if (ret)
		return cmd_process_error(cmdtp, ret);

	if (argc > 2) {
		light = simple_strtoul(argv[2], NULL, 16);
		ret = demo_set_light(demo_dev, light);
	} else {
		ret = demo_get_light(demo_dev);
		if (ret >= 0) {
			printf("Light: %x\n", ret);
			ret = 0;
		}
	}

	return cmd_process_error(cmdtp, ret);
}

int do_demo_list(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct udevice *dev;
	int i, ret;

	puts("Demo uclass entries:\n");

	for (i = 0, ret = uclass_first_device(UCLASS_DEMO, &dev);
	     dev;
	     ret = uclass_next_device(&dev)) {
		printf("entry %d - instance %08x, ops %08x, platdata %08x\n",
		       i++, (uint)map_to_sysmem(dev),
		       (uint)map_to_sysmem(dev->driver->ops),
		       (uint)map_to_sysmem(dev_get_platdata(dev)));
	}

	return cmd_process_error(cmdtp, ret);
}

U_BOOT_CMD_WITH_SUBCMDS(demo,
	"Driver model (dm) demo operations",
	"list                     List available demo devices\n"
	"demo hello <num> [<char>]     Say hello\n"
	"demo light <num> [<val>]      Set or get the lights\n"
	"demo status <num>             Get demo device status\n",
	U_BOOT_SUBCMD_MKENT(list, 1, 1, do_demo_list),
	U_BOOT_SUBCMD_MKENT(hello, 3, 1, do_demo_hello),
	U_BOOT_SUBCMD_MKENT(light, 3, 1, do_demo_light),
	U_BOOT_SUBCMD_MKENT(status, 2, 1, do_demo_status));
