#include <linux/printk.h>

void multi_file_test(void)
{
    printk(KERN_INFO "calling from diff file");
}


