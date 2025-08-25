#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "../utils.h"

#define STUDENT_FILE "data/students.txt"
#define MENU_FILE "data/menus.txt"
#define ORDER_FILE "data/orders.txt"

// ---------------- Student Main Menu ----------------
void studentMainMenu(int studentID) {
    int choice;
    char buffer[10];

    while (1) {
        printf("\n===== Student Menu =====\n");
        printf("1. View Profile\n");
        printf("2. Update Profile\n");
        printf("3. Delete Profile\n");
        printf("4. Place Order\n");
        printf("5. View Order History\n");
        printf("6. Manage Balance\n");
        printf("7. Logout\n");
        printf("Enter your choice: ");

        fgets(buffer, sizeof(buffer), stdin);
        choice = atoi(buffer);

        switch (choice) {
            case 1: viewProfile(studentID); break;
            case 2: updateProfile(studentID); break;
            case 3: deleteProfile(studentID); return; // exit to main menu after deleting
            case 4: placeOrder(studentID); break;
            case 5: viewOrderHistory(studentID); break;
            case 6: manageBalance(studentID); break;
            case 7: printf("Logging out...\n"); return;
            default: printf("Invalid choice. Try again.\n");
        }
    }
}

// ---------------- Student Registration ----------------
void studentRegistration() {
    Student s; 
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
    
    // Generate new id
    s.id = generateNewID("data/students.txt", "student");
    
    FILE *fp = fopen(STUDENT_FILE, "a");
    if (!fp) {
        printf("Error while opening the file.\n");
        return;
    }
    
    fprintf(fp, "%d|%s|%s|%s|%.2f|%s|%s\n", 
        s.id, s.name, s.email, s.password, s.balance, s.phone_number, s.address
    );
    fclose(fp);
    printf("\nYou have been registered successfully! Your ID is %d\n", s.id);
}

// ---------------- View Profile ----------------
void viewProfile(int studentID) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    Student s;
    while (fscanf(fp, "%d|%49[^|]|%99[^|]|%99[^|]|%lf|%19[^|]|%255[^\n]\n",
                  &s.id, s.name, s.email, s.password,
                  &s.balance, s.phone_number, s.address) == 7) {
        if (s.id == studentID) {
            printf("\n--- Student Profile ---\n");
            printf("ID: %d\nName: %s\nEmail: %s\nBalance: %.2f\nPhone: %s\nAddress: %s\n",
                   s.id, s.name, s.email, s.balance, s.phone_number, s.address);
            fclose(fp);
            return;
        }
    }

    fclose(fp);
    printf("Student not found!\n");
}

// ---------------- Update Profile ----------------
void updateProfile(int studentID) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("data/temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    Student s;
    char buffer[256];
    int found = 0;

    while (fscanf(fp, "%d|%49[^|]|%99[^|]|%99[^|]|%lf|%19[^|]|%255[^\n]\n",
                  &s.id, s.name, s.email, s.password,
                  &s.balance, s.phone_number, s.address) == 7) {
        if (s.id == studentID) {
            found = 1;
            printf("Updating profile for %s\n", s.name);

            // New name
            printf("Enter New Name (leave empty to keep current): ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            if (strlen(buffer) > 0) {
                strcpy(s.name, buffer);
            }

            // New email
            printf("Enter New Email (leave empty to keep current): ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            if (strlen(buffer) > 0) {
                if (emailCheck(buffer)) {
                    strcpy(s.email, buffer);
                } else {
                    printf("Email already exists. Keeping old email.\n");
                }
            }

            // New phone
            printf("Enter New Phone (leave empty to keep current): ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            if (strlen(buffer) > 0) {
                strcpy(s.phone_number, buffer);
            }

            // New address
            printf("Enter New Address (leave empty to keep current): ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            if (strlen(buffer) > 0) {
                strcpy(s.address, buffer);
            }
        }

        // Always write updated record
        fprintf(temp, "%d|%s|%s|%s|%.2f|%s|%s\n",
                s.id, s.name, s.email, s.password,
                s.balance, s.phone_number, s.address);
    }

    fclose(fp);
    fclose(temp);
    remove(STUDENT_FILE);
    rename("data/temp.txt", STUDENT_FILE);

    if (found)
        printf("Profile updated!\n");
    else
        printf("Student ID %d not found!\n", studentID);
}

