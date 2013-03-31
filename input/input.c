/*
 * TODO: rmmod input will cause system crash
 */
#include <linux/input.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>

struct ex_data {
    struct input_dev *idev;
    struct work_struct i_work;
    struct workqueue_struct *i_workqueue;
};


static struct ex_data *this_idev;

static void input_work(struct work_struct *work)
{
    while(1)
    {
        input_report_abs(this_idev->idev, ABS_X, 0x01);
        input_report_abs(this_idev->idev, ABS_DISTANCE, 0);
//        input_report_key(this_idev->idev, KEY_VOLUMEUP, 1);
        input_sync(this_idev->idev);
    //    msleep(20);
//        input_report_key(this_idev->idev, KEY_VOLUMEUP, 0);
  //      input_sync(this_idev->idev);
        msleep(4000);
    }

}
static int __init input_init(void)
{
    struct input_dev *idev;
    struct ex_data *pedata = kzalloc(sizeof(struct ex_data)
            , GFP_KERNEL);
    if(!pedata) {
        printk("err here\n");
        return -ENOMEM;
    }
    this_idev = pedata;

    idev = input_allocate_device();
    pedata->idev = idev;

    idev->name = "input_example";
    idev->phys = "/sys/fakepath"; //useless
    idev->id.bustype = BUS_HOST; //useless
    idev->id.vendor = 0x0001;       //useless
    idev->id.product = 0x0001;      //uselses
    idev->id.version = 0x0010;   //useless


    set_bit(EV_KEY, idev->evbit);
    set_bit(KEY_VOLUMEUP, idev->keybit);//115

#define MIN -0xfff
#define MAX 0xfff
    idev->evbit[0] |= BIT_MASK(EV_ABS);
    input_set_abs_params(idev, ABS_X, MIN, MAX, 0, 0);//g-sensor, TP

    __set_bit(EV_SYN, idev->evbit);
    input_set_abs_params(idev, ABS_DISTANCE, 0, 1, 0, 0);


    input_register_device(this_idev->idev);
    
    INIT_WORK(&pedata->i_work, input_work);
    pedata->i_workqueue = create_singlethread_workqueue("i_work");
    schedule_work(&pedata->i_work);
    return 0;
}

static void __exit input_exit(void)
{
    destroy_workqueue(this_idev->i_workqueue);
    input_unregister_device(this_idev->idev);
    kfree(this_idev);
}

module_init(input_init);
module_exit(input_exit);

MODULE_AUTHOR("Dalian China, pengzhang8601@hotmail.com");
MODULE_DESCRIPTION("input driver example");
MODULE_LICENSE("GPL");
