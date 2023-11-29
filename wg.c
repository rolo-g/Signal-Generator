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



void setmodeA(uint8_t mode)
{
	uint32_t value = ioread32(base+OFS_MODE_REG);
	value &= ~(0x7 << OFS_MODE_A_FIELD);
	iowrite32(value | (mode << OFS_MODE_A_FIELD),base+OFS_MODE_REG);
}

uint32_t getmodeA(void)
{
	uint32_t value = ioread32(base+OFS_MODE_REG);
	return value;
}

void setmodeB(uint8_t mode)
{
	uint32_t value = ioread32(base+OFS_MODE_REG);
	value &= ~(0x7 << OFS_MODE_B_FIELD);
	iowrite32(value | (mode << OFS_MODE_B_FIELD),base+OFS_MODE_REG);
}

uint32_t getmodeB(void)
{
	uint32_t value = ioread32(base+OFS_MODE_REG);
	return value;
}

void setrunA(bool running)
{
	uint32_t value = ioread32(base+OFS_RUN_REG);
	value &= ~(0x1 << OFS_RUN_A_FIELD);
	iowrite32(value | (running << OFS_RUN_A_FIELD),base+OFS_RUN_REG);
}

uint32_t getrunA(void)
{
	uint32_t value = ioread32(base+OFS_RUN_REG);
	return value;
}

void setrunB(bool running)
{
	uint32_t value = ioread32(base+OFS_RUN_REG);
	value &= ~(0x1 << OFS_RUN_B_FIELD);
	iowrite32(value | (running << OFS_RUN_B_FIELD),base+OFS_RUN_REG);
}

uint32_t getrunB(void)
{
	uint32_t value = ioread32(base+OFS_RUN_REG);
	return value;
}




void setfreqA(uint32_t frequency)
{
	uint32_t value = ioread32(base+OFS_FREQ_A_REG);
	value &= 0x0;
    iowrite32(value | frequency, base+OFS_FREQ_A_REG);
}

int32_t getfreqA(void)
{
	uint32_t value = ioread32(base+OFS_FREQ_A_REG);
	return value;
}

void setfreqB(uint32_t frequency)
{
	uint32_t value = ioread32(base+OFS_FREQ_B_REG);
	value &= 0x0;
    iowrite32(value | frequency, base+OFS_FREQ_B_REG);
}

int32_t getfreqB(void)
{
	uint32_t value = ioread32(base+OFS_FREQ_B_REG);
	return value;
}


void setAmplitudeA(int16_t amplitude)
{
	int32_t value = ioread32(base+OFS_AMPLITUDE_REG);
	value &= ~(0xFFFF << OFS_AMPL_A_FIELD);
	iowrite32(value | (amplitude << OFS_AMPL_A_FIELD),base+OFS_AMPLITUDE_REG);
}

int32_t getAmplitudeA(void)
{
	int32_t value = ioread32(base+OFS_AMPLITUDE_REG);
	return value;
}



void setAmplitudeB(int16_t amplitude)
{
	
	int32_t value = ioread32(base+OFS_AMPLITUDE_REG);
	value &= ~(0xFFFF << OFS_AMPL_B_FIELD);
	iowrite32(value | (amplitude << OFS_AMPL_B_FIELD),base+OFS_AMPLITUDE_REG);
}

int32_t getAmplitudeB(void)
{
	
	int16_t value = ioread32(base+OFS_AMPLITUDE_REG);
	return value;
}


void setoffsetA(int16_t offset)
{
	int32_t value = ioread32(base+OFS_OFFSET_REG);
	value &= ~(0xFFFF << OFS_OFFSET_A_FIELD);
	iowrite32(value | (offset << OFS_OFFSET_A_FIELD),base+OFS_OFFSET_REG);
}

int32_t getoffsetA(void)
{
	int32_t value = ioread32(base+OFS_OFFSET_REG);
	return value;
}

