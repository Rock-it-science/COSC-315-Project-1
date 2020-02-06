// closh.c - COSC 315, Winter 2020
// Will McFarland 86184900
// Alvin Krisnanto Putra 54658380
// (Winter) Manassawin Rotsawatsuk 12682936

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

int child_pids[10];
int child_pid;
int sound_pid;

void kill_process(int sig) {//kill processes and its children
	kill(0, SIGTERM);
}

// tokenize the command string into arguments - do not modify
void readCmdTokens(char* cmd, char** cmdTokens) {
	cmd[strlen(cmd) - 1] = '\0'; // drop trailing newline
	int i = 0;
	cmdTokens[i] = strtok(cmd, " "); // tokenize on spaces
	while (cmdTokens[i++] && i < sizeof(cmdTokens)) {
		cmdTokens[i] = strtok(NULL, " ");
	}
}

// read one character of input, then discard up to the newline - do not modify
char readChar() {
	char c = getchar();
	while (getchar() != '\n');
	return c;
}

//Function to tell user that command timed out
void timeout_handler(int signum){
    for (int i =0; i < sizeof(child_pids)/4; i++) {
        if(child_pids[i] != 0 && waitpid(child_pids[i],0,WNOHANG) == 0) {
            kill(child_pids[i],SIGTERM);
            printf("pid %d timed out\n",child_pids[i]);
        }
    }
    if(child_pid != 0 && waitpid(child_pid,0,WNOHANG) == 0) {
        kill(child_pid,SIGTERM);
        printf("pid %d timed out\n",child_pid);
    }
	sound_pid = fork();
	if(sound_pid == 0) {//Child process
		execlp("totem", "--enqueue", "term.mp3", NULL);
		exit(1);
	}
    return;
}

// main method - program entry point
int main() {
	char cmd[81]; // array of chars (a string)
	char* cmdTokens[20]; // array of strings
	int count; // number of times to execute command
	int parallel; // whether to run in parallel or sequentially
	int timeout; // max seconds to run set of commands (parallel) or each command (sequentially)
	
	while (TRUE) { // main shell input loop
		
		// begin parsing code - do not modify
		printf("closh> ");
		fgets(cmd, sizeof(cmd), stdin);
		if (cmd[0] == '\n') continue;
		readCmdTokens(cmd, cmdTokens);
		do {
			printf("  count> ");
			count = readChar() - '0';
		} while (count <= 0 || count > 9);
		
		printf("  [p]arallel or [s]equential> ");
		parallel = (readChar() == 'p') ? TRUE : FALSE;
		do {
			printf("  timeout> ");
			timeout = readChar() - '0';
		} while (timeout < 0 || timeout > 9);
		// end parsing code
		
		
		////////////////////////////////////////////////////////
		//													//
		// TODO: use cmdTokens, count, parallel, and timeout  //
		// to implement the rest of closh					 //
		//													//
		// /////////////////////////////////////////////////////
		
		if(parallel){//Run in parallel
			//Run program 'count' times, and set a timeout for the duration of the execution as 'timeout' seconds
			signal(SIGALRM, timeout_handler);
			alarm(timeout); //Set the timeout alarm
			for (int i = 0; i < count; i++) {
				
				child_pids[i] = fork(); //Fork process
				
				if (child_pids[i] == 0) {//Child process
					//printf("pid: %d\n", getpid()); //print process id
					execvp(cmdTokens[0], cmdTokens);
					printf("Can't execute %s\n", cmdTokens[0]); // only reached if running the program failed
					// doesn't return unless the calling failed
					exit(1);
				}
			}
			//Wait for all child processes to finish or timeout (whichever is first)
			sleep(0.1);//Give program a chance to start
			for(int j=0; j<count; j++){
				waitpid(child_pids[j], 0, 0);
			}
			alarm(0); //Disable timeout alarm
			//printf("All Processses executed successfully\n");
		}
		
		else{//Run sequentially
			//Run program 'count' times, waiting for timeout each time
			for (int i = 0; i < count; i++) {
				
				child_pid = fork(); //Fork process
				
				if (child_pid == 0) {//Child process
					//printf("\npid: %d\n", getpid()); //print process id
					execvp(cmdTokens[0], cmdTokens);
					printf("Can't execute %s\n", cmdTokens[0]); // only reached if running the program failed
					// doesn't return unless the calling failed
					exit(1);
				}
				
				else{//Parent process
					//Wait for child process to finish or timeout (whichever is first)
					signal(SIGALRM, timeout_handler);
					alarm(timeout);
					waitpid(child_pid, 0, 0);
					alarm(0); // Cancel alarm
					//printf("Processs successful\n");
				}
			}
		}	  
	}
}
