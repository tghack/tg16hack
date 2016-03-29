#include <linux/list.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define MAX_USERS 3
#define LOL_ALLOC 0x1
#define LOL_DEBUG 0x2
#define LOL_ADD_USER 0x3

static const char template[] = "Size: %zu. PID: %zu. Data: %s";

struct lol_buf {
	pid_t owner;
	pid_t users[MAX_USERS];
	char *data;
	int debug;
	size_t size;
	size_t num_users;
	struct list_head list;
};

struct lol_alloc {
	size_t size;
};

LIST_HEAD(lol_bufs);

static inline void free_buf(struct lol_buf *buf)
{
	list_del(&buf->list);
	kfree(buf->data);
	kfree(buf);
}

static inline int check_users(struct lol_buf *buf, pid_t pid)
{
	int i;

	for (i = 0; i < MAX_USERS; i++) {
		if (buf->users[i] == pid)
			return 1;
	}

	return 0;
}

static inline struct lol_buf *find_buf(pid_t pid)
{
	struct lol_buf *buf;

	list_for_each_entry(buf, &lol_bufs, list) {
		if (buf->owner == pid || check_users(buf, pid))
			return buf;
	}

	return NULL;
}

static ssize_t lol_read_debug(struct lol_buf *lbuf,
			      char __user *buf, size_t len)
{
	char *tmp;
	ssize_t ret;
	size_t to_copy, size;

	/* XXX: 32-bit numbers can be 10 digits long */
	size = 20 + sizeof(template) + len;

	tmp = kmalloc(size, GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;

	snprintf(tmp, size, template, lbuf->size, lbuf->owner, lbuf->data);
	to_copy = min(strlen(tmp), len);
	ret = copy_to_user(buf, tmp, to_copy);

	kfree(tmp);
	if (ret)
		return -EFAULT;

	return to_copy;
}

/*
 * Allocate a new lol_buf struct with the specified data size.
 */
static inline struct lol_buf *new_buf(size_t size)
{
	struct lol_buf *buf;

	buf = kmalloc(sizeof(struct lol_buf), GFP_KERNEL);
	if (!buf)
		return NULL;

	buf->data = kmalloc(size, GFP_KERNEL);
	if (!buf->data) {
		kfree(buf);
		return NULL;
	}

	buf->size = size;
	buf->owner = current->pid;
	memset(buf->users, 0, sizeof(buf->users));
	buf->num_users = 0;
	list_add(&buf->list, &lol_bufs);

	return buf;
}

static long lol_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;

	switch (cmd) {
	case LOL_ALLOC:
	{
		struct lol_buf *buf;
		struct lol_alloc l = { 0 };

		if (copy_from_user(&l, argp, sizeof(l)))
			return -EFAULT;

		buf = new_buf(l.size);
		if (!buf)
			return -ENOMEM;

		break;
	}
	case LOL_DEBUG:
	{
		int yes;
		struct lol_buf *lbuf;

		lbuf = find_buf(current->pid);
		if (!lbuf)
			return -EPERM;

		if (copy_from_user(&yes, argp, sizeof(yes)))
			return -EFAULT;

		lbuf->debug = yes ? 1 : 0;
		break;
	}
	case LOL_ADD_USER:
	{
		pid_t pid;
		struct lol_buf *lbuf;

		lbuf = find_buf(current->pid);
		if (!lbuf)
			return -EPERM;

		if (lbuf->num_users > MAX_USERS)
			return -EINVAL;

		if (copy_from_user(&pid, argp, sizeof(pid)))
			return -EFAULT;

		lbuf->users[lbuf->num_users++] = pid;
		break;
	}
	default:
		return -EINVAL;
	}

	return 0;
}

static ssize_t lol_write(struct file *file, const char __user *buf,
			 size_t len, loff_t *off)
{
	struct lol_buf *lbuf;

	lbuf = find_buf(current->pid);
	if (!lbuf)
		return -EPERM;

	if (current->pid != lbuf->owner)
		return -EPERM;

	if (len > lbuf->size)
		return -EINVAL;

	if (copy_from_user(lbuf->data, buf, len))
		return -EFAULT;

	return len;
}

static ssize_t lol_read(struct file *file, char __user *buf,
			size_t len, loff_t *off)
{
	size_t to_copy;
	struct lol_buf *lbuf;

	lbuf = find_buf(current->pid);
	if (!lbuf)
		return -EPERM;

	if (!lbuf->size)
		return 0;

	to_copy = min(lbuf->size, len);
	if (copy_to_user(buf, lbuf->data, to_copy))
		return -EFAULT;

	return to_copy;
}

static int lol_flush(struct file *file, fl_owner_t id)
{
	struct lol_buf *lbuf;

	lbuf = find_buf(current->pid);
	if (!lbuf)
		return -EPERM;

	free_buf(lbuf);

	return 0;
}

static const struct file_operations lol_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = lol_ioctl,
	.write = lol_write,
	.read = lol_read,
	.flush = lol_flush
};

static struct miscdevice lol_dev = {
	.name = "loldev",
	.fops = &lol_fops,
	.mode = S_IRUGO | S_IWUGO
};

static int __init lol_init(void)
{
	return misc_register(&lol_dev);
}

static void __exit lol_exit(void)
{
	struct lol_buf *pos, *n;

	list_for_each_entry_safe(pos, n, &lol_bufs, list)
		free_buf(pos);

	misc_deregister(&lol_dev);
}

module_init(lol_init);
module_exit(lol_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Christian Resell");
MODULE_DESCRIPTION("TG16 shit");
