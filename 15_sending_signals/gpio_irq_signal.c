#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/sched/signal.h>
#include <linux/ioctl.h>

#define MYMAJOR 64
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ruben Lacerda");
MODULE_DESCRIPTION("A simple Kernel Module which sends to an userspace app whe GPIO 17 has a rising edge");

/* variable contains pin number of interrupt controller to which GPIO 17 is mapped to */
unsigned int irq_number;


/* Global variables and defines for userspace app registracion */
#define REGISTER_UAPP _IO('R','g')
static struct task_struct *task = NULL;

/* define for Signal sending */
#define SIGNR 44


/*
 * @brief Interrupet service routine is called, when interrupt is triggered
 */
static irqreturn_t gpio_irq_signal_handler(int irq, void *dev_id)
{
	struct siginfo info;
	pr_info("gpio_irq_signal - Interrupt was triggered and ISR was called!\n");
	
	if(task != NULL) {
		memset(&info, 0, sizeof(info));
		info.si_signo = SIGNR;
		info.si_code = SI_QUEUE;

		/* Send the signal */
		if(send_sig_info(SIGNR, (struct kernel_siginfo *) &info, task) < 0)
			printk("gpio_irq_signal - Error sending signal!\n");
	}
	return IRQ_HANDLED;

}
/*
 * @brief This function is called, when the device file is closed
 */
static int my_close(struct inode *device_file, struct file *instance) {
	if(task != NULL)
		task = NULL;
	return 0;
}


/*
 * @brief IOCTL for registering the Userspace app to the kernel module
 */ 
static long int my_ioctl(struct file *file, unsigned cmd, unsigned long arg) {

	if(cmd == REGISTER_UAPP) {
		task = get_current();
		printk("gpio_irq_signal - Userpace app with PID %d is registered\n",task->pid);
	}
	return 0;
}

static struct file_operations fops = {
	.owner= THIS_MODULE,
	.release = my_close,
	.unlocked_ioctl = my_ioctl,
};

/**
 * @brief Function that is called when driver is loaded to the kernel
 */
static int __init ModuleInit(void)
{
    	pr_info("gpio_irq - Loading Module...\n");

    	/* Setup the gpio */
	if(gpio_request(17, "rpi-gpio-17")){
	pr_err("Error!\nCan not allocate GPIO 17 to input!\n");
	return -1;
	};
	
	/* Set gpio direction */
	if(gpio_direction_input(17)){
		pr_err("Error!\nCan not set GPIO 17 o input!\n");
		gpio_free(17);
		return -1;
	}

    	/* Setup the interrupt */
	irq_number = gpio_to_irq(17);
	
	if(request_irq(irq_number,gpio_irq_signal_handler, IRQF_TRIGGER_RISING, "my_gpio_irq", NULL) != 0){
	pr_err("Error!\nCan not request interrupt nr.: %d\n", irq_number);
	gpio_free(17);
	return -1;
	}

	if(register_chrdev(MYMAJOR, "gpio_irq_signal", &fops) <0){
		printk("Error!\n Can´t register device Number!\n");
		gpio_free(17);
		free_irq(irq_number, NULL);
	}	
	
    	pr_info("Done!\n");
	pr_info("GPIO 17 is mapped to IRQ Nr.: %d\n", irq_number);
    	return 0;
}

/**
 * @brief Function that is called when driver is removed from the kernel
 */
static void __exit ModuleExit(void)
{
    pr_info("gpio_irq - Unloading module...\n");
    free_irq(irq_number,NULL);
    gpio_free(17);
    unregister_chrdev(MYMAJOR, "gpio_irq_signal");

}

module_init(ModuleInit);
module_exit(ModuleExit);





