#ifndef STATION_H
#define STATION_H

#include "Port.h"
#include "Car.h"
#include "LineOfCars.h"

/*
 * Coordinate structure for station location
 */
typedef struct {
    double x;
    double y;
} Coord;

/*
 * Station structure - represents a charging station
 */
typedef struct Station {
    int id;                     // Station ID - unique integer
    char* name;                 // Station name - dynamic string
    int nPorts;                 // Number of ports in station
    Coord coord;                // Station coordinates
    Port* portsList;            // Head of ports linked list
    int nCars;                  // Number of cars in queue
    qCar carQueue;              // Car queue manager
    struct Station* right;      // Pointer to right child in BST
    struct Station* left;       // Pointer to left child in BST
} Station;

/**
 * Binary search tree manager for stations
 */
typedef struct {
    Station* root;              // Pointer to root of BST
} StationBST;

// Function pointer type for comparing stations
typedef int (*CompareFunc)(Station*, void*);

/*
 * Function prototypes for Station operations
 */

 /*
  * Initialize a new station
  *  id - station ID
  *  name - station name
  *  x - x coordinate
  *  y - y coordinate
  * return pointer to new Station or NULL if failed
  */

Station* initStation(int id, const char* name, int numPorts, double x, double y);

/*
 * Create station BST manager
 * return initialized StationBST structure
 */
StationBST* initStationBST();

/*
 * Insert station into BST
 *  root - pointer to root of BST
 *  station - pointer to Station to insert
 * return pointer to root of BST
 */
Station* insertStation(Station* root, Station* station);

/*
 * Search station by ID
 *  root - pointer to root of BST
 *  id - station ID to search for
 * return pointer to Station if found, NULL otherwise
 */
int compareById(Station* station, void* searchKey);
/**
 * Compare station by name
 *  station - pointer to Station
 *  searchKey - name string to compare
 * return 1 if name matches, 0 otherwise
 */
int compareByName(Station* station, void* searchKey);
/*
 * Compare station by license number
 *  station - pointer to Station
 *  searchKey - license number string to compare
 * return 1 if license matches, 0 otherwise
	 */
int compareByLicense(Station* station, void* searchKey);

/*
 * Search station in BST by ID, name or coordinates
 *  root - pointer to root of BST
 *  searchKey - key to search for (can be ID, name or coordinates)
 *  compare - comparison function pointer
 * return pointer to Station if found, NULL otherwise
 */
Station* searchStation(Station* root, void* searchKey, CompareFunc compare);

/*
 * Search nearest station by distance
 *  root - pointer to root of BST
 *  x - user x coordinate
 *  y - user y coordinate
 * return pointer to nearest Station or NULL
 */
Station* locNearSt(Station* root, double userX, double userY);


/*
 * Find the closest station in BST
 *  root - pointer to root of BST
 *  userX - user x coordinate
 *  userY - user y coordinate
 * bestStation - pointer to pointer to best station found
 * minDistance - pointer to minimum distance found
 */
void locNearInBST(Station* root, double userX, double userY, Station** bestStation, double* minDistance);


/*
 * Count total of active ports on station
 *  station - pointer to Station
 * return number of active ports
 */
int countActivePortsInStation(const Station* station);

/*
 * Recursively display all stations in order
 * root - pointer to root of BST
 */
void stationsInOrder(Station* root);

/*
* Print a single station's information
* station - pointer to Station to print
*/
void printStation(Station* station);

/*
* Print all cars currently charging at a station
* station - pointer to Station
*/
void printChargingCarsOnStation(Station* station);

/*
 * Simple universal station input function
 *  stationsBST - pointer to StationBST for validation
 * return pointer to selected Station or NULL if user wants to return to menu
 */
Station* getStationFromUser(StationBST* stationsBST);

// Helper function declarations (add to Station.h)

/*
 * Helper function to find minimum station node in a subtree
 *  root - pointer to root of subtree
 * return pointer to minimum station node or NULL if subtree is empty
 */
Station* findMinStation(Station* root);

/*
 * Remove station from BST by station ID
 *  root - pointer to root of BST
 *  stationId - ID of station to remove
 * return pointer to root of BST after removal
 */
Station* removeStation(Station* root, int stationId);


#endif