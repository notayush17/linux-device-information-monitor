#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/uptime.h>
#include <linux/uaccess.h>

static ssize_t hw_health_read(struct file *file, char __user *user_buffer,
                              size_t count, loff_t *offset)
{
    char data[256];
    unsigned long free_pages;
    int length;
    if (*offset != 0) return 0;
    free_pages = global_zone_page_state(NR_FREE_PAGES);
    length = scnprintf(data, sizeof(data),
        "cpu_count=%u\nonline_cpus=%u\nmem_total_kb=%lu\nmem_free_kb=%lu\nuptime_seconds=%llu\n",
        num_possible_cpus(), num_online_cpus(),
        (unsigned long)(totalram_pages() * (PAGE_SIZE / 1024)),
        (unsigned long)(free_pages * (PAGE_SIZE / 1024)),
        (unsigned long long)get_jiffies_64() / HZ);
    if (copy_to_user(user_buffer, data, length)) return -EFAULT;
    *offset = length;
    return length;
}

static const struct file_operations hw_health_fops = {
    .owner = THIS_MODULE,
    .read = hw_health_read,
};

static struct miscdevice hw_health_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "hw_health",
    .fops = &hw_health_fops,
    .mode = 0444,
};

static int __init hw_health_init(void)
{
    int result = misc_register(&hw_health_device);
    if (result == 0) pr_info("hw_health: registered /dev/hw_health\n");
    return result;
}

static void __exit hw_health_exit(void)
{
    misc_deregister(&hw_health_device);
    pr_info("hw_health: removed\n");
}

module_init(hw_health_init);
module_exit(hw_health_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Individual Student");
MODULE_DESCRIPTION("Educational read-only Linux hardware health character driver");
