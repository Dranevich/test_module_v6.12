#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define MODULE_PATH "../kernel/test_module.ko"
#define BASE_DIR    "/var/tmp/test_module"
#define CMD_SIZE    512
#define PATH_SIZE   256

/* Checks if test_module folder exists & creates if needed*/
static int getPath(const char *path)
{
    struct stat st;
    
    /* Case 1: file with same name exists, but it is not a directory */
    if (stat(path, &st) == 0) {
        if (!S_ISDIR(st.st_mode)) {
            fprintf(stderr,
                    "Error: %s exists but is not a directory\n",
                    path);
            return -1;
        }
        return 0;
    }
    
    /* Case 2: no file or directory with same name */
    if (errno != ENOENT) {
        perror("Error at: stat");
        return -1;
    }
    
    /* 3. Trying to create directory with name given */
    /* 0755 means what owner has full access to the directory... */
    /* ... while others may only read and write it */
    if (mkdir(path, 0755) < 0) {
        perror("Error at: mkdir");
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    char cmd[CMD_SIZE];
    char full_path[PATH_SIZE];
    const char *file_name;
    int period;
    
    /* Check if the user input is correct */
    if (argc != 3) {
        fprintf(stderr,
                "Sorry, your input is incorrect");
        fprintf(stderr,
                "Usage: %s <file_name> <period_sec>\n",
                argv[0]);
        return 1;
    }

    /* Do we already have required directory (i.e., /var/tmp/test_module)? */
    if (getPath(BASE_DIR) < 0)
        return 1;

    file_name = argv[1];
    period    = atoi(argv[2]);

    if (period <= 0) {
        fprintf(stderr, "Error: Period must be greater than zero!\n");
        return 1;
    }

    /* Prevents user from changing output directory */
    if (strchr(file_name, '/')) {
        fprintf(stderr,
                "Error: file name must not contain '/'\n");
        return 1;
    }
    
    /* Forming full filename... */
    snprintf(full_path, sizeof(full_path),
             "%s/%s", BASE_DIR, file_name);
    
    /* Demonstrate the command to execute */
    snprintf(cmd, sizeof(cmd),
             "sudo insmod %s log_path=%s period_sec=%d",
             MODULE_PATH, full_path, period);

    printf("Executing: %s\n", cmd);

    return system(cmd);
}

