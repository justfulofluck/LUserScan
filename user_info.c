
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <utmpx.h>

// Function declarations
void list_users();
void list_groups();
void user_groups(const char *username);
void file_permissions(const char *path);
void search_user(const char *query);
void search_group(const char *query);
void list_logged_in_users();

// Main function
int main() {
    int choice;
    char input[100];

    while (1) {
        printf("\n\033[1;34m====== Linux User Information ======\033[0m\n");
        printf("1. List all Users\n");
        printf("2. List all Groups\n");
        printf("3. Show groups of a user\n");
        printf("4. Show file or folder Permissions\n");
        printf("5. Search User (Name or UID)\n");
        printf("6. Search Group (Name or GID)\n");
        printf("7. List Currently Logged-in Users\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0) continue;

        char *endptr;
        choice = strtol(input, &endptr, 10);
        if (*endptr != '\0') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

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
                printf("Enter search query (Username or UID): ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                search_user(input);
                break;
            case 6:
                printf("Enter search query (Group name or GID): ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                search_group(input);
                break;
            case 7:
                list_logged_in_users();
                break;
            case 8:
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
    printf("\n\033[1;32m--- Users on the System ---\033[0m\n");

    while ((pw = getpwent()) != NULL) {
        printf("Username: \033[1m%-15s\033[0m UID: %-5d Home: %s\n", pw->pw_name, pw->pw_uid, pw->pw_dir);
    }

    endpwent();
}

// List all groups
void list_groups() {
    struct group *gr;
    printf("\n\033[1;32m--- Groups on the System ---\033[0m\n");

    while ((gr = getgrent()) != NULL) {
        printf("Group: \033[1m%-15s\033[0m GID: %-5d ", gr->gr_name, gr->gr_gid);
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

    int ngroups = 0;
    // Call getgrouplist once to find out how many groups the user belongs to
    getgrouplist(username, pw->pw_gid, NULL, &ngroups);

    gid_t *groups = malloc(ngroups * sizeof(gid_t));
    if (groups == NULL) {
        perror("malloc");
        return;
    }

    if (getgrouplist(username, pw->pw_gid, groups, &ngroups) == -1) {
        printf("Unable to get groups for user: %s\n", username);
        free(groups);
        return;
    }

    printf("\n\033[1;32mGroups for user '%s':\033[0m\n", username);
    for (int i = 0; i < ngroups; ++i) {
        struct group *gr = getgrgid(groups[i]);
        if (gr) {
            printf(" - \033[1m%s\033[0m (GID: %d)\n", gr->gr_name, gr->gr_gid);
        }
    }
    free(groups);
}

// Show file/directory permission details
void file_permissions(const char *path) {
    struct stat filestat;

    if (stat(path, &filestat) < 0) {
        perror("stat");
        return;
    }

    printf("\n\033[1;32m--- File Information ---\033[0m\n");
    printf("Path: \033[1m%s\033[0m\n", path);
    printf("Size: %ld bytes\n", (long)filestat.st_size);
    printf("Owner UID: %d\n", filestat.st_uid);
    printf("Group GID: %d\n", filestat.st_gid);
    printf("Last modified: %s", ctime(&filestat.st_mtime));

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

// Search for a user by name or UID
void search_user(const char *query) {
    struct passwd *pw;
    int found = 0;
    char *endptr;
    uid_t uid = (uid_t)strtol(query, &endptr, 10);
    int query_is_numeric = (*endptr == '\0' && strlen(query) > 0);

    printf("\n\033[1;32m--- User Search Results for '%s' ---\033[0m\n", query);

    setpwent();
    while ((pw = getpwent()) != NULL) {
        if (strcmp(pw->pw_name, query) == 0 || (query_is_numeric && pw->pw_uid == uid)) {
            printf("Username: \033[1m%-15s\033[0m UID: %-5d Home: %s\n", pw->pw_name, pw->pw_uid, pw->pw_dir);
            found = 1;
            if (!query_is_numeric) break; // If searching by name, name is unique, so we can stop
        }
    }
    endpwent();

    if (!found) {
        printf("No user found matching '%s'.\n", query);
    }
}

// Search for a group by name or GID
void search_group(const char *query) {
    struct group *gr;
    int found = 0;
    char *endptr;
    gid_t gid = (gid_t)strtol(query, &endptr, 10);
    int query_is_numeric = (*endptr == '\0' && strlen(query) > 0);

    printf("\n\033[1;32m--- Group Search Results for '%s' ---\033[0m\n", query);

    setgrent();
    while ((gr = getgrent()) != NULL) {
        if (strcmp(gr->gr_name, query) == 0 || (query_is_numeric && gr->gr_gid == gid)) {
            printf("Group: \033[1m%-15s\033[0m GID: %-5d ", gr->gr_name, gr->gr_gid);
            printf("Members: ");
            if (gr->gr_mem[0]) {
                for (int i = 0; gr->gr_mem[i] != NULL; i++) {
                    printf("%s ", gr->gr_mem[i]);
                }
            } else {
                printf("None");
            }
            printf("\n");
            found = 1;
            if (!query_is_numeric) break;
        }
    }
    endgrent();

    if (!found) {
        printf("No group found matching '%s'.\n", query);
    }
}

// List currently logged-in users
void list_logged_in_users() {
    struct utmpx *ut;
    printf("\n\033[1;32m--- Currently Logged-in Users ---\033[0m\n");
    printf("\033[1m%-15s %-10s %-20s %s\033[0m\n", "User", "Line", "Host", "Login Time");

    setutxent();
    while ((ut = getutxent()) != NULL) {
        if (ut->ut_type == USER_PROCESS) {
            time_t login_time = ut->ut_tv.tv_sec;
            char *time_str = ctime(&login_time);
            if (time_str) {
                time_str[strlen(time_str) - 1] = '\0'; // Remove newline
            } else {
                time_str = "Unknown";
            }
            printf("%-15s %-10s %-20s %s\n", ut->ut_user, ut->ut_line, ut->ut_host, time_str);
        }
    }
    endutxent();
}