void setoffsetB(int16_t offset)
{
	int32_t value = ioread32(base+OFS_OFFSET_REG);
	value &= ~(0xFFFF << OFS_OFFSET_B_FIELD);
	iowrite32(value | (offset << OFS_OFFSET_B_FIELD),base+OFS_OFFSET_REG);
}

int32_t getoffsetB(void)
{
	int32_t value = ioread32(base+OFS_OFFSET_REG);
	return value;
}


void setdutycyleA(int16_t dtycyc)
{
	int32_t value = ioread32(base+OFS_DTYCYC_REG);
	value &= ~(0xFFFF << OFS_DC_A_FIELD);
	iowrite32(value | (dtycyc << OFS_DC_A_FIELD),base+OFS_DTYCYC_REG);
}

int32_t getdutycyleA(void)
{
	int32_t value = ioread32(base+OFS_DTYCYC_REG);
	return value;
}

void setdutycyleB(int16_t dtycyc)
{
	int32_t value = ioread32(base+OFS_DTYCYC_REG);
	value &= ~(0xFFFF << OFS_DC_B_FIELD);
	iowrite32(value | (dtycyc << OFS_DC_B_FIELD),base+OFS_DTYCYC_REG);
}

int32_t getdutycyleB(void)
{
	int32_t value = ioread32(base+OFS_DTYCYC_REG);
	return value;
}






//-----------------------------------------------------------------------------
// Kernel Objects
//-----------------------------------------------------------------------------

//Mode A
static int modeA = 0;
module_param(modeA, int, S_IRUGO);
MODULE_PARM_DESC(modeA, " Mode Channel A");



static ssize_t modeAStore(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	//int modeA = 0;
	if (strncmp(buffer,"dc", strlen("dc")) == 0)
	{
		setmodeA (0);
		modeA = 0;
	}
	else if(strncmp(buffer,"sine", strlen("sine")) == 0)
	{
		setmodeA (1);
		modeA = 1;
	}
	else if(strncmp(buffer,"sawtooth", strlen("sawtooth")) == 0)
	{
		setmodeA (2);
		modeA = 2;
	}
	else if(strncmp(buffer,"triangle", strlen("triangle")) == 0)
	{
		setmodeA (3);
		modeA = 3;
	}
	else if(strncmp(buffer,"square", strlen("square")) == 0)
	{
		setmodeA (4);
		modeA = 4;
	}
	
	
    //int result = kstrtouint(buffer, 0, &modeA);
    //if (result == 0)
    //setmodeA (modeA);
    return count;
}

static ssize_t modeAShow(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    modeA = getmodeA();
	if(modeA == 0)
	{
		strcpy(buffer, "dc\n");
	}
	else if(modeA == 1)
	{
		strcpy(buffer, "sine\n");
	}
	else if(modeA == 2)
	{
		strcpy(buffer, "sawtooth\n");
	}
	else if(modeA == 3)
	{
		strcpy(buffer, "triangle\n");
	}
	else if(modeA == 4)
	{
		strcpy(buffer, "square\n");
	}
	
	return strlen(buffer);
    //return sprintf(buffer, "%d\n", modeA);
}

static struct kobj_attribute modeAAttr = __ATTR(modeA, 0664, modeAShow, modeAStore);


//Mode B
static int modeB = 0;
module_param(modeB, int, S_IRUGO);
MODULE_PARM_DESC(modeB, " Mode Channel B");

static ssize_t modeBStore(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
    if (strncmp(buffer,"dc", strlen("dc")) == 0)
	{
		setmodeB (0);
		modeB = 0;
	}
	else if(strncmp(buffer,"sine", strlen("sine")) == 0)
	{
		setmodeB (1);
		modeA = 1;
	}
	else if(strncmp(buffer,"sawtooth", strlen("sawtooth")) == 0)
	{
		setmodeB (2);
		modeA = 2;
	}
	else if(strncmp(buffer,"triangle", strlen("triangle")) == 0)
	{
		setmodeB (3);
		modeA = 3;
	}
	else if(strncmp(buffer,"square", strlen("square")) == 0)
	{
		setmodeB (4);
		modeB = 4;
	}
    return count;
}

