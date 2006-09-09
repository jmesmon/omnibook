/*
 * omnibook.h -- High level data structures and functions of omnibook
 *               support code
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * Written by Soós Péter <sp@osb.hu>, 2002-2004
 * Modified by Mathieu Bérard <mathieu.berard@crans.org>, 2006
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/version.h>

/*
 * Module informations
 */

#define OMNIBOOK_MODULE_NAME		"omnibook"
#define OMNIBOOK_MODULE_VERSION		"2.20060000"

/*
 * EC types
 */

extern enum omnibook_ectype_t {
	NONE   = 0,	  /* 0  Default/unknown EC type */ 
	XE3GF  = (1<<0),  /* 1  HP OmniBook XE3 GF, most old Toshiba Satellites */
	XE3GC  = (1<<1),  /* 2  HP OmniBook XE3 GC, GD, GE and compatible */
	OB500  = (1<<2),  /* 3  HP OmniBook 500 and compatible */
	OB510  = (1<<3),  /* 4  HP OmniBook 510 */
	OB6000 = (1<<4),  /* 5  HP OmniBook 6000 */
	OB6100 = (1<<5),  /* 6  HP OmniBook 6100 */
	XE4500 = (1<<6),  /* 7  HP OmniBook xe4500 and compatible */
	OB4150 = (1<<7),  /* 8  HP OmniBook 4150 */
	XE2    = (1<<8),  /* 9  HP OmniBook XE2 */
	AMILOD = (1<<9),  /* 10 Fujitsu Amilo D */
	TSP10  = (1<<10), /* 11 Toshiba Satellite P10, P15, P20 and compatible */
	TSM30X = (1<<11), /* 12 Toshiba Satellite M30X, M35X, M40X, M70 and compatible */
	TSM40  = (1<<12), /* 13 Toshiba Satellite M40 */
	TSA105 = (1<<13)  /* 14 Toshiba Satellite A105 */
} omnibook_ectype;

/*
 * This represent a feature provided by this module
 */

struct omnibook_operation;

struct omnibook_feature {
	char *name;		/* Name */
	char *proc_entry; 	/* Specify proc entry relative to /proc (will be omnibook/name otherwise) */
	int enabled;		/* Set from module parameter */
	int (*read) (char *,struct omnibook_operation *);	/* Procfile read function */
	int (*write) (char *,struct omnibook_operation *);/* Procfile write function */
	int (*init) (struct omnibook_operation *);	/* Specific Initialization function */
	void (*exit) (struct omnibook_operation *);	/* Specific Cleanup function */
	int (*suspend) (struct omnibook_operation *);	/* PM Suspend function */
	int (*resume) (struct omnibook_operation *);	/* PM Resume function */
	int ectypes;		/* Type(s) of EC we support for this feature (bitmask) */
	struct omnibook_tbl *tbl;
	struct omnibook_operation *io_op;
	struct list_head list;
};

struct omnibook_battery_info {
	u8 type;		/* 1 - Li-Ion, 2 NiMH */
	u16 sn;			/* Serial number */
	u16 dv;			/* Design Voltage */
	u16 dc;			/* Design Capacity */
};
struct omnibook_battery_state {
	u16 pv;			/* Present Voltage */
	u16 rc;			/* Remaining Capacity */
	u16 lc;			/* Last Full Capacity */
	u8 gauge;		/* Gauge in % */
	u8 status;		/* 0 - unknown, 1 - charged, 2 - discharging, 3 - charging, 4 - critical) */
};

enum {
	OMNIBOOK_BATTSTAT_UNKNOWN,
	OMNIBOOK_BATTSTAT_CHARGED,
	OMNIBOOK_BATTSTAT_DISCHARGING,
	OMNIBOOK_BATTSTAT_CHARGING,
	OMNIBOOK_BATTSTAT_CRITICAL
};


/*
 * State of a Wifi/Bluetooth adapter
 */
