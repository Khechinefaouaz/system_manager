#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"
#include "file.h"

void displayMenu() {
    printf("\n=== Game Rental Management System ===\n");
    printf("1. Add New Customer\n");
    printf("2. Add New Game\n");
    printf("3. Create New Rental\n");
    printf("4. Display All Customers\n");
    printf("5. Display All Games\n");
    printf("6. Display All Rentals\n");
    printf("7. Search Rental by ID\n");
    printf("8. Edit Customer\n");
    printf("9. Edit Game\n");
    printf("10. Edit Rental\n");
    printf("11. Delete Customer\n");
    printf("12. Delete Game\n");
    printf("13. Delete Rental\n");
    printf("0. Exit\n");
    printf("Choose an option: ");
}

Date inputDate() {
    Date date;
    do {
        printf("Enter day (1-31): ");
        scanf("%d", &date.day);
        printf("Enter month (1-12): ");
        scanf("%d", &date.month);
        printf("Enter year (2000-2100): ");
        scanf("%d", &date.year);
        getchar(); // Clear buffer
        
        if (date.day < 1 || date.day > 31 || date.month < 1 || date.month > 12 || 
            date.year < 2000 || date.year > 2100) {
            printf("Invalid date! Please try again.\n");
        }
    } while (date.day < 1 || date.day > 31 || date.month < 1 || date.month > 12 || 
             date.year < 2000 || date.year > 2100);
    return date;
}

