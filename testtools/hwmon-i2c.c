#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>

int main(int argv, char ** argc)
{
    int err = 0;
    int hfd = -1;
    int i2cfd = -1;
    int infd =-1;
    char buf[64] = {0};

    system("chmod 777 /sys/class/hwmon/hwmon3/device/name");
    hfd = open("/sys/class/hwmon/hwmon3/device/name", O_RDWR);

    if(hfd < 0) {
        printf("hwmon err\n");
        return 0;
    }

    err = read(hfd, buf, 64);
    if (err <= 0){
        printf("read hwmon err!\n");
    }

    printf("hwmon driver name is %s", buf);
    close (hfd);




    return 0;
}
