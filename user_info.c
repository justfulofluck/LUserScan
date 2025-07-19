
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Function declarations
void list_users();
void list_groups();
void user_groups(const char *username);
void file_permissions(const char *path);

// Main function
int main() {
    int choice;
    char input[100];

    while (1) {
        printf("\n====== Linux User Information ======\n");
        printf("1. List all Users\n");
        printf("2. List all Groups\n");
        printf("3. Show groups of a user\n");
        printf("4. Show file or folder Permissions\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear newline

        switch (choice) {
            case 1:
                list_users();
                break;
            case 2:
                list_groups();
                break;
            case 3:
                printf("Enter username: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                user_groups(input);
                break;
            case 4:
                printf("Enter file or directory path: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                file_permissions(input);
                break;
            case 5:
                printf("Goodbye!\n");
                exit(0);
            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}

// List all users
void list_users() {
    struct passwd *pw;
    printf("\n--- Users on the System ---\n");

    while ((pw = getpwent()) != NULL) {
        printf("Username: %s\tUID: %d\tHome: %s\n", pw->pw_name, pw->pw_uid, pw->pw_dir);
    }

    endpwent();
}

// List all groups
void list_groups() {
    struct group *gr;
    printf("\n--- Groups on the System ---\n");

    while ((gr = getgrent()) != NULL) {
        printf("Group: %s\tGID: %d\t", gr->gr_name, gr->gr_gid);
        printf("Members: ");
        if (gr->gr_mem[0]) {
            for (int i = 0; gr->gr_mem[i] != NULL; i++) {
                printf("%s ", gr->gr_mem[i]);
            }
        } else {
            printf("None");
        }
        printf("\n");
    }

    endgrent();
}

// Show group membership for a user
void user_groups(const char *username) {
    struct passwd *pw = getpwnam(username);
    if (!pw) {
        printf("User not found: %s\n", username);
        return;
    }

    gid_t groups[32];
    int ngroups = sizeof(groups) / sizeof(gid_t);

    if (getgrouplist(username, pw->pw_gid, groups, &ngroups) == -1) {
        printf("Unable to get groups for user: %s\n", username);
        return;
    }

    printf("\nGroups for user '%s':\n", username);
    for (int i = 0; i < ngroups; ++i) {
        struct group *gr = getgrgid(groups[i]);
        if (gr) {
            printf(" - %s (GID: %d)\n", gr->gr_name, gr->gr_gid);
        }
    }
}

// Show file/directory permission details
void file_permissions(const char *path) {
    struct stat filestat;

    if (stat(path, &filestat) < 0) {
        perror("stat");
        return;
    }

    printf("\nPermissions for: %s\n", path);
    printf("Owner UID: %d\n", filestat.st_uid);
    printf("Group GID: %d\n", filestat.st_gid);

    printf("File Type: ");
    if (S_ISREG(filestat.st_mode)) printf("Regular File\n");
    else if (S_ISDIR(filestat.st_mode)) printf("Directory\n");
    else if (S_ISLNK(filestat.st_mode)) printf("Symbolic Link\n");
    else printf("Other\n");

    printf("Permissions: ");
    printf((filestat.st_mode & S_IRUSR) ? "r" : "-");
    printf((filestat.st_mode & S_IWUSR) ? "w" : "-");
    printf((filestat.st_mode & S_IXUSR) ? "x" : "-");
    printf((filestat.st_mode & S_IRGRP) ? "r" : "-");
    printf((filestat.st_mode & S_IWGRP) ? "w" : "-");
    printf((filestat.st_mode & S_IXGRP) ? "x" : "-");
    printf((filestat.st_mode & S_IROTH) ? "r" : "-");
    printf((filestat.st_mode & S_IWOTH) ? "w" : "-");
    printf((filestat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n\n");
}

