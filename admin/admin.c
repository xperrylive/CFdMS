#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "admin.h"
#include "../utils.h"
#define LINE_SIZE 256

void adminMenu() {
    int choice;
    char buffer[10];

    while (1) {
        printf("\n==========================================\n");
        printf("          ADMIN CONTROL PANEL\n");
        printf("==========================================\n");
        printf("1. User Management\n");
        printf("2. Applications Management\n");
        printf("3. Reports\n");
        printf("4. Backup & Restore\n");
        printf("5. Logout\n");
        printf("==========================================\n");
        printf("Enter your choice (1-5): ");

        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        choice = atoi(buffer);

        if (choice == 1) {
            userManagementMenu();
        } else if (choice == 2) {
            applicationManagementMenu();
        } else if (choice == 3) {
            generateReports();
        } else if (choice == 4) {
            backupRestoreMenu();
        } else if (choice == 5) {
            printf("Logging out...\n");
            return;
        } else {
            printf("Invalid choice! Please try again.\n");
        }
    }
}

void userManagementMenu() {
    int choice;
    char buffer[10];

    printf("\n--- USER MANAGEMENT ---\n");
    printf("1. View All Users\n");
    printf("2. Update User Details\n");
    printf("3. Delete User\n");
    printf("Enter choice: ");

    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    choice = atoi(buffer);

    if (choice == 1) {
        viewAllUsers();
    } else if (choice == 2) {
        updateUser();
    } else if (choice == 3) {
        deleteUser();
    } else {
        printf("Invalid choice!\n");
    }
}

void applicationManagementMenu() {
    int choice;
    char buffer[10];

    printf("\n--- APPLICATIONS MANAGEMENT ---\n");
    printf("1. View Pending Applications\n");
    printf("2. Approve Application\n");
    printf("3. Reject Application\n");
    printf("Enter choice: ");

    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    choice = atoi(buffer);

    if (choice == 1) {
        viewApplications();
    } else if (choice == 2) {
        approveApplication();
    } else if (choice == 3) {
        rejectApplication();
    } else {
        printf("Invalid choice!\n");
    }
}

void backupRestoreMenu() {
    int choice;
    char buffer[10];

    printf("\n--- BACKUP & RESTORE ---\n");
    printf("1. Backup Data\n");
    printf("2. Restore Data\n");
    printf("Enter choice: ");

    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    choice = atoi(buffer);

    if (choice == 1) {
        backupData();
    } else if (choice == 2) {
        restoreData();
    } else {
        printf("Invalid choice!\n");
    }
}


void viewAllUsers() {
    FILE *fp;
    char line[LINE_SIZE];

    printf("\n========== ALL USERS ==========\n");

    // Students
    printf("\n--- STUDENTS ---\n");
    fp = fopen("data/students.txt", "r");
    if (fp == NULL) {
        printf("No students file found.\n");
    } else {
        while (fgets(line, sizeof(line), fp)) {
            line[strcspn(line, "\n")] = '\0';
            printf("%s\n", line);
        }
        fclose(fp);
    }

    // Restaurants
    printf("\n--- RESTAURANTS ---\n");
    fp = fopen("data/restaurants.txt", "r");
    if (fp == NULL) {
        printf("No restaurants file found.\n");
    } else {
        while (fgets(line, sizeof(line), fp)) {
            line[strcspn(line, "\n")] = '\0';
            printf("%s\n", line);
        }
        fclose(fp);
    }

    // Delivery
    printf("\n--- DELIVERY PERSONNEL ---\n");
    fp = fopen("data/delivery.txt", "r");
    if (fp == NULL) {
        printf("No delivery file found.\n");
    } else {
        while (fgets(line, sizeof(line), fp)) {
            line[strcspn(line, "\n")] = '\0';
            printf("%s\n", line);
        }
        fclose(fp);
    }

    // Admins
    printf("\n--- ADMINS ---\n");
    fp = fopen("data/admin.txt", "r");
    if (fp == NULL) {
        printf("No admin file found.\n");
    } else {
        while (fgets(line, sizeof(line), fp)) {
            line[strcspn(line, "\n")] = '\0';
            printf("%s\n", line);
        }
        fclose(fp);
    }
}


