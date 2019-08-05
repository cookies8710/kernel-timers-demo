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

struct td_my_struct {
	int data1;
	struct timer_list tmr;
	const char *str;
};

//struct td_my_struct td_instance;

static void td_timer_func(struct timer_list* timer)
{
	struct td_my_struct *ms = from_timer(ms, timer, tmr);
	TD_LOG("td_timer_func, ms->data: %d", ms->data1);

	unsigned long tgt = jiffies + msecs_to_jiffies(5000);
	TD_LOG("td_timer_func called, %d; jiffies: %ld, tgt: %ld", td_counter++, jiffies, tgt);
	if (td_counter < 10)
	{
		TD_LOG("td_timer_func, rescheduling");
		mod_timer(timer, tgt);
	}
	else
	{
		TD_LOG("td_timer_func no more scheduling");
	}
}

struct td_my_struct *ptr = NULL;

static int __init td_module_init(void)
{
	TD_LOG("td_module_init: will create a timer");

	unsigned long tgt = jiffies + msecs_to_jiffies(5000);

	ptr = kzalloc(sizeof(struct td_my_struct), GFP_KERNEL);
	ptr->data1 = 667;

	//td_instance.data1 = 23;

	timer_setup(&ptr->tmr, td_timer_func, 0);
	mod_timer(&ptr->tmr, tgt);
	//timer_setup(&td_instance.tmr, td_timer_func, 0);
	//mod_timer(&td_instance.tmr, tgt);

	TD_LOG("jiffies: %ld, tgt: %ld", jiffies, tgt);

	return 0;
}

static void __exit td_module_exit(void)
{
//	del_timer(&td_instance.tmr);

	if(ptr)
	{
		del_timer(&ptr->tmr);
		kfree(ptr);
		ptr = NULL;
	}
	TD_LOG("td_module_exit");
}

module_init(td_module_init);
module_exit(td_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tomas Surovec");
MODULE_DESCRIPTION("Kernel timer demo");
