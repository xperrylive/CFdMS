#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "restaurant.h" 
#include "../utils.h"







// this function is the main page of the restaurant management 
void restaurantmain(int restaurantId) {
    int option ;
    while (1) {
        printf("\n--- Restaurant Management ---\n");
        printf("1. Menu Item Management\n");
        printf("2. Order Queue Management\n") ;
        printf("3. Daily Sales Reporting\n");
        printf("4. Inventory tracking\n");
        printf("5. Exit\n");
        printf("Please select an option: ");
        scanf("%d", &option) ;
        getchar(); // it helps scanf(with numbers/intgers) + fgets (strings) by acting like a clean up setup as it consumes the newline

        if (option == 1) {
            menumain(restaurantId);
        } else if (option == 2) {
            orderQueue(restaurantId);
        } else if (option == 3) {
            dailySalesReport(restaurantId);
        } else if (option == 4) {
            trackInventory(restaurantId);
        } else if (option == 5) {
            printf("Exiting program.\n");
            break;
        } else {
            printf("Invalid option... Please try again.\n");
        }
    }
    return;
}








// this function is the main page of the menu item management 
void menumain(int restaurantId) {
    int option;
    while (1) {
        printf("\n--- Menu Item Management ---\n");
        printf("1. View Menu\n");
        printf("2. Add Item\n");
        printf("3. Update Item\n");
        printf("4. Delete Item\n");
        printf("5. Back to Main Menu\n");
        printf("Please select an option: ");
        scanf("%d", &option);
        getchar(); 

        if (option == 1) {
            viewMenu(restaurantId);
        } else if (option == 2) {
            addMenuItem(restaurantId);
        } else if (option == 3) {
            updateMenuItem(restaurantId);
        } else if (option == 4) {
            deleteMenuItem(restaurantId);
        } else if (option == 5) {
            break;
        } else {
            printf("Invalid Option.... Please try again.\n");
        }
    }
    return;
}












// this function is responsible for registring restaurants(uses structures and pointers)
void registerRestaurant() {
    struct restaurant newRestu ;             // newRestu is a temporary holder of information(email,name,etc..) for the new restaurant. it also uses the structure restaurant as it's data type
    FILE *fp;       // this is a file pointer that holds the address of the file info when it's opened 
    
    printf("\n---  Restaurant Application ---\n");

    printf("Enter Restaurant Name: ") ;
    fgets(newRestu.name, 100, stdin);
    newRestu.name[strcspn(newRestu.name, "\n")] = '\0';         // anywhere strcspn and ='\0' is used: to remove \n the fgets always includes and then proeperly ends the string

    do{ printf("Enter Email: ");
        fgets(newRestu.email, 100, stdin);
        newRestu.email[strcspn(newRestu.email, "\n")] = '\0';
    } while (!emailCheck(newRestu.email));
    

    printf("Enter Password: ");
    fgets(newRestu.password, 50, stdin);
    newRestu.password[strcspn(newRestu.password, "\n")] = '\0' ;

    printf("Enter Location: ");
    fgets(newRestu.location, 100, stdin);
    newRestu.location[strcspn(newRestu.location, "\n")] = '\0';

    printf("Enter Phone Number: ");
    fgets(newRestu.phone, 20, stdin);
    newRestu.phone[strcspn(newRestu.phone, "\n")] = '\0' ;

    fp = fopen("data/applications.txt", "a");
    if (fp == NULL) {
        printf("Error: Could not open applications.txt for writing.\n");
        return;
    }
    fprintf(fp, "%s|%s|%s|%s|%s|restaurant\n", newRestu.name, newRestu.email, newRestu.password, newRestu.location, newRestu.phone); // adding all the info to the restaurants file
    fclose(fp);
    printf("\n'%s' was applied successfully!\n", newRestu.name) ;

}











