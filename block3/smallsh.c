#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>

#define  MAXLENGTH 2048
#define  MAXARG 512
//globals
char temp[MAXLENGTH];
int background = 1; // variable to check if the background is occupied
int status = 0;
// struct thats holds all the pieces of the command line
struct command{
char *commandline;
char *inputfile;
char *outputfile;
int bg;
char *symbol1;
char *symbol2;
char *cmd;
char *arg;
int timer;
};
//declare func
struct command *doublemoney(struct command *cmd);

//get shell satus prints the exit status 
void shellstat(int stat){
    //printf("status\n");
    if(WIFEXITED(stat)){
            printf("exit value %d\n", WEXITSTATUS(stat));
        }else{
            printf("terminated by signal %d\n", WTERMSIG(stat));
            fflush(stdout);
        }
    }


//exits the program
void exitProg(){
    printf("goodbye \n");
    exit(1);
}
// gets the users input
char* getuserinput(){
    char *userinput = NULL;
    size_t maxlength =0;
    char *input;
    printf(": ");
    fflush(stdout);
    getline(&userinput, &maxlength, stdin);
    
    
    return userinput;
}
// intializes the struct
struct command *intializestruct( struct command *intstruct){
    //struct command *intstruct = malloc(sizeof(struct command));
    intstruct->bg =0;
    intstruct->timer = 0;
    intstruct->commandline = NULL;
    intstruct->inputfile = NULL;
    intstruct->outputfile = NULL;
    intstruct->symbol1 = NULL;
    intstruct->symbol2 = NULL;
    intstruct->cmd = NULL;
    intstruct->arg = NULL;
    
}

// breaks apart in the command into the instructions required
struct command* processcmdline(struct command *cmd, int status, pid_t forkingid){
        char *saveptr;
        char *saveptr2;
        int strlength;  //length of the string
        char argarr[256];//made an arr to hold the cmdline
        
      

        cmd->timer = 0; // a variable used to hold int variables we convert from a string
       
        strlength =strlen(cmd->commandline); // gets the totla length of the command line 
        char *token = strtok_r(cmd->commandline, " \n", &saveptr); // get the 1st command
        //char *token = strtok(cmd->commandline, " ");
        cmd->cmd = calloc(strlen(token), sizeof(char));
        //printf("token %s \n", token);
        strcpy(cmd->cmd, token); // set the 1st command
        
