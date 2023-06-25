#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<dos.h>




FILE* file; // Declare the file variable as a global variable

// Function prototypes
void displaySplashScreen();
int login();
void createUser();
void displayMenu();
void processOrder();
void viewInventory();
void exitProgram();
void processOrder();
void paymentProcessing();
void populatePriceList();

typedef struct {
    char itemName[100];
    int quantity;
    float price;
} Order;

typedef struct {
    char itemName[100];
    float price;
} Item;


Order orders[100];
int numOrders = 0;
Item priceList[100]; // Array to store the price list
int numItems = 0;

int main() {
    // Display splash screen
    displaySplashScreen();

    // Login
    int loggedIn = login();
    if (!loggedIn) {
        printf("Login failed. Exiting program.\n");
        return 0;
    }

    // Display main menu
    displayMenu();

    return 0;
}


//splashscreen dont touch
void displaySplashScreen() {
    FILE *file = fopen("splash.txt", "r");
    if (file == NULL) {
        printf("Error opening splash.txt file.\n");
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);
    sleep(2);
}

//login hehe

int login() {
    char username[20];
    char password[20];
    int loginAttempts = 3; // Number of login attempts allowed

    do {
        printf("1. Login\n");
        printf("2. Create a new user\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Please enter your username: ");
            scanf("%s", username);

            printf("Please enter your password: ");
            scanf("%s", password);

            FILE *file = fopen("login.txt", "r");
            if (file == NULL) {
                printf("Error opening login.txt file.\n");
                return 0;
            }

            char fileUsername[20];
            char filePassword[20];
            while (fscanf(file, "%s %s", fileUsername, filePassword) == 2) {
                if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
                    fclose(file);
                    printf("Login successful!\n");
                    return 1;
                }
            }

            fclose(file);
            printf("Incorrect username or password. Remaining attempts: %d\n", loginAttempts - 1);
            loginAttempts--;
        } else if (choice == 2) {
            createUser();
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    } while (loginAttempts > 0);

    printf("Login failed. Exiting program.\n");
    return 0;
}




//usercreate
void createUser() {

    char username[20];
    char password[20];

    printf("Enter a new username: ");
    scanf("%s", username);

    printf("Enter a new password: ");
    scanf("%s", password);

    FILE *file = fopen("login.txt", "a");
    if (file == NULL) {
        printf("Error opening login.txt file.\n");
        return;
    }

    fprintf(file, "%s %s\n", username, password);

    fclose(file);

    printf("User created successfully!\n");
}

void displayMenu() {
    int choice;
    do {
        printf("\n========= MENU =========\n");
        printf("1. Place an order\n");
        printf("2. Payment processing\n");
        printf("3. View Menu\n");
        printf("4. Exit\n");
        printf("========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                processOrder();
                break;
            case 2:
                paymentProcessing();
                break;
            case 3:
                viewInventory();
                break;
            case 4:
                exitProgram();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 4);
}

void processOrder() {
    char itemName[100];
    int quantity;

    printf("Enter the item name ('q' to quit): ");
    scanf("%s", itemName);

    while (strcmp(itemName, "q") != 0) {
        printf("Enter the quantity: ");
        scanf("%d", &quantity);

        // Open the inventory file in read mode
        FILE *file = fopen("inventory.txt", "r");
        if (file == NULL) {
            printf("Error opening inventory.txt file.\n");
            return;
        }

        char inventoryItem[100];
        int inventoryQuantity;
        float price;
        int itemFound = 0;

        // Read the inventory from the file and find the specified item
        while (fscanf(file, "%s %d %f", inventoryItem, &inventoryQuantity, &price) != EOF) {
            if (strcmp(itemName, inventoryItem) == 0) {
                itemFound = 1;
                if (inventoryQuantity >= quantity) {
                    inventoryQuantity -= quantity;
                    printf("Order placed successfully.\n");

                    // Add the order to the orders array
                    strcpy(orders[numOrders].itemName, itemName);
                    orders[numOrders].quantity = quantity;
                    orders[numOrders].price = price;
                    numOrders++;
                } else {
                    printf("Insufficient quantity in inventory.\n");
                }
            }
        }

        fclose(file);

        if (!itemFound) {
            printf("Item not found in inventory.\n");
        }

        printf("Enter the item name ('q' to quit): ");
        scanf("%s", itemName);
    }
}

void paymentProcessing() {
    printf("Order Summary:\n");
    printf("--------------\n");

    float totalCost = 0.0;

    // Display each order and calculate the total cost
    for (int i = 0; i < numOrders; i++) {
        printf("Item: %s\n", orders[i].itemName);
        printf("Quantity: %d\n", orders[i].quantity);

        // Find the price of the item in the price list
        float price = 0.0;
        for (int j = 0; j < numItems; j++) {
            if (strcmp(orders[i].itemName, priceList[j].itemName) == 0) {
                price = priceList[j].price;
                break;
            }
        }

        printf("Price: $%.2f\n", price);
        float orderCost = orders[i].quantity * price;
        printf("Order Cost: $%.2f\n", orderCost);
        printf("--------------\n");
        totalCost += orderCost;
    }

    printf("Total Cost: $%.2f\n", totalCost);
}


void viewInventory() {
    // Open the inventory file in read mode
    FILE *file = fopen("inventory.txt", "r");
    if (file == NULL) {
        printf("Error opening inventory.txt file.\n");
        return;
    }

    char itemName[1000];
    int quantity;
    float price;

    printf("Current Inventory:\n");
    printf("------------------\n");
    printf("Item Name\t\t\tPrice\t\t\tQuantity\n"); // Header with tabs

    // Read the item name, quantity, and price from the file and display them
    while (fscanf(file, "%s %d %f", itemName, &quantity, &price) == 3)
        {
        printf("%s\t\t\t%.2f\t\t\t%d\n", itemName, price, quantity); // Print with tabs
        // Add the item to the price list


        strcpy(priceList[numItems].itemName, itemName);
        priceList[numItems].price = price;
        numItems++;
        }

    fclose(file);
}



void exitProgram() {
    printf("Exiting program...\n");

    printf("Are you sure you want to exit? (Y/N): ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'Y' || choice == 'y') {
        printf("Thank you for using the coffee shop management program. Goodbye!\n");
        exit(0); // Terminate the program
    } else if (choice == 'N' || choice == 'n') {
        displayMenu(); // Go back to the main menu
    } else {
        printf("Invalid choice. Please try again.\n");
        exitProgram(); // Recursively call exitProgram() if the choice is invalid
    }
}


void populatePriceList() {
    // Open the inventory file in read mode
    FILE *file = fopen("inventory.txt", "r");
    if (file == NULL) {
        printf("Error opening inventory.txt file.\n");
        return;
    }

    char itemName[100];
    int quantity;
    float price;

    // Read the item name, quantity, and price from the file and populate the price list
    while (fscanf(file, "%s %d %f", itemName, &quantity, &price) == 3) {
        // Add the item to the price list
        strcpy(priceList[numItems].itemName, itemName);
        priceList[numItems].price = price;
        numItems++;
    }

    fclose(file);
}


