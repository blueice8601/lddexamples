/*
 * This c file create a fake driver use i2c0
 */
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/kernel.h>


static int i2c_probe(struct i2c_client *client,
                const struct i2c_device_id *id)
{
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
MODULE_DESCRIPTION("i2c device driver");
MODULE_LICENSE("GPL");
