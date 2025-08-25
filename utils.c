#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define LINE_SIZE 256

// --- 4. Check Credentials ---
struct loginResult checkCredentials(const char* email, const char* password) 
{
    struct loginResult result = {1, -1, ""}; // default = email not found

    const char *files[] = {
        "data/admin.txt", 
        "data/students.txt",
        "data/restaurants.txt",
        "data/delivery.txt" 
    };

    const char *role[] = {
        "admin",
        "student",
        "restaurant",
        "delivery"
    };
    int fileCount = sizeof(files) / sizeof(files[0]);
    
    char line[256];
    char *token;
    userDetials user;

    // --- check normal active accounts ---
    for (int i = 0; i < fileCount; i++)
    {
        FILE *fp = fopen(files[i], "r");
        if (!fp) continue;
        while(fgets(line, sizeof(line), fp))
        {
            token = strtok(line, "|");  
            if (!token) continue;
            user.id = atoi(token);
            
            token = strtok(NULL, "|");  
            if (!token) continue;
            strcpy(user.name, token);
            user.name[strcspn(user.name, "\n")] = '\0';  

            token = strtok(NULL, "|");  
            if (!token) continue;       
            strcpy(user.email, token);
            user.email[strcspn(user.email, "\n")] = '\0';   

            token = strtok(NULL, "|");  
            if (!token) continue;
            strcpy(user.password, token);
            user.password[strcspn(user.password, "\n")] = '\0';

            if (strcmp(user.email, email) == 0)
            {
                if (strcmp(user.password, password) == 0)
                {
                    fclose(fp);                    
                    result.status = 0;
                    result.id = user.id;
                    strcpy(result.role, role[i]);
                    return result;   // successful login
                }
                else 
                {
                    fclose(fp);
                    result.status = 2;   // wrong password
                    return result;
                }
            }
        }
        fclose(fp);
    }

    // --- check pending applications ---
    FILE *applications = fopen("data/applications.txt", "r");
    if (applications) {
        while (fgets(line, sizeof(line), applications)) {
            char *roleApp, *nameApp, *emailApp, *passApp, *extra1, *extra2;
            nameApp = strtok(line, "|");
            emailApp = strtok(NULL, "|");
            passApp = strtok(NULL, "|");
            extra1= strtok(NULL, "|");
            extra2= strtok(NULL, "|");
            roleApp= strtok(NULL, "|");

            if (emailApp && strcmp(emailApp, email) == 0) {
                fclose(applications);
                result.status = 3;   // pending / not activated
                strcpy(result.role, roleApp ? roleApp : "pending");
                return result;
            }
        }
        fclose(applications);
    }

    result.status = 1;   // email not found anywhere
    return result;
}

 


// Returns true if email is valid and unique
bool emailCheck(const char *email) {
    // 1. Check format
    if (strchr(email, '@') == NULL) {
        printf(" Invalid email. Must contain '@'.\n");
        return false;
    }

    // 2. Files to check
    const char *files[] = {
        "data/students.txt",
        "data/restaurants.txt",
        "data/delivery.txt",
        "data/admin.txt"
    };
    int fileCount = sizeof(files) / sizeof(files[0]);

    char line[512];
    for (int i = 0; i < fileCount; i++) {
        FILE *fp = fopen(files[i], "r");
        if (!fp) continue;

        while (fgets(line, sizeof(line), fp)) {
            char fileEmail[100];
            // Format: ID|Name|Email|Password|...
            sscanf(line, "%*d|%*[^|]|%99[^|]|", fileEmail);

            if (strcmp(fileEmail, email) == 0) {
                fclose(fp);
                printf(" Email already exists.\n");
                return false;
            }
        }
        fclose(fp);
    }

    return true; // passed all checks
}


//generate new IDs
int generateNewID(const char *filename, const char *role) 
{

    int Default_ID;

    if (strcmp(role, "student") == 0)
    {
        Default_ID = 1000;
    }
    else if (strcmp(role, "restaurant") == 0)
    {
         Default_ID = 2000;
    } 
    else if (strcmp(role, "delivery") == 0)
    {
        Default_ID = 3000;
    }
    else if (strcmp(role, "menuItem") == 0)
    {
        Default_ID = 100;
    }
    else
    {
        printf("Couldnt identify the role.\n");
        return -1; 
    } 

    FILE *fp = fopen(filename, "r");
    if (!fp) return Default_ID; // start IDs from 1000

    // checking if the file is empty 
    char line[256];
    int lastID = 0;

    if(fgets(line, sizeof(line), fp) == NULL)
    {
        fclose(fp);
        return Default_ID; 
    }

    rewind(fp); // return to the beginning of the file

    while (fgets(line, sizeof(line), fp)) 
    {
        int id;
        sscanf(line, "%d|", &id);
        if (id > lastID) lastID = id;
    }
    fclose(fp);
    return lastID + 1;  
}