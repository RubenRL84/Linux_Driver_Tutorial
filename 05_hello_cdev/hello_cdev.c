#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>


static int major;



ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);

static ssize_t my_read (struct file *f, char __user *u, size_t, loff_t *o)
{
    printk("hello cdev - Read is called\n");
    return 0;
}

static struct  file_operations fops = {
    .read = my_read,
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