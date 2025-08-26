#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "student/student.h"
#include "restaurant/restaurant.h"
#include "delivery/delivery.h"
#include "admin/admin.h"

void displayMainMenu();
void registerUser();
void registrationMenu();
void studentRegistration();
void loginUser();



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
                adminMenu();

            } else if (strcmp(result.role, "student") == 0) {
                studentMainMenu(result.id);
                return; // return to main menu after student logs out
    
            } else if (strcmp(result.role, "restaurant") == 0) {
                restaurantmain(result.id);
                return;
    
            } else if (strcmp(result.role, "delivery") == 0) {
                deliveryMainMenu(result.id);
                return;
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
        else if (result.status == 3) 
        {
            printf("Your account is still pending approval by the admin.\n");
            return;

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
            registerRestaurant() ;// write your code to register restaurants (only call the function from your file)
            return;
        }
        else if (choice == 3)
        {
            registerDeliveryPerson();
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