void updateUser() {
    char userType[20];
    char buffer[20];
    int userId;
    int found = 0;

    printf("\n--- UPDATE USER ---\n");
    printf("Enter user type (student / restaurant / delivery): ");
    fgets(userType, sizeof(userType), stdin);
    userType[strcspn(userType, "\n")] = '\0';

    printf("Enter user ID: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    userId = atoi(buffer);

    char filename[50];
    if (strcmp(userType, "student") == 0) {
        strcpy(filename, "data/students.txt");
    } else if (strcmp(userType, "restaurant") == 0) {
        strcpy(filename, "data/restaurants.txt");
    } else if (strcmp(userType, "delivery") == 0) {
        strcpy(filename, "data/delivery.txt");
    } else {
        printf("Invalid user type!\n");
        return;
    }

    FILE *fp = fopen(filename, "r");
    FILE *temp = fopen("data/temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), fp)) {
        int id;
        sscanf(line, "%d|", &id);

        if (id == userId) {
            found = 1;
            printf("Current: %s", line);

            char newName[100] = "";
            char newPhone[50] = "";
            char newAddress[200] = "";
            char newLocation[200] = "";
            char newStatus[50] = "";

            printf("Enter new name (leave blank to keep): ");
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = '\0';

            if (strcmp(userType, "student") == 0) {
                char name[100], email[100], password[100], phone[50], address[200];
                float balance;

                sscanf(line, "%d|%99[^|]|%99[^|]|%99[^|]|%f|%49[^|]|%199[^\n]",
                       &id, name, email, password, &balance, phone, address);

                if (strlen(newName) > 0) strcpy(name, newName);

                printf("Enter new phone (leave blank to keep): ");
                fgets(newPhone, sizeof(newPhone), stdin);
                newPhone[strcspn(newPhone, "\n")] = '\0';
                if (strlen(newPhone) > 0) strcpy(phone, newPhone);

                printf("Enter new address (leave blank to keep): ");
                fgets(newAddress, sizeof(newAddress), stdin);
                newAddress[strcspn(newAddress, "\n")] = '\0';
                if (strlen(newAddress) > 0) strcpy(address, newAddress);

                fprintf(temp, "%d|%s|%s|%s|%.2f|%s|%s\n",
                        id, name, email, password, balance, phone, address);

            } else if (strcmp(userType, "restaurant") == 0) {
                char name[100], email[100], password[100], location[200], phone[50];

                sscanf(line, "%d|%99[^|]|%99[^|]|%99[^|]|%199[^|]|%49[^\n]",
                       &id, name, email, password, location, phone);

                if (strlen(newName) > 0) strcpy(name, newName);

                printf("Enter new location (leave blank to keep): ");
                fgets(newLocation, sizeof(newLocation), stdin);
                newLocation[strcspn(newLocation, "\n")] = '\0';
                if (strlen(newLocation) > 0) strcpy(location, newLocation);

                printf("Enter new phone (leave blank to keep): ");
                fgets(newPhone, sizeof(newPhone), stdin);
                newPhone[strcspn(newPhone, "\n")] = '\0';
                if (strlen(newPhone) > 0) strcpy(phone, newPhone);

                fprintf(temp, "%d|%s|%s|%s|%s|%s\n",
                        id, name, email, password, location, phone);

            } else if (strcmp(userType, "delivery") == 0) {
                char name[100], email[100], password[100], status[50];
                float earnings;

                sscanf(line, "%d|%99[^|]|%99[^|]|%99[^|]|%49[^|]|%f",
                       &id, name, email, password, status, &earnings);

                if (strlen(newName) > 0) strcpy(name, newName);

                printf("Enter new status (leave blank to keep): ");
                fgets(newStatus, sizeof(newStatus), stdin);
                newStatus[strcspn(newStatus, "\n")] = '\0';
                if (strlen(newStatus) > 0) strcpy(status, newStatus);

                fprintf(temp, "%d|%s|%s|%s|%s|%.2f\n",
                        id, name, email, password, status, earnings);
            }

        } else {
            fputs(line, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(filename);
        rename("data/temp.txt", filename);
        printf("User updated successfully!\n");
    } else {
        remove("data/temp.txt");
        printf("User not found!\n");
    }
}



void deleteUser() {
    char userType[20];
    char buffer[10];
    int userId;

    printf("\n--- DELETE USER ---\n");
    printf("Enter user type (student/restaurant/delivery): ");
    fgets(userType, sizeof(userType), stdin);
    userType[strcspn(userType, "\n")] = '\0';

    printf("Enter user ID to delete: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    userId = atoi(buffer);

    char filename[50];

    if (strcmp(userType, "student") == 0) {
        strcpy(filename, "data/students.txt");
    } else if (strcmp(userType, "restaurant") == 0) {
        strcpy(filename, "data/restaurants.txt");
    } else if (strcmp(userType, "delivery") == 0) {
        strcpy(filename, "data/delivery.txt");
    } else {
        printf("Invalid user type!\n");
        return;
    }

    FILE *file = fopen(filename, "r");
    FILE *temp = fopen("data/temp.txt", "w");
    char line[500];
    int found = 0;

    if (file == NULL || temp == NULL) {
        printf("Error opening files!\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d|", &id);

        if (id == userId) {
            found = 1;
            printf("✅ User with ID %d deleted.\n", userId);
        } else {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove(filename);
        rename("data/temp.txt", filename);
    } else {
        remove("data/temp.txt");
        printf("❌ User not found!\n");
    }
}


void viewApplications() {
    FILE *fp = fopen("data/applications.txt", "r");
    char line[LINE_SIZE];

    printf("\n========== PENDING APPLICATIONS ==========\n");

    if (!fp) {
        printf("No applications found.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        printf("%s\n", line);   // print line directly
    }

    fclose(fp);
}

void approveApplication() {
    FILE *fp = fopen("data/applications.txt", "r");
    FILE *temp = fopen("data/temp.txt", "w");
    char line[LINE_SIZE];
    char email[100];
    int found = 0;

    if (!fp || !temp) {
        printf("Error opening files.\n");
        return;
    }

    printf("Enter email of applicant to approve: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0';

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';

        char *token;
        char role[20], name[100], fileEmail[100], password[100];
        char extra1[200], extra2[100];

        token = strtok(line, "|"); if (!token) continue;
        strcpy(role, token);

        token = strtok(NULL, "|"); if (!token) continue;
        strcpy(name, token);

        token = strtok(NULL, "|"); if (!token) continue;
        strcpy(fileEmail, token);

        token = strtok(NULL, "|"); if (!token) continue;
        strcpy(password, token);

        token = strtok(NULL, "|"); if (!token) continue;
        strcpy(extra1, token);

        token = strtok(NULL, "|"); if (!token) strcpy(extra2, ""); 
        else strcpy(extra2, token);

        if (strcmp(fileEmail, email) == 0) {
            found = 1;
            int newId;

            if (strcmp(role, "restaurant") == 0) {
                newId = generateNewID("data/restaurants.txt", "restaurant");
                FILE *restFile = fopen("data/restaurants.txt", "a");
                if (restFile) {
                    fprintf(restFile, "%d|%s|%s|%s|%s|%s\n",
                            newId, name, fileEmail, password, extra1, extra2);
                    fclose(restFile);
                    printf("Application approved → added to restaurants.\n");
                }
            } else if (strcmp(role, "delivery") == 0) {
                newId = generateNewID("data/delivery.txt", "delivery");
                FILE *delFile = fopen("data/delivery.txt", "a");
                if (delFile) {
                    fprintf(delFile, "%d|%s|%s|%s|%s\n",
                            newId, name, fileEmail, password, extra2);
                    fclose(delFile);
                    printf("Application approved → added to delivery.\n");
                }
            } else {
                printf("Unknown role: %s (skipped)\n", role);
            }
        } else {
            fprintf(temp, "%s|%s|%s|%s|%s|%s\n", 
                    role, name, fileEmail, password, extra1, extra2);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("data/applications.txt");
        rename("data/temp.txt", "data/applications.txt");
    } else {
        remove("data/temp.txt");
        printf("Application not found.\n");
    }
}

void rejectApplication() {
    FILE *fp = fopen("data/applications.txt", "r");
    FILE *temp = fopen("data/temp.txt", "w");
    char line[LINE_SIZE];
    char email[100];
    int found = 0;

    if (!fp || !temp) {
        printf("Error opening files.\n");
        return;
    }

    printf("Enter email of applicant to reject: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0';

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';

        char *token;
        char role[20], name[100], fileEmail[100], password[100];
        char extra1[200], extra2[100];

        token = strtok(line, "|"); if (!token) continue;
        strcpy(role, token);

        token = strtok(NULL, "|"); if (!token) continue;
        strcpy(name, token);

        token = strtok(NULL, "|"); if (!token) continue;
        strcpy(fileEmail, token);

        token = strtok(NULL, "|"); if (!token) continue;
        strcpy(password, token);

        token = strtok(NULL, "|"); if (!token) strcpy(extra1, ""); 
        else strcpy(extra1, token);

        token = strtok(NULL, "|"); if (!token) strcpy(extra2, ""); 
        else strcpy(extra2, token);

        if (strcmp(fileEmail, email) == 0) {
            found = 1;
            printf("Application rejected and removed.\n");
        } else {
            fprintf(temp, "%s|%s|%s|%s|%s|%s\n", 
                    role, name, fileEmail, password, extra1, extra2);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("data/applications.txt");
        rename("data/temp.txt", "data/applications.txt");
    } else {
        remove("data/temp.txt");
        printf("Application not found.\n");
    }
}


void generateReports() {
    FILE *fp;
    char line[LINE_SIZE];
    int studentCount = 0;
    int restaurantCount = 0;
    int deliveryCount = 0;

    printf("\n========== SYSTEM REPORTS ==========\n");

    // --- Count Students ---
    fp = fopen("data/students.txt", "r");
    if (fp) {
        while (fgets(line, sizeof(line), fp)) {
            studentCount++;
        }
        fclose(fp);
    }

    // --- Count Restaurants ---
    fp = fopen("data/restaurants.txt", "r");
    if (fp) {
        while (fgets(line, sizeof(line), fp)) {
            restaurantCount++;
        }
        fclose(fp);
    }

    // --- Count Delivery Personnel ---
    fp = fopen("data/delivery.txt", "r");
    if (fp) {
        while (fgets(line, sizeof(line), fp)) {
            deliveryCount++;
        }
        fclose(fp);
    }

    printf("Total Students: %d\n", studentCount);
    printf("Total Restaurants: %d\n", restaurantCount);
    printf("Total Delivery Personnel: %d\n", deliveryCount);
    printf("Total Users: %d\n", studentCount + restaurantCount + deliveryCount);
    printf("=====================================\n");
}

// helper function to copy one file
void copyFile(const char *src, const char *dest) {
    FILE *fSrc = fopen(src, "r");
    if (!fSrc) {
        // silently skip if file doesn't exist
        return;
    }

    FILE *fDest = fopen(dest, "w");
    if (!fDest) {
        fclose(fSrc);
        printf("Error: could not create %s\n", dest);
        return;
    }

    char buffer[512];
    while (fgets(buffer, sizeof(buffer), fSrc)) {
        fputs(buffer, fDest);
    }

    fclose(fSrc);
    fclose(fDest);
}

// --- Backup Data ---
void backupData() {
    printf("\n--- BACKUP DATA ---\n");
    system("mkdir -p backup");

    // copy all data files to backup folder
    copyFile("data/students.txt", "backup/students.txt");
    copyFile("data/restaurants.txt", "backup/restaurants.txt");
    copyFile("data/delivery.txt", "backup/delivery.txt");
    copyFile("data/applications.txt", "backup/applications.txt");
    copyFile("data/admin.txt", "backup/admin.txt");

    printf("Backup completed! Files copied to backup/ folder.\n");
}

// --- Restore Data ---
void restoreData() {
    printf("\n--- RESTORE DATA ---\n");

    char confirm[10];
    printf("This will overwrite current data files. Are you sure? (yes/no): ");
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\n")] = '\0';

    if (strcmp(confirm, "yes") == 0) {
        copyFile("backup/students.txt", "data/students.txt");
        copyFile("backup/restaurants.txt", "data/restaurants.txt");
        copyFile("backup/delivery.txt", "data/delivery.txt");
        copyFile("backup/applications.txt", "data/applications.txt");
        copyFile("backup/admin.txt", "data/admin.txt");

        printf("Restore completed! Files copied back to data/ folder.\n");
    } else {
        printf("Restore cancelled.\n");
    }
}