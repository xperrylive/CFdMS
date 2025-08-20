#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"

#define LINE_SIZE 256

// // --- 4. Check Credentials ---
// int checkCredentials(const char* filename, const char* email, const char* password, char* role) {
//     FILE* file = fopen(filename, "r");
//     if (!file) {
//         printf("Error: Could not open %s\n", filename);
//         return 0;
//     }

//     char line[LINE_SIZE];
//     while (fgets(line, sizeof(line), file)) {
//         char fileEmail[50], filePassword[50], fileRole[20];
//         int id;

//         // Expected format: id|role|name|email|password|... 
//         // Adjust strtok parsing depending on your file design
//         char* token = strtok(line, "|"); // id
//         if (!token) continue;
//         id = atoi(token);

//         token = strtok(NULL, "|"); // role
//         if (!token) continue;
//         strcpy(fileRole, token);

//         token = strtok(NULL, "|"); // name (skip)
//         token = strtok(NULL, "|"); // email
//         if (!token) continue;
//         strcpy(fileEmail, token);

//         token = strtok(NULL, "|"); // password
//         if (!token) continue;
//         strcpy(filePassword, token);

//         // Compare
//         if (strcmp(fileEmail, email) == 0 && strcmp(filePassword, password) == 0) {
//             if (role) strcpy(role, fileRole);
//             fclose(file);
//             return 1; // Found
//         }
//     }

//     fclose(file);
//     return 0; // Not found
// }


// Returns true if email is valid and unique
bool emailCheck(const char *email) {
    // 1. Check format
    if (strchr(email, '@') == NULL) {
        printf("❌ Invalid email. Must contain '@'.\n");
        return false;
    }

    // 2. Files to check
    const char *files[] = {
        "data/students.txt",
        "data/restaurants.txt",
        "data/delivery.txt"
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
                printf("❌ Email already exists in %s.\n", files[i]);
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