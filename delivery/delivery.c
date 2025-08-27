#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "delivery.h"
#include "../utils.h"

#define DELIVERY_FILE "data/delivery.txt"
#define ORDERS_FILE   "data/orders.txt"
#define DELIVERIES_FILE "data/deliveries.txt"
#define APPLICATIONS_FILE "data/applications.txt"

void registerDeliveryPerson() {
    DeliveryPerson dp;
    
    printf("Name: "); 
    fgets(dp.name, 50, stdin); 
    dp.name[strcspn(dp.name,"\n")] = 0;
    
    // Email validation
    do {
        printf("Email: "); 
        fgets(dp.email, 50, stdin); 
        dp.email[strcspn(dp.email,"\n")] = 0;
    } while (!emailCheck(dp.email));
    
    printf("Password: "); 
    fgets(dp.password, 20, stdin); 
    dp.password[strcspn(dp.password,"\n")] = 0;
    
    printf("Phone: "); 
    fgets(dp.phone, 15, stdin); 
    dp.phone[strcspn(dp.phone,"\n")] = 0;
    
    // Set default status
    strcpy(dp.status, "pending");

    FILE *fp = fopen(APPLICATIONS_FILE, "a");
    if (!fp) {
        printf("Error: Could not open applications file.\n");
        return;
    }
    
    // Format
    fprintf(fp, "%s|%s|%s|%s|%s|delivery\n",
        dp.name, dp.email, dp.password, dp.status, dp.phone);
    fclose(fp);

    printf("Delivery Person application submitted successfully.\n");
    printf("Your application is pending admin approval.\n");
    return;
}

void assignOrder() {
    FILE *fp = fopen(ORDERS_FILE, "r");
    if (!fp) { 
        printf("No orders yet.\n"); 
        return; 
    }

    char line[256];
    printf("\n=== Available Pending Orders ===\n");
    int foundPending = 0;
    
    // Display all pending orders
    while (fgets(line, sizeof(line), fp)) {
        Order o;
        sscanf(line, "%d|%d|%d|%d|%d|%[^|]|%[^\n]", 
               &o.orderID, &o.studentID, &o.restaurantID, &o.menuID,
               &o.quantity, o.status, o.date);
        if (strcmp(o.status, "Pending") == 0) {
            printf("Order ID: %d | Student ID: %d | Restaurant ID: %d | Quantity: %d | Date: %s\n",
                   o.orderID, o.studentID, o.restaurantID, o.quantity, o.date);
            foundPending = 1;
        }
    }
    fclose(fp);
    
    if (!foundPending) {
        printf("No pending orders available.\n");
        return;
    }
    
    int dpID, orderID;
    printf("\nEnter your DeliveryPerson ID: ");
    scanf("%d", &dpID); 
    getchar();
    
    printf("Enter Order ID to assign: ");
    scanf("%d", &orderID); 
    getchar();
    
    // Check if the order exists and is pending
    fp = fopen(ORDERS_FILE, "r");
    if (!fp) return;
    
    int orderFound = 0;
    while (fgets(line, sizeof(line), fp)) {
        Order o;
        sscanf(line, "%d|%d|%d|%d|%d|%[^|]|%[^\n]", 
               &o.orderID, &o.studentID, &o.restaurantID, &o.menuID,
               &o.quantity, o.status, o.date);
        if (o.orderID == orderID && strcmp(o.status, "Pending") == 0) {
            orderFound = 1;
            break;
        }
    }
    fclose(fp);
    
    if (!orderFound) {
        printf("Order not found or not pending.\n");
        return;
    }
    
    // Create new delivery assignment
    int newDeliveryID = generateNewID(DELIVERIES_FILE, "delivery");
    FILE *df = fopen(DELIVERIES_FILE, "a");
    if (!df) {
        printf("Error opening deliveries file.\n");
        return;
    }
    
    fprintf(df, "%d|%d|%d|Assigned|0.0\n", newDeliveryID, orderID, dpID);
    fclose(df);

    printf("Order %d assigned to Delivery %d\n", orderID, newDeliveryID);
}

void updateDeliveryStatus(int deliveryID) {
    FILE *in = fopen(DELIVERIES_FILE, "r");
    FILE *out = fopen("./data/tmp.txt", "w");
    if (!in || !out) { 
        printf("Error opening deliveries file.\n"); 
        if (in) fclose(in);
        if (out) fclose(out);
        return; 
    }

    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), in)) {
        Delivery d;
        sscanf(line, "%d|%d|%d|%[^|]|%lf", 
               &d.deliveryID, &d.orderID, &d.deliveryPersonID, d.status, &d.earnings);

        if (d.deliveryID == deliveryID) {
            printf("Current status: %s\n", d.status);
            printf("Available status options: Assigned, In Transit, Delivered, Cancelled\n");
            char newStatus[20];
            printf("Enter new status: ");
            fgets(newStatus, 20, stdin); 
            newStatus[strcspn(newStatus,"\n")] = 0;
            
            strcpy(d.status, newStatus);
            if (strcmp(newStatus, "Delivered") == 0 || strcmp(newStatus, "delivered") == 0) {
                d.earnings += 5.0; // Fixed earnings for delivery
            }
            fprintf(out, "%d|%d|%d|%s|%.2f\n",
                    d.deliveryID, d.orderID, d.deliveryPersonID, d.status, d.earnings);
            found = 1;
        } else {
            fputs(line, out);
        }
    }
    fclose(in); 
    fclose(out);
    remove(DELIVERIES_FILE);
    rename("./data/tmp.txt", DELIVERIES_FILE);

    if (found) {
        printf("Delivery %d status updated successfully.\n", deliveryID);
    } else {
        printf("Delivery ID %d not found.\n", deliveryID);
    }
}

