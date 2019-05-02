#ifndef PHILON_ARM_LINUX_LEARNING_MEME_H
#define PHILON_ARM_LINUX_LEARNING_MEME_H

enum {
    MEME_STATE_OPENED = 0x1,
    MEME_STATE_CLOSED,
    MEME_STATE_READING,
    MEME_STATE_WRITING,
};
// /dev/meme文件描述符的访问状态
extern int meme_cdev_state;

// 模块的全局procfs入口目录，即/proc/meme目录
extern struct proc_dir_entry* meme_proc_entry;

// meme procfs相关函数
extern int meme_proc_init(void);
extern void meme_proc_exit(void);
extern int meme_seq_init(void);
extern void meme_seq_exit(void);

#endif // !PHILON_ARM_LINUX_LEARNING_MEME_H