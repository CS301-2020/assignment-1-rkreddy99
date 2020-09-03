// int makedir(char *dirname){
// 	int check; 
//     check = mkdir(dirname,0777); 
//     if (!check) 
//         printf("%s created\n", dirname); 
//     else { 
//         printf("ERROR: Unable to create directory or the dir already exists\n"); 
//     }
//     return 0; 
// }

int copy_file_to_file(char *source_file, char *target_file){
	char ch;
	FILE *source, *target;

	// char *source_file = parameters[1];

	source = fopen(source_file, "r");

	if( source == NULL ){
		printf("ERROR: unable to open %s\n", source_file);
	}

	// char *target_file = parameters[2];

	target = fopen(target_file, "w");

	if( target == NULL )
	{
	  fclose(source);
	  printf("ERROR: unable to open %s\n", source_file);
	}

	while( ( ch = fgetc(source) ) != EOF )
	  fputc(ch, target);

	printf("File copied successfully.\n");

	fclose(source);
	fclose(target);
}

int copy_file_to_dir(char *curr_file, char *final_dir){
	char tet;
	FILE *curr, *final, *check_op;

	char *dup_curr_file = strdup(curr_file);
	char *curr_dir_name = dirname(dup_curr_file);
	char *curr_file_name = basename(dup_curr_file);

	char *target_file = strdup(final_dir);
	target_file = strcat(target_file, "/");
	target_file = strcat(target_file, curr_file_name);

	if (check_op = fopen(target_file, "r")) {
		fclose(check_op);
		remove(target_file);
	}

	curr = fopen(curr_file, "r");
	if (curr == NULL) {
		fclose(curr);
		printf("ERROR: unable to open %s\n", curr_file);
		return 1;
	}

	final = fopen(target_file, "w");
	if (final == NULL) {
		fclose(final);
		printf("ERROR: unable to create %s\n", target_file);
		return 1;
	}

	while( (tet = fgetc(curr)) != EOF){
		fputc(tet, final);
	}
	fclose(curr);
	fclose(final);
	// int del = remove(curr_file);
	// if(!del){
	// 	printf("moved %s to %s\n", curr_file_name, final_dir);
	// }
	// else{
	// 	printf("unable to move %s to %s\n", curr_file_name, final_dir);
	// }
	return 0;
}

int copy_dir_to_dir(char *source_dir, char *target_dir){
	struct dirent *de;
	DIR *dr = opendir(source_dir);
	if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("ERROR: Could not open current directory %s\n\n", source_dir ); 
        closedir(dr);
        return 1;
    } 
    char *dup_add_dir = strdup(source_dir);
    char *base_source_dir = basename(dup_add_dir);
    char *parent_source_dir = dirname(dup_add_dir);

    char *dup_final_target_dir = strdup(target_dir);
    char *base_target_dir = basename(dup_final_target_dir);
    char *parent_target_dir = dirname(dup_final_target_dir);

    char *final_target_dir;
    size_t len_fin = strlen(parent_target_dir) + strlen(base_target_dir) + strlen(base_source_dir) + 3;
    final_target_dir = malloc(len_fin);
    snprintf(final_target_dir, len_fin, "%s/%s/%s", parent_target_dir, base_target_dir, base_source_dir);
    
    struct stat makeddir;
    if(!(stat(final_target_dir, &makeddir))){
    	if(S_ISDIR(makeddir.st_mode)){
    		printf("exists\n");
    	}
    }
    else{
    	makedir(final_target_dir);
    }

    while ((de = readdir(dr)) != NULL) {
    	if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") ){
            continue;
        }

		char *ini_file;
		size_t len_ini_file;
		if(!strcmp(&source_dir[strlen(source_dir)], "/")) {
			len_ini_file = strlen(source_dir) + strlen(de->d_name) + 1;
		}
		else{
			len_ini_file = strlen(source_dir) + strlen(de->d_name) + 2;
		}
		ini_file = malloc(len_ini_file);
		snprintf(ini_file, len_ini_file, "%s/%s", source_dir, de->d_name);

        struct stat checkdir;
        if(!stat(ini_file, &checkdir)){
        	if(S_ISDIR(checkdir.st_mode)){
        		continue;
        	}
        }

		char c;
		FILE *curr, *final, *check_op;

		size_t len_target_file = strlen(final_target_dir) + strlen(de->d_name) + 2;
		char *target_file;
		target_file = malloc(len_target_file);
		snprintf(target_file, len_target_file, "%s/%s", final_target_dir, de->d_name);

		if (check_op = fopen(target_file, "r")) {
			fclose(check_op);
			remove(target_file);
		}

		curr = fopen(ini_file, "r");
		if (curr == NULL) {
			fclose(curr);
			printf("ERROR: unable to open %s\n", ini_file);
			return 1;
		}

		final = fopen(target_file, "w");
		if (final == NULL) {
			fclose(final);
			printf("ERROR: unable to create %s\n", target_file);
			return 1;
		}

		int max_len = 1024;
		char str[max_len];
		while(fgets(str, max_len-1, curr)) {
			fputs(str,final);
		}

		fclose(curr);
		fclose(final);
		free(target_file);
		free(ini_file);
		}
		closedir(dr);
		// free(final_target_dir);

	return 0;
}