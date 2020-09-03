int makedir(char *dirname){
	int check; 
    check = mkdir(dirname,0777); 
    if (!check) 
        printf("%s created\n", dirname); 
    else { 
        printf("ERROR: Unable to create directory or the dir already exists\n"); 
    }
    return 0; 
}