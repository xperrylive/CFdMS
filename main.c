#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"


void displayMainMenu();
int registerUser();
void registrationMenu();
void studentRegistration();


typedef struct 
{
    int id;
    char name[50];
    char email[100];
    char password[100];
    char address[256];
    char phone_number[20];
    double balance;

}students;



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
            //loginUser();
            break;
        case 3:
            return 0;
    
        default:
            printf("Please choose a valid option");
            continue;
        }
    }
}



int registerUser()
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
        }
    
        else if (choice == 2)
        {
            // write your code to register restaurants (only call the function from your file)
        }
        else if (choice == 3)
        {
            // write your code to register delivery personnel (only call the function from your file)
        }
        else if (choice == 4) return 0;
        else 
        {
            printf("Please choose a valid option"); 
            continue;
        }
    }
}





void studentRegistration()
{
    students student;
    char buffer[256];
    
    // Name
    printf("Enter your name: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(student.name, buffer);
    
    // Email (Unique)
    do {
        printf("Enter your email: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        strcpy(student.email, buffer);
    } while (!emailCheck(student.email));
    
    
    // Password
    printf("Enter your password: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(student.password, buffer);
    
    // Address
    printf("Enter your address: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(student.address, buffer);
    
    // Phone
    printf("Enter your phone number: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(student.phone_number, buffer);
    
    // Balance (numeric input)
    printf("Enter your initial balance: ");
    fgets(buffer, sizeof(buffer), stdin);
    student.balance = atof(buffer);   // atof converts string -> double
    
    //generate new id
    student.id = generateNewID("data/students.txt", "student");
    
    FILE *fp = fopen("data/students.txt", "a");
    if (!fp) 
    {
        printf("Error while opening the file.\n");
        return 1;
    }
    
    fprintf( fp, "%d|%s|%s|%s|%.2f|%s|%s\n", 
        student.id, student.name, student.email, student.password, student.balance, student.phone_number, student.address
    );
    fclose(fp);
    printf("\nYou have been registered successfully! Your ID is %d\n", student.id);
    return 0;
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