#include <asm/ioctl.h>

#define DRIVER_NAME "platform_driver"

/* Defining ioctl commands */
#define MAGIC_NUMBER 'G'
#define READ_IOCTL _IOR(MAGIC_NUMBER, 0, int)
#define WRITE_IOCTL _IOW(MAGIC_NUMBER, 1, int)
