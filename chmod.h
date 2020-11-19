void cmod(char *flag, char *file_name){
    int i = strtol(file_name, 0, 8);
    if (chmod (flag,i) < 0)
    {
        printf("ERROR: chmod didnt exectue");
        exit(1);
    }
}