// function to add menu items 
void addMenuItem(int restaurantId) {
    struct menuItem newItem ;        //  newItem is being used as a temporary container to hold item details before saving them
    FILE *fp = fopen("data/menus.txt", "a") ;        //  process of opening the file (using fp because it's where the file is stored)
    if (fp == NULL) {
        printf("Error: Could not open menus.txt.\n") ;  //  appears if the file is not found 
        return;
    }
    printf("\n--- Add New Menu Item ---\n");
    newItem.restaurantId = restaurantId;
    
    // Auto-generate Menu Item ID for a "menuItem".
    newItem.menuId = generateNewID("data/menus.txt", "menuItem");
    printf("New Menu Item ID will be: %d\n", newItem.menuId) ;

    printf("Enter Item Name: ");
    fgets(newItem.itemName, 100, stdin);
    newItem.itemName[strcspn(newItem.itemName, "\n")] = '\0';

    printf("Enter Price (RM): ");
    // only a float can be added
    while (scanf("%f", &newItem.price) != 1) {
        printf("Invalid input. Please enter a number for the price (e.g., 7.50): ");
        while (getchar() != '\n');
    }
    getchar();

    printf("Enter Initial Stock Quantity: ") ;
    while (scanf("%d", &newItem.stock) != 1) {
        printf("Invalid input. Please enter a whole number for the stock: ");
        while (getchar() != '\n');
    }
    getchar() ;

    fprintf(fp, "%d|%d|%s|%.2f|%d\n", newItem.menuId, newItem.restaurantId, newItem.itemName, newItem.price, newItem.stock);    // adding all the info to the menu file
    fclose(fp);         // closes the file and saves changes

    fclose(fp);
    printf("Menu item '%s' added successfully!\n", newItem.itemName);
}










// function to view the menu saved i n the menu text file 
void viewMenu(int restaurantId) {
    struct menuItem item ;       // item is is being used as a temporary container to hold item details before saving them
    FILE *fp = fopen("data/menus.txt", "r") ;        //  process of opening the file (using fp because it's where the file is stored)
    if (fp == NULL) {
        printf("Error: Could not open menu file.\n") ;
        return ;
    }

    printf("\n--- Menu for Restaurant ID: %d ---\n", restaurantId) ;
    printf("%-5s %-25s %-10s %-10s\n", "ID", "Item Name", "Price(RM)", "Stock");        // used this specific format so the printed info looks clean and mashed together 
    printf("----------------------------------------------------\n") ;

    // below here a while loop is being used to read the menu file line by line until the end of the file so they can be printed and viewed
    int found = 0;
    while (fscanf(fp, "%d|%d|%[^|]|%f|%d\n", &item.menuId, &item.restaurantId, &item.itemName, &item.price, &item.stock) == 5) {
        if (item.restaurantId == restaurantId) {
            printf("%-5d %-25s %-10.2f %-10d\n", item.menuId, item.itemName, item.price, item.stock);
            found = 1 ;
        }
    }

    if (!found) {   // if it's not found(found == 0)
        printf("No menu items found for this restaurant.\n");
    }
    fclose(fp) ;        // closes the file and saves changes
    return; 
}










// function to update any menu item  
void updateMenuItem(int restaurantId) {
    int itemID, found = 0;
    struct menuItem item;       // item is is being used as a temporary container to hold item details before saving them
    FILE *fp, *tempFp ;

    viewMenu(restaurantId);     // calling the viewMenu function to display the menu so the user can pick
    printf("\nEnter the ID of the menu item you want to update: ");
    scanf("%d", &itemID);
    getchar() ;

    fp = fopen("data/menus.txt", "r");
    tempFp = fopen("data/temp_menus.txt", "w");     // using a temporary file to place the new info in 
    if (fp == NULL || tempFp == NULL) {
        printf("Error: Could not open files for updating.\n");
        if (fp) fclose(fp);
        if (tempFp) fclose(tempFp) ;
        return;
    }

    // the while loop has 2 functions: 1. to check the menu item ID matches the one the user asked for 2. is the item in the menu or not
    while (fscanf(fp, "%d|%d|%[^|]|%f|%d\n", &item.menuId, &item.restaurantId, &item.itemName, &item.price, &item.stock) != EOF) {
        if (item.menuId == itemID && item.restaurantId == restaurantId) {
            found = 1 ;
            printf("Updating item: %s\n", item.itemName);
            printf("Enter new item name: ");
            fgets(item.itemName, 100, stdin) ;
            item.itemName[strcspn(item.itemName, "\n")] = '\0';
            while (scanf("%f", &item.price) != 1) {
                printf("Invalid input. Please enter a number for the price (e.g., 7.50): ");
                while (getchar() != '\n');
            }
            getchar() ;
            printf("Enter new stock: ");
            while (scanf("%d", &item.stock) != 1) {
                printf("Invalid input. Please enter a whole number for the stock: ");
                while (getchar() != '\n');
            }
            getchar();
            fprintf(tempFp, "%d|%d|%s|%.2f|%d\n", item.menuId, item.restaurantId, item.itemName, item.price, item.stock);
        } else {
            fprintf(tempFp, "%d|%d|%s|%.2f|%d\n", item.menuId, item.restaurantId, item.itemName, item.price, item.stock);
        }
    }

    fclose(fp);
    fclose(tempFp) ;
    remove("data/menus.txt") ;      // the keyword (remove) deletes the old, original menus.txt file.
    rename("data/temp_menus.txt", "data/menus.txt") ;       // then the keyword (rename) renames the temporary file (which now contains all the correct data) to menus.txt

    if (found) {
        printf("\nMenu item with ID %d updated successfully!\n", itemID);
    } else {
        printf("\nError: Menu item with ID %d not found.\n", itemID);
    }
}












