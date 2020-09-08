/*
 * @Author: hongxing.wu
 * @Date: 2020-09-08 15:17:59
 * @LastEditTime: 2020-09-08 18:14:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \6ull_dev_led\char_dev_led.c
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>


#define LED_DEV_MAJOR    200    /*主设备号*/
#define LED_DEV_NAME     "ldedev"


/* 寄存器物理地址 */
#define CCM_CCGR1_BASE              (0X020C406C)
#define SW_MUX_GPIO1_IO03_BASE      (0X020E0068)
#define SW_PAD_GPIO1_IO03_BASE      (0X020E02F4)
#define GPIO1_DR_BASE               (0X0209C000)
#define GPIO1_GDIR_BASE             (0X0209C004)

/*映射虚拟地址*/
static void __iomem *IMX6U_CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO03;
static void __iomem *SW_PAD_GPIO1_IO03;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;




static char readbuf[100];
static char writebuf[100];
static char kerneldata[] = {"kernel data!"};


/*
*打开设备DSF
* inode : 传递给驱动的 inode
* filp  : 设备文件，file 结构体有个叫做 private_data 的成员变量
* return :0 成功  其他 失败
*/
static int devLed_open(struct inode *inode,struct file *filp)
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
static ssizet_t devLed_read(struct file *filp, char __user *buf, size_t cnt, lofft *offt)
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
static ssize_t devLed_writh(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
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
static int devLed_release(struct inode *inode, struct file *file)
{
    return 0;
}


/*
*设备操作函数结构体
*/
static struct file_operations chrdevbase_fops = {
    .owner = THIS_MODULE,
    .open = devLed_open,
    .read = devLed_read,
    .write = devLed_writh,
    .release = devLed_release,
}


static int __init devLed_init(void)
{
    int ret = 0;
    u32 val = 0;
    //注册字符设备

    IMX6U_CCM_CCGR1 = ioremap(CCM_CCGR1_BASE, 4);
    SW_MUX_GPIO1_IO03 = ioremap(SW_MUX_GPIO1_IO03_BASE, 4);
    SW_PAD_GPIO1_IO03 = ioremap(SW_PAD_GPIO1_IO03_BASE, 4);
    GPIO1_DR = ioremap(GPIO1_DR_BASE, 4);
    GPIO1_GDIR = ioremap(GPIO1_GDIR_BASE, 4);

    /* 2、使能 GPIO1 时钟 */
    val = readl(IMX6U_CCM_CCGR1);
    val &= ~(3 << 26); /* 清除以前的设置 */
    val |= (3 << 26); /* 设置新值 */
    writel(val, IMX6U_CCM_CCGR1);

    /* 3、设置 GPIO1_IO03 的复用功能，将其复用为
    * GPIO1_IO03，最后设置 IO 属性。
    */
    writel(5, SW_MUX_GPIO1_IO03);
    /* 寄存器 SW_PAD_GPIO1_IO03 设置 IO 属性 */
    writel(0x10B0, SW_PAD_GPIO1_IO03);

     /* 4、设置 GPIO1_IO03 为输出功能 */
    val = readl(GPIO1_GDIR);
    val &= ~(1 << 3); /* 清除以前的设置 */
    val |= (1 << 3); /* 设置为输出 */
    writel(val, GPIO1_GDIR);

    /* 5、默认关闭 LED */
    val = readl(GPIO1_DR);
    val |= (1 << 3);
    writel(val, GPIO1_DR);

    ret = register_chrdev(LED_DEV_MAJOR, LED_DEV_NAME, &chrdevbase_fops);
    if(retvalue < 0){
        printk("led driver register failed\r\n");
        return -EIO;
    }
    printk("led init() \r\n")
    return 0;
}

static void __exit devLed_exit(void)
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





