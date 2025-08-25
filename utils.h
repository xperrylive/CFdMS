#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>


typedef struct {
    int id;
    char name[100];
    char email[256];
    char password[100];
} userDetials;

struct loginResult {
    int status; // 0 success, 1 wrong email, 2 wrong password, 3 account waiting for approval
    int id;
    char role[10];
};


// Helper functions

struct loginResult checkCredentials(const char* email, const char* password);
int generateNewID(const char *filename, const char *role);
bool emailCheck(const char *email);

#endif