static ssize_t modeBShow(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    modeB = getmodeB();
    if(modeB == 0)
	{
		strcpy(buffer, "dc\n");
	}
	else if(modeB == 1)
	{
		strcpy(buffer, "sine\n");
	}
	else if(modeB == 2)
	{
		strcpy(buffer, "sawtooth\n");
	}
	else if(modeB == 3)
	{
		strcpy(buffer, "triangle\n");
	}
	else if(modeB == 4)
	{
		strcpy(buffer, "square\n");
	}
	
	return strlen(buffer);
}

static struct kobj_attribute modeBAttr = __ATTR(modeB, 0664, modeBShow, modeBStore);

//------------------------------------------------------------------------------------
//Run A
static int runA = 0;
module_param(runA, int, S_IRUGO);
MODULE_PARM_DESC(runA, " Run Channel A");

static ssize_t runAStore(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
    int result = kstrtouint(buffer, 0, &runA);
    if (result == 0)
        setrunA (runA);
    return count;
}

static ssize_t runAShow(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    runA = getrunA();
    return sprintf(buffer, "%d\n", runA);
}

static struct kobj_attribute runAAttr = __ATTR(runA, 0664, runAShow, runAStore);


//Run B
static int runB = 0;
module_param(runB, int, S_IRUGO);
MODULE_PARM_DESC(runB, " Run Channel B");

static ssize_t runBStore(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
    int result = kstrtouint(buffer, 0, &runB);
    if (result == 0)
        setrunB (runB);	
    return count;
}

static ssize_t runBShow(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    runB = getrunB();
    return sprintf(buffer, "%d\n", runB);
}

static struct kobj_attribute runBAttr = __ATTR(runB, 0664, runBShow, runBStore);





//----------------------------------------------------------------------------------------

// Frequency A
static int freqA = 0;
module_param(freqA, int, S_IRUGO);
MODULE_PARM_DESC(freqA, " frequency Channel A");

static ssize_t freqAStore(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
    int result = kstrtouint(buffer, 0, &freqA);
    if (result == 0)
        setfreqA (freqA);
    return count;
}

static ssize_t freqAShow(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    freqA = getfreqA();
    return sprintf(buffer, "%d\n", freqA);
}

static struct kobj_attribute freqAAttr = __ATTR(freqA, 0664, freqAShow, freqAStore);


// Frequency B	
static int freqB = 0;
module_param(freqB, int, S_IRUGO);
MODULE_PARM_DESC(freqB, " frequency Channel B");

static ssize_t freqBStore(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
    int result = kstrtouint(buffer, 0, &freqB);
    if (result == 0)
        setfreqB (freqB);
    return count;
}

static ssize_t freqBShow(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    freqA = getfreqB();
    return sprintf(buffer, "%d\n", freqB);
}

static struct kobj_attribute freqBAttr = __ATTR(freqB, 0664, freqBShow, freqBStore);
//*******************************************************************


// Amplitude A
static int amplA = 0;
module_param(amplA, int, S_IRUGO);
MODULE_PARM_DESC(amplA, " Amplitude Channel A");

static ssize_t amplAStore(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	float temp = 0;
    int result = strtod(buffer, &temp);
	amplA = temp * 10;
    if (result == 0)
        setAmplitudeA (amplA);
    return count;
}

static ssize_t amplAShow(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    amplA = getAmplitudeA();
    return sprintf(buffer, "%d\n", amplA);
}

static struct kobj_attribute amplAAttr = __ATTR(amplA, 0664, amplAShow, amplAStore);




// Amplitude B
static int amplB = 0;
module_param(amplB, int, S_IRUGO);
MODULE_PARM_DESC(amplB, " Amplitude Channel B");

