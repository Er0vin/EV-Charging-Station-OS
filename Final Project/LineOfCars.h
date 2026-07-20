#pragma once
#ifndef LINE_OF_CARS_H
#define LINE_OF_CARS_H

#include "Car.h"


// Queue node for cars waiting for charging
typedef struct carNode {
    Car* car;               // Pointer to Car
    struct carNode* next;   // Pointer to next car in queue
} carNode;

// Queue manager (access to the front and back of the queue)

typedef struct {
    carNode* front;         // Pointer to front of queue
    carNode* rear;          // Pointer to rear of queue
} qCar;


/*
 * Initialize car queue
 * return initialized qCar structure
 */
qCar initCarQueue();

/*
 * Check if car queue is empty
 *  queue - pointer to qCar structure
 * return 1 if empty, 0 otherwise
 */
int isCarQueueEmpty(const qCar* queue);

/*
 * Counts how many cars are in the queue
 * return the number of cars waiting in queue
 */
int countCarQueue(const qCar* queue);

/*
 * Enqueue car to queue
 *  queue - pointer to qCar structure
 *  car - pointer to Car to enqueue
 * return 1 if successful, 0 otherwise
 */
int enqueueCarNode(qCar* queue, Car* car);

/*
 * Dequeue car from queue
 *  queue - pointer to qCar structure
 * return pointer to Car if successful, NULL if queue is empty
 */
int dequeueCarNode(qCar* queue);

/*
 * Dequeue specific car from queue by license number
 *  queue - pointer to qCar structure
 *  license - license number string to search for
 * return 1 if successful, 0 if car not found or queue is empty
 */
int dequeueSpecificCarNode(qCar* queue, const char* license);

/*
 * Search for car in queue by license number
 *  queue - pointer to qCar structure
 *  license - license number string to search for
 * return pointer to Car if found, NULL otherwise
 */
Car* searchCarInQueue(const qCar* queue, const char* license);

/*
 * Search for car position in queue by license number
 *  queue - pointer to qCar structure
 *  license - license number string to search for
 * return position of car in queue (0-based index) or -1 if not found
 */
int searchCarPositionInQueue(const qCar* queue, const char* license);

/*
 * Search for car in queue by port type
 *  queue - pointer to qCar structure
 *  portType - portSpeed enum value to search for
 * return pointer to Car if found, NULL otherwise
 */
Car* searchCarInQueueByPortType(const qCar* queue, portSpeed portType);

/*
 * Print all cars in queue
 *  queue - pointer to qCar structure
 */
void printCarQueue(const qCar* queue);


#endif 