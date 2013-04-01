#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <string.h>


int main(int argv, char ** argc)
{
    int err      = 0;
    int inamefd  = -1;
    int ieventfd = -1;
    char buf[512] = {0};

    printf("Get input driver name from /sys/class/input/inputX/name");
    system("chmod 777 /sys/class/input/input17/name");
    printf(".");
    inamefd = open("/sys/class/input/input17/name", O_RDWR);
    printf(".");
    if(inamefd < 0) {
        printf("get input name  err\n");
        return 0;
    }
    printf(".\n");
    err = read(inamefd, buf, 64);
    if (err <= 0){
        printf("read input name  err!\n");
    }
    printf("input driver's name is %s", buf);
    close (inamefd);

    printf("The /dev/input/eventX's name is ");
    system("chmod 777 /dev/input/event17");
    ieventfd = open("/dev/input/event17", O_RDWR);
    if(ieventfd <0){
        printf("ieventfd err\n");
    }
    system("chmod 777 /dev/input/event17");
    ieventfd = open("/dev/input/event17", O_RDWR);
    ioctl(ieventfd,EVIOCGNAME(sizeof(buf) - 1), &buf);
    printf("%s\n", buf);


    printf("Read input event\n");
    struct pollfd ipollfd;
    ipollfd.fd = ieventfd;
    ipollfd.events = POLLIN;   

    printf("the input event's fd is %d\n", ipollfd.fd);

    while(1)
    {
        printf(".");
        poll(&ipollfd, 1 , 50000);
        printf(".");
        err = read(ipollfd.fd, buf, sizeof(buf));
        printf(".");
        if(err < 0 ){
            printf("read input err\n");
            return -1;
        }
        struct input_event *ievent = (struct input_event *)buf;
        printf(".\n");
        printf("Input driver report--> TYPE: %x, CODE: %x, VALUE: %x\n",ievent[0].type, ievent[0].code, ievent[0].value);
        printf("Input driver report--> TYPE: %x, CODE: %x, VALUE: %x\n",ievent[1].type, ievent[1].code, ievent[1].value);
        memset(buf, 0, sizeof(buf));
    }
    return 0;
}
