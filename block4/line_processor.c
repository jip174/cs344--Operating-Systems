/*Justin Phillips(phillij6)
assignment 4 - producer- consumer
cs344 winter 2021
2/14/2021

Write a program that creates 4 threads to process input from standard input as follows

    Thread 1, called the Input Thread, reads in lines of characters from the standard input.
    Thread 2, called the Line Separator Thread, replaces every line separator in the input by a space.
    Thread, 3 called the Plus Sign thread, replaces every pair of plus signs, i.e., "++", by a "^".
    Thread 4, called the Output Thread, write this processed data to standard output as lines of exactly 80 characters.

Furthermore, in your program these 4 threads must communicate with each other using the Producer-Consumer approach. */
//https://repl.it/@cs344/65prodconspipelinec code adapted from module 6 example
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <math.h> // must link with -lm


// Size of the buffers
#define SIZE 1000

// Number of items that will be produced. This number is less than the size of the buffer. Hence, we can model the buffer as being unbounded.
#define NUM_ITEMS 50

// Buffer 1, shared resource between input thread and square-root thread
char *buffer_1[SIZE];
// Number of items in the buffer
int count_1 = 0;
// Index where the input thread will put the next item
int prod_idx_1 = 0;
// Index where the square-root thread will pick up the next item
int con_idx_1 = 0;
// Initialize the mutex for buffer 1
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 1
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;


// Buffer 2, shared resource between square root thread and output thread
char *buffer_2[SIZE];
// Number of items in the buffer
int count_2 = 0;
// Index where the square-root thread will put the next item
int prod_idx_2 = 0;
// Index where the output thread will pick up the next item
int con_idx_2 = 0;
// Initialize the mutex for buffer 2
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 2
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;

// Buffer 3, shared resource between square root thread and output thread
char *buffer_3[SIZE];
// Number of items in the buffer
int count_3 = 0;
// Index where the square-root thread will put the next item
int prod_idx_3 = 0;
// Index where the output thread will pick up the next item
int con_idx_3 = 0;
// Initialize the mutex for buffer 2
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 2
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;

/*
 Put an item in buff_1
*/
void put_buff_1(char *item){
   //printf("pu1\n");
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_1);
  // Put the item in the buffer
  buffer_1[prod_idx_1] = item;
   // Increment the index where the next item will be put.
  //printf("put item %s, buff %s\n", item, buffer_1[prod_idx_1]);
  prod_idx_1 = prod_idx_1 + 1;
  count_1++;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_1);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
}

/*
Get the next item from buffer 1
*/
char *get_buff_1(){
  //printf("get1\n");
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_1);
  while (count_1 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_1, &mutex_1);
    
  char *item = buffer_1[con_idx_1];
   // Increment the index from which the item will be picked up
  con_idx_1 = con_idx_1 + 1;
  count_1--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
  // Return the item
  return item;
}
/*
Get input from the user.
This function doesn't perform any error checking.
*/
char *get_user_input(){
  char *value = NULL;
  char *item;
  item = calloc(SIZE +1, sizeof(char));
  size_t lineSize = SIZE;
  
  getline(&value, &lineSize, stdin);
  //printf("value\n", value);
    strcpy(item, value);
    //printf(" 1: %s\n", item);
  
  return item;
  
}
/*
 Function that the input thread will run.
 Get input from the user.
 Put the item in the buffer.
*/
void *get_input(void *args)
{
  int flg = 1; // flag to exit while loop
  //printf("in gi\n");
  
    while(flg ==1){
      // Get the user input
      //printf("while\n");
      char *item = get_user_input();
      //printf("gi %s\n", item);
      put_buff_1(item);
      if(strcmp(item, "STOP")){
        flg =0;
      }
    } 
    
    return NULL;
   
}

/*
 Put an item in buff_2
*/
void put_buff_2(char *item){
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_2);
  // Put the item in the buffer
  buffer_2[prod_idx_2] = item;
  // Increment the index where the next item will be put.
  prod_idx_2 = prod_idx_2 + 1;
  count_2++;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_2);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);
}

