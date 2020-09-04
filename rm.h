int remdir(const char *path) {
  DIR *dr = opendir(path);
  int check1 = -1;
  int pathlen = strlen(path);

  if (dr==NULL) {
    printf("Could not open %s\n", path);
    return -1;
  }
  if(dr!=NULL){
    struct dirent *de;

    check1 = 0;
    while((de = readdir(dr)) && !check1){
      int check2 = -1;
      int length;
      char *file_path;

      if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") ){
              continue;
          }
      length = pathlen + strlen(de->d_name) + 2;
      file_path = malloc(length);

      if (file_path){
        struct stat path_stat;
        snprintf(file_path, length, "%s/%s", path, de->d_name);
        if(!stat(file_path, &path_stat)){
          if(S_ISDIR(path_stat.st_mode)){
            printf("removing dir %s\n", file_path);
            check2 = remdir(file_path);
          }
          else{
            printf("removing file %s\n", file_path);
            check2 = remove(file_path);
          }
        }
        free(file_path);
      }
      check1 = check2;
    }
    closedir(dr);
  }
  if(!check1){
    check1 = rmdir(path);
  }
  return check1;

}