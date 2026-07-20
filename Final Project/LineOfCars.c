#define _CRT_SECURE_NO_WARNINGS
#include "General.h"

int countCarQueue(const qCar* queue)
{
    if (!queue)
    {
        displayError(ERROR_NULL_POINTER);
        return -1;
    }
    int countCar = 0;
    carNode* temp = queue->front;
    while (temp != NULL)
    {
        countCar++;
        temp = temp->next;
    }
    return countCar;
}

int enqueueCarNode(qCar* queue, Car* car) {
    if (!queue || !car) {
        displayError(ERROR_NULL_POINTER);
        return 0;
    }
    carNode* newNode = (carNode*)malloc(sizeof(carNode));
    if (!newNode) {
        displayError(ERROR_MEMORY_ALLOCATION);
        return 0;
    }
    newNode->car = car;
    newNode->next = NULL;

    if (isCarQueueEmpty(queue)) {
        // Queue is empty
        queue->front = newNode;
        queue->rear = newNode;
    }
    else {
        // Queue is not empty
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
	car->inqueue = 1; // Mark the car as in queue
    return 1;
}

int isCarQueueEmpty(const qCar* queue) {
    if (!queue) return 1;
    return (queue->front == NULL);
}

Car* searchCarInQueue(const qCar* queue, const char* license)
{
    if (!queue || !license) {
        displayError(ERROR_NULL_POINTER);
        return NULL;
    }
    carNode* current = queue->front;
    while (current != NULL) {
        if (strcmp(current->car->nLicense, license) == 0) {
            return current->car; // Found the car
        }
        current = current->next;
    }
	return NULL; // Car not found in queue
}

int searchCarPositionInQueue(const qCar* queue, const char* license)
{
    if (!queue || !license) {
        displayError(ERROR_NULL_POINTER);
        return -1;
    }
    int position = 0;
    carNode* current = queue->front;
    while (current != NULL) {
        if (strcmp(current->car->nLicense, license) == 0) {
            return position; // Found the car at this position
        }
        position++;
        current = current->next;
    }
    return -1; // Car not found in queue
}

int dequeueCarNode(qCar* queue) {
    if (!queue || isCarQueueEmpty(queue)) {
        displayError(ERROR_NULL_POINTER);
        return 0; // Queue is empty
    }
    carNode* temp = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL; // Queue is now empty
    }
    free(temp);
    return 1; // Dequeue successful
}

int dequeueSpecificCarNode(qCar* queue, const char* license) {
    if (!queue || !license) {
        displayError(ERROR_NULL_POINTER);
        return 0; // Queue is empty or invalid input
    }
    carNode* current = queue->front;
    carNode* previous = NULL;
    while (current != NULL) {
        if (strcmp(current->car->nLicense, license) == 0) {
            if (previous == NULL) {
                // Deleting the front node
                queue->front = current->next;
                if (queue->front == NULL) {
                    queue->rear = NULL; // Queue is now empty
                }
            } else {
                // Deleting a middle or rear node
                previous->next = current->next;
                if (current->next == NULL) {
                    queue->rear = previous; // Update rear if last node was removed
                }
            }
            free(current);
            return 1; // Dequeue successful
        }
        previous = current;
        current = current->next;
    }
    return 0; // Car not found in queue
}

Car* searchCarInQueueByPortType(const qCar* queue, portSpeed portType)
{
    if (!queue) {
        displayError(ERROR_NULL_POINTER);
        return NULL;
    }
    carNode* current = queue->front;
    while (current != NULL) {
        if (current->car->portType == portType) {
            return current->car; // Found the car with the specified port type
        }
        current = current->next;
    }
	return NULL; // No car found with the specified port type
}

void printCarQueue(const qCar* queue)
{
    if (!queue || isCarQueueEmpty(queue)) {
        printf("  No cars waiting in queue.\n");
        return;
    }

    int position = 1;
    carNode* current = queue->front;

    while (current != NULL) {
        printf("  %d. ", position);
        if (position == 1) {
            printf("NEXT: ");
        }
        printCar(current->car);
        current = current->next;
        position++;
    }
}