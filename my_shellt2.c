#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#define MAX_INPUT_SIZE 2048
#define MAX_TOKEN_SIZE 128
#define MAX_NUM_TOKENS 128

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0;
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }

  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

void execArgs(char** tokens)
{
    // Forking a child
	pid_t pid = fork();
 	int i;
    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
        if (execvp(tokens[0], tokens) < 0) { //executing internal commands
            printf("\nCould not execute command");
        }
        exit(0);
    } else {
        // waiting for child to terminate
	wait(NULL);
        return;
    }
}

int counter(char** tokens){
	int i=0;
	while(i>=0){
		if(tokens[i]==NULL){ //end of string is reached
			return i-1; //returns total number of tokens 
			break;
		}else{
		i++; //keep incrementing
		}
	}
}

void exitf(char** tokens,int pid){
   	kill(pid,SIGKILL); //terminating a process
   	exit(0);
}

void background(char** parsed)
{
    // Forking a child
    pid_t pid = fork();
	exitf(parsed,pid); //terminating parent
    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) { //executing internal command
            printf("\nCould not execute command..");
        }
        exit(0);
    } else {
        
        return;
    }
}



int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];
	char  **tokens;
	int i,x,count;
	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}

	while(x>=0) {
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		if(argc == 2) { // batch mode
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;
			}
			line[strlen(line) - 1] = '\0';
		} else { // interactive mode
			printf("$ ");
			scanf("%[^\n]", line);
			
			getchar();
		}
		x++;
		printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
       //do whatever you want with the commands, here we just print them
       
       	count=counter(tokens); //y i the total number of tokens
       	
		if(strcmp(tokens[count],"&")==0){ // chk for last token
		tokens[count]=NULL; // excluding & symbol from command
		background(tokens); // calling background function
		}
		else{
		execArgs(tokens); //internal commands
		}
		
       	if(strcmp(tokens[0],"exit")==0){ //chk for exit command
   			      exitf(tokens,0);
   			  }
   			  
        if (strcmp(tokens[0],"cd")==0){ //chk for change directory

            if (tokens[1]!= NULL && tokens[1]!= ".."){ //cd to a specific directory
                chdir(tokens[1]); 
                }
            else if (tokens[1]==NULL){ //cd to home
                chdir(getenv("HOME"));
            }
            else if (tokens[1]==".."){ //cd to parent directory
                chdir("..");
            }
        }

		for(i=0;tokens[i]!=NULL;i++){
			printf("found token %s (remove this debug output later)\n", tokens[i]);
		}

		// Freeing the allocated memory
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		}
		
		free(tokens);
	        
	return 0;
}

