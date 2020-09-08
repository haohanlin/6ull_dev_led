/*
 * @Author: hongxing.wu
 * @Date: 2020-09-08 02:04:53
 * @LastEditTime: 2020-09-08 02:09:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \6ull_qd\chrdevbase.c
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>


#define CHRDEVBASE_MAJOR    200    /*主设备号*/
#define CHRDEVBASE_NAME     "chrdevbase"


static char readbuf[100];
static char writebuf[100];
staric char kerneldata[] = {"kernel data!"};


/*
*打开设备
* inode : 传递给驱动的 inode
* filp  : 设备文件，file 结构体有个叫做 private_data 的成员变量
* return :0 成功  其他 失败
*/
static int chrdevbase_open(struct inode *inode,struct file *filp)
{

    return 0;
}

/*
*从设备读取数据
*filp   :要打开的设备文件
*buf    :返回给用户空间的数据缓冲区
*cnt    :要读取的数据长度
*offt   :相对于文件首地址偏移
*retuen：读取的字节数，如果为负数，表示读取失败
*/
static ssizet_t chrdevbase_read(struct file *filp, char __user *buf, size_t cnt, lofft *offt)
{
    int retvalue = 0;
    memcpy(readbuf,kerneldata,sizeof(kerneldata));
    retvalue = copy_to_user(buf, readbuf, cnt);
    if(retvalue == 0)
    {
        printk("kernel senddata ok \r\n");
    }
    else
    {
        printk("kernel send data failed \r\n");
    }
    
    return 0;
}

/*
*向设备写数据
*file   :设备文件 表示打开的文件描述符
*buf    ：要写给设备的写入数据
*cnt    ：要写入的数据长度
*offt   ：相对于文件首地址的偏移
*return ：写入的字节数，如果为负数，表示失败
*/
static ssize_t chrdevbase_writh(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
    int ret = 0;
    ret = copy_from_user(writebuf, buf, cnt);
    if(ret == 0)
    {
        printk("kernel recev deat: %s \r\n", writebuf);
    }
    else
    {
        printk("kernel recev data failed \r\n");
    }
    return 0;
}

/*
*关闭/释放设备
*filp:要关闭的设备文件(文件描述符)
*return :
*/
static int chrdevbase_release(struct inode *inode, struct file *file)
{
    return 0;
}


/*
*设备操作函数结构体
*/
static struct file_operations chrdevbase_fops = {
    .owner = THIS_MODULE,
    .open = chrdevbase_open,
    .read = chrdevbase_read,
    .write = chrdevbase_writh,
    .release = chrdevbase_release,
}


static int __init chrdevbase_init(void)
{
    int retvalue = 0;
    //注册字符设备
    retvalue = register_chrdev(CHRDEVBASE_MAJOR, CHRDEVBASE_NAME, &chrdevbase_fops);
    if(retvalue < 0){
        printk("chrdevbase driver register failed\r\n");
    }
    printk("chrdevbase init() \r\n")
    return 0;
}

static void __exit chrdevbase_exit(void)
{
    /*
    *注销字符设备驱动
    */
   unregister_chardev(CHRDEVBASE_MAJOR,CHRDEVBASE_NAME);
   printk("chardevbase_exit() \r\n")
}


module_init(chrdevbase_init);
module_exit(chrdevbase_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wuhongxing");





