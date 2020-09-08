# 6ull_qd

开始编写6ull这款正点原子的板驱动学习
在 Linux 内核文件 include/linux/fs.h 中
有个叫做 file_operations 的结构体，此结构体就是 Linux 内核驱动操作函数集合

 struct file_operations {
    struct module *owner; //owner 拥有该结构体的模块的指针，一般设置为 THIS_MODULE。
    loff_t (*llseek) (struct file *, loff_t, int); //llseek 函数用于修改文件当前的读写位置。
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t*); //read 函数用于读取设备文件。
    ssize_t (*write) (struct file *, const char __user *, size_t,loff_t *); //write 函数用于向设备文件写入(发送)数据。
    ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
    ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
    int (*iterate) (struct file *, struct dir_context *);
    unsigned int (*poll) (struct file *, struct poll_table_struct*);  //poll 是个轮询函数，用于查询设备是否可以进行非阻塞的读写。
    long (*unlocked_ioctl) (struct file *, unsigned int, unsignedlong);//unlocked_ioctl 函数提供对于设备的控制功能，与应用程序中的 ioctl 函数对
    //应
    long (*compat_ioctl) (struct file *, unsigned int, unsignedlong); //compat_ioctl 函数与 unlocked_ioctl 函数功能一样，区别在于在 64 位系统
    //上，32 位的应用程序调用将会使用此函数。在 32 位的系统上运行 32 位的应用程序调用的是unlocked_ioctl。
    int (*mmap) (struct file *, struct vm_area_struct *); //mmap 函数用于将将设备的内存映射到进程空间中(也就是用户空间)，
    //一般帧缓冲设备会使用此函数，比如 LCD 驱动的显存，将帧缓冲(LCD 显存)映射到用户空间中以后应用程序就可以直接操作显存了，
    //这样就不用在用户空间和内核空间之间来回复制。
    int (*mremap)(struct file *, struct vm_area_struct *);
    int (*open) (struct inode *, struct file *); //open 函数用于打开设备文件
    int (*flush) (struct file *, fl_owner_t id);
    int (*release) (struct inode *, struct file *); //release 函数用于释放(关闭)设备文件，与应用程序中的 close 函数对应。
    int (*fsync) (struct file *, loff_t, loff_t, int datasync); //fasync 函数用于刷新待处理的数据，用于将缓冲区中的数据刷新到磁盘中。
    int (*aio_fsync) (struct kiocb *, int datasync); //aio_fsync 函数与 fasync 函数的功能类似，只是 aio_fsync 是异步刷新待处理的数据。
    int (*fasync) (int, struct file *, int);
    int (*lock) (struct file *, int, struct file_lock *);
    ssize_t (*sendpage) (struct file *, struct page *, int, size_t,loff_t *, int);
    unsigned long (*get_unmapped_area)(struct file *, unsigned long,unsigned long, unsigned long, unsigned long);
    int (*check_flags)(int);
    int (*flock) (struct file *, int, struct file_lock *);
    ssize_t (*splice_write)(struct pipe_inode_info *, struct file *,loff_t *, size_t, unsigned int);
    ssize_t (*splice_read)(struct file *, loff_t *, structpipe_inode_info *, size_t, unsigned int);
    int (*setlease)(struct file *, long, struct file_lock **, void**);
    long (*fallocate)(struct file *file, int mode, loff_t offset,
    loff_t len);
    void (*show_fdinfo)(struct seq_file *m, struct file *f);
    #ifndef CONFIG_MMU
    unsigned (*mmap_capabilities)(struct file *);
    #endif
 };
