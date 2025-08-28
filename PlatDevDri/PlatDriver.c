// SPDX-License-Identifier: MIT
#include <linux/module.h>
#include <linux/platform_device.h>

static int my_probe(struct platform_device *pdev)
{
    pr_info("my_probe called for %s\n", pdev->name);
    return 0;
}

static void  my_remove(struct platform_device *pdev)
{
    pr_info("my_remove called for %s\n", pdev->name);
}

static struct platform_driver my_driver = {
    .probe  = my_probe,
    .remove = my_remove,
    .driver = {
        .name = "my_plat_dev", // must match platform_device name
        .owner = THIS_MODULE,
    },
};


static int __init my_driver_init(void)
{
    return platform_driver_register(&my_driver);
}

static void __exit my_driver_exit(void)
{
    platform_driver_unregister(&my_driver);
}

module_init(my_driver_init);
module_exit(my_driver_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adithya");
MODULE_DESCRIPTION("Minimal Platform Driver");