// function for deleting menu items 
void deleteMenuItem(int restaurantId) {
    int itemID, found = 0;
    struct menuItem item ;       // item is is being used as a temporary container to hold item details before saving them 
    FILE *fp, *tempFp ;

    viewMenu(restaurantId) ;     // calling the viewMenu function to display the menu so the user can pick
    
    printf("\nEnter the ID of the menu item you want to delete: ");
    scanf("%d", &itemID) ;
    getchar() ;
    fp = fopen("data/menus.txt", "r");
    tempFp = fopen("data/temp_menus.txt", "w") ;
    if (fp == NULL || tempFp == NULL) { /* ... */ return; }

     // the while loop checks if the menu item ID matches the one the user asked for
    while (fscanf(fp, "%d|%d|%[^|]|%f|%d\n", &item.menuId, &item.restaurantId, &item.itemName, &item.price, &item.stock) != EOF) {
        if (item.menuId == itemID && item.restaurantId == restaurantId) {
            found = 1 ;
        } else {
            fprintf(tempFp, "%d|%d|%s|%.2f|%d\n", item.menuId, item.restaurantId, item.itemName, item.price, item.stock);
        }
    }
    
    fclose(fp);
    fclose(tempFp) ;
    
    remove("data/menus.txt");       // the keyword (remove) deletes the old, original menus.txt file.
    rename("data/temp_menus.txt", "data/menus.txt");        // then the keyword (rename) renames the temporary file (which now contains all the correct data) to menus.txt
    
    if (found) {
        printf("\nMenu item with ID %d deleted successfully!\n", itemID) ;
    } else {
        printf("\nError: Menu item with ID %d not found.\n", itemID) ;
    }
}












// function to track the inventory and low stock warnings 
void trackInventory(int restaurantId) {
    const int LOW_STOCK_THRESHOLD = 10 ;     // setting the 10 to be the number to consider an item to be low in stock
    struct menuItem item;
    FILE *fp = fopen("data/menus.txt", "r") ;
    if (fp == NULL) { /* ... */ return; }
    printf("\n--- Inventory Status (Low Stock Alert below %d items) ---\n", LOW_STOCK_THRESHOLD) ;
    int found_low_stock = 0 ;
    // while loop to read the file line by line until the end  
    while (fscanf(fp, "%d|%d|%[^|]|%f|%d\n", &item.menuId, &item.restaurantId, &item.itemName, &item.price, &item.stock) == 5) {
        if (item.restaurantId == restaurantId) {        // checks if the item is from the correct restaurant
            if (item.stock < LOW_STOCK_THRESHOLD) {     // then checks if it's lower than 10 
                printf("! ALERT: '%s' is low on stock (%d remaining).\n", item.itemName, item.stock) ;      // if yes it prints this warning 
                found_low_stock = 1;
            }
        }
    }
    if (!found_low_stock) {
        printf("All items are sufficiently stocked.\n");
    }
    fclose(fp);
}








