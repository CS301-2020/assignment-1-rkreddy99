#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <libgen.h>
#include <stdbool.h>
#include <time.h>
#include "ls.h"
#include "grep.h"
#include "cat.h"
#include "mkdir.h"
#include "mv.h"
#include "cp.h"
#include "rm.h"
#include "chmod.h"

void read_line(char cmd[], char *par[], int *fin, char *str_cmd){
	int i=0, j=0;
	char *array[100], *args;

	int bufsize = 1024;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if (!buffer) {
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (1) {
		c = getchar();
		if (c == EOF || c == '\n') {
		  buffer[position] = '\0';
		  break;
		} else {
		  buffer[position] = c;
		}
		position++;
		// reallocate the buffer, if needed.
		if (position >= bufsize) {
			bufsize += 1024;
			buffer = realloc(buffer, bufsize);
			if (!buffer) {
			fprintf(stderr, "lsh: allocation error\n");
			exit(EXIT_FAILURE);
			}
		}
	}
	if (position==1){
		return;
	}
	strcpy(str_cmd, buffer);
	args = strtok(buffer," ");

	while(args!=NULL) {
		array[i++] = strdup(args);
		args = strtok(NULL, " ");
	}
	strcpy(cmd, array[0]);

	for (int j=0; j<i; j++){
		par[j] = array[j];
	}
	par[i] = NULL;
	*fin = i-1;
}

int main(int argc, char const *argv[])
{
	char cmd[100], command [100], *parameters[20], str_command[1024];
	int end, last;

	int bckgrnd = 0;
	printf(">>");

	while(1) {
		read_line( command, parameters, &end, str_command);
		bool bck = false;
		if(!strcmp(parameters[end], "&")) bck=true;
		if(bck) last = end - 1;
		else last = end;
		int pid = fork();
		if(pid == -1){
			printf("error occured while forking\n");
		}
		else if( pid > 0 ){
			if(!strcmp(command, "cd")){
				char curr_dir[100];
				char *new_dir = parameters[1];
				int check = chdir(new_dir);
				if(!check) printf("changed to %s\n", getcwd(curr_dir, 100));
				else printf("ERROR: unable to change the directory\n");
			}
			if(!strcmp(command, "exit")){
				exit(0);
			}
			if(!bck){
				// printf("waiting for child to finish\n");
				waitpid(-1, NULL, 0);
			}
		}
		else if (pid==0) {
			if( !strcmp(command,"ls")){
				if(last==0){
					lsdir(".");
			    }
			    for (int q = 1; q<=last; q++) {
			    	lsdir(parameters[q]);
			    }
			}

			else if( !strcmp(command, "grep")){
				if(last<2){
					printf("ERROR: give pattern name at least 1 file\n");
					exit(1);
				}
				for(int q=2; q<=last; q++){
					match(parameters[1], parameters[q]);
				}
			}

			else if (!strcmp(command, "cat")) {
				for (int q=1; q<=last; q++){
					viewfile(parameters[q]);
				}
			}

			else if (!strcmp(command, "pwd")) {
				char cwd[1024];
				getcwd(cwd, sizeof(cwd));
				printf("%s\n", cwd);
			}

			else if (!strcmp(command, "mkdir")){
				for(int q=1; q<=last; q++){
					makedir(parameters[q]);
				}
			}

			else if(!strcmp(command, "cp")){

				if( last<2 ){
					printf("ERROR: insufficient args, needed source file(s) and target dir/file\n");
					printf(">>");
					exit(1);
				}

				int checki=0,checklast=0;
				char *arglast = parameters[last];
				struct stat arg_last;
				if(!stat(arglast, &arg_last)){
					if(S_ISDIR(arg_last.st_mode)){
						checklast=1;
					}	
				}
				if(last>=3 && !checklast){
					printf("ERROR: trying to copy multiple things to a file\n");
					printf(">>");
					exit(1);
				}
				for(int q=1; q<last; q++){
					struct stat arg_i;
					if(!stat(parameters[q], &arg_i)){
						if(S_ISDIR(arg_i.st_mode)){
							if(checklast) copy_dir_to_dir(parameters[q], arglast);
						}
						else if(checklast) copy_file_to_dir(parameters[q], arglast);
						else if(!checklast) copy_file_to_file(parameters[q], arglast);
					}
				}
			}

			else if (!strcmp(command, "mv")){
				if(last<2){
					printf("ERROR: need one file and one directory\n");
					printf(">>");
					exit(1);
				}
				for(int q=1; q<last; q++){
					move(parameters[q], parameters[last]);
				}
			}

			else if(!strcmp(command, "rm")){
				int i;
				for(i=0; i<20; i++){
					if (parameters[i]==NULL){
						break;
					}
				}
				if(i<2){
					printf("ERROR: give file to delete\n");
				}
				else if(i==2){
					char *comp_file_name = parameters[1];

					struct stat statbuf;
					if (!stat(comp_file_name, &statbuf))
					{
						if (S_ISREG(statbuf.st_mode))
						{
							int d = remove(comp_file_name);
							if(!d) printf("removed %s\n", comp_file_name);
							else printf("couldn't remove %s\n", comp_file_name);
						}
						else{
							printf("ERROR: directory should be removed using -r flag\n");
						}
					}
				}
				else if(i==3){
					printf("%s\n", parameters[1]);
					if(strcmp(parameters[1], "-r")){
						printf("ERROR: Check the flags\n");
						printf(">>");
						exit(1);
					}
					int d = remdir(parameters[2]);
					if(!d) printf("removed dir %s\n", parameters[2]);
					else printf("couldn't remove %s\n", parameters[2]);
				}
			}
			else if(!strcmp(command, "chmod")) {
				if (last<2) {
					printf("Error: insufficient arguments\n");
					printf(">>");
					exit(1);
				}
				cmod(parameters[1], parameters[2]);
			}

			else if(!strcmp(command, "exit")) {
				exit(0);
			}
			else if(strcmp(command,"cd")){
				system(str_command);
			}
			// printf("exiting the child\n");
			printf(">>");
			exit(0);
		}
	}
	return 0;
}