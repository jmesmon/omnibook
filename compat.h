/*
 * compat.h -- Older kernel (=> 2.6.8) support 
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
 * Written by Mathieu Bérard <mathieu.berard@crans.org>, 2006
 */

/*
 * For compatibility with kernel older than 2.6.11
 */

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,11))
typedef u32 pm_message_t;
#endif

static int __init omnibook_probe(struct platform_device *dev);
static int __exit omnibook_remove(struct platform_device *dev);
static int omnibook_suspend(struct platform_device *dev, pm_message_t state);
static int omnibook_resume(struct platform_device *dev);

/*
 * For compatibility with kernel older than 2.6.15
 */

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15))

#define to_platform_device(x) container_of((x), struct platform_device, dev)

static int __init compat_omnibook_probe(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	return omnibook_probe(pdev);
}

static int __exit compat_omnibook_remove(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	return omnibook_remove(pdev);
}

static int compat_omnibook_suspend(struct device *dev, pm_message_t state, u32 level)
{
	struct platform_device *pdev = to_platform_device(dev);
	return omnibook_suspend(pdev, state);
}

static int compat_omnibook_resume(struct device *dev, u32 level)
{
	struct platform_device *pdev = to_platform_device(dev);
	return omnibook_resume(pdev);
}

#endif


/*
 * For compatibility with kernel older than 2.6.14
 */

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,14))
void inline *kzalloc(size_t size, int flags)
{
	void *ret = kmalloc(size, flags);
	if (ret)
		memset(ret, 0, size);
	return ret;
}
#endif




/* End of file */
