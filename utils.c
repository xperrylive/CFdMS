#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define LINE_SIZE 256

// --- 4. Check Credentials ---
int checkCredentials(const char* filename, const char* email, const char* password, char* role) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open %s\n", filename);
        return 0;
    }

    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), file)) {
        char fileEmail[50], filePassword[50], fileRole[20];
        int id;

        // Expected format: id|role|name|email|password|... 
        // Adjust strtok parsing depending on your file design
        char* token = strtok(line, "|"); // id
        if (!token) continue;
        id = atoi(token);

        token = strtok(NULL, "|"); // role
        if (!token) continue;
        strcpy(fileRole, token);

        token = strtok(NULL, "|"); // name (skip)
        token = strtok(NULL, "|"); // email
        if (!token) continue;
        strcpy(fileEmail, token);

        token = strtok(NULL, "|"); // password
        if (!token) continue;
        strcpy(filePassword, token);

        // Compare
        if (strcmp(fileEmail, email) == 0 && strcmp(filePassword, password) == 0) {
            if (role) strcpy(role, fileRole);
            fclose(file);
            return 1; // Found
        }
    }

    fclose(file);
    return 0; // Not found
}


// --- 6. Check Approval Status ---
int emailCheck(const char* filename, const char* email) 
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open %s\n", filename);
        return 0;
    }

    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), file)) {
        char fileEmail[50], status[20];
        char* token;

        token = strtok(line, "|"); // id
        token = strtok(NULL, "|"); // role
        token = strtok(NULL, "|"); // name
        token = strtok(NULL, "|"); // email
        if (!token) continue;
        strcpy(fileEmail, token);

        // skip password
        token = strtok(NULL, "|");
        token = strtok(NULL, "|"); // phone or location etc.
        token = strtok(NULL, "|"); // status
        if (!token) continue;
        strcpy(status, token);

        if (strcmp(fileEmail, email) == 0) {
            fclose(file);
            return (strcmp(status, "Approved") == 0);
        }
    }

    fclose(file);
    return 0; // Not approved or not found
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