        // loops through the commandline un till we reach the end of the line
        while(token != NULL ){
            //if else statement to compare the cmd received 
             if ((strncmp(cmd->cmd, "ls", 2) == 0) && strlength == 3){
                    //printf("elsels\n");
                    char *newarg[] = {"/bin/ls", NULL};
                    execvp(newarg[0], newarg);
                }
            // checks to see if theres anything after the ls for redirection
            else if((strncmp(cmd->cmd, "ls", 2) == 0) && strlength > 3){
                //printf("inls\n");
                token = strtok_r(NULL, " \n", &saveptr); 
                cmd->symbol1 = calloc(strlen(token), sizeof(char));
                strcpy(cmd->symbol1, token);
               
               
                if(strlength > 3){
                   // int i =0;
                    char *token2 = strtok_r(NULL, " \n", &saveptr);
                    
                   while(token2 != NULL){
                        //output
                        if(strcmp(cmd->symbol1,">")== 0){
                            //printf("in>\n");
                            //char *token2 = strtok_r(token, " \n", &saveptr);
                            cmd->outputfile = calloc(strlen(token), sizeof(char));
                            strcpy(cmd->outputfile, token2);
                            //printf("output %s\n", cmd->outputfile);
                            //opens the file with a fd to use in dup2
                            int oput = open(cmd->outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0666); 
                                if(oput == -1){
                                    perror("Can't open file\n");
                                    exit(1);
                                }
                            int oputsuccess = dup2(oput, 1);
                                if(oputsuccess == -1){
                                    perror("Can't copy file\n");
                                    exit(2);
                                }
                        }
                        //input
                       
                        else{
                            //printf("an arg %s \n", token2);
                            cmd->arg = calloc(strlen(token), sizeof(char));
                            strcpy(cmd->arg, token2);
                        }
                        
                        token2 = strtok_r(NULL, " \n", &saveptr);
                       // i++;
                       // prints the ls to a file 
                        char *newarg[] = {"/bin/ls", cmd->inputfile ,NULL};
                        execvp(newarg[0], newarg);
                    }
                   
                    //
                }
               
            }
            //cat prints the content of the arguement
            else if(strcmp(cmd->cmd, "cat") == 0){
                    token = strtok_r(NULL, " \n", &saveptr);
                    //printf("cmd cmd %s len %d\n", cmd->cmd, strlength);
                    //printf("t4 %s\n", token);
                    cmd->symbol1 = calloc(strlen(token), sizeof(char));
                    strcpy(cmd->symbol1, token);
                    char *newarg[] = {"/bin/cat", cmd->symbol1 ,NULL};
                    execvp(newarg[0], newarg);
                }
            //wc counts the number lines, words and characters
            else if(strcmp(cmd->cmd, "wc") == 0){
                int iput, oput;
               
                token = strtok_r(NULL, " \n", &saveptr);
                cmd->symbol1 = calloc(strlen(token), sizeof(char));
                strcpy(cmd->symbol1, token);
                char *strcpptr; // ptr to a string character
                char greaterth = '>'; // the char we are looking for
                strcpptr = strchr(argarr, greaterth); 
                //printf("strpt %s, cmdline %s, %c\n", strcpptr, cmd->commandline, greaterth);
                    
                    if(strcpptr != NULL){
                        int i =0;
                        char *token2 = strtok_r(NULL, " \n", &saveptr);
                      
                        do{
                            // handles wc if theres multiple redirections
                            //output set the next arguement after > to the files name
                            if(strcmp(cmd->symbol1,">")== 0){
                                
                                cmd->outputfile = calloc(strlen(token), sizeof(char));
                                strcpy(cmd->outputfile, token2);
                            }
                            //input set the next arguement after < to the file name
                            else if(strcmp(cmd->symbol1, "<") == 0){
                              
                                    cmd->inputfile = calloc(strlen(token), sizeof(char));
                                    strcpy(cmd->inputfile, token2);
                                   // printf("input %s\n", cmd->inputfile);
                                    token2 = strtok_r(NULL, " \n", &saveptr);
                            }
                                                                           
                            // sets the symbol1 to the other symbol
                                if(strcmp(token2, ">") == 0 ){
                                    strcpy(cmd->symbol1, token2);
                               }
                         // moves token       
                         token2 = strtok_r(NULL, " \n", &saveptr);
                        // if both files are filled handle the opening of files and set up the dup2
                         if(cmd->inputfile != NULL && cmd->outputfile != NULL){
                            // printf("2 newafter if in: %s, out: %s\n", cmd->inputfile, cmd->outputfile);
                            iput = open(cmd->inputfile, O_RDONLY);
                            
                            //fflush(stdout);  
                             if(iput == -1){
                                    perror("Can't open file\n");
                                    exit(2);
                                }
                           
                            int iputsuccess = dup2(iput, 0);
                                if(iputsuccess == -1){
                                    perror("Can't copy file\n");
                                    exit(1);
                                }
                              
                            oput = open(cmd->outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                                if(oput == -1){
                                    perror("Can't open file\n");
                                    exit(1);
                                }
                                
                            int oputsuccess = dup2(oput, 1);
                            
                                if(oputsuccess == -1){
                                    perror("Can't copy file\n");
                                    exit(2);
                                }
                           // excute the wc command and set results to the input file
                            char *newarg[] = {"/bin/wc", cmd->inputfile, NULL};

                            execvp(newarg[0], newarg);
                            fcntl(oput, F_SETFD, FD_CLOEXEC);
                            fcntl(iput, F_SETFD, FD_CLOEXEC);  
                         }
                        } while(token2 != NULL);

                        
                    // if wc ony has one arguement
                    }else{
                       // printf("not in >\n"); 
                        char *token2 = strtok_r(NULL, " \n", &saveptr);
                        //printf("t2\n", token2);
                        cmd->inputfile = calloc(strlen(token), sizeof(char));
                        strcpy(cmd->inputfile, token2);
                        //printf("input %s\n", cmd->inputfile);
                        iput = open(cmd->inputfile, O_RDONLY); 
                            if(iput == -1){
                                perror("Can't open file\n");
                                exit(2);
                            }
                        int iputsuccess = dup2(iput, 0);
                             if(iputsuccess == -1){
                                perror("Can't copy file\n");
                                    exit(1);
                            }  
                            //printf("check baby");
                            char *newarg[] = {"/bin/wc", cmd->inputfile, NULL};
                            execvp(newarg[0], newarg);      
                            fcntl(iput, F_SETFD, FD_CLOEXEC);
                    }
          
                }
                   
            // print process information
            else if(strcmp(cmd->cmd, "ps") == 0){
                //printf("inwps\n");
                char *newarg[] = {"/bin/ps", NULL};
                execvp(newarg[0], newarg);
            }
            //test used to check file  types and compare values
            else if(strcmp(cmd->cmd, "test") == 0){
                    //printf("ls\n");
                    FILE *file; // hold a file name

                    token = strtok_r(NULL, " \n", &saveptr);
                    cmd->symbol1 = calloc(strlen(token), sizeof(char));
                    strcpy(cmd->symbol1, token);
                    token = strtok_r(NULL, " \n", &saveptr);
                    //printf("t5 %s\n", token);
                    cmd->arg = calloc(strlen(token), sizeof(char));
                    strcpy(cmd->arg, token);
                  
                    //printf("arg %s\n", cmd->arg);
                    char *newarg[] = {"/bin/test",cmd->symbol1, cmd->arg ,NULL};
                    execvp(newarg[0], newarg);
                    //status = 1;
                    exit(1);
            }
            //sleep
            else if(strcmp(cmd->cmd, "sleep") == 0){
                //printf("sleep\n");
                token = strtok_r(NULL, " \n", &saveptr);
                //printf("cmd cmd %s len %d\n", cmd->cmd, strlength);
                //printf("t4 %s\n", token);
                cmd->symbol1 = calloc(strlen(token), sizeof(char));
                strcpy(cmd->symbol1, token);
                cmd->timer = atoi(cmd->symbol1);
                //printf("timer: %d ", cmd->timer);
                char *newarg[] = {"/bin/sleep",cmd->symbol1, NULL};
                execvp(newarg[0], newarg);
                exit(0);
            }
            //kills a pid command
            else if(strcmp(cmd->cmd, "kill") == 0){
                //printf("ls\n");
                token = strtok_r(NULL, " \n", &saveptr);
                    //printf("cmd cmd %s len %d\n", cmd->cmd, strlength);
                    //printf("t4 %s\n", token);
                    cmd->symbol1 = calloc(strlen(token), sizeof(char));
                    strcpy(cmd->symbol1, token);
                    token = strtok_r(NULL, " \n", &saveptr);
                    while(token != NULL){
                        //printf("while toke %s", token);
                        strcpy(cmd->arg, token);
                        token = strtok_r(NULL, " \n", &saveptr);
                    }

                    char *newarg[] = {"/bin/kill", cmd->symbol1, cmd->arg ,NULL};
                    execvp(newarg[0], newarg);
            }
            //pwd prints current direct
            else if(strcmp(cmd->cmd, "pwd") == 0){
                //printf("ls\n");
                char *newarg[] = {"/bin/pwd", NULL};
                execvp(newarg[0], newarg);
            }
            //echos last entry
            else if(strcmp(cmd->cmd, "echo") == 0){
                //printf("ls\n");
                token = strtok_r(NULL, " \n", &saveptr);
              
                cmd->symbol1 = calloc(strlen(token), sizeof(char));
                strcpy(cmd->symbol1, token);
                char *newarg[] = {"/bin/echo", cmd->symbol1, NULL};
                execvp(newarg[0], newarg);
            }
            //date prints date
            else if(strcmp(cmd->cmd, "date") == 0){
                //printf("date\n");
                char *newarg[] = {"/bin/date", NULL};
                execvp(newarg[0], newarg);

            }
           
            // create a directory
            else if(strcmp(cmd->cmd, "mkdir") == 0){
                char currdir[256];
                token = strtok_r(NULL, " \n", &saveptr);
               
                cmd->symbol1 = calloc(strlen(token), sizeof(char));
                strcpy(cmd->symbol1, token);
                char *newarg[] = {"/bin/mkdir", cmd->symbol1, NULL};
                execvp(newarg[0], newarg);
                //printf("%s\n", getcwd(currdir, 100));
            }
            //
            else if(strcmp(cmd->cmd, "pkill") == 0){
                token = strtok_r(NULL, " \n", &saveptr);
              
                cmd->symbol1 = calloc(strlen(token), sizeof(char));
                strcpy(cmd->symbol1, token);
                char *newarg[] = {"/bin/pkill", cmd->symbol1, NULL};
                execvp(newarg[0], newarg);
            }
            
            else{
                printf("%s: invalid command \n", cmd->cmd);
            //break;
            }
        token = strtok_r(NULL, " \n", &saveptr);
    }
}
// all the built in function calls
struct command *builtin(struct command *cmd){
        char *saveptr;
        char *saveptr2;
        int strlength;
        char argarr[256];
        //int stat;
        int x;
        for(x =0; x < MAXLENGTH; x++){
            temp[x] = '\0';
        }
    
