/*Justin Phillips 
phillij6@oregonstate.edu
cs344 assignment 2 files
winter 2021
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

#define PREFIX "movies_"
#define SUFFIX "csv"
#define ID "phillij6"

//Global
int count = 0;

struct movie{
    char *title;
    int year;
    char *languages;
    float rating;
    struct movie *next;
};



struct movie *createMovie(char *currLine){
    struct movie *currentMovie = malloc(sizeof(struct movie));
    char *saveptr;

    // get the title of the movie
    char *token = strtok_r ( currLine, ",", &saveptr);
    currentMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currentMovie->title, token);

    // get the year of the movie
    token = strtok_r(NULL, ",", &saveptr);
    int yr = atoi(token);
    currentMovie->year = yr;
    
    //get the languages the movie is in 
    token = strtok_r( NULL, ",", &saveptr);
    currentMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currentMovie->languages, token);
    
    //get the rating of the movie
    token = strtok_r( NULL, "\n", &saveptr);
    float rate = strtof(token, NULL);
    currentMovie->rating = rate;

    currentMovie->next = NULL;
    //printMovie(currentMovie);
    return currentMovie;
           
}

struct movie *processFile(char *filePath){
    printf("Processed file %s\n ", filePath);
    //printf("the file is processing");
    FILE *movieFile = fopen(filePath, "r"); // open and read the file passed in
    count = 0;//reset the count each call to process the file
    int random = 0; 
    char dirpath[256];
   

    //check to see if the file openned successfully
    if (movieFile == NULL){
        printf("failed to open file\n");
        exit(0);
    }

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;

    struct movie *head = NULL;
    struct movie *tail = NULL;
    //printf("step check\n");
    while((nread = getline(&currLine, &len, movieFile)) != -1){
       
        struct movie *newNode = createMovie(currLine);
        //printf("newnode %s\n" ,newNode->title);
        count++; 
       // printf("count %d\n", count);
        if(head == NULL){
            head = newNode;
            tail = newNode;
           
        }else{
            // printf("newnode %s\n" ,newNode->title);
            tail->next = newNode;
            tail = newNode;
        }
    }
     
    free(currLine);
    fclose(movieFile);
    //printf("Processed file %s and parsed data for %i movies\n ", filePath, count);
    printf("Now processing the chosen file named %s \n", filePath);
    char currdir[100]; // string i made to track the cwd
    char randstr[6]; // string to hold the converted int
    //srand(time(0));
    random = rand() % ((99999+1) -0) +0;
    sprintf(randstr, "%d",random); // convert int to string
    strcpy(dirpath, ID);            //steps to create the directory
    strcat(dirpath, ".movies.");
    strcat(dirpath, randstr);
    printf("Created directory with name %s\n", dirpath);
    int check = mkdir(dirpath, 0750);

    if(!check){
        printf("Dir created successfully\n");
        printf("\n");
    }else{
        printf("dir unsuccessfully created\n");
         printf("\n");
    }
    chdir(dirpath);     //change dir to the newly created dir
    //printf("%s\n", getcwd(currdir, 100));
    
    filePath = '\0';  //set fileplath to null
   
    return head;
}

// function to print and create to a file
void printtofile(char *filepath){  
    //printf("You entered 1\n");
    int yearIn = 0; //holds the current year for the file
    int i = 0;
    int tracker= 0; // variable to track how many movies were in that year
    FILE *fp;       // file pointer
    char intconv[4];    //string to hold converted int
    char fname[256];    // dtring to hold the file name that is created
    char movtitle[256];
    DIR* currDir = opendir(".");    //open the current dir
    struct dirent *opDir;           //pointer for dir entries
    struct stat dirStat;            // struct for stats in the dir
    struct movie *list = processFile(filepath);
               
        while (list->next != NULL){
        // printf("list title %s, %d, yr: %d\n", list->title, list->year, yearIn);
            memset(fname,'\0', sizeof(fname));
            yearIn = list->year;
            i++;
            // printf("%s\n", list->title);
            sprintf(intconv, "%d",yearIn);      //convert int to a string
            strcat(intconv, ".txt");
            strcat(fname, intconv);         //combine strings to get full filename

            //loop through the dir searching to see if file is there  
                while((opDir = readdir(currDir)) != NULL){
                    //printf("cd1 %s\n", list->title);
                   // printf("dirname %s, fname %s\n", opDir->d_name, fname);
                // if filename matches print in that file   
                    if( strcmp(opDir->d_name, fname) == 0){
                        //printf("cd %s\n", list->title);
                        fp = fopen(fname, "a+" );  // open file with correct permissions
                        fprintf(fp,"%s\n", list->title); // print the title to the file
                    }
                }
                tracker++; // inc tracker
                //printf("print %s, 2. %s\n", list->title, movtitle);
                fp = fopen(fname, "a+" );   // open file with correct permissions
                fprintf(fp,"%s\n", list->title);        // print the title to the file
                list = list->next; // move nodes
                
            }
            fclose(fp);     //close file
            char dirch[256]; // string to hold cwd used for debugging
            //printf("%s\n", getcwd(dirch, 100));
                if(tracker == 0){
                    printf("No movies found in that year\n\n");
                }
       // return 0;
    }


int main(int argc, char *argv[]){
    int input; //holds input selection from user
    int input2;
    DIR* currDir = opendir(".");  // pointer to current dir
    struct dirent *aDir;
    off_t sizeoffile;       //hold the size of the files
    struct stat dirStat;       //struct to hold file stats
    int i =0;
    int flg = 1;        //variable to hold value to exit loop
    char name[256];     // holds the name of the file
    char name2[256];
    char fullfilename[256];  // the full filename used to break up using tokens
    char fileex[256];       // holds the file extension
    char exten[256];
    srand(time(0));     //seeds the rand to get unique random numbers
   
        //struct movie *list = processFile(argv[1]);
            while(true){
               
                printf("1. Select file to process\n");
                printf("2. Exit the program\n");
                printf(" Enter a choice 1 or 2:");
                scanf("%d", &input);
                printf("\n");
                //printf("entered %d\n", input);
               
                if (input == 1){
                    printf(" Which file you want to process\n");
                    printf("Enter 1 to pick the largest file\n");
                    printf("Enter 2 to pick the smallest file\n");
                    printf("Enter 3 to specify the name of a file\n");
                    printf("Enter a choice from 1 to 3: ");
                    scanf("%d", &input2);
                    printf("\n");
                    flg=1;
                    while(flg == 1){

                    // user selected to find the largest file   
                    if(input2 == 1){
                        //printf("In 1\n");
                        DIR* currDir = opendir(".");  //opens current dir to look through
                        if (currDir == NULL){
                            printf("Can't open that file\n");
                            exit(0);
                        }

                        off_t tempSize = 0;
                        while((aDir = readdir(currDir)) != NULL){
                            //printf("dir %s \n", currDir);
                            // compares string prefix and file names based on the length of the prefix
                            if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0){
                                stat(aDir->d_name, &dirStat);       //find the file name
                                //printf("In 3\n");
                                memset(fullfilename, '\0', sizeof(fullfilename));
                                strcpy(fullfilename, aDir->d_name); // copy name to break up
                                //use token to get a file ex.
                                memset(fileex, '\0', sizeof(fileex));
                                char* token = strtok(fullfilename,".");     // divide file to get the extension
                                //printf("token1 %s\n", token);
                                memset(exten, '\0', sizeof(exten));
                                strcpy(exten, token);
                                token = strtok(NULL, "\0");
                                strcpy(fileex, token);
                                //printf(" fileex %s \n", fileex);
                                //printf("suff %s \n", SUFFIX);
                    //compare the extension with our defined suffix to find the largest file
                            if(strcmp(fileex, SUFFIX) == 0){

                                if(dirStat.st_size > tempSize ){
                                    //printf("In 4\n");
                                    tempSize = dirStat.st_size;     //set the file size to a variable
                                    memset(name, '\0', sizeof(name));
                                    strcpy(name, aDir->d_name);
                                   //printf("The largest file is %s \n", name);
                                }
                            }
                            }
                            
                        }
                        printf("The largest file is %s \n", name);
                        //processFile(name);
                        printtofile(name);  //function to print titles to file
                        flg=0;      //change flg to exit loop
                        chdir(".."); // change dir to the previous dir since it was changed to the created one
                        char dirch[256];    //hold the cwd for debugging
                        //printf("%s\n", getcwd(dirch, 100));
                        //closedir(currDir);
                    }
                   else if(input2 == 2){
                       //printf("in2\n");
                        DIR* currDir = opendir(".");   // pointer to the open dir
                        if (currDir == NULL){
                            printf("Can't open that file\n");
                            exit(0);
                        }

                        off_t tempSize = 0; // to hold the file size
                        int i = 0;
                        //printf("dir %s \n", currDir);
                        // loop throught the dir while not NULL
                        while((aDir = readdir(currDir)) != NULL){
                           // printf("dir %s \n", currDir);
                            // compares string prefix and file names based on the length of the prefix 
                            if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0){
                                stat(aDir->d_name, &dirStat);
                                memset(fullfilename, '\0', sizeof(fullfilename));
                                strcpy(fullfilename, aDir->d_name);     //copy file name to a string to break down
                                //use token to get a file ex.
                                memset(fileex, '\0', sizeof(fileex));
                                char* token = strtok(fullfilename,".");
                                //printf("token1 %s\n", token);
                                memset(exten, '\0', sizeof(exten));
                                strcpy(exten, token);
                                token = strtok(NULL, "\0");
                                strcpy(fileex, token);      //files extension
                               // printf(" fileex %s \n", fileex);
                               // printf("suff %s \n", SUFFIX);

                    // compare the extension with the defined  to search files for smallest size
                            if(strcmp(fileex, SUFFIX) == 0){

                                if( i == 0 || dirStat.st_size < tempSize ){
                                    printf("In 4\n");
                                    tempSize = dirStat.st_size;     //holds smallest file size
                                    memset(name2, '\0', sizeof(name2));
                                    strcpy(name2, aDir->d_name);
                                    printf("The smallest file is %s \n", name2);
                                }
                                i++;
                            }
                            }
                            
                        }

                        printf("The smallest file is %s \n", name2);
                        printtofile(name2);     //print titles to a file
                        flg=0;              // switch flag to exit loop
                        chdir("..");        //change back to orginal dir
                        char dirch[256];       //holds cwd for debugging
                       // printf("%s\n", getcwd(dirch, 100));
                        //closedir(currDir);
                    }
                    //user selected to enter the file name to search
                    else if(input2 == 3){
                        char filename[256];
                        DIR* currDir = opendir(".");
                        if (currDir == NULL){
                            printf("Can't open that file\n");
                            exit(0);
                        }

                        memset(filename, '\0', sizeof(filename));
                        int filefound = 0; // flag to exit loop
                        printf("Enter the complete file name: ");
                        scanf("%s", filename);
                        //printf("out while %s\n", filename);
                    //search the dir for the file entered
                        while((aDir = readdir(currDir)) != NULL){
                            //printf("cfn %s, fn %s\n", aDir->d_name, filename);
                            if(strcmp(filename, aDir->d_name) == 0){
                               // printf("file found %s\n", filename );
                               printtofile(filename);       //print the titles of the file to a new file
                                filefound = 1;  //set flag finding a file
                                flg = 0;
                                chdir("..");    //return to previous dir was changed in the printtofile func
                            }
                         }
                         if (filefound == 0){
                             printf("The file %s was not found. Try again.\n", filename);
                             flg =1; // set flag to reenter file name
                         }
                    
                    }else{
                        printf("Invalid entry. Please try again\n");
                        flg = 0;
                    }
                }
                }
                else if(input == 2){
                    printf("Goodbye\n");
                    closedir(currDir);
                    exit(0);
                }else{
                    printf("Invalid entry. Please try again\n");
                }
            }
            
}