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
void IORedirect(char **args, int k, int ioMode)
{
    pid_t pid, wpid;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int fd, status = 0;
    
    pid = fork();
    if (pid == 0) {
        // Child process
        if(ioMode == 0)   // Input mode
            fd = open(args[k+1], O_RDONLY, mode);
        else              // Output mode
            fd = open(args[k+1], O_WRONLY|O_CREAT|O_TRUNC, mode);
        if(fd < 0)
            fprintf(stderr, "File error");
        else {
        
            dup2(fd, ioMode);   // Redirect input or output according to ioMode
            close(fd);          // Close the corresponding pointer so child process can use it
            args[k] = NULL;
            args[k+1] = NULL;

            if (execvp(args[0], args) == -1) {
                perror("SHELL");
            }
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0) { // Error forking process
        perror("SHELL");
    } 
    else {
        // Parent process. Wait till it finishes execution
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } 
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }        
}
int arr(char** tokens){
	int i=0;
	while(i>=0){
		if(tokens[i]==NULL){
			return i-1;
			break;
		}else{
		i++;
		}
	}
}
void background(char** parsed)
{
    // Forking a child
    pid_t pid = fork();
	exitf(parsed,pid);
    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command..");
        }
        exit(0);
    } else {
        
        return;
    }
}
void exitf(char** tokens,int pid){
if(strcmp(tokens[0],"exit")==0)
   			 {
   			 kill(pid,SIGKILL);
   		     exit(0);
  		  }
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
        if (execvp(tokens[0], tokens) < 0) {
            printf("\nCould not execute command");
        }
        exit(0);
    } else {
        // waiting for child to terminate
	wait(NULL);
        return;
        }
    }
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


int main(int argc, char* argv[]) {
        char *options[2] = {"<", ">"};
	char  line[MAX_INPUT_SIZE];
	char  **tokens;
	int i,x,y;
	int k = 0, option, found;
	int h=0;
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
       	y=arr(tokens);
		if(strcmp(tokens[y],"&")==0){
		tokens[y]=NULL;
		background(tokens);
		}else{
		execArgs(tokens);
		}
       	if(strcmp(tokens[0],"exit")==0)
   			 {
   			      exitf(tokens,0);
  		  }

               if (strcmp(tokens[0],"cd")==0)
               {
    
                    if (tokens[1]!= NULL && tokens[1]!= "..")
                    {
                        chdir(tokens[1]);
                    }
                    else if (tokens[1]==NULL){
                        chdir(getenv("HOME"));
                    }
                    else if (tokens[1]==".."){
                            chdir("..");
                    }
                }

                else
                {
                 k = 1;
                 
            while(tokens[k] != NULL) {           // Check for any of the redirect or process operators <,>
                for(option = 0; option < 2; option++) {
                    if(strcmp(tokens[k],options[option]) == 0) 
                        break;
                }
                if(option < 2) {
                    found = 1;
                    if(option < 2 && tokens[k+1] == NULL) {   // For IORedirect and Pipe, argument is necessary
                        fprintf(stderr, "SHELL: parameter missing\n"); 
                        break;
                    }
                    if(option < 2)    {                      // Redirect IO, option=0 for Input, option=1 for Output
                        IORedirect(tokens, k, option);
                        execArgs(tokens);}                
                }}
                        execArgs(tokens);}

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

