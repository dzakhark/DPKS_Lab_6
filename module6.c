#define DEBUG

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

#include <linux/ktime.h>
#include <linux/slab.h>

static LIST_HEAD(lab_list_head);

struct time_keeper {
	ktime_t time_before;
	ktime_t time_after;
	struct list_head time_list;
};

static void print_text(unsigned int repeats)
{
	unsigned int repeat;
	struct time_keeper *ptr;

	for (repeat = 0; repeat < repeats; repeat++) {
		if (repeat == 2)
			ptr = 0;
		else
			ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);
		ptr->time_before = ktime_get();
		pr_info("Hello there!\n");
		ptr->time_after = ktime_get();
		list_add(&ptr->time_list, &lab_list_head);
	}
}

static unsigned int repeats = 1;

module_param(repeats, uint, 0444);
MODULE_PARM_DESC(repeats, "How many times to print hello");

static int __init module6_init(void)
{
	BUG_ON(repeats > 10);

	if (repeats >= 5 && repeats <= 10)
		pr_warn("Repeatition from 5 to 10 times\n");

	if (repeats == 0)
		pr_warn("No repeatition\n");

	print_text(repeats);
	return 0;
}

static void __exit module6_exit(void)
{
	struct list_head *p;
	struct list_head *n;
	struct time_keeper *curr;

	pr_info("Module 5 exit\n");

	list_for_each_safe(p, n, &lab_list_head) {
		curr = list_entry(p, struct time_keeper, time_list);
		pr_info("Time needed for printing is: %lld(ns).\n",
				curr->time_after - curr->time_before);
		list_del(p);
		kfree(curr);
	}
}

module_init(module6_init);
module_exit(module6_exit);

MODULE_AUTHOR("Dmytro Zakharchuk");
MODULE_DESCRIPTION("Test work with debug");
MODULE_LICENSE("Dual BSD/GPL");
