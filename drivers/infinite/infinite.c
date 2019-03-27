#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

static int major = 254;   // 主设备号
dev_t devno = 0;

struct cdev infinite_cdev; // 字符设备结构体
void* space = NULL; // 内存空间，稍后分配

// 打开设备描述符，用户通过open()触发
static int infinite_open(struct inode* inode, struct file* filp)
{
    printk(KERN_ALERT"INFINITE_MOD: open\n");
    filp->private_data = NULL;
    return 0;
}

// 关闭设备描述符，用户通过close()触发
static int infinite_close(struct inode* inode, struct file* filp)
{
    printk(KERN_ALERT"INFINITE_MOD: release\n");
    return 0;
}

// 读操作，用户通过read()触发该函数
static ssize_t infinite_read(struct file* filp, char __user *buf, size_t size, loff_t* ppos)
{
    printk(KERN_ALERT"INFINITE_MOD: read\n");
    int rc = 0;
    return rc;
}

// 写操作，用户通过write()触发该函数
static ssize_t infinite_write(struct file* filp, const char __user *buf, size_t size, loff_t* ppos)
{
    printk(KERN_ALERT"INFINITE_MOD: write\n");
    return 0;
}

// 寻址操作，用户通过seek()触发该函数
static loff_t infinite_llseek(struct file* filp, loff_t offset, int orig)
{
    printk(KERN_ALERT"INFINITE_MOD: seek\n");
    return 0;
}

// 控制操作，用户通过ioctl()触发该函数
static long infinite_ioctl(struct file* filp, unsigned int cmd, unsigned long arg)
{
    printk(KERN_ALERT"INFINITE_MOD: ioctl\n");
    return 0;
}

// 各种用户态下的文件api触发句柄集合
struct file_operations infinite_fops = {
    .owner = THIS_MODULE,
    .open = infinite_open,
    .release = infinite_close,
    .read = infinite_read,
    .write = infinite_write,
    .llseek = infinite_llseek,
    .unlocked_ioctl = infinite_ioctl,
};


// 模块加载函数，注册设备号——初始化字符设备——指向文件操作句柄
static int __init infinite_init(void)
{
    int rc = 0;
    printk(KERN_ALERT"INFINITE_MODULE: init\n");
    devno = MKDEV(major, 0);
    rc = register_chrdev_region(devno, 1, "infinite");
    if (rc < 0) {
        goto fail_init;
    }

    cdev_init(&infinite_cdev, &infinite_fops);
    rc = cdev_add(&infinite_cdev, devno, 1);
    if (rc < 0) {
        goto fail_init;
    }

    return 0;

fail_init:
    printk(KERN_ERR"INIFITE_MOD: Failed to init module\n");
    unregister_chrdev_region(devno, 1);
    return rc;
}
module_init(infinite_init);

// 模块卸载函数，删除字符设备——注销设备号
static void __exit infinite_exit(void)
{
    cdev_del(&infinite_cdev);
    unregister_chrdev_region(devno, 1);
    printk(KERN_ALERT"INFINITE_MOD: exit\n");
}
module_exit(infinite_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("a character device for memory management");
MODULE_AUTHOR("Philon");