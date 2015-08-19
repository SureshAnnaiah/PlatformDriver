#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include "platform.h"

#define BASE_ADDR 0x00000
#define END_ADDR  0x000ff

/* Defining device resource like register, irq */
static struct resource device_resource[] = {
	{
		.start = BASE_ADDR,
		.end  =  END_ADDR,
		.name = "io-memory",
		.flags = IORESOURCE_MEM,
	},
	{
		.start = 5,
		.end = 5,
		.flags = IORESOURCE_IRQ,
		.name = "irq",
	}
	};



static struct platform_device my_platform_device = {
	.name = DRIVER_NAME,
	.id   = 4,
	.resource  = &device_resource ,
	.num_resources = 2,
};

static int __init  init_proc( void )
{
	/* Registering platform device */	
	platform_device_register (&my_platform_device);
	return 0;

}
static void  __exit exit_proc( void )
{
	/* Unregistering platform device */
	platform_device_unregister (&my_platform_device);
}

MODULE_LICENSE("GPL");

module_init(init_proc);
module_exit(exit_proc);
