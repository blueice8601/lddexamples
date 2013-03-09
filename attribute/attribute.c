/*
 * This c file create a fake driver use i2c0
 */
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/device.h>


static int attribute_value = 22;
static ssize_t show_attribute(struct device *dev, struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", attribute_value);
}

static ssize_t store_attribute(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    return kstrtoint(buf, 10, &attribute_value);
}
static DEVICE_ATTR(attribute, S_IRUGO | S_IWUSR, show_attribute, store_attribute);


static int i2c_probe(struct i2c_client *client,
                const struct i2c_device_id *id)
{
    device_create_file(&client->dev, &dev_attr_attribute);
    return 0;
}

static int __devexit i2c_remove(struct i2c_client *client)
{
    return 0;
}

static const struct i2c_device_id i2c_example_id[] = {
    {"__i2c_example__", 0},
    { }
};

static struct i2c_driver i2c_example_driver = {
    .driver = {
        .owner = THIS_MODULE,
        .name  = "__i2c_example__",
    },
    .id_table = i2c_example_id,
    .probe = i2c_probe,
    .remove = __devexit_p(i2c_remove),
};

static struct i2c_board_info i2c_example_board_info = {
    I2C_BOARD_INFO("__i2c_example__", 0x76),
};

static int i2c_static_add_device(void)
{
    struct i2c_adapter *adapter;
    struct i2c_client  *client;
    
    adapter = i2c_get_adapter(0);
    if(!adapter) {
        printk("can't get i2c-0 adapter!!!!\n");
        return -ENODEV;
    }

    client = i2c_new_device(adapter, &i2c_example_board_info);
    if(!client) {
        printk("can't add i2c_example device\n");
        return -ENODEV;
    }
    i2c_put_adapter(adapter);

    return 0;
}

static int __init i2c_init(void)
{
    int err = 0;
    printk("%s\n", __func__);
    err = i2c_static_add_device();
    if(err < 0) {
        printk("add i2c device example failed!\n");
        return err;
    }
    return i2c_add_driver(&i2c_example_driver);
}

static void __exit i2c_exit(void)
{
    i2c_del_driver(&i2c_example_driver);
}

module_init(i2c_init);
module_exit(i2c_exit);
MODULE_AUTHOR("Dalian China, pengzhang8601@hotmail.com");
MODULE_DESCRIPTION("attribute description");
MODULE_LICENSE("GPL");
