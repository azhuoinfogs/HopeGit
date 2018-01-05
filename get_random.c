#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#define NUM 10

void get_random_bytes(void *buf, int nbytes);



static int get_random_number(void)
{
	unsigned long random[10];
	int i=0;
	printk("get random number\n");
	for(i=0; i< NUM; i++)
	{
		get_random_bytes(&random[i],sizeof(unsigned long));
		printk("random number=%ld\n",random[i]);
	}
	return 0;
}

static void random_exit(void)
{
	printk(KERN_ALERT"**************random exit******************** \n");
}


module_init(get_random_number);
module_exit(random_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GS");


