#include<linux/printk.h>
#include<linux/module.h>
#include<linux/init.h>

void multi_file_test(void);

static int __init say_hello(void)
{
    printk(KERN_INFO "Hello World!!");
    multi_file_test();
    return 0;
}
 
static void __exit say_bye(void)
{
    printk(KERN_INFO "Bye World");
}

module_init(say_hello);
module_exit(say_bye);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adithya");

