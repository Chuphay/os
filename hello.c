#include "types.h"
#include "user.h"
#include "stat.h"

int
main(void)
{

    printf(1, "Hello\n");
    int x1 = getreadcount();
    printf(1, "%d\n", x1);
    x1 = getreadcount();
    int x2 = getreadcount();
    char buf[100];
    (void) read(4, buf, 1);
    int x3 = getreadcount();
    int i;
    for (i = 0; i < 1000; i++) {
        (void) read(4, buf, 1);
    }
    int x4 = getreadcount();
    printf(1, "XV6_TEST_OUTPUT %d %d %d\n", x2-x1, x3-x2, x4-x3);



    x1 = getreadcount();

    int rc = fork();

    int total = 0;
    for (i = 0; i < 100000; i++) {
        char buf[100];
        (void) read(4, buf, 1);
    }
    // https://wiki.osdev.org/Shutdown
    // (void) shutdown();

    if (rc > 0) {
        (void) wait();
        x2 = getreadcount();
        total += (x2 - x1);
        printf(1, "XV6_TEST_OUTPUT %d\n", total);
    }

    exit();
}
