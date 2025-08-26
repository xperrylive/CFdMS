#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "student.h"
#include "../utils.h"

#define STUDENT_FILE "data/students.txt"
#define MENU_FILE "data/menus.txt"
#define ORDER_FILE "data/orders.txt"

static int nextOrderID(void);

static int nextOrderID(void) {
    FILE *fp = fopen(ORDER_FILE, "r");
    if (!fp) return 5000;  // start from 5000 if file doesn't exist

    int maxID = 5000;
    int oid, sID, rID, mID, qty;
    char status[32], date[32];

    while (fscanf(fp, "%d|%d|%d|%d|%d|%31[^|]|%31[^\n]\n",
                  &oid, &sID, &rID, &mID, &qty, status, date) == 7) {
        if (oid > maxID) maxID = oid;
    }
    fclose(fp);
    return maxID + 1;
}

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
    
    // Phone (digits only)
    while (1) {
        printf("Enter your phone number: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        int valid = 1;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] < '0' || buffer[i] > '9') {
                valid = 0;
                break;
            }
        }

        if (valid && strlen(buffer) > 0) {
            strcpy(s.phone_number, buffer);
            break;
        } else {
            printf("Invalid phone number. Digits only.\n");
        }
    }

    
    // Balance (numeric input with validation)
    while (1) {
        printf("Enter your initial balance: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        char *endptr;
        s.balance = strtod(buffer, &endptr);

        // Check if entire string was a valid number
        if (endptr != buffer && *endptr == '\0' && s.balance >= 0) {
            break; // valid input
        } else {
            printf("Invalid input. Please enter a valid number.\n");
        }
    }

    
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
    //Show menu
    FILE *menuFile = fopen(MENU_FILE, "r");
    if (!menuFile) {
        printf("Error opening menus file!\n");
        return;
    }

    printf("\n--- Available Menu ---\n");
    int menuID, restID, stock;
    char item[50];
    double price;
    while (fscanf(menuFile, "%d|%d|%49[^|]|%lf|%d\n",
                  &menuID, &restID, item, &price, &stock) == 5) {
        printf("%d) %s - RM %.2f (Stock: %d)\n", menuID, item, price, stock);
    }
    fclose(menuFile);

    //Read and validate Menu ID (must exist)
    int choice = -1, qty = 0;
    int chosenMenuID = -1, chosenRestID = -1, chosenStock = 0;
    double chosenPrice = 0.0;
    char chosenItem[50];
    char buf[64];

    for (;;) {
        printf("Enter Menu ID to order: ");
        if (!fgets(buf, sizeof(buf), stdin)) return;
        choice = atoi(buf);

        menuFile = fopen(MENU_FILE, "r");
        if (!menuFile) {
            printf("Error opening menus file!\n");
            return;
        }

        int found = 0;
        while (fscanf(menuFile, "%d|%d|%49[^|]|%lf|%d\n",
                      &menuID, &restID, item, &price, &stock) == 5) {
            if (menuID == choice) {
                found = 1;
                chosenMenuID = menuID;
                chosenRestID = restID;
                chosenStock  = stock;
                chosenPrice  = price;
                strcpy(chosenItem, item);
                break;
            }
        }
        fclose(menuFile);

        if (found) break;
        printf("Invalid Menu ID! Please choose a valid ID from the list.\n");
    }

    //Quantity (must be positive and <= stock)
    for (;;) {
        printf("Enter Quantity: ");
        if (!fgets(buf, sizeof(buf), stdin)) return;
        qty = atoi(buf);
        if (qty <= 0) {
            printf("Quantity must be positive.\n");
            continue;
        }
        if (qty > chosenStock) {
            printf("Sorry, only %d available. Try again.\n", chosenStock);
            continue;
        }
        break;
    }

    double totalCost = chosenPrice * qty;

    //Load student, check balance, then write back updated balance
    FILE *sf = fopen(STUDENT_FILE, "r");
    FILE *st = fopen("data/temp.txt", "w");
    if (!sf || !st) {
        if (sf) fclose(sf);
        if (st) fclose(st);
        printf("Error opening student file!\n");
        return;
    }

    Student s;
    int studentFound = 0;
    while (fscanf(sf, "%d|%49[^|]|%99[^|]|%99[^|]|%lf|%19[^|]|%255[^\n]\n",
                  &s.id, s.name, s.email, s.password,
                  &s.balance, s.phone_number, s.address) == 7) {
        if (s.id == studentID) {
            studentFound = 1;
            if (s.balance < totalCost) {
                printf("Insufficient balance! Need RM %.2f, you have RM %.2f.\n",
                       totalCost, s.balance);
                fclose(sf); fclose(st);
                remove("data/temp.txt");
                return;
            }
            s.balance -= totalCost;
        }
        fprintf(st, "%d|%s|%s|%s|%.2f|%s|%s\n",
                s.id, s.name, s.email, s.password,
                s.balance, s.phone_number, s.address);
    }
    fclose(sf);
    fclose(st);
    remove(STUDENT_FILE);
    rename("data/temp.txt", STUDENT_FILE);

    if (!studentFound) {
        printf("Student not found!\n");
        return;
    }

    //Update stock in menus.txt
    menuFile = fopen(MENU_FILE, "r");
    FILE *menuTmp = fopen("data/menu_temp.txt", "w");
    if (!menuFile || !menuTmp) {
        if (menuFile) fclose(menuFile);
        if (menuTmp) fclose(menuTmp);
        printf("Error updating menu file!\n");
        return;
    }
    while (fscanf(menuFile, "%d|%d|%49[^|]|%lf|%d\n",
                  &menuID, &restID, item, &price, &stock) == 5) {
        if (menuID == chosenMenuID) stock -= qty;
        fprintf(menuTmp, "%d|%d|%s|%.2f|%d\n", menuID, restID, item, price, stock);
    }
    fclose(menuFile);
    fclose(menuTmp);
    remove(MENU_FILE);
    rename("data/menu_temp.txt", MENU_FILE);

    //Append order with a safe, non-negative ID and today’s date
    int orderID = nextOrderID();

    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d", lt);

    FILE *of = fopen(ORDER_FILE, "a");
    if (!of) {
        printf("Error opening orders file!\n");
        return;
    }
    fprintf(of, "%d|%d|%d|%d|%d|Pending|%s\n",
            orderID, studentID, chosenRestID, chosenMenuID, qty, date);
    fclose(of);

    printf("Order placed successfully! Order ID: %d, Total: RM %.2f\n",
           orderID, totalCost);
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
