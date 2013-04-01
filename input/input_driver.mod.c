#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xa8c16cf3, "module_layout" },
	{ 0x37a0cba, "kfree" },
	{ 0x7fab3707, "input_unregister_device" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0xf89843f9, "schedule_work" },
	{ 0x43a53735, "__alloc_workqueue_key" },
	{ 0xcd0c1b6c, "input_register_device" },
	{ 0x2974a052, "input_allocate_device" },
	{ 0x27e1a049, "printk" },
	{ 0x783c7933, "kmem_cache_alloc_trace" },
	{ 0x352091e6, "kmalloc_caches" },
	{ 0xf9a482f9, "msleep" },
	{ 0x292be38d, "input_event" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "1B1C5AA69D792DAC2D49415");
