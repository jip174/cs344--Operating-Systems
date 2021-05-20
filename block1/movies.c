//#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

//used https://repl.it/@cs344/studentsc#main.c 

//Global to keep count of the number of movies
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
    //printf("Processed file %s\n ", filePath);
    //printf("the file is processing");
    FILE *movieFile = fopen(filePath, "r"); // open and read the file passed in
    count = 0;//reset the count each call to process the file

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
    printf("Processed file %s and parsed data for %i movies\n ", filePath, count);
    //printf("head %s, next %s\n", head->title, head->next->title);
    return head;
}

// print the full movie info
void printMovie(struct movie* moviex){
    printf("%s, %i, %s, %f\n", moviex->title, moviex->year, moviex->languages,
        moviex->rating);
}

//prints the full movies list
void printMovieList(struct movie *list){
    while (list != NULL){
        printMovie(list);
        list = list->next;
    }
}

int main(int argc, char *argv[]){
    int input;

   // make sure the user entered a file name to process
    if (argc < 2){
        printf("must give a file name to process\n");
       exit(0);
    }
        //struct movie *list = processFile(argv[1]);
            while(true){
               
                printf("1. Show movies released in a specified year(between 1900-2021)\n");
                printf("2. Show highest rated movie for each year\n");
                printf("3. Show the title and year of release of all movies in a specific language\n");
                printf("4. Exit from the program\n");
                printf("Enter a choice from 1 to 4:\n");
                scanf("%d", &input);
                //printf("entered %d\n", input);

                if (input == 1){
                    //printf("You entered 1\n");
                    int yearIn; 
                    int i = 0;
                    int tracker= 0; // variable to track how many movies were in that year
                    struct movie *list = processFile(argv[1]);
                    printf("Enter the year for which you want to see movies: ");
                    scanf("%d", &yearIn);
                    //printf("year selected: %d\n", yearIn );
                   
                    while (list->next != NULL){
                        //printf("list title %s, %d\n", list->title, list->year);
                         i++;
                            if (yearIn == list->year){
                                printf("%s\n", list->title);
                                tracker++;
                            }
                            list = list->next;
                        //printMovieList(list);
                    }
                    if(tracker == 0){
                        printf("No movies found in that year\n\n");
                    }
                }
                else if( input == 2){
                int j = 0;
                int i = 0;
                int year;
                int year2;
                float rating = 0;
                float rating2 = 0;
                struct movie *list = processFile(argv[1]);
                struct movie *hrating = list;
                struct movie *defaultlist = list;
                /*struct movie *templist[count];
                //struct movie *temp[count];*/
                
                //while( list->next != NULL){
                // a loop to get the highest rating for each year
                while( j <= count){              
                                    
                    year2 = hrating->year;
                    //printf("year2 %d, hr: %d\n", year2, hrating->year);
                    year = list->year;
                    // a for loop to compare years and ratings
                    for (i = 0 ; i < count-1 ; i++){   
                    
                        year = list->year;
                        //printf("year %d, year2: %d\n", year, year2);
                       //compare to see if years are equal and then compare ratings
                        if(year == year2){
                            rating = list->rating;
                            rating2 = hrating->rating;
                           // printf("movie1 %s: %f %d, movie2 %s: %f %d\n", list->title, list->rating, list->year , hrating->title , hrating->rating, hrating->year);
                                                  
                            // compare ratings and set to the highest list
                            if(rating > rating2){
                               // printf("in\n");
                                hrating = list;
                                //printf("highest rated %s %d:\n", hrating->title, hrating->year);
                                //printf("other rated %s %d:\n", list->title, list->year);
                            }
                        list = list->next; // move pointers
                        year = list->year;
                         
                        }else{
                            
                            list = list->next;// move pointers
                            year = list->year;
                            //printf("year %d\n:", year);
                        }
                     }
                   
                    //templist[j] = hrating;
                    printf("%d %.1f %s\n",hrating->year, hrating->rating, hrating->title);
                    //printf("t %s\n", templist[j]->title);
                    list = defaultlist; // reset list to default
                    hrating = hrating->next; // move pointers
                    
                    j++; // increment loop
                }
               
            }
            //https://www.w3resource.com/c-programming-exercises/string/c-string-exercise-14.php
            else if (input == 3){
               int track = 0;
                    char *langcomp;
                    //char langfromstruct[] = "\0";
                    char str[] = "\0";
                    char search[] = "\0'";	
                    int c1=0,c2=0,i,j,flg;
                    struct movie *list = processFile(argv[1]);
                    printf(" Enter the language for which you want to see movies:");
                    printf("**Case Sensitive**\n");
                    scanf("%s", &search);
                    //printf("input: %s\n", search);
                    while(list->next != NULL){
                                                           
                        strcpy( str, list->languages);
                       if(strlen(str) < 4) { // if str is to small set flag
                            flg=0;
                            break;
                        }
                        //printf("lang2 %s\n", str); 
                       // check that strings arent null
                            while (str[c1]!='\0')
                                c1++;
                                        //c1--;

                            while (search[c2]!='\0')
                                c2++;
                                        //c2--;

                            //loop through strings to check if they are the same 
                            for(i=0;i<=c1-c2;i++)
                            { 
                                for(j=i;j<i+c2;j++)
                                {
                                    flg=1;
                                    if (str[j]!=search[j-i])
                                    {
                                        flg=0;
                                    break;
                                    }
                                }
                                if (flg==1) // flag is 1 if we find a match
                                    break;
                            }
                            if (flg==1){
                                //printf("The substring exists in the string.\n\n");
                                printf(" %d %s\n", list->year, list->title);
                                
                                //printf("Movies in %s are %s in %d\n", str, list->title, list->year);
                            }/*else
                                printf("No data about movies released in %s\n", search);
                            */
                        list= list->next;

                        }
                        // flag 0 if no match is found
                        if(flg==0){
                            printf("No data about movies released in %s\n", search);
                        }
            }
            else if( input == 4){
                printf("Goodbye\n"); // say goodbye
                exit(0);
            }else{
                printf("You entered an incorrect choice. Try again\n\n");
            }
    }   

return 0;     
}