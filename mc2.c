#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define NUM_THREAD 2
int *pc;
void* circle_point(void *param);
pthread_t tid[NUM_THREAD]={0};
int count[NUM_THREAD]={0};
static long int total_point;
static long int count_circle=0;
pthread_mutex_t lock;


int main(int argc, char const *argv[]){
   if(argc==1){
       printf("Enter number point\n");
       return -1;
      }

   if(argc!=2){
       printf("Argument is wrong\n");
       return -1;
     }

   total_point=atoll(argv[1])/NUM_THREAD;
   srand(time(NULL));
   static int i;
   if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }
   for(i=0;i<NUM_THREAD;i++){
   pthread_create(&tid[i],NULL,&circle_point,&count[i]);
       pthread_join(tid[i],NULL);
       count_circle+=count[i]; }
   double pi=4.0*(double)count_circle/(double)total_point/(double)NUM_THREAD;

   printf("PI = %5.3f\n",pi);
   pthread_mutex_destroy(&lock);

   return 0;
  }

 void* circle_point(void *param){
   
   pc= (int*)param;
   int i;
   pthread_mutex_lock(&lock);
   for(i=0; i<total_point;i++){
       double x= (double)rand()/(double)RAND_MAX;
       double y=(double)rand()/(double)RAND_MAX;
       double r= sqrt(x*x+y*y);
       if(r<=1) *pc=*pc+1;
     }pthread_mutex_unlock(&lock);
     pthread_exit(0);
  
 }