int main() {
    // Move file paths to the top before they are used
    const char* customerFile = "customers.dat";
    const char* gameFile = "games.dat";
    const char* rentalFile = "rentals.dat";

    // Initialize file handling properly
    TOF *customerTOF = NULL, *gameTOF = NULL, *rentalTOF = NULL;
    Block* rentalBlock = NULL;
    
    int choice;
    int nextCustomerId = 1;
    int nextGameId = 1;
    int nextRentalId = 1;

    // Proper file initialization
    customerTOF = open_file((char*)customerFile, 'N');
    if (!customerTOF) {
        printf("Error creating customer file!\n");
        return 1;
    }
    
    gameTOF = open_file((char*)gameFile, 'N');
    if (!gameTOF) {
        printf("Error creating game file!\n");
        fclose(customerTOF->file);
        free(customerTOF);
        return 1;
    }
    
    rentalTOF = open_file((char*)rentalFile, 'N');
    if (!rentalTOF) {
        printf("Error creating rental file!\n");
        fclose(customerTOF->file);
        fclose(gameTOF->file);
        free(customerTOF);
        free(gameTOF);
        return 1;
    }

    // Initialize rental block
    rentalBlock = createBlock();
    if (!rentalBlock) {
        printf("Error creating rental block!\n");
        // Clean up files
        fclose(customerTOF->file);
        fclose(gameTOF->file);
        fclose(rentalTOF->file);
        free(customerTOF);
        free(gameTOF);
        free(rentalTOF);
        return 1;
    }

    do {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // Clear input buffer

        switch (choice) {
            case 1: {
                // Add New Customer
                char firstName[MAX_NAME_LENGTH];
                char lastName[MAX_NAME_LENGTH];
                char contactInfo[MAX_CONTACT_LENGTH];

                printf("Enter First Name: ");
                fgets(firstName, MAX_NAME_LENGTH, stdin);
                firstName[strcspn(firstName, "\n")] = 0;

                printf("Enter Last Name: ");
                fgets(lastName, MAX_NAME_LENGTH, stdin);
                lastName[strcspn(lastName, "\n")] = 0;

                printf("Enter Contact Info: ");
                fgets(contactInfo, MAX_CONTACT_LENGTH, stdin);
                contactInfo[strcspn(contactInfo, "\n")] = 0;

                Customer customer = createCustomer(firstName, lastName, contactInfo);
                CustomerReference custRef = {nextCustomerId++};
                saveCustomerToFile(customerFile, custRef, customer);
                printf("Customer added successfully!\n");
                break;
            }

            case 2: {
                // Add New Game
                char title[MAX_TITLE_LENGTH];
                float price;

                printf("Enter Game Title: ");
                fgets(title, MAX_TITLE_LENGTH, stdin);
                title[strcspn(title, "\n")] = 0;

                printf("Enter Rental Price: ");
                scanf("%f", &price);
                getchar();

                Game game = createGame(title, price);
                GameReference gameRef = {nextGameId++};
                saveGameToFile(gameFile, gameRef, game);
                printf("Game added successfully!\n");
                break;
            }

            case 3: {
                // Create New Rental
                int customerId, gameId;
                printf("Enter Customer ID: ");
                scanf("%d", &customerId);
                printf("Enter Game ID: ");
                scanf("%d", &gameId);

                printf("Enter Rental Date:\n");
                Date rentalDate = inputDate();
                printf("Enter Return Date:\n");
                Date returnDate = inputDate();

                Rental rental = createRental(nextRentalId++, customerId, gameId, rentalDate, returnDate);
                addRentalToBlock(rentalBlock, rental);
                saveRentalsToFile(rentalFile, rentalBlock);
                printf("Rental created successfully!\n");
                break;
            }

            case 4:{
                // Display All Customers
                loadCustomersFromFile(customerFile);
                break;
            }
            case 5:{
                // Display All Games
                loadGamesFromFile(gameFile);
                break;
            }
            case 6:{
                // Display All Rentals
                displayBlock(rentalBlock);
                break;
            }   
            case 7: {
                // Search Rental by ID
                int searchId;
                printf("Enter Rental ID to search: ");
                if (scanf("%d", &searchId) != 1 || searchId < 1) {
                    printf("Invalid rental ID!\n");
                    getchar();
                    break;
                }
                Rental* found = searchRental(rentalBlock, searchId);
                if (found) {
                    displayRental(found);
                } else {
                    printf("Rental not found.\n");
                }
                break;
            }

            case 8: {
                // Edit Customer
                int customerId;
                printf("Enter Customer ID to edit: ");
                if (scanf("%d", &customerId) != 1 || customerId < 1) {
                    printf("Invalid customer ID!\n");
                    getchar();
                    break;
                }
                getchar();

                char firstName[MAX_NAME_LENGTH];
                char lastName[MAX_NAME_LENGTH];
                char contactInfo[MAX_CONTACT_LENGTH];

                printf("Enter New First Name: ");
                fgets(firstName, MAX_NAME_LENGTH, stdin);
                firstName[strcspn(firstName, "\n")] = 0;

                printf("Enter New Last Name: ");
                fgets(lastName, MAX_NAME_LENGTH, stdin);
                lastName[strcspn(lastName, "\n")] = 0;

                printf("Enter New Contact Info: ");
                fgets(contactInfo, MAX_CONTACT_LENGTH, stdin);
                contactInfo[strcspn(contactInfo, "\n")] = 0;

                Customer newCustomer = createCustomer(firstName, lastName, contactInfo);
                editCustomer(customerFile, customerId, newCustomer);
                break;
            }

            case 9: {
                // Edit Game
                int gameId;
                printf("Enter Game ID to edit: ");
                scanf("%d", &gameId);
                getchar();

                char title[MAX_TITLE_LENGTH];
                float price;

                printf("Enter New Game Title: ");
                fgets(title, MAX_TITLE_LENGTH, stdin);
                title[strcspn(title, "\n")] = 0;

                printf("Enter New Rental Price: ");
                scanf("%f", &price);
                getchar();

                Game newGame = createGame(title, price);
                editGame(gameFile, gameId, newGame);
                break;
            }

            case 10: {
                // Edit Rental
                int rentalId;
                printf("Enter Rental ID to edit: ");
                scanf("%d", &rentalId);
                getchar();

                Rental* rental = searchRental(rentalBlock, rentalId);
                if (!rental) {
                    printf("Rental not found.\n");
                    break;
                }

                int customerId, gameId;
                printf("Enter new Customer ID (current: %d): ", rental->customer.customerID);
                scanf("%d", &customerId);
                printf("Enter new Game ID (current: %d): ", rental->game.gameID);
                scanf("%d", &gameId);

                printf("Enter new Rental Date:\n");
                Date rentalDate = inputDate();
                printf("Enter new Return Date:\n");
                Date returnDate = inputDate();

                rental->customer.customerID = customerId;
                rental->game.gameID = gameId;
                rental->rentalDate = rentalDate;
                rental->returnDate = returnDate;

                saveRentalsToFile(rentalFile, rentalBlock);
                printf("Rental updated successfully!\n");
                break;
            }

            case 11: {
                // Delete Customer
                int customerId;
                printf("Enter Customer ID to delete: ");
                scanf("%d", &customerId);
                getchar();

                deleteCustomer(customerFile, customerId);
                break;
            }

            case 12: {
                // Delete Game
                int gameId;
                printf("Enter Game ID to delete: ");
                scanf("%d", &gameId);
                getchar();

                deleteGame(gameFile, gameId);
                break;
            }

            case 13: {
                // Delete Rental
                int rentalId;
                printf("Enter Rental ID to delete: ");
                scanf("%d", &rentalId);
                getchar();

                deleteRental(rentalBlock, rentalId);
                saveRentalsToFile(rentalFile, rentalBlock);
                printf("Rental deleted successfully!\n");
                break;
            }

            case 0:
                printf("Exiting program...\n");
                break;

            default:
                printf("Invalid option. Please try again.\n");
        }
    } while (choice != 0);

    // Proper cleanup at the end
    if (customerTOF) {
        fclose(customerTOF->file);
        free(customerTOF);
    }
    if (gameTOF) {
        fclose(gameTOF->file);
        free(gameTOF);
    }
    if (rentalTOF) {
        fclose(rentalTOF->file);
        free(rentalTOF);
    }
    
    // Free rental block
    Block* current = rentalBlock;
    while (current != NULL) {
        Block* next = current->next;
        free(current);
        current = next;
    }

    return 0;
}

