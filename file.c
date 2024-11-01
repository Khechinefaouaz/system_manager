#include <stdio.h>
#include <stdlib.h>
#include "file.h"

// Function to open a file with header management
TOF* open_file(char* path, char mode) {
    TOF* tof = (TOF*)malloc(sizeof(TOF));
    if (mode == 'N' || mode == 'n') {
        tof->file = fopen(path, "wb+");
        tof->entete.nbCustomers = 0;
        tof->entete.nbGames = 0;
        tof->entete.nbRentals = 0;
        write_entete(tof->entete, tof);
    } else if (mode == 'A' || mode == 'a') {
        tof->file = fopen(path, "rb+");
        if (tof->file) read_entete(tof);
    }
    return tof;
}

// Function to read file header
void read_entete(TOF* tof) {
    rewind(tof->file);
    fread(&tof->entete, sizeof(Entete), 1, tof->file);
}

// Function to write file header
void write_entete(Entete et, TOF* tof) {
    rewind(tof->file);
    fwrite(&et, sizeof(Entete), 1, tof->file);
}

// Function to save rentals to a file
void saveRentalsToFile(const char* filename, Block* block) {
    TOF* tof = open_file((char*)filename, 'N');
    if (!tof || !tof->file) {
        perror("Failed to open file for writing");
        return;
    }

    fseek(tof->file, sizeof(Entete), SEEK_SET);
    Block* current = block;
    int totalRentals = 0;
    
    while (current) {
        fwrite(current->rentals, sizeof(Rental), current->rentalCount, tof->file);
        totalRentals += current->rentalCount;
        current = current->next;
    }

    tof->entete.nbRentals = totalRentals;
    write_entete(tof->entete, tof);
    
    fclose(tof->file);
    free(tof);
}

// Function to load rentals from a file
Block* loadRentalsFromFile(const char* filename) {
    TOF* tof = open_file((char*)filename, 'A');
    if (!tof || !tof->file) {
        perror("Failed to open file for reading");
        return NULL;
    }

    fseek(tof->file, sizeof(Entete), SEEK_SET);
    Block* head = NULL;
    Block* last = NULL;

    while (1) {
        Block* newBlock = createBlock();
        size_t readCount = fread(newBlock->rentals, sizeof(Rental), BLOCK_SIZE, tof->file);
        newBlock->rentalCount = readCount;

        if (readCount == 0) {
            free(newBlock);
            break;
        }

        if (!head) {
            head = newBlock;
        } else {
            last->next = newBlock;
        }
        last = newBlock;
    }

    fclose(tof->file);
    free(tof);
    return head;
}

// Function to save a customer to a file
void saveCustomerToFile(const char* filename, CustomerReference custRef, Customer customer) {
    TOF* tof = open_file((char*)filename, 'A');
    if (!tof || !tof->file) {
        perror("Failed to open customer file for writing");
        return;
    }

    fseek(tof->file, 0, SEEK_END);
    fwrite(&custRef, sizeof(CustomerReference), 1, tof->file);
    fwrite(&customer, sizeof(Customer), 1, tof->file);
    
    tof->entete.nbCustomers++;
    write_entete(tof->entete, tof);
    
    fclose(tof->file);
    free(tof);
}

// Function to edit a customer
void editCustomer(const char* filename, int customerID, Customer newData) {
    FILE* file = fopen(filename, "rb+");
    if (!file) {
        perror("Failed to open customer file for editing");
        return;
    }
    fseek(file, sizeof(Entete), SEEK_SET);
    CustomerReference custRef;
    Customer customer;
    
    while (fread(&custRef, sizeof(CustomerReference), 1, file)) {
        if (custRef.customerID == customerID) {
            fwrite(&newData, sizeof(Customer), 1, file);
            printf("Customer updated successfully!\n");
            fclose(file);
            return;
        }
        fseek(file, sizeof(Customer), SEEK_CUR);
    }
    
    printf("Customer with ID %d not found.\n", customerID);
    fclose(file);
}

// Function to load and display all customers
void loadCustomersFromFile(const char* filename) {
    TOF* tof = open_file((char*)filename, 'A');
    if (!tof || !tof->file) {
        perror("Failed to open customer file for reading");
        return;
    }

    fseek(tof->file, sizeof(Entete), SEEK_SET);
    CustomerReference custRef;
    Customer customer;
    printf("Customers:\n");
    while (fread(&custRef, sizeof(CustomerReference), 1, tof->file) && 
           fread(&customer, sizeof(Customer), 1, tof->file)) {
        printf("ID: %d, Name: %s %s, Contact: %s\n", 
                custRef.customerID, customer.firstName, customer.lastName, customer.contactInfo);
    }

    fclose(tof->file);
    free(tof);
}

// Function to save a game to a file
void saveGameToFile(const char* filename, GameReference gameRef, Game game) {
    TOF* tof = open_file((char*)filename, 'A');
    if (!tof || !tof->file) {
        perror("Failed to open game file for writing");
        return;
    }

    fseek(tof->file, 0, SEEK_END);
    fwrite(&gameRef, sizeof(GameReference), 1, tof->file);
    fwrite(&game, sizeof(Game), 1, tof->file);
    
    tof->entete.nbGames++;
    write_entete(tof->entete, tof);
    
    fclose(tof->file);
    free(tof);
}

