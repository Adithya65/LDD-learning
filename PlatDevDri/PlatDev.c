#include <linux/module.h>
#include <linux/platform_device.h>

static void my_release(struct device *dev )
{
    pr_info("platform device released\n");
}

static struct platform_device my_pdev = {
    .name = "my_plat_dev",
    .id   = -1,
    .dev = {
        .release = my_release,
    },
};

static int __init my_pdev_init(void)
{
    return platform_device_register(&my_pdev);
}

static void __exit my_pdev_exit(void)
{
    platform_device_unregister(&my_pdev);
}

module_init(my_pdev_init);
module_exit(my_pdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adithya");
MODULE_DESCRIPTION("Minimal Platform Device");

