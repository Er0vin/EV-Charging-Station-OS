#pragma once
#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>


#include "Date.h"
#include "Car.h"
#include "Port.h"
#include "Station.h"
#include "ErrorHandler.h"
#include "LineOfCars.h"

/**
 * File names for data storage
 */
#define STATIONS_FILE "Stations.txt"
#define CARS_FILE "Cars.txt"
#define PORTS_FILE "Ports.txt"
#define LINEOFCARS_FILE "LineOfCars.txt"

 /**
  * System constants
  */
#define M_PI 3.14159265358979323846
#define EARTH_RADIUS_KM 6371.0
#define MAX_NAME_LENGTH 100
#define MAX_LICENSE_LENGTH 9 // 8 digits license plate
#define CHARGING_RATE 1.2  // 1.2 shekels per minute
#define MAX_CHARGING_HOURS 10 // Maximum charging time in hours
#define TOP_CUSTOMERS_COUNT 5

  // Loading files declarations
int loadStationsFromFile(const char* filename, StationBST* stationBST);
int loadCarsFromFile(const char* filename, carBST* carBST);
int loadPortsFromFile(const char* filename, StationBST* stationBST, carBST* carBST);
int loadLineOfCarsFromFile(const char* filename, StationBST* stationBST, carBST* carBST);


int loadFiles(StationBST* stationsBST, carBST* carsBST);

// Updating files declarations
int updateStationsFile(Station* root);
int updateCarsFile(tCar* root);
int updatePortsFile(Station* root);
int updateLineOfCarsFile(Station* root);


// Helper functions 
int updateStationsFile(Station* root);
int updateCarsFile(tCar* root, Station* stationsRoot);
int updatePortsFile(Station* root);
int updateLineOfCarsFile(Station* root);
void writeStationsInOrder(FILE* file, Station* root);
void writeCarsInOrder(FILE* file, tCar* root, Station* stationsRoot);
void writePortsForAllStations(FILE* file, Station* root);
void writeQueuesForAllStations(FILE* file, Station* root);

int updateFiles(StationBST* stationsBST, carBST* carsBST);


void freeAllMemory(StationBST* stationsBST, carBST* carsBST);
int freeAllMemoryWithCheck(StationBST* stationsBST, carBST* carsBST);

// Static helper functions
void freeStationsBST(Station* root);
void freeStation(Station* station);
void freePortsList(Port* head);
void freeCarQueue(qCar* queue);
void freeCarsBST(tCar* root);

/*
 * Calculate distance between two points
 *  x1 - first point x coordinate
 *  y1 - first point y coordinate
 *  x2 - second point x coordinate
 *  y2 - second point y coordinate
 * return distance between points in kilometers
 */
double calculateDistance(double lat1, double lon1, double lat2, double lon2);

/*
* Converts degrees to radians
*  degree - angle in degrees
*  return angle in radians
*/
double toRadians(double degree);

// Menu functions declarations

/*
 * Prints the main menu options
 */
void displayMenu();

/*
 * Asks the user if they want to return to the main menu
 * prints message and waits for user input
 * returns 1 if user wants to return, 0 otherwise
 */
int returnToMenu();

/*
 * Find the closest station to user
 *  station - pointer to Station
 */
void locateNearestStation(Station* root);

/*
 * Charge car at station
 * stationsBST - pointer to StationBST
 * carsBST - pointer to carBST
 */
void chargeCar(StationBST* stationsBST, carBST* carsBST);

/*
 * Check car status at station
 *  stationsBST - pointer to StationBST
 *  carsBST - pointer to carBST
 */
void checkCarStatus(StationBST* stationsBST, carBST* carsBST);

/*
 * Stop charging car at station
 *  stationsBST - pointer to StationBST
 *  carsBST - pointer to carBST
 */
void stopCharge(StationBST* stationsBST, carBST* carsBST);

/*
 * Display all stations in the system
 *  stationsBST - pointer to StationBST
 */
void dispAllSt(StationBST* stationsBST);

/*
 * Display cars at a specific station
 *  station - pointer to Station
 */
void dispCarsAtSt(StationBST* stationsBST);

/*
 * Report statistics of all stations
 *  stationsBST - pointer to StationBST
 */
void reportStStat(StationBST* stationsBST);

/*
 * Display top customers based on total amount paid
 *  carsBST - pointer to carBST
 */
void dispTopCustomers(carBST* carsBST);

/*
 * Add new charging port to a station
 *  stationsBST - pointer to StationBST
 */
void addNewPort(StationBST* stationsBST);

/*
 * Release all ports on station that charging above MAX_CHARGING_HOURS
 *  stationsBST - pointer to StationBST
 */

void releasePorts(StationBST* stationsBST);

/*
 * Remove out of order ports from a station
 *  stationsBST - pointer to StationBST
 */
void remOutOrderPort(StationBST* stationsBST);

/*
 * Remove customer from system
 *  carsBST - pointer to carBST
 */
void remCustomer(carBST* carsBST);

/*
 * Close station permanently
 *  stationsBST - pointer to StationBST
 */
void closeSt(StationBST* stationsBST);

/*
 * exit the system update all files and free all memory
 * stationsBST - pointer to StationBST
 * carsBST - pointer to carBST
 */

void exitSys(StationBST* stationsBST, carBST* carsBST);

#endif 