#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/ioctl.h>

#include <linux/poll.h>
#include <linux/wait.h>

#define MYMAJOR 64

/* Meta information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ruben Lacerda");
MODULE_DESCRIPTION("A simple Kernel Module which sends to an userspace app whe GPIO 17 has a rising edge");

/* variable contains pin number of interrupt controller to which GPIO 17 is mapped to */
unsigned int irq_number;
static int irq_ready = 0;
static wait_queue_head_t waitqueue;

/*
 * @brief Interrupet service routine is called, when interrupt is triggered
 */
static irqreturn_t gpio_irq_poll_handler(int irq, void *dev_id)
{	printk("gpio_irq_poll - Interrupt was triggered and ISR was called!\n");
	irq_ready = 1;
	wake_up(&waitqueue);
	return  IRQ_HANDLED;

}

/*
 * @brief Poll callback to allow userspace app to poll for button being pressed
 */
static unsigned int my_poll(struct file *file, poll_table *wait) {
	poll_wait(file, &waitqueue, wait);
	if(irq_ready == 1) {
		irq_ready = 0;
		return POLLIN;
	}
	return 0;
}


static struct file_operations fops = {
	.owner = THIS_MODULE,
	.poll = my_poll,
};


/**
 * @brief Function that is called when driver is loaded to the kernel
 */
static int __init ModuleInit(void)
{	
    	pr_info("gpio_irq - Loading Module...\n");

	/* Init waitqueue */
	init_waitqueue_head(&waitqueue);

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
	
	if(request_irq(irq_number,gpio_irq_poll_handler, IRQF_TRIGGER_RISING, "my_gpio_irq", NULL) != 0){
	pr_err("Error!\nCan not request interrupt nr.: %d\n", irq_number);
	gpio_free(17);
	return -1;
	}

	if(register_chrdev(MYMAJOR, "gpio_irq_poll", &fops) <0){
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





