/*
 * This is an example for misc device
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define MISC_IOC_MAGIC 'k'
#define MISC_IOCW   _IOW(MISC_IOC_MAGIC, 1, int)
#define MISC_IOCR   _IOR(MISC_IOC_MAGIC, 2, int)
#define MISC_IOC_MAXNR 10
static int misc_opened = 0;
static int misc_to_user = 0;
static int misc_from_user = 0;

static int misc_open(struct inode *inode, struct file *file)
{
    printk("%s", __func__);
    if(misc_opened)
        return -EBUSY;
    misc_opened = 1;
    return 0;
}

static int misc_release(struct inode *inode, struct file *file)
{
    printk("%s", __func__);
    misc_opened = 0;
    return 0;
}

static long misc_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    if(_IOC_TYPE(cmd) != MISC_IOC_MAGIC)
        return -ENOTTY;

    if(_IOC_NR(cmd) > MISC_IOC_MAXNR)
        return -ENOTTY;

    switch (cmd) {
        case MISC_IOCW:
            __get_user(misc_from_user, (int __user *)arg);
            printk("misc_example ioctl cmd: %d\n", misc_from_user);
            break;
        case MISC_IOCR:
            __put_user(misc_to_user, (int __user *)arg);
            misc_to_user ++;
            if(misc_to_user>255)
                misc_to_user = 0;
            printk("misc_example ioctl cmd :2 \n");
            break;
        default:
            printk("misc_example ioctl err cmd: %d", cmd);
            break;
    }
    return 0;
}

static const struct file_operations misc_ops = {
    .owner = THIS_MODULE,
    .open = misc_open,
    .release = misc_release,
    .unlocked_ioctl = misc_ioctl,

};

static struct miscdevice misc_dev = {
    .name = "misc_example",
    .fops = &misc_ops,
    .minor = MISC_DYNAMIC_MINOR
};

static int __init misc_init(void)
{
    printk("MISC DEVICE INIT!\n");
    return misc_register(&misc_dev);
}

static void __exit misc_exit(void)
{
    misc_deregister(&misc_dev);
}

module_init(misc_init);
module_exit(misc_exit);
MODULE_AUTHOR("Dalian China, pengzhang8601@hotmail.com");
MODULE_DESCRIPTION("MISC driver");
MODULE_LICENSE("GPL");
