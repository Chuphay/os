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

void redirection(char * command, char **arguments, char * file_name){
    //This is the answer that actually worked: 
    //https://stackoverflow.com/questions/29154056/redirect-stdout-to-a-file
    int fp = open(file_name, O_CREATE | O_RDWR);
    int rc = fork(); 
    if (rc == 0) { 
        close(1);
        dup(fp);
        exec(command, arguments);
        printf(2, "exec failed :(\n");
        exit(); //if it failed, we should exit out of this process
    } else { 
        rc = wait();
        close(fp);   
    }
}


void execute(int n, char **arguments){
    int i;
    int m; //number to store length of first 
    int redirect= 0;
    char *myargv[n+1];
    
    for (i = 0; i < n; i++){
        if (strcmp(arguments[i], ">") == 0){
            printf(1, "redirection!\n");
            if (i != n - 2){
                printf(1, "syntax for redirection should be something > file_name\n");
                exit();
            }
            redirect = 1;
            break;
        }
        myargv[i] = arguments[i];
    }
    myargv[n] = 0;

    m = strlen(myargv[0]);
    char mainarg[m+2]; //+1 to hold the '/' and +1 to hold the 0
    mainarg[0] = '/';
    for (i = 0; i < m; i++){
        mainarg[i+1] = myargv[0][i];
    }
    mainarg[m+1] = 0;

    if (redirect == 1){
        redirection(mainarg, myargv, arguments[n-1]);
        printf(1, "end of redirection\n");
        exit();
    } else {
        printf(1, "mainarg: %s\n", mainarg);
        exec(mainarg, myargv);   
        printf(2, "exec failed :(\n");
        exit(); //if it failed, we should exit out of this process
    }
}

void loop(char * buf, char **arguments){
    int n = args(buf, arguments);
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
            execute(n, arguments); 
        } else { //parent process
            rc = wait();
        }
    }
}


int main(int argc, char * argv[]){

    printf(1, "Welcome to %s!\n", argv[0]);
    char buf[BUFF_SIZE]; 
    char *arguments[MAX_ARGS];
    for (int i = 0; i < MAX_ARGS ; i++){
        //allocate for the duration of the program
        arguments[i] = (char *) malloc((BUFF_SIZE/MAX_ARGS) * sizeof(char));
    }

    while (1) {

        printf(1, "wish> "); //prompt
        loop(buf, arguments);

    }

    exit();
} 
