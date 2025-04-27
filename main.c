#include <dirent.h> 
#include <stdio.h> 
#include <time.h>
#include <sys/stat.h>

const char* directory_path = "SomeFiles";

char* check_file(const char* path);

int main() {
    DIR *dir = opendir(directory_path);
    if (!dir) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;

    char filePath[100];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(filePath, sizeof(filePath), "%s/%s", directory_path, entry->d_name);

            char* result = check_file(filePath);

            printf("%s: %s\n", entry->d_name, result);
        }
    }
    closedir(dir);

    return 0;
}


char* check_file(const char* path) {
    static char time_str[100];
    char check_str[100];

    struct stat file_stat;
    
    int err = stat(path, &file_stat);
    if (err != 0) {
        perror("error");
        return 0;
    }

    time_t now = time(NULL);
    time_t seven_days_ago = now - (7 * 24 * 60 * 60);

    int check = file_stat.st_mtime >= seven_days_ago;

    if (check) {
        struct tm *time_info = localtime(&file_stat.st_mtime);
        
        strftime(time_str, sizeof(time_str), "%a %b %d %H:%M:%S %Y", time_info);

        snprintf(check_str, sizeof(check_str), "\nModified: %d", check);
        
        strcat(time_str, check_str);

        return time_str;
    }
    else
        return 0;
}