void syscmd(char **params) {
	int h = execvp(params[0], params);
	if (h<0){
		printf("ERROR: Couldn't execute the command\n");
	}
	printf("out dgasdg \n");
	return;
}