/*
Get the next item from buffer 2
this function finds \n and replaces it with a space " "
*/
void *line_seperator(void *args){
    int i;
    //char *saveptr;
    //char *token;
    char *foundEnd;
    char *findSTOP = "STOP";
    char *item = calloc(SIZE +1, sizeof(char));
    item = get_buff_1(); // get the string from the buffer
    int getStrSize = strlen(item); // get size of the string for the loop
   // printf("lp\n");
    for (i = 0; i < getStrSize; i++){
        
      // replace the \n with " "  
        if(item[i] == '\n'){ 
          //printf("found a nl\n");
            item[i]= ' ';
        }
       
        foundEnd = strstr(item, findSTOP); // make sure we dont process and STOP

        if(foundEnd && strlen(foundEnd) == strlen(findSTOP)){
          //printf("found stop\n");
           put_buff_2(item);
           i = getStrSize;
        }
      put_buff_2(item); // place new item in the next put
    }
    return NULL;
}
/*
Get the next item from buffer 2
*/
char *get_buff_2(){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_2);
  while (count_2 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
  pthread_cond_wait(&full_2, &mutex_2);
  
  char *item = buffer_2[con_idx_2];
  // Increment the index from which the item will be picked up
  con_idx_2 = con_idx_2 + 1;
  count_2--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);
  // Return the item
  return item;
}


/*
 Put an item in buff_3
*/
void put_buff_3(char *item){
  // Lock the mutex before putting the item in the buffer
 // printf("p3\n");
  pthread_mutex_lock(&mutex_3);
  // Put the item in the buffer
  buffer_3[prod_idx_3] = item;
  // Increment the index where the next item will be put.
  prod_idx_3 = prod_idx_3 + 1;
  count_3++;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_3);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_3);
}
/*
 finds any ++ in the string and changes it to ^
*/
void *plus_sign(void *args){
    char *item = calloc(SIZE +1, sizeof(char));
    //char *updatedItem = calloc(SIZE +1, sizeof(char));
    int i, j;
    int plusTrack = 0; // tracker for debugging make sure i caught the correct amount
    item = get_buff_2(); // get the item from the buffer
    int getStrSize = strlen(item);  // get the size of the string from the item
    //printf("item: %s, size %d\n", item, getStrSize);
    char *temp = calloc(SIZE +1, sizeof(char));
    
  // loop through the string finding ++ and switching it to ^ then moving the chars up a spot
    for (i = 0; i <= getStrSize; i++){
        if(item[i] == '+' && item[i+1] == '+'){
          //printf("++\n");
          temp[i] = '^';
          item[i] = temp[i];
         // j = i+1;
          for( j = i+1 ; j <= getStrSize ; j++){
             
              temp[j] = item[j+1];
              item[j] = temp[j];
          }
          
          plusTrack++;
        }        
        //printf("t %c, item: %c\n", temp[i], item[i]);
        put_buff_3(item); // put the items in the buffer
         
    }
    
    return NULL;

}

/*
Get the next item from buffer 3 final output
*/

char *get_buff_3(){
  //printf("G 3\n");

  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_3);
  while (count_3 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
  pthread_cond_wait(&full_3, &mutex_3);
  char *item = buffer_3[con_idx_3];
  // Increment the index from which the item will be picked up
  con_idx_3 = con_idx_3 + 1;
  count_3--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_3);
  // Return the item
  return item;
}

/*
 Function that the output thread will run. 
 Consume an item from the buffer shared with output.
 Print the item.
*/
void *write_output(void *args)
{
  //printf("op\n");
    int i;
    int j = 0;
    int count = 0;
    char *item = calloc(SIZE +1, sizeof(char));
    item = get_buff_3();
    int itemlgth = strlen(item);
   // printf("op : strlenght %d\n", itemlgth);
   float totalLines = itemlgth/ 80;
  // printf("lines %.0f, tl: %d\n", totalLines, itemlgth);
   do{
     // print out 80 chars at a time 
    for (i = 0; i < 80; i++){
      printf("%c", item[j]);
      fflush(stdout);
      j++; // j holds the index of the array of item so we got a reentry spot next loop
    }

    count++; // inc out count to find out the correct amount of times to run the loop
    printf("\n");
    fflush(stdout);
   }while(count < totalLines && count <= NUM_ITEMS);

   count = 0; // reset the count and j
   j = 0;
    return NULL;
}

int main(int argc, char *argv[])
{
  
  //printf("start lp\n");
    srand(time(0));
    pthread_t input_t, line_seperator_t, plus_sign_t,output_t;
    // Create the threads
    pthread_create(&input_t, NULL, get_input, NULL);
    pthread_create(&line_seperator_t, NULL, line_seperator, NULL);
    pthread_create(&plus_sign_t, NULL, plus_sign, NULL);
    pthread_create(&output_t, NULL, write_output, NULL);
    // Wait for the threads to terminate
    pthread_join(input_t, NULL);
    pthread_join(line_seperator_t, NULL);
    pthread_join(plus_sign_t, NULL);
    pthread_join(output_t, NULL);
   
    return EXIT_SUCCESS;
}