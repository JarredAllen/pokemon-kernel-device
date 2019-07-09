#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "pokemon"
#define SONG_LENGTH 87

static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);

char* messages[SONG_LENGTH] = {"I want to be the very best,\n",
                               "Like no one ever was.\n",
                               "To catch them is my real test,\n",
                               "To train them is my cause!\n",
                               "\n",
                               "I will travel across the land,\n",
                               "Searching far and wide.\n",
                               "Each Pokemon to understand,\n",
                               "The power that's inside!\n",
                               "\n",
                               "Pokemon!\n",
                               "Gotta catch 'em all!\n",
                               "\n",
                               "It's you and me,\n",
                               "I know it's my destiny!\n",
                               "\n",
                               "Pokemon!\n",
                               "Oh, you're my best friend.\n",
                               "In a world we must defend!\n",
                               "\n",
                               "Pokemon!\n",
                               "Gotta catch 'em all!\n",
                               "A heart so true,\n",
                               "Our courage will pull us through!\n",
                               "You teach me and I'll teach you,\n",
                               "Pokemon!\n",
                               "Gotta catch 'em all!\n",
                               "\n",
                               "Gotta catch 'em all!\n",
                               "\n",
                               "Every challenge along the way,\n",
                               "With courage I will face!\n",
                               "I will battle every day,\n",
                               "To claim my rightful place!\n",
                               "\n",
                               "Come with me, the time is right,\n",
                               "There's no better team!\n",
                               "Arm in arm, we'll win the fight,\n",
                               "It's always been our dream!\n",
                               "\n",
                               "Pokemon!\n",
                               "Gotta catch 'em all!\n",
                               "It's you and me,\n",
                               "I know it's my destiny!\n",
                               "\n",
                               "Pokemon!\n",
                               "Oh, you're my best friend.\n",
                               "In a world we must defend!\n",
                               "\n",
                               "Pokemon!\n",
                               "Gotta catch 'em all!\n",
                               "A heart so true,\n",
                               "Our courage will pull us through!\n",
                               "You teach me and I'll teach you,\n",
                               "Pokemon!\n",
                               "\n",
                               "Gotta catch 'em all!\n",
                               "\n",
                               "Gotta catch 'em all!\n",
                               "\n",
                               "Gotta catch 'em all!\n",
                               "\n",
                               "Gotta catch 'em all!\n",
                               "\n",
                               "Gotta catch 'em all!\n",
                               "\n",
                               "Yeah",
                               "\n",
                               "Pokemon!\n",
                               "Gotta catch 'em all!\n",
                               "It's you and me,\n",
                               "I know it's my destiny!\n",
                               "\n",
                               "Pokemon!\n",
                               "Oh, you're my best friend.\n",
                               "In a world we must defend!\n",
                               "\n",
                               "Pokemon!\n", 
                               "Gotta catch 'em all!\n",
                               "A heart so true,\n",
                               "Our courage will pull us through!\n",
                               "You teach me and I'll teach you,\n",
                               "Pokemon!\n",
                               "Gotta catch 'em all!\n",
                               "\n",
                               "Gotta catch 'em all!\n",
                               "\n",
                               };
int pos = 0;

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int major;

static int __init rickroll_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_INFO "Pokemon load failed.\n");
        return major;
    }
    printk(KERN_INFO "Pokemon module has been loaded: %d\n", major);
    return 0;
}

static void __exit rickroll_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);

    printk(KERN_INFO "Pokemon module has been unloaded.\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Pokemon device opened.\n");
    return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer,
                         size_t len, loff_t *offset) {
    printk(KERN_INFO "The pokemon song is already perfect. No changes needed.");
    return -EFAULT;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Pokemon device closed.\n");
    return 0;
}

static ssize_t dev_read(struct file* filep, char* buffer,
                    size_t len, loff_t* offset) {
    if (pos == SONG_LENGTH) {
        pos = 0;
        return 0;
    }
    else {
        int errors = 0;
        const char* message = messages[pos];
        int message_len = strlen(message);

        errors = copy_to_user(buffer, message, message_len);
        pos++;

        return errors == 0 ? message_len : -EFAULT;
    }
}

module_init(rickroll_init);
module_exit(rickroll_exit);

