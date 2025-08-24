#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "delivery.h"

#define DELIVERY_FILE "./data/delivery_persons.txt"
#define ORDERS_FILE   "./data/orders.txt"
#define DELIVERIES_FILE "./data/deliveries.txt"

// Helper generate new IDs
int generateNewID(const char *filename, int startRange) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return startRange; // start from given range if file not exists
    char line[256];
    int lastID = startRange - 1;
    while (fgets(line, sizeof(line), fp)) {
        int id;
        sscanf(line, "%d|", &id);
        if (id > lastID) lastID = id;
    }
    fclose(fp);
    return lastID + 1;
}

void registerDeliveryPerson() {
    DeliveryPerson dp;
    dp.deliveryPersonID = generateNewID(DELIVERY_FILE, 3000); // delivery IDs start from 3000
    printf("Name: "); fgets(dp.name, 50, stdin); dp.name[strcspn(dp.name,"\n")] = 0;
    printf("Email: "); fgets(dp.email, 50, stdin); dp.email[strcspn(dp.email,"\n")] = 0;
    printf("Password: "); fgets(dp.password, 20, stdin); dp.password[strcspn(dp.password,"\n")] = 0;
    printf("Phone: "); fgets(dp.phone, 15, stdin); dp.phone[strcspn(dp.phone,"\n")] = 0;
    printf("Vehicle: "); fgets(dp.vehicle, 20, stdin); dp.vehicle[strcspn(dp.vehicle,"\n")] = 0;

    FILE *fp = fopen(DELIVERY_FILE, "a");
    fprintf(fp, "%d|%s|%s|%s|%s|%s\n",
            dp.deliveryPersonID, dp.name, dp.email, dp.password, dp.phone, dp.vehicle);
    fclose(fp);

    printf("Delivery Person Registered with ID %d\n", dp.deliveryPersonID);
}

int loginDeliveryPerson() {
    char email[50], password[20], line[256];
    printf("Email: "); fgets(email, 50, stdin); email[strcspn(email,"\n")] = 0;
    printf("Password: "); fgets(password, 20, stdin); password[strcspn(password,"\n")] = 0;

    FILE *fp = fopen(DELIVERY_FILE, "r");
    if (!fp) { printf("No delivery persons registered yet.\n"); return -1; }

    while (fgets(line, sizeof(line), fp)) {
        DeliveryPerson dp;
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
               &dp.deliveryPersonID, dp.name, dp.email, dp.password, dp.phone, dp.vehicle);
        if (strcmp(dp.email, email) == 0 && strcmp(dp.password, password) == 0) {
            fclose(fp);
            printf("Welcome %s! Your ID = %d\n", dp.name, dp.deliveryPersonID);
            return dp.deliveryPersonID;
        }
    }
    fclose(fp);
    printf("Login failed!\n");
    return -1;
}

void assignOrder() {
    FILE *fp = fopen(ORDERS_FILE, "r");
    if (!fp) { printf("No orders yet.\n"); return; }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        Order o;
        sscanf(line, "%d|%d|%d|%d|%d|%[^|]|%[^\n]", 
               &o.orderID, &o.studentID, &o.restaurantID, &o.menuID,
               &o.quantity, o.status, o.date);
        if (strcmp(o.status, "Pending") == 0) {
            int dpID;
            printf("Enter your DeliveryPerson ID: ");
            scanf("%d", &dpID); getchar();

            int newDeliveryID = generateNewID(DELIVERIES_FILE, 5000);
            FILE *df = fopen(DELIVERIES_FILE, "a");
            fprintf(df, "%d|%d|%d|Assigned|0.0\n", newDeliveryID, o.orderID, dpID);
            fclose(df);

            printf("Order %d assigned to Delivery %d\n", o.orderID, newDeliveryID);
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    printf("No pending orders.\n");
}

void updateDeliveryStatus(int deliveryID) {
    FILE *in = fopen(DELIVERIES_FILE, "r");
    FILE *out = fopen("./data/tmp.txt", "w");
    if (!in || !out) { printf("Error opening deliveries file.\n"); return; }

    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), in)) {
        Delivery d;
        sscanf(line, "%d|%d|%d|%[^|]|%lf", 
               &d.deliveryID, &d.orderID, &d.deliveryPersonID, d.status, &d.earnings);

        if (d.deliveryID == deliveryID) {
            char newStatus[20];
            printf("Enter new status: ");
            fgets(newStatus, 20, stdin); newStatus[strcspn(newStatus,"\n")] = 0;
            strcpy(d.status, newStatus);
            if (strcmp(newStatus, "Delivered") == 0) d.earnings += 5.0; // ثابت للتجربة
            fprintf(out, "%d|%d|%d|%s|%.2f\n",
                    d.deliveryID, d.orderID, d.deliveryPersonID, d.status, d.earnings);
            found = 1;
        } else {
            fputs(line, out);
        }
    }
    fclose(in); fclose(out);
    remove(DELIVERIES_FILE);
    rename("./data/tmp.txt", DELIVERIES_FILE);

    if (found) printf("Delivery %d updated.\n", deliveryID);
    else printf("Delivery not found.\n");
}

void viewPerformance(int deliveryID) {
    FILE *fp = fopen(DELIVERIES_FILE, "r");
    if (!fp) { printf("No deliveries yet.\n"); return; }
    char line[256];
    int total = 0; double earnings = 0;
    while (fgets(line, sizeof(line), fp)) {
        Delivery d;
        sscanf(line, "%d|%d|%d|%[^|]|%lf", 
               &d.deliveryID, &d.orderID, &d.deliveryPersonID, d.status, &d.earnings);
        if (d.deliveryPersonID == deliveryID) {
            total++;
            earnings += d.earnings;
        }
    }
    fclose(fp);
    printf("Performance for DeliveryPerson %d:\n", deliveryID);
    printf("Total Deliveries: %d, Total Earnings: RM %.2f\n", total, earnings);
}

void calculateEarnings(int deliveryID) {
    viewPerformance(deliveryID);
}

void deliveryMainMenu(int deliveryID) {
    int choice;
    while (1) {
        printf("\n--- Delivery Menu ---\n");
        printf("1. Assign Order\n");
        printf("2. Update Delivery Status\n");
        printf("3. View Performance\n");
        printf("4. Calculate Earnings\n");
        printf("0. Logout\n");
        printf("Choose: ");
        scanf("%d", &choice); getchar();

        switch (choice) {
            case 1: assignOrder(); break;
            case 2: updateDeliveryStatus(deliveryID); break;
            case 3: viewPerformance(deliveryID); break;
            case 4: calculateEarnings(deliveryID); break;
            case 0: return;
            default: printf("Invalid!\n");
        }
    }
}