static ssize_t amplBStore(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
    int result = kstrtouint(buffer, 0, &amplB);
    if (result == 0)
        setAmplitudeB (amplB);
    return count;
}

static ssize_t amplBShow(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    amplB = getAmplitudeB();
    return sprintf(buffer, "%d\n", amplB);
}

static struct kobj_attribute amplBAttr = __ATTR(amplB, 0664, amplBShow, amplBStore);

//__________________________________________________________________________________


// offset A
static int offsetA = 0;
module_param(offsetA, int, S_IRUGO);
MODULE_PARM_DESC(offsetA, " offset Channel A");

static ssize_t offsetAStore(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
    int result = kstrtouint(buffer, 0, &offsetA);
    if (result == 0)
        setoffsetA (offsetA);
    return count;
}

static ssize_t offsetAShow(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    offsetA = getoffsetA();
    return sprintf(buffer, "%d\n", offsetA);
}

static struct kobj_attribute offsetAAttr = __ATTR(offsetA, 0664, offsetAShow, offsetAStore);


// offset B
static int offsetB = 0;
module_param(offsetB, int, S_IRUGO);
MODULE_PARM_DESC(offsetB, " offset Channel B");

static ssize_t offsetBStore(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
    int result = kstrtouint(buffer, 0, &offsetB);
    if (result == 0)
        setoffsetB (offsetB);
    return count;
}

static ssize_t offsetBShow(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    offsetB = getoffsetB();
    return sprintf(buffer, "%d\n", offsetB);
}

static struct kobj_attribute offsetBAttr = __ATTR(offsetB, 0664, offsetBShow, offsetBStore);

//-----------------------------------------------------------------------------------------------
// dutycycle A

static int dutycycA = 0;
module_param(dutycycA, int, S_IRUGO);
MODULE_PARM_DESC(dutycycA, " dutycycle Channel A");

static ssize_t dutycycAStore(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
    int result = kstrtouint(buffer, 0, &dutycycA);
    if (result == 0)
        setdutycyleA (dutycycA);
    return count;
}

static ssize_t dutycycAShow(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    dutycycA = getdutycyleA();
    return sprintf(buffer, "%d\n", dutycycA);
}

static struct kobj_attribute dutycycAAttr = __ATTR(dutycycA, 0664, dutycycAShow, dutycycAStore);

// dutycycle B

static int dutycycB = 0;
module_param(dutycycB, int, S_IRUGO);
MODULE_PARM_DESC(dutycycB, " dutycycle Channel B");

static ssize_t dutycycBStore(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
    int result = kstrtouint(buffer, 0, &dutycycB);
    if (result == 0)
        setdutycyleB (dutycycB);
    return count;
}

static ssize_t dutycycBShow(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
    dutycycB = getdutycyleB();
    return sprintf(buffer, "%d\n", dutycycB);
}

static struct kobj_attribute dutycycBAttr = __ATTR(dutycycB, 0664, dutycycBShow, dutycycBStore);




// Attributes

static struct attribute *attrs0[] = {&modeAAttr.attr,&freqAAttr.attr, &amplAAttr.attr,&offsetAAttr.attr,&dutycycAAttr.attr , NULL};
static struct attribute *attrs1[] = {&modeBAttr.attr,&freqBAttr.attr, &amplBAttr.attr,&offsetBAttr.attr,&dutycycBAttr.attr , NULL};
static struct attribute *attrs2[] = {&runAAttr.attr,&runBAttr.attr, NULL};

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

static struct attribute_group group2 =
{
    .name = "run",
    .attrs = attrs2
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
	{
        return result;
	}

    result = sysfs_create_group(kobj, &group1);
    if (result !=0)
	{
        return result;
	}
	
	result = sysfs_create_group(kobj, &group2);
    if (result !=0)
	{
        return result;
	}
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
