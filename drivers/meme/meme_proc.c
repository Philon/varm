#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/seq_file.h>

#include "meme.h"

// 全局变量，meme的设备节点状态
int meme_cdev_state = 0;

static ssize_t meme_proc_read(struct file* filp, char __user* buf, size_t len, loff_t* off)
{
    int rc = 0;
    const char* state = NULL;

    if (*off > 0) {
        return 0;
    }

    switch (meme_cdev_state) {
        case MEME_STATE_OPENED: state = "opened"; break;
        case MEME_STATE_CLOSED: state = "closed"; break;
        case MEME_STATE_READING: state = "reading"; break;
        case MEME_STATE_WRITING: state = "writing"; break;
        default: state = "unknown"; break;
    }

    len = strlen(state);
    if ((rc = copy_to_user(buf, state, len)) < 0) {
        return rc;
    }
    buf[len++] = '\n';
    buf[len++] = '\0';
    return len;
}

const struct file_operations proc_fops = {
    .owner = THIS_MODULE,
    .read = meme_proc_read,
};

// procfs入口目录
struct proc_dir_entry* meme_proc_entry = NULL;

int __init meme_proc_init(void)
{
    // 即/proc/meme/state，用于记录设备描述符/dev/meme的文件访问状态
    struct proc_dir_entry* meme_state_file = NULL;

    // 创建meme模块的proc目录，即/proc/meme/
    meme_proc_entry = proc_mkdir("meme", NULL);
    if (meme_proc_entry == NULL) {
        printk(KERN_ERR"failed to create '/proc/meme/'");
        return -EINVAL;
    }

    // 创建/proc/meme/state文件，且该文件为只读
    meme_state_file = proc_create("state", 0500, meme_proc_entry, &proc_fops);
    if (meme_state_file == NULL) {
        printk(KERN_ERR"failed to create '/proc/meme/state'\n");
        return -EINVAL;
    }

    printk(KERN_DEBUG"meme proc init\n");

    return 0;
}

void __exit meme_proc_exit(void)
{
    // 卸载模块时，自动删除所有/proc/meme所创建的文件及目录
    remove_proc_entry("state", meme_proc_entry);
    remove_proc_entry("meme", NULL);
}
