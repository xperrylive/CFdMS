#ifndef UTILS_H
#define UTILS_H

typedef struct {
    int id;
    char name[100];
    char email[256];
    char password[100];
} userDetials;

struct loginResult {
    int status; // 0 success, 1 wrong email, 2 wrong password
    int id;
    char role[10];
};


// Helper functions

struct loginResult checkCredentials(const char* email, const char* password);
int generateNewID(const char *filename, const char *role);
bool emailCheck(const char *email);

#endif
