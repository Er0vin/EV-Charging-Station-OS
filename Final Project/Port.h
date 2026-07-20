#pragma once
#ifndef PORT_H
#define PORT_H

#include "Date.h"
#include "Car.h"


// Port status enumeration
 
typedef enum {
    OCCUPIED = 1,
    FREE = 2,
    OUT_OF_ORDER = 3
} portStatus;

// Port structure
 
typedef struct Port {
    int num;                // Port number
    portSpeed portType;     // Charging port type (FAST, MID, SLOW)
    portStatus status;      // Port status ( 1 - occupied, 2 - free, 3 - Out of order )
    Car* car2p;            // Pointer to car being charged (NULL if not connected)
    Date tin;              // Charging start time
    struct Port* next;      // Pointer to next port in linked list
} Port;

// Function declarations for ports

/*
 * Create a new port node
 *  num - port number
 *  type - charging port type
 * return pointer to new Port or NULL if failed
 */
Port* initPort(int num, char* type, int status, Date portDate);

/*
 * Add port to linked list in a specific station
 *  head - pointer to head of ports list
 *  port - pointer to Port to add
 * return pointer to head of list
 */
Port* addPortToStation(Port* head, Port* port);

/*
 * Find available port by type
 *  head - pointer to head of ports list
 *  type - charging port type needed
 * return pointer to available Port or NULL if none found
 */
Port* findAvailablePort(Port* head, portSpeed type);

/*
 * Assign car to port
 *  port - pointer to Port
 *  car - pointer to Car
 * return 1 if successful, 0 otherwise
 */
int assignCarToPort(Port* port, Car* car);

/*
 * Count ports by status
 *  head - pointer to head of ports list
 *  status - port status to count
 * return number of ports with given status
 */
int countPortsByStatus(Port* head, portStatus status);

/*
 * Count ports by type
 *  head - pointer to head of ports list
 * param type - port type to count
 * return number of ports with given type
 */
int countPortsByType(Port* head, portSpeed type);

/*
 * Select port type for car
 * return selected port type (FAST, MID, SLOW) or -1 if user wants to return to menu
 */
portSpeed selectPortType();

/*
 * Helper function to remove a port from station's port list
 *  head - pointer to pointer to head of ports list
 *  portNumber - number of port to remove
 * return 1 if successful, 0 if failed
 */
int removePortFromStation(Port** head, int portNumber);


#endif