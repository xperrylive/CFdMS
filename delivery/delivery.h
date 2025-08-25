#ifndef DELIVERY_H
#define DELIVERY_H

typedef struct {
    int deliveryPersonID;
    char name[50];
    char email[50];
    char password[20];
    char phone[15];
    char vehicle[20];
    char status[20];
} DeliveryPerson;

typedef struct {
    int orderID;
    int studentID;
    int restaurantID;
    int menuID;
    int quantity;
    char status[20];
    char date[20];
} Order;

typedef struct {
    int deliveryID;
    int orderID;
    int deliveryPersonID;
    char status[20];
    double earnings;
} Delivery;

// Required functions
void registerDeliveryPerson();
void assignOrder();
void updateDeliveryStatus(int deliveryID);
void viewPerformance(int deliveryID);
void calculateEarnings(int deliveryID);
//main menu for delivery
void deliveryMainMenu(int deliveryID);

#endif
