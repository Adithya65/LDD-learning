#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define MAX_MEM_SIZE    512
#define MAX_DEV_COUNT   4

#define RD_ONLY         0x11

uint8_t max_buf_mem_dev0[MAX_MEM_SIZE];
uint8_t max_buf_mem_dev1[MAX_MEM_SIZE];
uint8_t max_buf_mem_dev2[MAX_MEM_SIZE];
uint8_t max_buf_mem_dev3[MAX_MEM_SIZE];

typedef struct 
{
    struct cdev char_dev;
    char* buf;
    int perm;
}dev_data_t;

typedef struct 
{
    dev_t dev_num;
    struct class* class_dev;
    struct device* device_dev;
    dev_data_t dev_data[MAX_DEV_COUNT];
}driver_data_t;

driver_data_t driver_data =
{
    .dev_data[0] =
    {
        .buf = max_buf_mem_dev0,
        .perm = RD_ONLY,
    },
    .dev_data[1] =
    {
        .buf = max_buf_mem_dev1,
        .perm = RD_ONLY,
    },
    .dev_data[2] =
    {
        .buf = max_buf_mem_dev2,
        .perm = RD_ONLY,
    },
    .dev_data[3] =
    {
        .buf = max_buf_mem_dev3,
        .perm = RD_ONLY,
    }
};

int dev_open(struct inode *inode, struct file *filp)
{
	int ret,minor_num;
	dev_data_t *pcdev_data;

	minor_num = MINOR(inode->i_rdev);
	pr_info("minor access = %d\n",minor_num);

	pcdev_data = container_of(inode->i_cdev,dev_data_t,char_dev);

	filp->private_data = pcdev_data;
		
#if 0
	ret = check_permission(pcdev_data->perm,filp->f_mode);

	(!ret)?pr_info("open was successful\n"):pr_info("open was unsuccessful\n");
#endif
    pr_info("device_opened");
	return ret;
}

int dev_release(struct inode *inode, struct file *flip)
{
    pr_info("release was successful\n");
    return 0;
}

ssize_t dev_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
	dev_data_t *pcdev_data = (dev_data_t*)filp->private_data;
	
	pr_info("Write requested for %zu bytes\n",count);
	pr_info("Current file position = %lld\n",*f_pos);

	
	if((*f_pos + count) > 512)
		count = 512 - *f_pos;

	if(!count){
		pr_err("No space left on the device \n");
		return -ENOMEM;
	}

	/*copy from user */
	if(copy_from_user(pcdev_data->buf+(*f_pos),buff,count)){
		return -EFAULT;
	}

	/*update the current file postion */
	*f_pos += count;

	pr_info("Number of bytes successfully written = %zu\n",count);
	pr_info("Updated file position = %lld\n",*f_pos);

    pr_info("device_released");
	return count;
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
    ret = alloc_chrdev_region(&driver_data.dev_num,0,4,"dev_num");
    if(ret < 0)
    {
        return -1;
    }
    driver_data.class_dev = class_create("device_class");
    for( int i = 0 ; i < MAX_DEV_COUNT;i++)
    {
        driver_data.device_dev = device_create(driver_data.class_dev,NULL,driver_data.dev_num+i,NULL,"devices-%d",i);
        cdev_init(&driver_data.dev_data[i].char_dev,&dev_fops);
        cdev_add(&driver_data.dev_data[i].char_dev,driver_data.dev_num+i,1);
    }

    return 0;
}


static void kern_driver_exit(void)
{
    for( int i = 0 ; i < MAX_DEV_COUNT;i++)
    {
        cdev_del(&driver_data.dev_data[i].char_dev);
        device_destroy(driver_data.class_dev,driver_data.dev_num+i);
    }
    class_destroy(driver_data.class_dev);
    unregister_chrdev_region(driver_data.dev_num,1);
}

module_init(kern_driver_init);
module_exit(kern_driver_exit);

MODULE_AUTHOR("Adithya");
MODULE_LICENSE("GPL");
