#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create a customer
Customer createCustomer(const char* firstName, const char* lastName, const char* contactInfo) {
    Customer customer;
    strncpy(customer.firstName, firstName, sizeof(customer.firstName) - 1);
    customer.firstName[sizeof(customer.firstName) - 1] = '\0';
    strncpy(customer.lastName, lastName, sizeof(customer.lastName) - 1);
    customer.lastName[sizeof(customer.lastName) - 1] = '\0';
    strncpy(customer.contactInfo, contactInfo, sizeof(customer.contactInfo) - 1);
    customer.contactInfo[sizeof(customer.contactInfo) - 1] = '\0';
    return customer;
}

// Function to display customer information
void displayCustomer(const Customer* customer) {
    printf("Customer: %s %s, Contact: %s\n", customer->firstName, customer->lastName, customer->contactInfo);
}

// Function to create a game
Game createGame(const char* title, float rentalPrice) {
    Game game;
    strncpy(game.title, title, sizeof(game.title) - 1);
    game.title[sizeof(game.title) - 1] = '\0';
    game.rentalPrice = rentalPrice;
    return game;
}

// Function to display game information
void displayGame(const Game* game) {
    printf("Game Title: %s, Rental Price: %.2f\n", game->title, game->rentalPrice);
}

// Function to create a rental
Rental createRental(int rentalID, int customerID, int gameID, Date rentalDate, Date returnDate) {
    Rental rental;
    rental.rentalID = rentalID;
    rental.customer.customerID = customerID;
    rental.game.gameID = gameID;
    rental.rentalDate = rentalDate;
    rental.returnDate = returnDate;
    return rental;
}

// Function to display rental information
void displayRental(const Rental* rental) {
    printf("Rental ID: %d\n", rental->rentalID);
    printf("Customer ID: %d\n", rental->customer.customerID);
    printf("Game ID: %d\n", rental->game.gameID);
    printf("Rental Date: %02d/%02d/%04d\n", rental->rentalDate.day, rental->rentalDate.month, rental->rentalDate.year);
    printf("Return Date: %02d/%02d/%04d\n", rental->returnDate.day, rental->returnDate.month, rental->returnDate.year);
}

// Function to create a rental block
Block* createBlock() {
    Block* block = (Block*)malloc(sizeof(Block));
    block->rentalCount = 0;
    block->next = NULL;
    return block;
}

// Function to add a rental to a block
void addRentalToBlock(Block* block, Rental rental) {
    if (block->rentalCount < BLOCK_SIZE) {
        block->rentals[block->rentalCount++] = rental;
    } else {
        Block* newBlock = createBlock();
        newBlock->rentals[0] = rental;
        newBlock->rentalCount = 1;
        block->next = newBlock;
    }
}

// Function to display all rentals in a block
void displayBlock(const Block* block) {
    const Block* current = block;
    while (current != NULL) {
        for (int i = 0; i < current->rentalCount; i++) {
            displayRental(&current->rentals[i]);
            printf("\n");
        }
        current = current->next;
    }
}

// Function to search for a rental by ID
Rental* searchRental(Block* block, int rentalID) {
    Block* current = block;
    while (current != NULL) {
        for (int i = 0; i < current->rentalCount; i++) {
            if (current->rentals[i].rentalID == rentalID) {
                return &current->rentals[i];
            }
        }
        current = current->next;
    }
    return NULL;
}