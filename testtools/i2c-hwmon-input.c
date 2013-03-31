#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>


#define MISC_SYS_PATH "/sys/class/misc/misc_example"
#define MISC_IOCTL_PATH "/dev/misc_example"
#define MISC_IOC_MAGIC 'k'
#define MISC_IOCW   _IOW(MISC_IOC_MAGIC, 1, int)
#define MISC_IOCR   _IOR(MISC_IOC_MAGIC, 2, int)



int main(int argv, char ** argc)
{
    int i;
    int fd = 0;
    int err = 0;
    int read_from_driver = 0;
    int wirte2driver = 21;
    fd = open(MISC_IOCTL_PATH, O_RDWR);
    if (fd < 0) {
        printf("too sad, file path error!\n");
        return -1;
    }

    err = ioctl(fd, MISC_IOCW, &wirte2driver);
    
    if(err < 0){
        printf("ioctl err in MISC_IOCW!\n");
        return -1;
    }

    for(i=0; i<4; i++) {
        err = ioctl(fd, MISC_IOCR, &read_from_driver);
        if(err<0){
            printf("ioctl err in MISC_IOCW!\n");
            return -1;
        }
        printf("read from driver: %d\n", read_from_driver);
    }
    close(fd);
    return 0;
}
