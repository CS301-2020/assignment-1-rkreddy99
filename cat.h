int viewfile(const char *file_name)
{
	FILE *fp = fopen(file_name, "r");
	if (fp==NULL){
		printf("cannot open file\n");
		return 1;
	}
	int max_len = 10244;
	char str[max_len];
	if( fgets (str, max_len, fp)!=NULL ) {
		puts(str);
	}
	while(fgets(str,max_len-1,fp)) {
		printf("%s",str);
	}
	fclose(fp);
	return 0;
}