void viewPerformance(int deliveryID) {
    FILE *fp = fopen(DELIVERIES_FILE, "r");
    if (!fp) { 
        printf("No deliveries yet.\n"); 
        return; 
    }
    
    char line[256];
    int totalDeliveries = 0, completedDeliveries = 0;
    double totalEarnings = 0;
    
    printf("\n=== Performance Report for Delivery Person %d ===\n", deliveryID);
    printf("%-12s %-10s %-15s %-10s\n", "Delivery ID", "Order ID", "Status", "Earnings");
    printf("-----------------------------------------------\n");
    
    while (fgets(line, sizeof(line), fp)) {
        Delivery d;
        sscanf(line, "%d|%d|%d|%[^|]|%lf", 
               &d.deliveryID, &d.orderID, &d.deliveryPersonID, d.status, &d.earnings);
        if (d.deliveryPersonID == deliveryID) {
            printf("%-12d %-10d %-15s RM%.2f\n", 
                   d.deliveryID, d.orderID, d.status, d.earnings);
            totalDeliveries++;
            totalEarnings += d.earnings;
            if (strcmp(d.status, "Delivered") == 0) {
                completedDeliveries++;
            }
        }
    }
    fclose(fp);
    
    printf("-----------------------------------------------\n");
    printf("Total Deliveries: %d\n", totalDeliveries);
    printf("Completed Deliveries: %d\n", completedDeliveries);
    printf("Total Earnings: RM %.2f\n", totalEarnings);
    
    if (totalDeliveries > 0) {
        printf("Success Rate: %.1f%%\n", (completedDeliveries * 100.0) / totalDeliveries);
    }
}

void calculateEarnings(int deliveryID) {
    FILE *fp = fopen(DELIVERIES_FILE, "r");
    if (!fp) { 
        printf("No deliveries yet.\n"); 
        return; 
    }
    
    char line[256];
    double totalEarnings = 0;
    int deliveryCount = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        Delivery d;
        sscanf(line, "%d|%d|%d|%[^|]|%lf", 
               &d.deliveryID, &d.orderID, &d.deliveryPersonID, d.status, &d.earnings);
        if (d.deliveryPersonID == deliveryID) {
            totalEarnings += d.earnings;
            deliveryCount++;
        }
    }
    fclose(fp);
    
    printf("\n=== Earnings Summary for Delivery Person %d ===\n", deliveryID);
    printf("Total Deliveries: %d\n", deliveryCount);
    printf("Total Earnings: RM %.2f\n", totalEarnings);
    
    if (deliveryCount > 0) {
        printf("Average Earnings per Delivery: RM %.2f\n", totalEarnings / deliveryCount);
    }
}

void deliveryMainMenu(int deliveryID) {
    int choice;
    while (1) {
        printf("\n=== Delivery Person Menu (ID: %d) ===\n", deliveryID);
        printf("1. View Available Orders\n");
        printf("2. Assign Order to Myself\n");
        printf("3. Update Delivery Status\n");
        printf("4. View My Performance\n");
        printf("5. Calculate My Earnings\n");
        printf("0. Logout\n");
        printf("-----------------------------\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice); 
        getchar();

        switch (choice) {
            case 1:
                // Just display orders without assigning
                {
                    FILE *fp = fopen(ORDERS_FILE, "r");
                    if (!fp) { 
                        printf("No orders yet.\n"); 
                        break;
                    }
                    char line[256];
                    printf("\n=== Available Pending Orders ===\n");
                    int found = 0;
                    while (fgets(line, sizeof(line), fp)) {
                        Order o;
                        sscanf(line, "%d|%d|%d|%d|%d|%[^|]|%[^\n]", 
                               &o.orderID, &o.studentID, &o.restaurantID, &o.menuID,
                               &o.quantity, o.status, o.date);
                        if (strcmp(o.status, "Pending") == 0) {
                            printf("Order ID: %d | Student: %d | Restaurant: %d | Qty: %d | Date: %s\n",
                                   o.orderID, o.studentID, o.restaurantID, o.quantity, o.date);
                            found = 1;
                        }
                    }
                    if (!found) printf("No pending orders available.\n");
                    fclose(fp);
                }
                break;
            case 2: 
                assignOrder(); 
                break;
            case 3: 
                {
                    int delID;
                    printf("Enter Delivery ID to update: ");
                    scanf("%d", &delID); 
                    getchar();
                    updateDeliveryStatus(delID); 
                }
                break;
            case 4: 
                viewPerformance(deliveryID); 
                break;
            case 5: 
                calculateEarnings(deliveryID); 
                break;
            case 0: 
                printf("Logging out...\n");
                return;
            default: 
                printf("Invalid choice! Please try again.\n");
        }
    }
}
