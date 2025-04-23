#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>

#include "ioctl_test.h"
/* Meta information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ruben Lacerda");
MODULE_DESCRIPTION("A simple example for ioctl in a LKM");

static int major;

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *inode, struct file *filp){

    pr_info("ioctl - Major: %d, Minor: %d\n", imajor(inode), iminor(inode));

    pr_info("ioctl - filp->f_pos: %lld\n", filp->f_pos);
    pr_info("ioctl - filp->f_mode: 0x%x\n", filp->f_mode);
    pr_info("ioctl - filp->f_flags: 0x%x\n", filp->f_flags);
    
 return 0;
}

/**
 * @brief This function is called, when the device file is closed
 */
static int driver_close(struct inode *inode, struct file *filp){

    pr_info("hello_cdev - File is closed");
    return 0;
}

/* Global variable for reading and writing*/
int32_t answer = 42;

static long int my_ioctl(struct file *file, unsigned cmd, unsigned long arg){
    struct mystruct test;
    switch (cmd)
    {
    case WR_VALUE:
        if(copy_from_user(&answer, (int32_t *) arg, sizeof(answer)))
            printk("ioctl - Error copying data from user!\n");
        else
            printk("ioctl - Update the answer to %d\n", answer);
        break;
    case RD_VALUE:
        if(copy_to_user( (int32_t *) arg,&answer, sizeof(answer)))
            printk("ioctl - Error copying data to user!\n");
        else
            printk("ioctl - The answer was copied\n");
        break;
    case GREETER:
        if(copy_from_user(&test, (struct mystruct *) arg, sizeof(test)))
            printk("ioctl - Error copying data from user!\n");
        else
            printk("ioctl -  %s greets to %s\n", test.repeat, test.name);
        break; 
    }
    return 0;
}

static struct  file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .unlocked_ioctl = my_ioctl,
};
/**
 * @brief Function that is called when driver is loaded to the kernel
 */
static int __init ModuleInit(void)
{   
    major = register_chrdev(0,"ioctl", &fops);
    if (major < 0) {
        printk("ioctl - Error registering chrdev\n");
        return major;
    }
    printk("ioctl - Major device Number: %d\n", major);
    return 0;
}

/**
 * @brief Function that is called when driver is removed from the kernel
 */
static void __exit ModuleExit(void)
{   
    unregister_chrdev(major, "ioctl");
    printk("ioctl - Unregistering device.\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

