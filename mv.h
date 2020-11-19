int move(const char *curr_file, const char *final_dir){
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
	int del = remove(curr_file);
	if(!del){
		printf("moved %s to %s\n", curr_file_name, final_dir);
	}
	else{
		printf("unable to move %s to %s\n", curr_file_name, final_dir);
	}
	return 0;
}