        cmd->timer = 0;
        strlength =strlen(cmd->commandline)+1;
        char *token = strtok_r(cmd->commandline, " \n", &saveptr);
        cmd->cmd = calloc(strlen(token), sizeof(char));
        //printf("token %s \n", token);
        strcpy(cmd->cmd, token);
        
        while(token != NULL ){
            //exits the program
            if(strcmp(cmd->cmd, "exit") == 0){
                        //printf("in exit\n");
                        exitProg();
                    }
                    //gets and prints exit status
                    else if(strcmp(cmd->cmd, "status") == 0){
                        //printf("in status\n");
                        shellstat(status);
                    }
                    //change the directory if one isnt given changes it to home dir
                    else if(strcmp(cmd->cmd, "cd") == 0){
                        //printf("in cd\n");
                        char currdir[256];
                        char *dirname = strtok_r(NULL, " \n", &saveptr);
                        //printf("dir %s\n", dirname);
                        if(dirname != NULL){ 
                            //chdir(dirname);
                            if(chdir(dirname) == -1){ //see if dir was found
                                printf("Dir not found\n");
                                fflush(stdout);
                                }
                        }else{
                            chdir(getenv("HOME"));
                        }
                        //printf("%s\n", getcwd(currdir, 100));
                    }
               token = strtok_r(NULL, " \n", &saveptr);     
        }
}
// handles the sigstp action
void catchSIGTSTP(){
    if (background == 1){ // check to see if background is open
        write(1, "Entering foreground-only mode(& is now ignored).\n", 50);
        fflush(stdout);
        background = 0; // close background
    }else{
        write(1, "Exiting foreground-only mode\n", 29);
        fflush(stdout);
        background = 1; // opens background back up
    }
}
// expand the $$ into the pid and replaces $$ with the pid whereever it is in the commandline
struct command *doublemoney(struct command *cmd){

