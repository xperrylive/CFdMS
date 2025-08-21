#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"


void displayMainMenu();
void registerUser();
void registrationMenu();
void studentRegistration();
void loginUser();


typedef struct 
{
    int id;
    char name[50];
    char email[100];
    char password[100];
    char address[256];
    char phone_number[20];
    double balance;

}student;



int main (void)
{
    int choice;
    char buffer[10];

    while (true) 
    {
        displayMainMenu();
        fgets(buffer, sizeof(buffer), stdin);
        choice = atoi(buffer);
    
    
        switch (choice)
        {
        case 1:
            registerUser();
            continue;
        case 2:
            loginUser();
            break;
        case 3:
            return 0;
    
        default:
            printf("Please choose a valid option\n");
            continue;
        }
    }
}




void loginUser() 
{
    char email[256];
    char password[100];
    
    for (int attempt = 1; attempt <= 3; attempt++)
    {
        printf("\n===== Login =====\n");
    
        // Email
        printf("Enter your email: ");
        fgets(email, sizeof(email), stdin);
        email[strcspn(email, "\n")] = '\0';
    
        // Password
        printf("Enter your password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = '\0';
    
        // Call checkCredentials
        struct loginResult result = checkCredentials(email, password);
    
        if (result.status == 0) 
        {
            printf("\nLogin successful! Welcome, %s (ID: %d, Role: %s)\n", 
                    email, result.id, result.role);
    
            // Redirect based on role
            if (strcmp(result.role, "admin") == 0) {
                // adminMenu();

            } else if (strcmp(result.role, "student") == 0) {
            // harsh write your main menu function here
    
            } else if (strcmp(result.role, "restaurant") == 0) {
                restraurantMenu(result.id)
    
            } else if (strcmp(result.role, "delivery") == 0) {
                // zakaryia write your main menu function here
            }
            else{
                printf("something went wrong. please contatct the administration regarding your role.\n");
                return;
            }
    
        } 
        else if (result.status == 2) 
        {
            printf("Wrong password. Try again (%d/3 attempts left).\n", 3 - attempt);
        } 
        else 
        {
            printf("Email not found. Try again (%d/3 attempts left).\n", 3 - attempt);
        }

        if (attempt == 3)
        {
            printf("\nMaximum login attempts reached. Returning to main menu.\n");
        }
    }
}


void registerUser()
{
    char buffer[10];
    while (true)
    {
        registrationMenu();
        fgets(buffer, sizeof(buffer), stdin);
        int choice = atoi(buffer);
    
        if (choice == 1)
        {
            studentRegistration();
            return;
        }
    
        else if (choice == 2)
        {
            // write your code to register restaurants (only call the function from your file)
            return;
        }
        else if (choice == 3)
        {
            // write your code to register delivery personnel (only call the function from your file)
            return;
        }
        else if (choice == 4) return;
        else 
        {
            printf("Please choose a valid option\n"); 
            continue;
        }
    }
}





void studentRegistration()
{
    student s;
    char buffer[256];
    
    // Name
    printf("Enter your name: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(s.name, buffer);
    
    // Email (Unique)
    do {
        printf("Enter your email: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        strcpy(s.email, buffer);
    } while (!emailCheck(s.email));
    
    
    // Password
    printf("Enter your password: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(s.password, buffer);
    
    // Address
    printf("Enter your address: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(s.address, buffer);
    
    // Phone
    printf("Enter your phone number: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(s.phone_number, buffer);
    
    // Balance (numeric input)
    printf("Enter your initial balance: ");
    fgets(buffer, sizeof(buffer), stdin);
    s.balance = atof(buffer);   // atof converts string -> double
    
    //generate new id
    s.id = generateNewID("data/students.txt", "student");
    
    FILE *fp = fopen("data/students.txt", "a");
    if (!fp) 
    {
        printf("Error while opening the file.\n");
        return;
    }
    
    fprintf( fp, "%d|%s|%s|%s|%.2f|%s|%s\n", 
        s.id, s.name, s.email, s.password, s.balance, s.phone_number, s.address
    );
    fclose(fp);
    printf("\nYou have been registered successfully! Your ID is %d\n", s.id);
}


void displayMainMenu()
{
    printf("\n=============================\n");
    printf("   Campus Food Delivery System\n");
    printf("=============================\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf("-----------------------------\n");
    printf("Enter your choice: ");
}
void registrationMenu()
{

    printf("\n=============================\n");
    printf("   Registration Menu \n");
    printf("=============================\n");
    printf("   Select Your Role \n");
    printf("\n1. Student\n");
    printf("2. Restaurant\n");
    printf("3. Delivery Personnel\n");
    printf("4. Exit\n");
    printf("-----------------------------\n");
    printf("Enter your choice: ");
}