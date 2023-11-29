// WG IP Example
// WH Driver (wg_driver.c)


//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: Xilinx XUP Blackboard

// Hardware configuration:
// 
// AXI4-Lite interface
//   Mapped to offset of 0x10000
// 
// WG 0 and 1 interface:
//   GPIO[11-10] are used for WG 0 inputs
//   GPIO[9-8] are used for WG 1 inputs

// Load kernel module with insmod qe_driver.ko [param=___]

//-----------------------------------------------------------------------------

#include <linux/kernel.h>     // kstrtouint
#include <linux/module.h>     // MODULE_ macros
#include <linux/init.h>       // __init
#include <linux/kobject.h>    // kobject, kobject_atribute,
                              // kobject_create_and_add, kobject_put
#include <asm/io.h>           // iowrite, ioread, ioremap_nocache (platform specific)



#define AXI4_LITE_BASE         0x43C00000
#define WG_BASE_OFFSET         0x00020000
// register offsets
#define OFS_MODE_REG 0
#define OFS_RUN_REG 1
#define OFS_FREQ_A_REG 2
#define OFS_FREQ_B_REG 3
#define OFS_OFFSET_REG 4
#define OFS_AMPLITUDE_REG 5
#define OFS_DTYCYC_REG 6
#define OFS_CYCLES_REG 7

// field offsets
#define OFS_MODE_A_FIELD 0
#define OFS_MODE_B_FIELD 3
#define OFS_RUN_A_FIELD 0
#define OFS_RUN_B_FIELD 1
#define OFS_OFFSET_A_FIELD 0
#define OFS_OFFSET_B_FIELD 16
#define OFS_AMPL_A_FIELD 0
#define OFS_AMPL_B_FIELD 16
#define OFS_DC_A_FIELD 0
#define OFS_DC_B_FIELD 16
#define OFS_CYCLES_A_FIELD 0
#define OFS_CYCLES_B_FIELD 16

#define SPAN_IN_BYTES 32
//-----------------------------------------------------------------------------
// Kernel module information
//-----------------------------------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jason Losh");
MODULE_DESCRIPTION("WG IP Driver");

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

static unsigned int *base = NULL;

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------




void setRunA(bool running)
{
	uint8_t value = ioread32(base+OFS_RUN_REG);
	iowrite32( value & ~(0x1 << OFS_RUN_A_FIELD), base+OFS_RUN_REG);
	iowrite32( value | (running << OFS_RUN_A_FIELD), base+OFS_RUN_REG);
}

bool getRunA()
{
    uint8_t value = ioread32(base + OFS_RUN_REG);
    return (value >> OFS_RUN_A_FIELD) & 1;
}


//-----------------------------------------------------------------------------
// Kernel Objects
//-----------------------------------------------------------------------------


// Run A
static int runA = 0;
module_param(runA, int, S_IRUGO);
MODULE_PARM_DESC(runA, " Run Channel A");

static ssize_t runAStore(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
    int result = kstrtouint(buffer, 0, &runA);
    if (result == 0)
        setRunA(0, runA);
    return count;
}

static ssize_t runAShow(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    runA = getRunA(0);
    return sprintf(buffer, "%d\n", runA);
}

static struct kobj_attribute position0Attr = __ATTR(position0, 0666, runAShow, runAStore);






















// Attributes

//static struct attribute *attrs0[] =
static struct attribute *attrs0[] = {&modeAAttr.attr, &runAAttr.attr, &freqAAttr.attr, &offsetAAttr.attr, &amplAAttr.attr, &dcAAttr.attr, &cyclesAAttr.attr, NULL};
static struct attribute *attrs1[] = {&modeBAttr.attr, &runBAttr.attr, &freqBAttr.attr, &offsetBAttr.attr, &amplBAttr.attr, &dcBAttr.attr, &cyclesBAttr.attr, NULL};

/*static struct attribute_group group0 =
{
    .name = "run",
    .attrs = attrs0
};
*/
static struct attribute_group group0 =
{
    .name = "a",
    .attrs = attrs0
};

static struct attribute_group group1 =
{
    .name = "b",
    .attrs = attrs1
};

static struct kobject *kobj;



//-----------------------------------------------------------------------------
// Initialization and Exit
//-----------------------------------------------------------------------------

static int __init initialize_module(void)
{
    int result;

    printk(KERN_INFO "WG driver: starting\n");

    // Create wavegen directory under /sys/kernel
    kobj = kobject_create_and_add("wavegen", kernel_kobj); //kernel_kobj);
    if (!kobj)
    {
        printk(KERN_ALERT "WG driver: failed to create and add kobj\n");
        return -ENOENT;
    }

    // Create run and a and b groups
    result = sysfs_create_group(kobj, &group0);
    if (result !=0)
        return result;
    result = sysfs_create_group(kobj, &group1);
    if (result !=0)
        return result;
    /*result = sysfs_create_group(kobj, &group2);
    if (result !=0)
        return result;
	*/
    // Physical to virtual memory map to access gpio registers
    base = (unsigned int*)ioremap(AXI4_LITE_BASE + WG_BASE_OFFSET,
                                          SPAN_IN_BYTES);
    if (base == NULL)
        return -ENODEV;

    printk(KERN_INFO "WG driver: initialized\n");

    return 0;
}

static void __exit exit_module(void)
{
    kobject_put(kobj);
    printk(KERN_INFO "WG driver: exit\n");
}

module_init(initialize_module);
module_exit(exit_module);