// ---------------- Delete Profile ----------------
void deleteProfile(int studentID) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("data/temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    Student s;
    int found = 0;
    while (fscanf(fp, "%d|%49[^|]|%99[^|]|%99[^|]|%lf|%19[^|]|%255[^\n]\n",
                  &s.id, s.name, s.email, s.password,
                  &s.balance, s.phone_number, s.address) == 7) {
        if (s.id == studentID) {
            found = 1;
            continue; // skip writing
        }
        fprintf(temp, "%d|%s|%s|%s|%.2f|%s|%s\n",
                s.id, s.name, s.email, s.password,
                s.balance, s.phone_number, s.address);
    }

    fclose(fp);
    fclose(temp);
    remove(STUDENT_FILE);
    rename("data/temp.txt", STUDENT_FILE);

    if (found) printf("Profile deleted!\n");
    else printf("Student not found!\n");
}

// ---------------- Place Order ----------------
void placeOrder(int studentID) {
    FILE *menu = fopen(MENU_FILE, "r");
    if (!menu) {
        printf("Error opening menus file!\n");
        return;
    }

    printf("\n--- Available Menu ---\n");
    int menuID, restID, stock;
    char item[50];
    double price;

    while (fscanf(menu, "%d|%d|%49[^|]|%lf|%d\n",
                  &menuID, &restID, item, &price, &stock) == 5) {
        printf("%d) %s - RM %.2f (Stock: %d)\n", menuID, item, price, stock);
    }
    fclose(menu);

    int choice, qty;

    printf("Enter Menu ID to order: ");
    scanf("%d", &choice);

    printf("Enter Quantity: ");
    scanf("%d", &qty);

    FILE *fp = fopen(ORDER_FILE, "a");
    if (!fp) {
        printf("Error opening orders file!\n");
        return;
    }

    int orderID = generateNewID(ORDER_FILE, "order");
    fprintf(fp, "%d|%d|%d|%d|%d|Pending|2025-08-21\n",
            orderID, studentID, restID, choice, qty);
    fclose(fp);

    printf("Order placed successfully! Order ID: %d\n", orderID);
}

// ---------------- View Order History ----------------
void viewOrderHistory(int studentID) {
    FILE *fp = fopen(ORDER_FILE, "r");
    if (!fp) {
        printf("Error opening orders file!\n");
        return;
    }

    printf("\n--- Order History ---\n");
    int orderID, sID, restID, menuID, qty;
    char status[20], date[20];
    while (fscanf(fp, "%d|%d|%d|%d|%d|%19[^|]|%19[^\n]\n",
                  &orderID, &sID, &restID, &menuID, &qty, status, date) == 7) {
        if (sID == studentID) {
            printf("Order %d | Menu %d | Qty %d | %s | %s\n",
                   orderID, menuID, qty, status, date);
        }
    }
    fclose(fp);
}

// ---------------- Manage Balance ----------------
void manageBalance(int studentID) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("data/temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    Student s;
    while (fscanf(fp, "%d|%49[^|]|%99[^|]|%99[^|]|%lf|%19[^|]|%255[^\n]\n",
                  &s.id, s.name, s.email, s.password,
                  &s.balance, s.phone_number, s.address) == 7) {
        if (s.id == studentID) {
            int amount;
            printf("Current Balance: RM %.2f\n", s.balance);

            while (1) {
                printf("Enter amount to add (integer only): ");
                if (scanf("%d", &amount) == 1) {
                    if (amount >= 0) break;
                    else printf("Amount must be positive.\n");
                } else {
                    printf("Invalid input. Please enter a number.\n");
                }
                while (getchar() != '\n'); // clear buffer
            }
            getchar(); // consume leftover newline
            s.balance += amount;
        }
        fprintf(temp, "%d|%s|%s|%s|%.2f|%s|%s\n",
                s.id, s.name, s.email, s.password,
                s.balance, s.phone_number, s.address);
    }

    fclose(fp);
    fclose(temp);
    remove(STUDENT_FILE);
    rename("data/temp.txt", STUDENT_FILE);

    printf("Balance updated!\n");
}
