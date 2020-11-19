int lsdir(const char *path) {
	struct dirent *de;
	DIR *dr = opendir(path);
	if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("ERROR: Could not open current directory %s\n\n", path ); 
        closedir(dr);
        return 1;
    } 
    printf("%s:\n", path);
    while ((de = readdir(dr)) != NULL) {
    	if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") ){
            continue;
        }
        printf("%s\n", de->d_name);
    }
    printf("\n");
    closedir(dr);
    return 0;
}
