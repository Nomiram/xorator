#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#define BUFSIZE 20
int main(int argc, char* argv[]) {
int data_processed1;
int data_processed2;
int file_pipes1[2];
int file_pipes2[2];
char* filename1[250]={0};
char* filename2[250]={0};
pid_t proc1;
pid_t proc2;
int fp1;
int fp2;

if (argc != 3){
printf("Too few arguments\n");
exit(EXIT_FAILURE);
}
fp1 = open("d.txt", O_WRONLY);
if (fp1 == -1) {
printf("Cannot open file.\n");
exit(EXIT_FAILURE);
}
else
{
	printf("file d.text created\n");
}
if ((fp2 = open("t.txt", O_WRONLY))==-1) 
{
printf("Cannot open file.\n");
exit(EXIT_FAILURE);
}else{
	printf("file t.text created\n");
}
if (!((pipe(file_pipes1) ==0 )&&(pipe(file_pipes2) == 0))){
	 exit(EXIT_FAILURE);
 }
printf("start working\n");
 proc2 = fork();
  if (proc2 == (pid_t)-1) {
   fprintf(stderr, "Fork failure");
   exit(EXIT_FAILURE);
  }
  if (proc2 == (pid_t)0) {
   close(1);
   dup(file_pipes1[1]);
   close(file_pipes1[0]);
   close(file_pipes1[1]);
   execlp("cat", "cat", argv[1], (char*)0);
   exit(EXIT_FAILURE);
  }
  printf("p1 created\n");
     proc1 = fork();
   if (proc1 == (pid_t)-1) {
    fprintf(stderr, "Fork failure");
    exit(EXIT_FAILURE);
   }
   if (proc1 == (pid_t)0) {
   close(1);
   dup(file_pipes2[1]);
   close(file_pipes2[0]);
   close(file_pipes2[1]);
   execlp("cat", "cat", argv[2], (char*)0);
   exit(EXIT_FAILURE);
   }
   printf("p2 created\n");
   close(file_pipes1[1]);
   close(file_pipes2[1]);
   char xored[BUFSIZE+1]={0};
   char buf1[BUFSIZE+1]={0};
   char buf2[BUFSIZE+1]={0};
   printf("waiting...\n");
   sleep(2);
   while(1){
   printf("\n\titeration\n");
   memset(buf1,(char)0,BUFSIZE);
   memset(buf2,(char)0,BUFSIZE);
   data_processed1 = read(file_pipes1[0], buf1, sizeof(char)*BUFSIZE);
   data_processed2 = read(file_pipes2[0], buf2, sizeof(char)*BUFSIZE);
   printf("%s read %d bytes:\n%s\n",argv[1], data_processed1, buf1);
   printf("%s read %d bytes:\n%s\n",argv[2], data_processed2, buf2);
   if((data_processed1 == data_processed2)&&(data_processed1 != 0)){
	    for(int i = 0;i < data_processed1; i++){
		    xored[i]=buf1[i]^buf2[i];
		    // printf("%x\n",buf1[i]);
		    // printf("%x\n",buf2[i]);
		    // printf("%2x\n",xored[i]);
		    
	    }
		    printf("xored:\n");
	    for(int t = 0;t < data_processed1; t++){
		    printf("%2x",xored[t]);
	    }
		    printf("\n");
		    printf("%s",xored);
		    printf("\n");
	    printf("writing %li bytes\n",write(fp1,xored,data_processed1));
	    printf("writing %li bytes\n",write(fp2,buf2 ,data_processed1));
   }
	else{
		break;
	}
   }
   close(file_pipes1[0]);
   close(file_pipes2[0]);
 exit(EXIT_SUCCESS);
}