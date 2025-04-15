#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>


static int major;




static int my_open(struct inode *inode, struct file *filp){

    pr_info("hello_cdev - Major: %d, Minor: %d\n", imajor(inode), iminor(inode));

    pr_info("hello_cdev - filp->f_pos: %lld\n", filp->f_pos);
    pr_info("hello_cdev - filp->f_mode: 0x%x\n", filp->f_mode);
    pr_info("hello_cdev - filp->f_flags: 0x%x\n", filp->f_flags);
    
 return 0;
}

static int my_release(struct inode *inode, struct file *filp){

    pr_info("hello_cdev - File is closed");
    return 0;
}
static struct  file_operations fops = {
    .open = my_open,
    .release = my_release,
};
/**
 * @brief Function that is called when driver is loaded to the kernel
 */
static int __init my_init(void)
{   
    major = register_chrdev(0,"hello_cdev", &fops);
    if (major < 0) {
        printk("hello_cdev - Error registering chrdev\n");
        return major;
    }
    printk("hello_cdev - Major device Number: %d\n", major);
    return 0;
}

/**
 * @brief Function that is called when driver is removed from the kernel
 */
static void __exit my_exit(void)
{   
    unregister_chrdev(major, "hello_cdev");
    printk("hello_cdev - Unregistering device.\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ruben Lacerda");
MODULE_DESCRIPTION("A sample for registering a character device");