     char *pid = calloc(sizeof(char), MAXLENGTH);
                char *saveptr;
                sprintf(pid, "%d", getpid());
                //printf("%s\n", pid);
                char *token = strtok_r(cmd->commandline, "$$", &saveptr);
                cmd->cmd = calloc(strlen(token), sizeof(char));
                //printf("token %s \n", token);
                strcpy(cmd->cmd, token);
                token = strtok_r(NULL, "$\n", &saveptr);
                    // sees if theres more after $$ then combines the pid and strings
                    if (token != NULL){
                        strcat(cmd->cmd, pid);
                        cmd->arg = calloc(strlen(token), sizeof(char));
                        strcpy(cmd->arg, token);
                        strcat(cmd->cmd, cmd->arg);
                       // printf("$$t %s\n", cmd->cmd);
                    }
                    //checks to see where the $$ is if at the beginning
                    else if(strncmp("$", cmd->commandline, 1) == 0){
                         strcat(pid, cmd->cmd);
                    
                    }else{
                        strcat(cmd->cmd, pid);
                    }
               // printf(" %s, %s , %s\n", pid, cmd->cmd, cmd->arg);
                    strcpy(cmd->commandline, cmd->cmd);
                   
                    printf(cmd->commandline);
                    printf("\n");
            return cmd;
}

int main(int argc, char *argv[]){
    struct command *cmd = malloc(sizeof(struct command));
    //int status = 0;
    int spawnId ; 
    char *userInput;
    char *tokptr;
    int getlinecheck;
    size_t inputsize = MAXLENGTH;
    int cmdlength = 0;
    int loopexit = 0;

    //ben brewster lecture 3.3 slides 34
    //https://www.youtube.com/watch?v=VwS3dx3uyiQ
    struct sigaction sigint = {0};
    sigint.sa_handler = SIG_IGN; // ignore signal
    sigfillset(&sigint.sa_mask); //block/delay all sigs while mask is in place
    sigint.sa_flags = 0;
    sigaction(SIGINT, &sigint, NULL);

    struct sigaction sigtstp = {0};
    sigtstp.sa_handler = catchSIGTSTP;
    sigfillset(&sigtstp.sa_mask);
    sigtstp.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sigtstp, NULL);


