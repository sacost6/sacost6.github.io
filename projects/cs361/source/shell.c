#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define DELIM "  \n\t\r\a"
#define BUFFER 1024
#define CHAINDELIM "|"
#define INPUTDELIM ";"  
#define EXIT "exit"

// Global variable ** used to keep track of the argument array
struct specialCommand {
	int *cLength;
	char **firstCommand;
	char **secondCommand; 
};


struct argarray {
	char ** argsarray; 
	int *arrayLength;
	char * input; 
};

struct specialCommand pc;
struct specialCommand sc;
struct argarray argsarrayGlobal  = {.argsarray = NULL, .input = NULL}; 
int pipedCommandFlag = 0;
int sequentialFlag = 0;

void get_args_array(int *size ) {
	// Set input and token pointers to NULL
	char* line = NULL;
	char* word = NULL;
	int i = 0; 
	// initialize argument array
	argsarrayGlobal.argsarray = (char**) malloc(sizeof(char*) * (BUFFER/2));
	for(i = 0; i < (BUFFER/2); i++) {
		argsarrayGlobal.argsarray[i] = (char*)malloc(sizeof(char)*BUFFER);
	}
	// get the size of the input
	size_t buffer = 0;
	// read the user input and store as variable
	getline(&line, &buffer, stdin);
	// get rid of newline or space characters in order to avoid system call errors	
	for(int i = 0; i < buffer; i++) {
		if(line[i] == '\n') {
			line[i] = '\0';
		}
	}
	argsarrayGlobal.input = (char*)malloc(BUFFER);
	strcpy(argsarrayGlobal.input, line);
	// split up input based on spaces
	word = strtok(line, DELIM); 
	i = 0;
	while(word) {
		// store each individual token in its own index in the argument array
		strcpy(argsarrayGlobal.argsarray[i], word);
		// if the pipeline character is detected then turn on the flag
		if(strchr(word, '|')){
		    	//set the flag for the while loop to detect piped commands 
			pipedCommandFlag = 1;
		} 
		else if(strchr(word, ';')){
			//set the flag so the while loop can execute sequential commands 
			sequentialFlag = 1;
		}
		word = strtok(NULL, DELIM);	
		i = i + 1;
	}
	// size of the input is stored in the address hheld by size
	*size = i;
	// last character of the array is set to null
	argsarrayGlobal.argsarray[i] = NULL;


	argsarrayGlobal.arrayLength = size;
	free(word);
}
	

void pipeline_parse() {
	char *word = strsep(&argsarrayGlobal.input, "|");
	char *temp[2];
	
	int i = 0;
	while((word != NULL)) {
		temp[i] = word;
		word = strsep(&argsarrayGlobal.input, "|");
		i = i + 1;
	}
	if(temp[1][0] == ' ') 
		memmove(temp[1], temp[1]+1, strlen(temp[1]));	
	pc.firstCommand = (char**) malloc(sizeof(char*)*BUFFER/2);
	pc.secondCommand = (char**) malloc(sizeof(char*)*BUFFER/2);
	
	word = strtok(temp[0], " ");
	for(int i = 0; i < BUFFER/2; i++) {
		pc.firstCommand[i] = (char*) malloc(BUFFER);
		pc.secondCommand[i] = (char*) malloc(BUFFER);

	}
	i = 0;
	int firstSize;
	while(word) {
		strcpy(pc.firstCommand[i], word);
		word = strtok(NULL, DELIM);
		i = i + 1;
	}

	firstSize = i;
	int secondSize = 0;
	i = 0;
	word = strtok(temp[1], " ");
	while(word) {
		strcpy(pc.secondCommand[i], word);
		word = strtok(NULL, DELIM);
		i = i + 1;
	}
	secondSize = i;
	
	pc.firstCommand[firstSize] = NULL;
	pc.secondCommand[secondSize] = NULL;

}

