#include "types.h"
#include "user.h"
#include "stat.h"
#include "fcntl.h"

#define BUFF_SIZE 1024
#define MAX_ARGS 8

int args(char * buf, char **output){
    int x; //Checking for EOF
    int i; //main loop over the whole input line
    int j = 0; //the number of space separated inputs
    int k = 0; //the length of each input
    char tmp[BUFF_SIZE]; //place to build each of the inputs

    x = read(0, buf, BUFF_SIZE);
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

void execute(int n, char **arguments){
    int i;
    int m; //number to store length of first 
    int redirection = 0;
    char *myargv[n+1];
    
    for (i = 0; i < n; i++){
        if (strcmp(arguments[i], ">") == 0){
            printf(1, "redirection!\n");
            if (i != n - 2){
                printf(1, "syntax for redirection should be something > file_name\n");
            }
            redirection = 1;
            break;
        }
        myargv[i] = arguments[i];
    }
    myargv[n] = 0;
    if (redirection == 1){
        printf(1, "yo\n");
    }

    m = strlen(myargv[0]);
    char mainarg[m+2]; //+1 to hold the '/' and +1 to hold the 0
    mainarg[0] = '/';
    for (i = 0; i < m; i++){
        mainarg[i+1] = myargv[0][i];
    }
    mainarg[m+1] = 0;
    printf(1, "mainarg: %s\n", mainarg);
    exec(mainarg, myargv);   
    printf(1, "exec failed :(\n");
    exit(); //if it failed, we should exit out of this process
    
}
void execute2(int n, char **arguments){
    char *a[2];
    a[0] = "/ls";
    a[1] = 0;
    int fp = open("test.txt", O_CREATE | O_RDWR);
    printf(fp, "could this possibly work?\n");
    close(fp);
    
    dup(1);
    exec("/ls", a);    
}


int main(void){

    printf(1, "Welcome to Wish!\n");
    char buf[BUFF_SIZE]; 
    int i; 
    int n; //number of arguments returned from args
    
    char *arguments[MAX_ARGS];
    for (i = 0; i < MAX_ARGS ; i++){
        arguments[i] = (char *)malloc((BUFF_SIZE/MAX_ARGS) * sizeof(char));
    }

    while (1) {

        printf(1, "wish> "); //prompt
        n = args(buf, arguments);


        if (strcmp(arguments[0], "") == 0){
            printf(1, "empty string\n");
        } else if (strcmp(arguments[0], "exit") == 0){
            printf(1, "exit\n");
            exit();
        } else if (strcmp(arguments[0], "cd") == 0){
            if (n != 2){
                printf(1, "changing directories always requires an argument\n");
            } else {
                int ret_code = chdir(arguments[1]);
                printf(1, "changing directory to %s , return code: %d\n", arguments[1], ret_code);
            }
        } else {
            int rc = fork(); //we need to fork because exec eats the process
            if (rc == 0) { //child process
                execute2(n, arguments); 
            } else { //parent process
                rc = wait();
            }
        }
    }

    exit();
} 
