#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/interrupt.h>

#include "platform.h"

struct cdev *char_dev;
unsigned long start, end;
unsigned int irq_num;
int major;

ssize_t platform_read(struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos)
{	
	return 0;
}

ssize_t platform_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos)
{
	return 0;
}

int platform_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{	
	char *temp = NULL;
	int  count=8;
	
        if(_IOC_TYPE(cmd) != MAGIC_NUMBER ) return -ENOTTY;	
 	switch (cmd)
	{
		case READ_IOCTL:
				 temp = kmalloc(count, GFP_KERNEL);
				 copy_from_user(temp, (void  __user *)arg, count);
				 break;
		case WRITE_IOCTL:
				 break;

		default: printk(KERN_ALERT "invalid ioctl command %d\n",cmd);
     	}
	return 0;
} 

int platform_release(struct inode *inode, struct file *filep)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);
	return 0;
}

irqreturn_t short_probing(int irq, void *dev_id, struct pt_regs *regs)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);
        return IRQ_HANDLED;
}


struct file_operations fops = {
		.owner = THIS_MODULE , 
		.read = platform_read ,
		.write = platform_write ,
		.unlocked_ioctl = platform_ioctl,
		.release = platform_release,
};

int probe_function(struct platform_device *my_platform_device)
{
	struct resource *mem_res, *irq_res;
	int rc;
        dev_t dev_num;

	// Reading device io-memory addresses
	mem_res = platform_get_resource_byname(my_platform_device, IORESOURCE_MEM, "io-memory");
	if (mem_res == NULL)
		printk("Requested resource not found\n");	
	
        start = mem_res->start;
	end   = mem_res->end;

	// Reading device irq number
	irq_res = platform_get_resource_byname(my_platform_device, IORESOURCE_IRQ, "irq");
	if (irq_res == NULL)
		printk("Requested resource not found\n");

	irq_num = irq_res->start;

	rc = request_irq(irq_num, short_probing, IRQF_SHARED, "platform", NULL);
	
	if(!rc)
		printk(KERN_ALERT "requesting irq passed\n");
	else
		printk(KERN_ALERT "Unable to allocate requested interrupt line\n");


	// Creating Character device node
        rc = alloc_chrdev_region(&dev_num ,0 ,1 ,"dev_node");

	if ( rc < 0)
		printk("Unable to register character device\n");
	else 
	{
		major = MAJOR(dev_num);
		printk("create char device using MAJOR dev_num %d\n", major);
        }

 
       char_dev = cdev_alloc();
       char_dev->ops = &fops;

       cdev_add (char_dev ,dev_num , 1); 
       return 0;
}

int remove_function(struct platform_device *my_platform_device)
{
	/* Unregistering char dev */
	unregister_chrdev(major, "dev_node");
	return 0;
}
	

static struct platform_driver my_platform_driver = {
	.probe = probe_function,
	.remove = remove_function,
	.driver = {
			.name = DRIVER_NAME,
		  },
};

static int __init init_proc( void )
{	
	/* Registering platform driver */
	platform_driver_register (&my_platform_driver);
	return 0;

}
static void __exit  exit_proc( void )
{
	/* Unregistering platform driver */
	platform_driver_unregister (&my_platform_driver);
}

MODULE_LICENSE("GPL");

module_init(init_proc);
module_exit(exit_proc);
