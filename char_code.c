#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

#define memorysize 1024 


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bhanu");
MODULE_DESCRIPTION("character driver (dynamically)");


dev_t dev = 0;

static struct class *dev_class;
static struct cdev my_cdev;
uint8_t *ker_bufer;


static int __init char_code_init(void);
static void __exit char_code_exit(void);
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode,struct file *file);
static ssize_t my_read(struct file *file,char __user *buf,size_t len,loff_t *off);
static ssize_t my_write(struct file *filp,const char *buf,size_t len,loff_t *off);

static struct file_operations fops=
{
	.owner	= THIS_MODULE,
	.read	= my_read,
	.write	= my_write,
	.open	= my_open,
	.release	= my_release,
};


static int my_open(struct inode *inode , struct file *file)
{
	if((ker_bufer = kmalloc(1024,GFP_KERNEL)) == 0)
	{
		printk(KERN_INFO "cannot allocate the memory to the kernel..\n");
		return -1;
	}
	printk(KERN_INFO "device file opened..\n");
	return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
	kfree(ker_bufer);
	printk(KERN_INFO "Device file closed..\n");
	return 0;
}

static ssize_t my_read(struct file *filp,char __user *buf,size_t len,loff_t *off)
{
	copy_to_user(buf,ker_bufer,1024);
	printk(KERN_INFO "Data read Done ..\n");
	return 1024;
}

static ssize_t my_write(struct file *filp,const char __user *buf,size_t len, loff_t *off)
{
	copy_from_user(ker_bufer,buf,len);
	printk(KERN_INFO "Data is wrtten successfully..\n");
	return len;
}



static int __init char_code_init(void)
{
	/* Allocating the major number */
	alloc_chrdev_region(&dev,0,1,"my_Dev");
	
	printk(KERN_INFO"Major = %d Minor = %d..\n",MAJOR(dev),MINOR(dev));
	
	/*creating cdev structure */

	cdev_init(&my_cdev,&fops);
	
	/* Adding character device to the system */

	cdev_add(&my_cdev,dev,1);
	

	/* creating struct class */
	if((dev_class = class_create(THIS_MODULE,"my_class")) == NULL) 
	{
		printk(KERN_INFO "cannot create the struct class..\n");
		unregister_chrdev_region(dev,1);
	}

	/* creating device */

	if((device_create(dev_class,NULL,dev,NULL,"my_device")) == NULL) 
	{
		printk(KERN_INFO "cannot create the device..\n");
		class_destroy(dev_class);
	}

	printk(KERN_INFO" Device Deriver is insert // done properly \n");
	return 0;


/*r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);*/
}

void __exit char_code_exit(void)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO "Device driver is remoued successfully ..\n");
}

module_init(char_code_init);
module_exit(char_code_exit);

