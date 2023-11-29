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
#include "../address_map.h"   // overall memory map
#include "wg_regs.h"          // register offsets in WG IP


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

void setMode(uint8_t mode, bool field)
{   
    uint8_t shift = field ? OFS_MODE_B_FIELD : OFS_MODE_A_FIELD;
	unsigned int value = ioread32(base+OFS_MODE_REG);
	iowrite32(value &= ~(0x7 << shift), base+OFS_MODE_REG);
	iowrite32(value |= (mode << shift), base+OFS_MODE_REG);
}

void getMode(uint8_t mode)


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

void setRunB(bool running)
{
	uint8_t value = ioread32(base+OFS_RUN_REG);
	iowrite32( value & ~(0x1 << OFS_RUN_B_FIELD), base+OFS_RUN_REG);
	iowrite32( value | (running << OFS_RUN_B_FIELD), base+OFS_RUN_REG);
}

bool getRunB()
{
    uint8_t value = ioread32(base + OFS_RUN_REG);
    return (value >> OFS_RUN_B_FIELD) & 1;
}


void setFreqA(uint32_t frequency)
{
    iowrite32(frequency, base+OFS_FREQ_A_REG);
}

uint32_t getFreqA()
{
	return ioread32(base+OFS_FREQ_A_REG);
}

void setFreqB(uint32_t frequency)
{
    iowrite32(frequency, base+OFS_FREQ_B_REG);
}

uint32_t getFreqB()
{
	return ioread32(base+OFS_FREQ_B_REG);
}



void setOffsetA(int16_t offset)
{
	unsigned int value = ioread32(base+OFS_OFFSET_REG);
	iowrite32(value &= ~(0xFFFF << OFS_OFFSET_A_FIELD), base+OFS_OFFSET_REG);
	iowrite32(value |= (offset << OFS_OFFSET_A_FIELD), base+OFS_OFFSET_REG);
}

int16_t getOffsetA()
{
    unsigned int value = ioread32(base + OFS_OFFSET_REG);
    return (value >> OFS_OFFSET_A_FIELD) & 0xFFFF);
}

void setOffsetB(int16_t offset)
{
	unsigned int value = ioread32(base+OFS_OFFSET_REG);
	iowrite32(value &= ~(0xFFFF << OFS_OFFSET_A_FIELD), base+OFS_OFFSET_REG);
	iowrite32(value |= (offset << OFS_OFFSET_A_FIELD), base+OFS_OFFSET_REG);
}

int16_t getOffsetB()
{
    unsigned int value = ioread32(base + OFS_OFFSET_REG);
    return (value >> OFS_OFFSET_B_FIELD) & 0xFFFF);
}


	

void setAmplitudeA(int16_t amplitude)
{
	unsigned int value = ioread32(base+OFS_AMPLITUDE_REG);
	iowrite32(value &= ~(0xFFFF << OFS_AMPL_A_FIELD), base+OFS_OFFSET_REG);
	iowrite32(value |= (amplitude << OFS_AMPL_A_FIELD), base+OFS_OFFSET_REG);
}

int16_t getAmplitudeA()
{
	unsigned int value = ioread32(base+OFS_AMPLITUDE_REG);
	return (value >> OFS_AMPL_A_FIELD) & 0xFFFF);
}

void setAmplitudeB(int16_t amplitude)
{
	unsigned int value = ioread32(base+OFS_AMPLITUDE_REG);
	iowrite32(value &= ~(0xFFFF << OFS_AMPL_B_FIELD), base+OFS_OFFSET_REG);
	iowrite32(value |= (amplitude << OFS_AMPL_B_FIELD), base+OFS_OFFSET_REG);
}

int16_t getAmplitudeB()
{
	unsigned int value = ioread32(base+OFS_AMPLITUDE_REG);
	return (value >> OFS_AMPL_B_FIELD) & 0xFFFF);
}

void setDutyCycleA(uint16_t dtycyc)
{
	unsigned int value = ioread32(base+OFS_DTYCYC_REG);
	iowrite32(value &= ~(0xFFFF << OFS_DC_A_FIELD), base+OFS_DTYCYC_REG);
	iowrite32(value &= |= (dtycyc << OFS_DC_A_FIELD), base+OFS_DTYCYC_REG);
}
uint16_t getDutyCycleA()
{
	unsigned int value = ioread32(base+OFS_DTYCYC_REG);
	return (value >> OFS_DC_A_FIELD) & 0xFFFF);
}

void setDutyCycleB(uint16_t dtycyc)
{
	unsigned int value = ioread32(base+OFS_DTYCYC_REG);
	iowrite32(value &= ~(0xFFFF << OFS_DC_B_FIELD), base+OFS_DTYCYC_REG);
	iowrite32(value &= |= (dtycyc << OFS_DC_B_FIELD), base+OFS_DTYCYC_REG);
}
uint16_t getDutyCycleB()
{
	unsigned int value = ioread32(base+OFS_DTYCYC_REG);
	return (value >> OFS_DC_B_FIELD) & 0xFFFF);
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
    kobj = kobject_create_and_add("wavegen", NULL); //kernel_kobj);
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
    base = (unsigned int*)ioremap(AXI4_LITE_BASE + QE_BASE_OFFSET,
                                          QE_SPAN_IN_BYTES);
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
