#pragma once
#ifndef CAR_H
#define CAR_H

#include "Date.h"

// Different speed of charging
typedef enum {
    FAST,
    MID,
    SLOW
} portSpeed;

 // Car structure

typedef struct {
    char nLicense[10];      // License number , 8 numbers
    portSpeed portType;     // Charging port type
    double totalPayed;      // Total amount paid until today
    struct Port* pPort;     // Pointer to charging port (NULL if not charging)
    int inqueue;           // 0 - charging, 1 - waiting for charging (in queue)
} Car;

 // Binary search tree node for cars

typedef struct tCar {
    Car* car;               // Pointer to Car
    struct tCar* right;     // Pointer to the right 
    struct tCar* left;      // Pointer to the left
} tCar;

typedef struct {
    tCar* root;              // Pointer to root of BST
} carBST;

// Functions declarations

 
/*
 * Initialize a new car
 *  license - license number string
 *  type - charging port type
 *  totalPayed - total amount paid until today
 *  inQueue - 0 if charging, 1 if waiting in queue
 * return pointer to new Car or NULL if failed
 */
Car* initCar(const char* license, portSpeed type, int totalPayed, int inQueue );

/*
 * Create station BST manager
 * return initialized StationBST structure
 */
carBST* initCarBST();

/*
 * Validate license number format
 *  license - license number string
 * return 1 if valid, 0 if invalid
 */
int validateLicense(const char* license);

/*
 * Create a new car BST node
 *  car - pointer to Car
 * return pointer to new tCar node or NULL if failed
 */
tCar* createCarNode(Car* car);

/*
 * Insert car into BST
 *  root - pointer to root of BST
 *  car - pointer to Car to insert
 * return pointer to root of BST
 */
tCar* insertCar(tCar* root, Car* car);

/*
 * Search for car in BST by license number
 *  root - pointer to root of BST
 *  license - license number to search
 * return pointer to Car if found, NULL otherwise
 */
Car* searchCar(tCar* root, const char* license);

/*
 * Print car details
 *  car - pointer to Car to print
 */
void printCar(Car* car);

/*
 * Simple universal car license input function
 *  license - output buffer to store the license (must be at least MAX_LICENSE_LENGTH)
 * return 1 if successful, 0 if user wants to return to menu
 */
int getLicenseFromUser(char* license);

/*
 * Display top customers based on total amount paid
 *  carsBST - pointer to carBST
 */
void dispTopCustomers(carBST* carsBST);

/*
 * Count total number of cars in BST
 *  root - pointer to root of car BST
 * return total number of cars
 */
int countCarsInBST(tCar* root);

/*
 * Helper function to collect all cars from BST in-order
 *  root - pointer to root of car BST
 *  allCars - array to store car pointers
 *  count - pointer to counter
 */
void collectCarsInOrder(tCar* root, Car** allCars, int* count);

/*
 * Comparison function for sorting cars by payment amount
 *  a - first car pointer
 *  b - second car pointer
 * return comparison result for qsort
 */
int compareCarsByPayment(const void* a, const void* b);

/*
 * Helper function to find minimum node in a subtree
 *  root - pointer to root of subtree
 * return pointer to minimum node or NULL if subtree is empty
 */
tCar* findMinNode(tCar* root);

/*
 * Remove car from BST by license number
 *  root - pointer to root of BST
 *  license - license number string of car to remove
 * return pointer to root of BST after removal
 */
tCar* removeCar(tCar* root, const char* license);


#endif