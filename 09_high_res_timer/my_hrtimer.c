#include <linux/module.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>


/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ruben Lacerda");
MODULE_DESCRIPTION("A simple LKM using High Resolution Timers");

/* hr timer*/
static struct  hrtimer my_hrtimer;
u64 start_t;

static enum hrtimer_restart test_hrtimer_handler(struct  hrtimer *timer) 
{
    /* get current time */
    u64 now_t = jiffies;
    pr_info("hr_timer - (start_t - now_t) = %u\n", jiffies_to_msecs(now_t - start_t));
    return HRTIMER_NORESTART;
};




/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init my_init(void)
{
    pr_info("hr_timer - Hello, Kernel!\n");

    /* Init if hrtimer*/
    hrtimer_init(&my_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    my_hrtimer.function = &test_hrtimer_handler;
    start_t = jiffies;
    hrtimer_start(&my_hrtimer, ms_to_ktime(100), HRTIMER_MODE_REL);
    return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit my_exit(void)
{   
    hrtimer_cancel(&my_hrtimer);
    pr_info("hr_timer - Goodbye, Kernel!\n");
}


module_init(my_init);
module_exit(my_exit);

