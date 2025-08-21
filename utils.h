#ifndef UTILS_H
#define UTILS_H

typedef struct 
{
    int id;
    char name[100];
    char email[256];
    char password[100];
} userDetials;

struct loginResult
{
    int status; // 0 success, 1 wrong email, 2 wrong password
    int id;
    char role[10];
};


// Check if email+password exists in a file
// Returns 1 if found, 0 otherwise
// Stores the matched role into role (optional, can be NULL)
struct loginResult checkCredentials(const char* email, const char* password);

// Generate a new unique ID based on the last ID in the file
// Assumes ID is the first field in each line (integer)
int generateNewID(const char *filename, const char *role);

// Check if a user is approved (only for restaurants & deliveries)
// Returns 1 if Approved, 0 if Pending/Rejected/Not Found
bool emailCheck(const char *email);

#endif
