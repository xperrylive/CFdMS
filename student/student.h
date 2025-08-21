#ifndef STUDENT_H
#define STUDENT_H

typedef struct {
    int id;
    char name[50];
    char email[100];
    char password[100];
    char address[256];
    char phone_number[20];
    double balance;
} Student;

// Student functionalities
void studentMainMenu(int studentID);

void viewProfile(int studentID);
void updateProfile(int studentID);
void deleteProfile(int studentID);
void placeOrder(int studentID);
void viewOrderHistory(int studentID);
void manageBalance(int studentID);

#endif
