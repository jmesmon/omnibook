/*
 * wireless.c Wifi feature
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * Written by Mathieu Bérard <mathieu.berard@crans.org>, 2006
 *
 */

#include "omnibook.h"
#include "ec.h"

static int omnibook_wifi_read(char *buffer,struct omnibook_operation *io_op)
{
	int len = 0;
	int retval;
	unsigned int state;
	
	if((retval = io_op->backend->aerial_get(io_op,&state)))
		return retval;
	
	len += sprintf(buffer + len,"Wifi adapter is %s", (state & WIFI_EX) ? "present" : "absent");
	if (state & WIFI_EX)
		len += sprintf(buffer + len," and %s", (state & WIFI_STA) ? "enabled" : "disabled");
	len += sprintf(buffer + len,".\n");
	len += sprintf(buffer + len,"Wifi Kill switch is %s.\n", (state & KILLSWITCH) ? "on" : "off");
	
	return len;
	
}

static int omnibook_wifi_write(char *buffer,struct omnibook_operation *io_op)
{
	int retval = 0;	
	unsigned int state;
	
	if((retval = io_op->backend->aerial_get(io_op,&state)))
		return retval;
	
	if(*buffer == '0' )
		state &= ~WIFI_STA;
	else if (*buffer == '1' )
		state |= WIFI_STA;
	else
		return -EINVAL;
	
	if((retval = io_op->backend->aerial_set(io_op, state)))
		return retval;

	return retval;	
}

static struct omnibook_feature wifi_feature;

static int __init omnibook_wifi_init(struct omnibook_operation *io_op)
{
	int retval = 0;
	unsigned int state;
	
/*
 *  Refuse enabling/disabling a non-existent device
 */
	
	if((retval = io_op->backend->aerial_get(io_op, &state)))
		return retval;
	
	if(!(state & WIFI_EX))
		wifi_feature.write = NULL;	
	
	return retval;
}

/*
 * Shared with bluetooth.c
 */
struct omnibook_tbl wireless_table[] __initdata = {
	{ TSM30X, {ACPI, 0, 0, 0, 0, 0}}, /* stubs to select backend */
	{ TSM40,  {SMI, 0, 0, 0, 0, 0}}, /* stubs to select backend */
	{ 0,}
};

static struct omnibook_feature __declared_feature wifi_driver = {
	.name = "wifi",
	.enabled = 1,
	.read = omnibook_wifi_read,
	.write = omnibook_wifi_write,
	.init = omnibook_wifi_init,
	.ectypes = TSM30X|TSM40,
	.tbl = wireless_table,
};

module_param_named(wifi, wifi_driver.enabled, int, S_IRUGO);
MODULE_PARM_DESC(wifi, "Use 0 to disable, 1 to enable Wifi adapter control");
