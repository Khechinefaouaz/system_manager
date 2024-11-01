#ifndef FILE_H
#define FILE_H

#include "library.h"

// Function declarations for file operations
TOF* open_file(char* path, char mode);
void read_entete(TOF* tof);
void write_entete(Entete et, TOF* tof);
void saveRentalsToFile(const char* filename, Block* block);
Block* loadRentalsFromFile(const char* filename);
void saveCustomerToFile(const char* filename, CustomerReference custRef, Customer customer);
void loadCustomersFromFile(const char* filename);
void saveGameToFile(const char* filename, GameReference gameRef, Game game);
void loadGamesFromFile(const char* filename);
void editCustomer(const char* filename, int customerID, Customer newData);
void editGame(const char* filename, int gameID, Game newData);
void deleteCustomer(const char* filename, int customerID);
void deleteGame(const char* filename, int gameID);
void deleteRental(Block* block, int rentalID);

#endif // FILE_H