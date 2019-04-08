#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>

// 对应用户层seek跳转函数
static loff_t meme_seek(struct file* filp, loff_t off, int n)
{
    printk("seek\n");
    return 0;
}

// 对应用户层read读取函数，通常用户接收设备内容，如串口收数据
static ssize_t meme_read(struct file* filp, char __user *buf, size_t size, loff_t* off)
{
    printk("read\n");
    return 0;
}

// 对应用户层write写入函数，通常用于写入设备内容，如串口发数据
static ssize_t meme_write(struct file* filp, const char __user *buf, size_t size, loff_t* off)
{
    printk("write\n");
    return 0;
}

// 对应用户层ioctl控制函数，通常用于一些指定下发，如led的开关控制
static long meme_ioctl(struct file* filp, unsigned int cmd, unsigned long arg)
{
    printk("ioctl\n");
    return 0;
}

// 对应用户层mmap映射函数，通常用于对设备的整块内存映射，共享内存等
static int meme_mmap(struct file* filp, struct vm_area_struct* area)
{
    printk("mmap\n");
    return 0;
}

// 对应用户层open打开函数，就是打开设备描述符
static int meme_open(struct inode* inode, struct file* filp)
{
    printk("open\n");
    return 0;
}

// 对应用户层close关闭函数，就是关闭设备文件
static int meme_close(struct inode* inode, struct file* filp)
{
    printk("close\n");
    return 0;
}

// 文件操作结构体，表示内核与用户层的函数操作对应关系
static const struct file_operations fops = {
    .owner = THIS_MODULE, // 这其实是个结构体，比如THIS_MODULE->name
    .llseek = meme_seek,
    .read = meme_read,
    .write = meme_write,
    .open = meme_open,
    .release = meme_close,
    .unlocked_ioctl = meme_ioctl,
    .mmap = meme_mmap,
};

// 字符设备结构体，该字符设备的核心数据表示
static struct cdev cdev;

// 驱动的主设备编号，每种类型的设备都有唯一编号
static dev_t devno = 0;

static int __init meme_init(void)
{
    // 1. 向内核申请分配一个主设备号，此设备号从0开始，分配1个，名称为meme
    if (alloc_chrdev_region(&devno, 0, 1, "meme") < 0 ) {
        printk(KERN_ERR"fail init\n");
        return (-1);
    }

    // 2. 初始化cdev结构体，并与fops文件操作之间建立联系
    cdev_init(&cdev, &fops);

    // 3. 正式向内核注册一个字符设备
    cdev_add(&cdev, devno, 1);

    printk(KERN_ALERT"meme init: %d:%d\n", MAJOR(devno), MINOR(devno));
    return 0;
}
module_init(meme_init);

static void __exit meme_exit(void)
{
    // 1. 向内核注销该字符设备
    cdev_del(&cdev);

    // 2. 向内核申请释放该设备号
    unregister_chrdev_region(devno, 1);

    printk("meme free\n");
}
module_exit(meme_exit);

MODULE_LICENSE("GPL v2");
MODULE_ALIAS("MyMemery");
MODULE_AUTHOR("Philon");
MODULE_DESCRIPTION("A character device driver");