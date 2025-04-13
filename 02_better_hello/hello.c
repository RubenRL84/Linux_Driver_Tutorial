#include <linux/module.h>
#include <linux/init.h>

/**
 * @brief Function that is called when driver is loaded to the kernel
 */
static int __init my_init(void)
{
    printk("hello - Hello, Kernel!\n");
    return 0;
}

/**
 * @brief Function that is called when driver is removed from the kernel
 */
static void __exit my_exit(void)
{
    printk("hello - Goodbye, Kernel!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ruben Lacerda");
MODULE_DESCRIPTION("A simple Hello World Linux Kernel Module");