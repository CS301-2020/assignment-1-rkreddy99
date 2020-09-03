int match(char *pat, char *file_name){
	int max_len = 1024;
	char str[max_len];
	int count = 0;

	FILE *fp = fopen(file_name, "r");

	if(fp==NULL){
		perror("unable to open file\n");
	}
	while(fgets(str, max_len-1, fp)){
		if (strstr(str, pat)){
			printf("%s\n", str);
			++count;
		}
	}
	fclose(fp);
	printf("%s occured %d times\n", pat, count);
}