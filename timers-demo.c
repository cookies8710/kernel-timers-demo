#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/notifier.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/init.h>

#define TD_LOG_START "tsurovec: "

#define TD_LOG(x, ...) printk(KERN_INFO TD_LOG_START x, ##__VA_ARGS__);

static int td_counter = 0;
static void td_timer_func(struct timer_list* timer);

DEFINE_TIMER(td_timer, td_timer_func);

static void td_timer_func(struct timer_list* timer)
{
	unsigned long tgt = jiffies + msecs_to_jiffies(5000);
	TD_LOG("td_timer_func called, %d; jiffies: %ld, tgt: %ld", td_counter++, jiffies, tgt);
	if (td_counter < 10)
	{
		TD_LOG("td_timer_func, rescheduling");
		mod_timer(&td_timer, tgt);
	}
	else
	{
		TD_LOG("td_timer_func no more scheduling");
	}
}

static int __init td_module_init(void)
{
	TD_LOG("td_module_init: will create a timer");

	unsigned long tgt = jiffies + msecs_to_jiffies(5000);

	timer_setup(&td_timer, td_timer_func, 0);

	mod_timer(&td_timer, tgt);

	TD_LOG("-%d-", &td_timer);
	TD_LOG("jiffies: %ld, tgt: %ld", jiffies, tgt);

	return 0;
}

static void __exit td_module_exit(void)
{
	del_timer(&td_timer);
	TD_LOG("td_module_exit");
}

module_init(td_module_init);
module_exit(td_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tomas Surovec");
MODULE_DESCRIPTION("Kernel timer demo");
