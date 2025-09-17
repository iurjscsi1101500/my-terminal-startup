#include <stdio.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/sysctl.h>

int main(void) {
    struct utsname u;
    struct timeval boottime;
    size_t len = sizeof(boottime);
    time_t now, uptime;
    char buf[64];

    uname(&u);
    time(&now);

    struct tm *lt = localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S %Z", lt);

    printf("Name:   %s\n", u.nodename);
    printf("Arch:   %s\n", u.machine);
    printf("Kernel: %s\n", u.version);
    printf("Local Time: %s\n", buf);

    sysctlbyname("kern.boottime", &boottime, &len, NULL, 0);
    uptime = now - boottime.tv_sec;

    int days    = uptime / 86400;
    int hours   = (uptime % 86400) / 3600;
    int minutes = (uptime % 3600) / 60;
    int seconds = uptime % 60;

    printf("Uptime: %d days, %02d:%02d:%02d\n",
           days, hours, minutes, seconds);

    return 0;
}

