#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>


/* variable contains pin number of interrupt controller to which GPIO 17 is mapped to */
unsigned int irq_number;

/*
 * @brief Interrupet service routine is called, when interrupt is triggered
 */
static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
	pr_info("gpio_irq - Interrupt was triggered and ISR was called!\n");
	return IRQ_HANDLED;

}

/**
 * @brief Function that is called when driver is loaded to the kernel
 */
static int __init my_init(void)
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
	
	if(request_irq(irq_number,gpio_irq_handler, IRQF_TRIGGER_RISING, "my_gpio_irq", NULL) != 0){
	pr_err("Error!\nCan not request interrupt nr.: %d\n", irq_number);
	gpio_free(17);
	return -1;
	}
		
	
    	pr_info("Done!\n");
	pr_info("GPIO 17 is mapped to IRQ Nr.: %d\n", irq_number);
    	return 0;
}

/**
 * @brief Function that is called when driver is removed from the kernel
 */
static void __exit my_exit(void)
{
    pr_info("gpio_irq - Unloading module...\n");
    free_irq(irq_number,NULL);
    gpio_free(17);

}

module_init(my_init);
module_exit(my_exit);

/* Meta Information  */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ruben Lacerda");
MODULE_DESCRIPTION("A simple Kernel Module for a gpio interrupts");