enum {
	WIFI_EX = (1<<0),	/* 1 1=present 0=absent */
	WIFI_STA = (1<<1),	/* 2 1=enabled 0=disabled */
	KILLSWITCH = (1<<2),	/* 4 1=radio on 0=radio off */
	BT_EX = (1<<3),		/* 8 1=present 0=absent */
	BT_STA = (1<<4),	/* 16 1=enabled 0=disabled */
};

/*
 * Hotkeys state backend neutral masks
 */
enum {
	HKEY_ONETOUCH = (1<<0),		/* 1  Ontetouch button scancode generation */
	HKEY_MULTIMEDIA = (1<<1),	/* 2  "Multimedia hotkeys" scancode generation */	
	HKEY_FN = (1<<2),		/* 4  Fn + foo hotkeys scancode generation */
	HKEY_STICK = (1<<3),		/* 8  Stick key (no clue what this is about) */
	HKEY_TWICE_LOCK = (1<<4),	/* 16 Press Fn twice to lock */
	HKEY_DOCK = (1<<5),		/* 32 (Un)Dock events scancode generation */
	HKEY_FNF5 = (1<<6),		/* 64 Fn + F5 (toggle display) is enabled */
};


/*
 * Display state backend neutral masks
 * _ON masks = port is powered up and running
 * _DET masks = a display have been detected to be plugged in the port 
 */

enum {	
	DISPLAY_LCD_ON = (1<<0),	/* 1 Internal LCD panel */
	DISPLAY_CRT_ON = (1<<1),	/* 2 External VGA port */
	DISPLAY_TVO_ON = (1<<2),	/* 4 External TV-OUT port */
	DISPLAY_DVI_ON = (1<<3),	/* 8 External DVI port */
	DISPLAY_LCD_DET = (1<<4),	/* 16 Internal LCD panel */
	DISPLAY_CRT_DET = (1<<5),	/* 32 External VGA port */
	DISPLAY_TVO_DET = (1<<6),	/* 64 External TV-OUT port */
	DISPLAY_DVI_DET = (1<<7),	/* 128 External DVI port */
};



int omnibook_lcd_blank(int blank);
int omnibook_get_ac(struct omnibook_operation *io_op);
int omnibook_get_battery_status(int num, struct omnibook_battery_state *battstat);
int set_omnibook_param(const char *val, struct kernel_param *kp);


#define __declared_feature __attribute__ (( __section__(".features"),  __aligned__(__alignof__ (struct omnibook_feature)))) __attribute_used__

/*
 * yet another printk wrapper
 */
#define O_INFO	KERN_INFO OMNIBOOK_MODULE_NAME ": "
#define O_WARN	KERN_WARNING OMNIBOOK_MODULE_NAME ": "
#define O_ERR	KERN_ERR OMNIBOOK_MODULE_NAME ": "

#ifdef OMNIBOOK_DEBUG
#define dprintk(fmt, args...) printk(KERN_INFO "%s: " fmt, OMNIBOOK_MODULE_NAME, ## args)
#define dprintk_simple(fmt, args...) printk(fmt, ## args)
#else
#define dprintk(fmt, args...)	do { } while(0)
#define dprintk_simple(fmt, args...) do { } while(0)
#endif

/* 
 * Configuration for standalone compilation: 
 * -Register as backlight depends on kernel config (requires 2.6.17+ interface)
 * -APM emulation is disabled by default
 */

#ifdef  OMNIBOOK_STANDALONE
#if     (defined (CONFIG_BACKLIGHT_CLASS_DEVICE_MODULE) || defined(CONFIG_BACKLIGHT_CLASS_DEVICE)) && (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,16))
#define CONFIG_OMNIBOOK_BACKLIGHT
#else
#undef  CONFIG_OMNIBOOK_BACKLIGHT
#endif /* BACKLIGHT_CLASS_DEVICE */
#undef	CONFIG_OMNIBOOK_LEGACY
#endif /* OMNIBOOK_STANDALONE */

/* End of file */
