#include "types.h"
#include "user.h"



int main(int argc, char *argv[]){
    printf(1, "hello\n");
    char *p = 0;
    printf(1, "--->%p<--->%c<---\n", *p, *p);
    printf(1, "p: %c %x\n", *p, p);
    char x = 'c';
    char y = 'd';
    printf(1, "c: %c %x\n", x, &x);
    printf(1, "d: %c %x\n", y, &y);
    char *pp = 0x2fcf; //should point to x
    *pp = 'y';
    printf(1, "c: %c %x\n", x, &x); 
    exit();
}
