/*
 * This c file create a fake driver use i2c0
 */
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/hwmon.h>
#include <linux/slab.h>
#include <linux/hwmon-sysfs.h>
#include <linux/sysfs.h>

struct hwmon_data {
    struct device   *hwmon_dev;
};

static int hwmon_temp = 22;

static ssize_t show_temp(struct device *dev, struct device_attribute *da,
                     char *buf)
{
    return sprintf(buf, "hwmon temp is %d\n", hwmon_temp);
}

static ssize_t set_temp(struct device *dev, struct device_attribute *da,
                    const char *buf, size_t count)
{
    int err = kstrtoint(buf, 10, &hwmon_temp);
    return err;
}

static SENSOR_DEVICE_ATTR(hwmon_temp, S_IWUSR | S_IRUGO, 
            show_temp, set_temp, 0);
//arg 0: used to show the axis of g-sensor

static struct attribute *hwmon_attributes[] = {
    &sensor_dev_attr_hwmon_temp.dev_attr.attr,
    NULL
};

static const struct attribute_group hwmon_group = {
        .attrs = hwmon_attributes,
};

static int i2c_probe(struct i2c_client *client,
                const struct i2c_device_id *id)
{
    int status ;
    struct hwmon_data *data = kmalloc(sizeof(struct hwmon_data), GFP_KERNEL);
    if(!data)
        return -ENOMEM;

    i2c_set_clientdata(client, data);

    data->hwmon_dev = hwmon_device_register(&client->dev);
    if (IS_ERR(data->hwmon_dev)) {
        status = PTR_ERR(data->hwmon_dev);
        goto exit_free;
    }

    status = sysfs_create_group(&data->hwmon_dev->kobj, &hwmon_group);
    if(status)
        goto exit_remove;

    return 0;

exit_remove:
    sysfs_remove_group(&data->hwmon_dev->kobj, &hwmon_group);
exit_free:
    kfree(data);
    return status;
}

static int __devexit i2c_remove(struct i2c_client *client)
{
    struct hwmon_data *data = i2c_get_clientdata(client);
    hwmon_device_unregister(data->hwmon_dev);
    kfree(data);
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
MODULE_DESCRIPTION("hwmon driver based on i2c");
MODULE_LICENSE("GPL");