// this is a helper function to the price of a menu item
float getPriceForMenuItem(int menuId) {
    FILE *fp = fopen("data/menus.txt", "r");
    if (fp == NULL) return 0.0;
    struct menuItem item;
    while (fscanf(fp, "%d|%d|%[^|]|%f|%d\n", &item.menuId, &item.restaurantId, &item.itemName, &item.price, &item.stock) != EOF) {
        if (item.menuId == menuId) {
            fclose(fp);
            return item.price;
        }
    }
    fclose(fp);
    return 0;
}









// function to provide the sales report form each day (using an external library = time + malloc and pointers)
void dailySalesReport(int restaurantId) {
    char report_date[12] ;      // variable for date 
    FILE *fp ;
    int menu_count = 0;
    char ch ;

    printf("\nEnter the date for the sales report (YYYY-MM-DD): ");
    fgets(report_date, 12, stdin);
    report_date[strcspn(report_date, "\n")] = '\0' ;
    fp = fopen("data/menus.txt", "r") ;
    if (fp == NULL) {
        printf("Error: Could not open menu file.\n");
        return;
    }
    while ((ch = fgetc(fp)) != EOF) {       //  counts the no. of menu items
        if (ch == '\n') {
            menu_count++;
        }
    }
    fclose(fp) ;

    if (menu_count == 0) {
        printf("No menu items found.\n");
        return;
    }
    typedef struct {        // local struct that holds the menu item's ID and its price
        int menuId;
        float price;
    } MenuItemPrice ;
    
    MenuItemPrice *menuPrices = (MenuItemPrice*) malloc(menu_count * sizeof(MenuItemPrice));        // uses malloc(dynamic memory) to store the ID and cost of each
    if (menuPrices == NULL) {
        printf("Error: Memory allocation failed.\n");
        return ;
    }
    fp = fopen("data/menus.txt", "r");
    struct menuItem tempItem;
    for (int i = 0; i < menu_count; i++) {
        fscanf(fp, "%d|%d|%[^|]|%f|%d\n", &tempItem.menuId, &tempItem.restaurantId, &tempItem.itemName, &tempItem.price, &tempItem.stock);
        menuPrices[i].menuId = tempItem.menuId;
        menuPrices[i].price = tempItem.price;
    }
    fclose(fp) ;

    fp = fopen("data/orders.txt", "r");
    if (fp == NULL) {
        printf("Error: Could not open orders file.\n");
        free(menuPrices) ;
        return;
    }

    struct Order {
        int orderID, studentID, restaurantId, menuId, quantity;
        char status[20], date[11];
    } local_order;
    
    float total_sales = 0.0 ;
    int orders_today = 0;

    printf("\n--- Daily Sales Report for %s ---\n", report_date);
    while (fscanf(fp, "%d|%d|%d|%d|%d|%[^|]|%[^\n]\n", &local_order.orderID, &local_order.studentID, &local_order.restaurantId, &local_order.menuId, &local_order.quantity, local_order.status, local_order.date) != EOF) {
        if (local_order.restaurantId == restaurantId && strcmp(local_order.status, "Delivered") == 0 && strcmp(local_order.date, report_date) == 0) {
            float price = 0.0;
            for (int i = 0; i < menu_count; i++) {
                if (menuPrices[i].menuId == local_order.menuId) {
                    price = menuPrices[i].price;
                    break;
                }
            }
            total_sales += price * local_order.quantity;
            orders_today++;
        }
    }
    fclose(fp);
    
    printf("Total Orders Completed: %d\n", orders_today);
    printf("Total Revenue: RM %.2f\n", total_sales);
    printf("---------------------------------------\n");

    free(menuPrices);       //the free keyword deallocates the memory that was reserved by malloc for the menuPrices
}