    //ben brewster
    //https://www.youtube.com/watch?v=VwS3dx3uyiQ
    //https://www.youtube.com/watch?v=1R9h-H2UnLs  ben brewster lecture 3.1
    printf("Shell Program \n");
    intializestruct(cmd);

do{
    //printf("\n");
    cmd->commandline = getuserinput(); // gets the pnput from the user and stores it in the struct
    // looks for the $$ input to expand in the pid and replaces the $$
       if(strstr(cmd->commandline, "$$") != NULL){
               cmd = doublemoney(cmd);
               //printf("cmd %s \n", cmd->commandline);
            }
    cmdlength =strlen(cmd->commandline); // get length of the command line
    //printf("length %d\n", cmdlength);
    strncpy(temp, cmd->commandline, cmdlength); // copies the cmdline to a temp the size of the length
    // checks for the background command if it not at the beginning
    if(cmdlength > 2){
        char *ptr = strrchr(cmd->commandline, '&');
       //sets background to the proper variable to see if its opne or closed
        if(ptr != NULL && strlen(ptr) <= 2){
            background =1;
            cmd->bg =1 ;
        }else{
            
           // background = 0;
            cmd->bg = 0;
        }
        
    }
    //printf("find the # %s\n", cmd->commandline[cmdlength-2]);
    // lloks for the built in commands then executes the function
    if (strstr(cmd->commandline, "exit") || strstr(cmd->commandline, "cd" ) || strstr(cmd->commandline, "status" )){
            builtin(cmd);
    }
    // if # is at the start ski comments 
    else if(strncmp(cmd->commandline, "#", 1) == 0){
       printf("\n");
    }
       
    else{
       //processcmdline(cmd, status);
        pid_t forkpid = -73;
        forkpid = fork();
        int forkcount; // variable to prevent a run away forking call
        switch (forkpid){
            forkcount++;
                if(forkcount > 50){ // count to prevent program from getting out of control
                    printf(" forking Abort\n");
                    abort();
                }
        case -1:{
            perror("it forking failed\n");
            exit(1);
            break;
        }
        //child
        case 0: { //
            //printf("in child\n");
            sigint.sa_handler = SIG_DFL; // default action for signal
            sigaction(SIGINT, &sigint, NULL);
            processcmdline(cmd, status,forkpid);
            exit(1);
            break;
        }
        //parent
        default:{
            //printf("in parent\n");
             if(cmd->bg == 1){ // checks to see if the bg is already running 
                pid_t pid = waitpid(forkpid, &status, WNOHANG); // blocks parent until specified child is terminated
                printf("background pid is %d\n", forkpid);
                fflush(stdout);
            }else{
                pid_t pid = waitpid(forkpid, &status, 0); // blocks parent until specified child is terminated
            }
            while((forkpid = waitpid(-1, &status, WNOHANG)) > 0){ // terminates zombies
                printf("child %d terminated\n", forkpid);
                shellstat(status);
                fflush(stdout);
            }
            break;
        }
    }
    }
} while(loopexit != 1);
    return 0;
}

