#ifndef UTILS_H
#define UTILS_H

// Check if email+password exists in a file
// Returns 1 if found, 0 otherwise
// Stores the matched role into role (optional, can be NULL)
// int checkCredentials(const char* filename, const char* email, const char* password, char* role);

// Generate a new unique ID based on the last ID in the file
// Assumes ID is the first field in each line (integer)
int generateNewID(const char *filename, const char *role);

// Check if a user is approved (only for restaurants & deliveries)
// Returns 1 if Approved, 0 if Pending/Rejected/Not Found
bool emailCheck(const char *email);

#endif
