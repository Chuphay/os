#include "types.h"
#include "stat.h"
#include "user.h"

//using this for the scheduler
#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"

struct pstat {
  int inuse[NPROC];   // whether this slot of the process table is in use (1 or 0)
  int tickets[NPROC]; // the number of tickets this process has
  int pid[NPROC];     // the PID of each process 
  int ticks[NPROC];   // the number of ticks each process has accumulated 
  int notusing;
};

#endif // _PSTAT_H_


int rand(int seed, int mod){
    int tmp = 1103515245 * seed + 12345; //broken but good enough
    return tmp % mod;

}

int main(int argc , char * argv[]){
    int r = rand(5, 2147483648);
    printf(1, "random: %d\n", r);
    int x1 = settickets(41);
    printf(1, "settickets: %d\n", x1);
    struct pstat s[1];
    s[0].notusing = 42;
    printf(1, "Not Using %d\n", s[0].notusing);
    x1 = getpinfo(s);
    printf(1, "getpinfo: %d\n", x1);




    if (argc != 3){
        printf(1, "proper usage: spin [NUM] [TICKETS]\n");
        exit();
    }
    // printf(1, "spinning\n");
    int n = atoi(argv[1]);
    x1 = settickets(argv[2]);
    printf(1, "settickets: %d\n", x1);
    int i, m = 0;
    for (i = 0; i < n; i++){
        m += i;
    }
    printf(1, "\nspin [%d] gets to %d\n", getpid() ,m);
    exit();
}
   


