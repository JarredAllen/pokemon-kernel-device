#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "pokemon"
#define SONG_LENGTH 87

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jarred Allen");
MODULE_DESCRIPTION("A device for printing the lyrics to the Pokemon theme song");
MODULE_VERSION("0.1");

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

int delays[SONG_LENGTH] = {7, 3, 3, 3, 2, 3, 2, 4, 2, 2, 3, 1, 0, 1, 2, 0, 1,
                           1, 3, 1, 1, 1, 2, 2, 3, 3, 2, 2, 2, 3, 3, 3, 4, 3,
                           2, 2, 3, 3, 3, 1, 1, 1, 2, 2, 1, 1, 2, 2, 1, 2, 1,
                           1, 3, 3, 3, 1, 1, 1, 1, 1, 2, 4, 5, 3, 4, 2, 2, 4,
                           19, 1, 1, 2, 1, 1, 1, 3, 1, 2, 1, 1, 2, 4, 2, 2, 1,
                           1, 1};
int pos = 0;

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int major;
static struct class *myclass;
static struct device *mydevice;
static struct cdev mycdev;

static int chmod_uevent(struct device *dev, struct kobj_uevent_env *env) {
    add_uevent_var(env, "DEVMODE=%#o", 0444);
    return 0;
}

static int __init pokemon_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_INFO "Pokemon failed at register_chrdev.\n");
        return major;
    }
    myclass = class_create(THIS_MODULE, DEVICE_NAME);
    if (myclass == NULL) {
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_INFO "Pokemon failed at class_create.\n.");
        return -1;
    }
    myclass->dev_uevent = chmod_uevent;
    cdev_init(&mycdev, &fops);
    if (cdev_add(&mycdev, major, 1) < 0) {
        device_destroy(myclass, major);
        class_destroy(myclass);
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_INFO "Pokemon failed at cdev_add.\n");
        return -1;
    }
    mydevice = device_create(myclass, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (mydevice == NULL) {
        class_destroy(myclass);
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_INFO "Pokemon failed at device_create.\n");
        return -1;
    }
    printk(KERN_INFO "Pokemon module has been loaded: %d\n", major);
    return 0;
}

static void __exit pokemon_exit(void) {
    device_destroy(myclass, MKDEV(major, 0));
    cdev_del(&mycdev);
    class_destroy(myclass);
    unregister_chrdev(major, DEVICE_NAME);

    printk(KERN_INFO "Pokemon module has been unloaded.\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Pokemon device opened.\n");
    pos = 0;
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

        msleep(delays[pos]*1000);
        errors = copy_to_user(buffer, message, message_len);
        pos++;

        return errors == 0 ? message_len : -EFAULT;
    }
}

module_init(pokemon_init);
module_exit(pokemon_exit);
