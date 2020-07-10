#include "types.h"
#include "user.h"
#include "stat.h"

#define BUFF_SIZE 1024
#define MAX_ARGS 8

int args(char * buf, char **output){
    int x; //Checking for EOF
    x = read(0, buf, BUFF_SIZE);
    //printf(1, "Finished reading %s\n", buf);
    int i = 0; //main loop over the whole input line
    int j = 0; //the number of space separated inputs
    int k = 0; //the length of each input
    char tmp[BUFF_SIZE]; //place to build each of the inputs

    if (x < 1){
        printf(1, "EOF reached. Exiting\n");
        exit();
    }
    for (i = 0; i < BUFF_SIZE; i++){
        
        if (buf[i] == '\n'){
            buf[i] = 0;
            tmp[k] = 0;
            output[j] = strcpy(output[j], tmp);
            break;
        } 
        if (buf[i] == ' '){
            tmp[k] = 0;
            output[j] = strcpy(output[j], tmp);
            k = 0;
            j++;
            if (j == MAX_ARGS){
                printf(1, "Error argument list too long\n");
                exit();
            }
        } else {
            tmp[k] = buf[i];
            k++;
        }
    }
    j++;
    return j;

}


int main(void){

    printf(1, "main; begin\n");
    char buf[BUFF_SIZE]; 
    int i; 
    int n; //number of arguments returned
    char *arguments[MAX_ARGS];
    for (i = 0; i < MAX_ARGS ; i++){
        arguments[i] = (char *)malloc((BUFF_SIZE/MAX_ARGS) * sizeof(char));
    }

    while (1) {
        n = args(buf, arguments);
        for (i = 0; i < n; i++){
            printf(1, "%d ", i);
            if (arguments[i] != 0){
                printf(1, "%d %s\n", i, arguments[i]); 
            } else {
                //we should never get here!
                printf(1, "ERROR\n");
            }

        }
        printf(1, "end of loop\n");

    }
    int rc = fork();
    
    if (rc == 0) { //child process
        printf(1, "In body %d %d\n", rc, getpid());
        printf(1, "I'm the child\n");
        char * ls;
        ls = "/ls";
        char *myargv[2];
        myargv[0] = ls;
        myargv[1] = 0;
        exec("/ls", myargv);   
        printf(1, "exec failed :(\n");
    } else { //parent process
        rc = wait();
        printf(1, "In body %d %d\n", rc, getpid());
        printf(1, "I'm the parent, and now rc is %d\n", rc);
    }
    exit();
} 
