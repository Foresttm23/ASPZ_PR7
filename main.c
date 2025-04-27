#include <dirent.h> 
#include <stdio.h> 
#include <time.h>
#include <sys/stat.h>

const char* directory_path = "SomeFiles";

int check_file(const char* path);

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

            int result = check_file(filePath);

            printf("%s: %d\n", entry->d_name, result);
        }
    }
    closedir(dir);

    return 0;
}


int check_file(const char* path) {
    struct stat file_stat;
    
    int err = stat(path, &file_stat);
    if (err != 0) {
        perror("error");
        return 0;
    }

    time_t now = time(NULL);
    time_t seven_days_ago = now - (7 * 24 * 60 * 60);

    return file_stat.st_mtime >= seven_days_ago;
}