// helper function to update the stock of a menu item
void updateStock(int menuId, int quantitySold) {
    FILE *fp, *tempFp;
    struct menuItem item;

    fp = fopen("data/menus.txt", "r");
    tempFp = fopen("data/temp_menus.txt", "w");

    if (fp == NULL || tempFp == NULL) {
        printf("Error: Could not open menu files for stock update.\n");
        if(fp) fclose(fp);
        if(tempFp) fclose(tempFp);
        return;
    }

    // Read from the original file, modify if necessary, and write to the temp file
    while (fscanf(fp, "%d|%d|%[^|]|%f|%d\n", &item.menuId, &item.restaurantId, item.itemName, &item.price, &item.stock) == 5) {
        // If this is the item whose stock we need to update
        if (item.menuId == menuId) {
            item.stock -= quantitySold; // Subtract the quantity sold
            if (item.stock < 0) {
                item.stock = 0; // Ensure stock doesn't go below zero
            }
        }
        // Write the (potentially updated) item to the temporary file
        fprintf(tempFp, "%d|%d|%s|%.2f|%d\n", item.menuId, item.restaurantId, item.itemName, item.price, item.stock);
    }

    fclose(fp);
    fclose(tempFp);

    // Replace the old menu file with the updated one
    remove("data/menus.txt");
    rename("data/temp_menus.txt", "data/menus.txt");
}















// function for order queue management with stock subtraction
void orderQueue(int restaurantId) {
    OrderNode *head = NULL, *current = NULL, *newNode = NULL, *tail = NULL;      // this first part loads the file into a linked list (consists of those 4 pointers)
    FILE *fp ;

    fp = fopen("data/orders.txt", "r");
    if (fp == NULL) { /* ... */ return; }
    // while loop that reads the entire file into memory
    while (!feof(fp)) {
        newNode = (OrderNode*) malloc(sizeof(OrderNode));      // this malloc reserves a new chunk of memory big enough to hold one orderthen the address of this memory is stored in newNode.
        if (fscanf(fp, "%d|%d|%d|%d|%d|%[^|]|%[^\n]\n", &newNode->orderID, &newNode->studentID, &newNode->restaurantId, &newNode->menuId, &newNode->quantity, newNode->status, newNode->date) == 7) {
            newNode->next = NULL;
            if (head == NULL) {
                head = newNode ;
                tail = newNode ;
            } else {
                tail->next = newNode;
                tail = newNode ;
            }
        } else {
            free(newNode);
            break ;
        }
    }
    fclose(fp);

    printf("\n--- Pending Orders (Your Queue) ---\n") ;      // the 2nd part of this function displays the queue and requests user input
    int found_pending = 0;
    current = head ;
    while (current != NULL) {
        if (current->restaurantId == restaurantId && strcmp(current->status, "Pending") == 0) {
            printf("Order ID: %d, Menu ID: %d, Quantity: %d\n", current->orderID, current->menuId, current->quantity);
            found_pending = 1  ;
        }
        current = current->next;
    }
    if (!found_pending) {
        printf("The order queue is empty.\n");
        while(head != NULL) {
            OrderNode *temp = head;
            head = head->next ;
            free(temp);
        }
        return;
    }

    int targetOrderID;
    char newStatus[20];
    printf("\nEnter the ID of the order to process: ") ;
    scanf("%d", &targetOrderID) ;
    getchar() ;

    printf("Enter the new status (Perpaing/Ready For Delievery/Completed): ");
    fgets(newStatus, 20, stdin);
    newStatus[strcspn(newStatus, "\n")] = '\0' ;
    
    // this last part updates the order and saves it back to file 
    current = head;
    int found_target = 0 ;
    while (current != NULL) {
        if (current->orderID == targetOrderID && current->restaurantId == restaurantId) {
            strcpy(current->status, newStatus) ;
            found_target = 1 ;

            
            // If the order is being processed (not just pending), update the stock.
            if (strcmp(newStatus, "Pending") != 0) {
                updateStock(current->menuId, current->quantity);
                printf("Stock for Menu ID %d has been updated.\n", current->menuId) ;
            }
            

            break;
        }
        current = current->next ;
    }
    
    if (found_target) {
        fp = fopen("data/orders.txt", "w");
        if (fp == NULL) { return; }
        current = head ;
        while (current != NULL) {
            fprintf(fp, "%d|%d|%d|%d|%d|%s|%s\n", current->orderID, current->studentID, current->restaurantId, current->menuId, current->quantity, current->status, current->date);
            current = current->next;
        }
        fclose(fp);
        printf("\nOrder #%d updated successfully.\n", targetOrderID);
    } else {
        printf("\nError: Order #%d not found.\n", targetOrderID) ;
    }

    current = head ;
    while(current != NULL) {
        OrderNode *temp = current;
        current = current->next ;
        free(temp) ;
    }
}
