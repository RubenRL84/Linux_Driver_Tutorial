#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>


static int major;
static char text[64];


ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);

static ssize_t my_read (struct file *filp, char __user *user_buff, size_t len, loff_t *off)
{	
	int not_copied, delta, to_copy = (len + *off) < sizeof(text) ? len : (sizeof(text) - *off);

	pr_info("hello cdev - Read is called,we want to read %ld bytes, but actually only copyuing %d bytes. The offset is %lld\n", len, to_copy, *off);

	if (*off <= sizeof(text))
		return 0;

	not_copied = copy_to_user(user_buff, &text[*off], to_copy);
	delta = to_copy - not_copied;
	if (not_copied)
		pr_warn("hello_cdev - Could only copy %d bytes\n", delta);

	*off += delta;
	return delta;
}

static ssize_t my_write(struct file *filp, const char __user *user_buff, size_t len, loff_t *off)
{	
	int not_copied, delta, to_copy = (len + *off) < sizeof(text) ? len : (sizeof(text) - *off);

	pr_info("hello cdev - Write is called,we want to write %ld bytes, but actually only write %d bytes. The offset is %lld\n", to_copy, len, *off);

	if (*off <= sizeof(text))
		return 0;

	not_copied = copy_from_user(&text[*off], user_buff, to_copy);
	delta = to_copy - not_copied;
	if (not_copied)
		pr_warn("hello_cdev - Could only copy %d bytes\n", delta);

	*off += delta;
	return delta;
}



static struct  file_operations fops = {
    .read = my_read,
    .write = my_write,
};
/**
 * @brief Function that is called when driver is loaded to the kernel
 */
static int __init my_init(void)
{   
    major = register_chrdev(0,"hello_cdev", &fops);
    if (major < 0) {
        pr_info("hello_cdev - Error registering chrdev\n");
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
    pr_info("hello_cdev - Unregistering device.\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ruben Lacerda");
MODULE_DESCRIPTION("A sample for registering a character device");
