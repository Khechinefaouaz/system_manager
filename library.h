#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_CONTACT_LENGTH 100
#define MAX_TITLE_LENGTH 100
#define BLOCK_SIZE 5

// Date structure
typedef struct {
    int day;
    int month;
    int year;
} Date;

// Customer structure
typedef struct {
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char contactInfo[MAX_CONTACT_LENGTH];
} Customer;

// Customer Reference structure
typedef struct {
    int customerID;
} CustomerReference;

// Game structure
typedef struct {
    char title[MAX_TITLE_LENGTH];
    float rentalPrice;
} Game;

// Game Reference structure
typedef struct {
    int gameID;
} GameReference;

// Rental structure
typedef struct {
    int rentalID;
    CustomerReference customer;
    GameReference game;
    Date rentalDate;
    Date returnDate;
} Rental;

// Block structure for rentals
typedef struct Block {
    Rental rentals[BLOCK_SIZE];
    int rentalCount;
    struct Block* next;
} Block;

// File header structure
typedef struct {
    int nbCustomers;  // Number of customers
    int nbGames;      // Number of games
    int nbRentals;    // Number of rentals
} Entete;

// Type Opened File structure
typedef struct {
    FILE* file;
    Entete entete;
} TOF;

// Function declarations
Customer createCustomer(const char* firstName, const char* lastName, const char* contactInfo);
void displayCustomer(const Customer* customer);
Game createGame(const char* title, float rentalPrice);
void displayGame(const Game* game);
Rental createRental(int rentalID, int customerID, int gameID, Date rentalDate, Date returnDate);
void displayRental(const Rental* rental);
Block* createBlock();
void addRentalToBlock(Block* block, Rental rental);
void displayBlock(const Block* block);
Rental* searchRental(Block* block, int rentalID);

#endif // LIBRARY_H