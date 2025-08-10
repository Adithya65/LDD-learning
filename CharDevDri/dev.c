#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>

dev_t dev_num;
struct cdev char_dev;
struct class* class_dev;
struct device* device_dev;

int dev_open(struct inode *inode, struct file *filp)
{
    pr_info("device_opened");
    return 0;
}

int dev_release(struct inode *inode, struct file *flip)
{
    pr_info("device_released");
    return 0;
}

ssize_t dev_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
    pr_info("write_called");
    return 0;
}

ssize_t dev_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
    pr_info("read_called");
    return 0;
}


struct file_operations dev_fops =
{
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write
};


static int kern_driver_init(void)
{
    int ret = 0;
    ret = alloc_chrdev_region(&dev_num,0,1,"dev_num");
    if(ret < 0)
    {
        return -1;
    }
    class_dev = class_create("device_class");
    device_dev = device_create(class_dev,NULL,dev_num,NULL,"devices");
    cdev_init(&char_dev,&dev_fops);
    cdev_add(&char_dev,dev_num,1);

    return 0;
}


static void kern_driver_exit(void)
{
    cdev_del(&char_dev);
    device_destroy(class_dev,dev_num);
    class_destroy(class_dev);
    unregister_chrdev_region(dev_num,1);
}

module_init(kern_driver_init);
module_exit(kern_driver_exit);

MODULE_AUTHOR("Adithya");
MODULE_LICENSE("GPL");