void pipeline_fork_helper(){ 
	pid_t command1, command2; 

	int f[2]; 
	// call pipe function with two index array for file descriptors
	pipe(f); 
	// call fork function
	command1 = fork(); 
	// first child takes care of first command
	if(command1 == 0) {
		//first command is writing so close reading pipe end
		close(f[0]);
		// write to connected file	
		dup2(f[1], STDOUT_FILENO);
		// close output stream
		// attempt to run first command from piped input
		if(execv(pc.firstCommand[0], pc.firstCommand) == -1){ 
			printf("first command failed\n");
		}	
	}
	//error with forking
	else if(command1 < 0) {
		printf("forking error\n");
		
	}
	close(f[1]);
	int status;
	pid_t child1; 
	child1 = wait(&status);
       	// create the second child process to be used for the second part of the piped input	
	command2 = fork(); 
	// if child then read first child's output to use as input
	if(command2 == 0) {
		// second command is reading so close writing pipe end
		close(f[1]);	
		// read from connected file
		dup2(f[0], STDIN_FILENO);
		//close input stream
		close(f[0]);

		//attempt to run second command with first commands input
		if(execv(pc.secondCommand[0], pc.secondCommand) == -1) {
			printf("second command error\n");
		}
	}
	//error with forking
	close(f[0]);	
	pid_t child2;
	int status1;
	child2 = wait(&status1);
	printf("pid:%d status:%d\n", child1, WEXITSTATUS(status));
        printf("pid:%d status:%d\n", child2, WEXITSTATUS(status1));	
}


void execv_helper() {
        if(execv(argsarrayGlobal.argsarray[0], argsarrayGlobal.argsarray) == -1)
                printf("error");
}


int fork_helper() {
	pid_t pid = fork();
	// This is the child process, so execute the command on this process
	if(pid == 0) {
		execv_helper();	
	}
	// Forking error
	else if(pid < 0) {
		perror("lsh");
	}
	// This is the parent process, so we should wait for the child process to finish
	else if(pid > 0) {
		int status;
		pid_t cpid;
		cpid = wait(&status);
		printf("pid:%d status:%d\n", cpid,  WEXITSTATUS(status)); 
	}


	return 1;
}	

// this function allows us to split the two commands 
void sequential_parse() {

        printf("input:%s\n", argsarrayGlobal.input);

        char *word = strsep(&argsarrayGlobal.input, ";");
        char *temp[2];

        int i = 0;
        while((word != NULL)) {
                temp[i] = word;
                word = strsep(&argsarrayGlobal.input, ";");
                i = i + 1;
        }
        if(temp[1][0] == ' ')
                memmove(temp[1], temp[1]+1, strlen(temp[1]));
        sc.firstCommand = (char**) malloc(sizeof(char*)*BUFFER/2);
        sc.secondCommand = (char**) malloc(sizeof(char*)*BUFFER/2);

        word = strtok(temp[0], " ");
        for(int i = 0; i < BUFFER/2; i++) {
                sc.firstCommand[i] = (char*) malloc(BUFFER);
                sc.secondCommand[i] = (char*) malloc(BUFFER);

        }
        i = 0;
        int firstSize;
        while(word) {
                strcpy(sc.firstCommand[i], word);
                word = strtok(NULL, DELIM);
                i = i + 1;
        }

        firstSize = i;
        int secondSize = 0;
        i = 0;
        word = strtok(temp[1], " ");
        while(word) {
                strcpy(sc.secondCommand[i], word);
                word = strtok(NULL, DELIM);
                i = i + 1;
        }
        secondSize = i;

        sc.firstCommand[firstSize] = NULL;
        sc.secondCommand[secondSize] = NULL;

}

void sequential_fork_helper() {
	pid_t command1, command2;
	
	command1 = fork(); 

	if(command1 == 0) {
		if(execv(sc.firstCommand[0], sc.firstCommand))
			printf("error executing command1\n");	
	}

	waitpid(command1, NULL, 0);

	command2 = fork(); 
	if(command2 == 0) {
		if(execv(sc.secondCommand[0], sc.secondCommand))
			printf("error executing command2\n"); 
	}	
	waitpid(command2, NULL, 0);
}
void sigintHandler(int sig_num) {
	signal(SIGINT, sigintHandler); 
	write(1, "caught sigint\n", 20); 
	fflush(stdout);
}

void sigstpHandler(int sig_num) {
	signal(SIGTSTP, sigstpHandler); 
	write(1, "caught sigstp\n", 20);
	fflush(stdout);
}


int main(){
	signal(SIGINT, sigintHandler);
	signal(SIGTSTP, sigstpHandler); 
	int size = 0;
	while(1) {
		pipedCommandFlag = sequentialFlag = 0;
		printf("CS361 >");
		get_args_array(&size);
		if(pipedCommandFlag && !sequentialFlag) {
			pipeline_parse();
			pipeline_fork_helper();
		}
		else if(!pipedCommandFlag && sequentialFlag) {
			sequential_parse();
			sequential_fork_helper();
		}
		else if(!strcmp(argsarrayGlobal.argsarray[0], EXIT))
			exit(EXIT_SUCCESS);
		else {
			fork_helper();
			}
	}
	
	

	free(argsarrayGlobal.argsarray); 
	return EXIT_SUCCESS;
}