// Function to edit a game
void editGame(const char* filename, int gameID, Game newData) {
    FILE* file = fopen(filename, "rb+");
    if (!file) {
        perror("Failed to open game file for editing");
        return;
    }

    fseek(file, sizeof(Entete), SEEK_SET);
    GameReference gameRef;
    Game game;
    
    while (fread(&gameRef, sizeof(GameReference), 1, file)) {
        if (gameRef.gameID == gameID) {
            fwrite(&newData, sizeof(Game), 1, file);
            printf("Game updated successfully!\n");
            fclose(file);
            return;
        }
        fseek(file, sizeof(Game), SEEK_CUR);
    }
    
    printf("Game with ID %d not found.\n", gameID);
    fclose(file);
}

// Function to load and display all games
void loadGamesFromFile(const char* filename) {
    TOF* tof = open_file((char*)filename, 'A');
    if (!tof || !tof->file) {
        perror("Failed to open game file for reading");
        return;
    }

    fseek(tof->file, sizeof(Entete), SEEK_SET);
    GameReference gameRef;
    Game game;
    printf("Games:\n");
    while (fread(&gameRef, sizeof(GameReference), 1, tof->file) && 
           fread(&game, sizeof(Game), 1, tof->file)) {
        printf("ID: %d, Title: %s, Price: %.2f\n", 
                gameRef.gameID, game.title, game.rentalPrice);
    }

    fclose(tof->file);
    free(tof);
}

// Function to delete a customer
void deleteCustomer(const char* filename, int customerID) {
    const char* tempFile = "temp.dat";
    TOF* sourceTOF = open_file((char*)filename, 'A');
    TOF* tempTOF = open_file((char*)tempFile, 'N');

    if (!sourceTOF || !tempTOF) {
        perror("Failed to open files for deletion");
        return;
    }

    fseek(sourceTOF->file, sizeof(Entete), SEEK_SET);
    CustomerReference custRef;
    Customer customer;
    int found = 0;

    while (fread(&custRef, sizeof(CustomerReference), 1, sourceTOF->file) &&
           fread(&customer, sizeof(Customer), 1, sourceTOF->file)) {
        if (custRef.customerID != customerID) {
            fwrite(&custRef, sizeof(CustomerReference), 1, tempTOF->file);
            fwrite(&customer, sizeof(Customer), 1, tempTOF->file);
            tempTOF->entete.nbCustomers++;
        } else {
            found = 1;
        }
    }

    if (!found) {
        printf("Customer with ID %d not found.\n", customerID);
    } else {
        printf("Customer deleted successfully!\n");
    }

    write_entete(tempTOF->entete, tempTOF);
    
    fclose(sourceTOF->file);
    fclose(tempTOF->file);
    free(sourceTOF);
    free(tempTOF);

    remove(filename);
    rename(tempFile, filename);
}

// Function to delete a game
void deleteGame(const char* filename, int gameID) {
    const char* tempFile = "temp.dat";
    TOF* sourceTOF = open_file((char*)filename, 'A');
    TOF* tempTOF = open_file((char*)tempFile, 'N');

    if (!sourceTOF || !tempTOF) {
        perror("Failed to open files for deletion");
        return;
    }

    fseek(sourceTOF->file, sizeof(Entete), SEEK_SET);
    GameReference gameRef;
    Game game;
    int found = 0;

    while (fread(&gameRef, sizeof(GameReference), 1, sourceTOF->file) &&
           fread(&game, sizeof(Game), 1, sourceTOF->file)) {
        if (gameRef.gameID != gameID) {
            fwrite(&gameRef, sizeof(GameReference), 1, tempTOF->file);
            fwrite(&game, sizeof(Game), 1, tempTOF->file);
            tempTOF->entete.nbGames++;
        } else {
            found = 1;
        }
    }

    if (!found) {
        printf("Game with ID %d not found.\n", gameID);
    } else {
        printf("Game deleted successfully!\n");
    }

    write_entete(tempTOF->entete, tempTOF);
    
    fclose(sourceTOF->file);
    fclose(tempTOF->file);
    free(sourceTOF);
    free(tempTOF);

    remove(filename);
    rename(tempFile, filename);
}

// Function to delete a rental
void deleteRental(Block* block, int rentalID) {
    Block* current = block;
    Block* prev = NULL;

    while (current != NULL) {
        for (int i = 0; i < current->rentalCount; i++) {
            if (current->rentals[i].rentalID == rentalID) {
                // Shift remaining rentals in the block
                for (int j = i; j < current->rentalCount - 1; j++) {
                    current->rentals[j] = current->rentals[j + 1];
                }
                current->rentalCount--;

                // If block is empty and it's not the first block, remove it
                if (current->rentalCount == 0 && prev != NULL) {
                    prev->next = current->next;
                    free(current);
                }
                return;
            }
        }
        prev = current;
        current = current->next;
    }
    printf("Rental with ID %d not found.\n", rentalID);
}