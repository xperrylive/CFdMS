#ifndef RESTAURANT_H
#define RESTAURANT_H

struct restaurant {
    int restaurantId;
    char name[100];
    char email[100] ;
    char password[50] ;
    char location[100];
    char phone[20] ;
};

struct menuItem {
    int menuId;
    int restaurantId ;
    char itemName[100] ;
    float price;
    int stock;
};

typedef struct OrderNode {
    int orderID, studentID, restaurantId, menuId, quantity;
    char status[20], date[11];
    struct OrderNode *next;
} OrderNode;

// Helper functions
int generateNewID(const char *filename, const char *role);
int isRestaurantIDExists(int id);
float getPriceForMenuItem(int menuId);
void updateStock(int menuId, int quantitySold) ;

// Main feature functions
void registerRestaurant();
void restaurantmain(int restaurantId) ;
void menumain(int restaurantId);
void addMenuItem(int restaurantId);
void viewMenu(int restaurantId);
void updateMenuItem(int restaurantId);
void deleteMenuItem(int restaurantId) ;
void trackInventory(int restaurantId);
void dailySalesReport(int restaurantId) ;
void orderQueue(int restaurantId);


#endif