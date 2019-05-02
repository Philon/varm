#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>

#include "meme.h"

#define MAXNUM 100

// 访问文件时首先调用的接口
static void* meme_seq_start(struct seq_file* m, loff_t* pos)
{
    int* v = NULL;
    if (*pos < MAXNUM) {
        v = kmalloc(sizeof(int), GFP_KERNEL);
        *v = *pos;
        seq_printf(m, "start: *(%p) = %d\n", v, *(int*)v);
    }

    // start函数返回NULL表示pos已到达文件末尾
    return v;
}

// 每次迭代时调用，其中v是之前一次迭代(start或next)的返回值
static void* meme_seq_next(struct seq_file* m, void* v, loff_t* pos)
{
    int num = *(int*)v;
    if (num++ >= MAXNUM) {
        // 返回NULL停止迭代
        kfree(v);
        return NULL;
    }

    // 每次迭代，v和文件游标都增加1
    *(int*)v = *pos = num;
    return v;
}

// 结束迭代时调用，如果在start中有内存分配，应该在这里进行内存清理
// 但由于next的最后一次迭代肯定返回NULL，所以这里的v地址一定为NULL
// 不需要作任何处理
static void meme_seq_stop(struct seq_file* m, void* v)
{

}

// 展示时调用，主要是将v的内容格式化并输出到用户空间
static int meme_seq_show(struct seq_file* m, void* v)
{
    seq_printf(m, "show: *(%p) = %d\n", v, *(int*)v);
    return 0;
}

// 映射seq的start、next、stop、show四个迭代器的函数
const struct seq_operations meme_seq_ops = {
    .start = meme_seq_start,
    .next = meme_seq_next,
    .stop = meme_seq_stop,
    .show = meme_seq_show,
};

static int meme_seq_open(struct inode* inode, struct file* filp)
{
    // 绑定迭代操作的4个函数到/proc/meme/info文件
    return seq_open(filp, &meme_seq_ops);
}

// /proc/meme/info的文件操作映射，除了open需要自己实现外，其他均使用内部定义好的
static const struct file_operations meme_seq_fops = {
    .owner = THIS_MODULE,
    .open = meme_seq_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = seq_release,
};

int __init meme_seq_init(void)
{
    // 创建seq入口文件，即/proc/meme/info，并绑定seq相关文件函数
    proc_create("info", 0500, meme_proc_entry, &meme_seq_fops);
    return 0;
}

void __exit meme_seq_exit(void)
{
    // 删除/proc/meme/info文件
    remove_proc_entry("info", meme_proc_entry);
}