#include <unistd.h>

int main(void) {
    close(0);
    return open("/dev/tty",0,0);
}

