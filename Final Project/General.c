#define _CRT_SECURE_NO_WARNINGS
#include "General.h"

double toRadians(double degree) {
    return degree * M_PI / 180.0;
}

double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = toRadians(lat2 - lat1);
    double dLon = toRadians(lon2 - lon1);

    lat1 = toRadians(lat1);
    lat2 = toRadians(lat2);

    double a = sin(dLat / 2) * sin(dLat / 2) +
        cos(lat1) * cos(lat2) *
        sin(dLon / 2) * sin(dLon / 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS_KM * c;
}

void displayMenu()
{
    printf("\n");
    printf("========================================================\n");
    printf("                    EASYCHARGE MENU                    \n");
    printf("========================================================\n");
    printf("                                                        \n");
    printf("  Please select a service:                             \n");
    printf("                                                        \n");
    printf("  CUSTOMER SERVICES:                                   \n");
    printf("  1. Locate nearest station                            \n");
    printf("  2. Charge car                                        \n");
    printf("  3. Check car status                                  \n");
    printf("  4. Stop charge                                       \n");
    printf("                                                        \n");
    printf("  STATION INFORMATION:                                 \n");
    printf("  5. Display all stations                              \n");
    printf("  6. Display cars at station                           \n");
    printf("  7. Report of station's statistics                    \n");
    printf("                                                        \n");
    printf("  MANAGEMENT TOOLS:                                    \n");
    printf("  8. Display top customers                             \n");
    printf("  9. Add new port                                      \n");
    printf("  10. Release charging ports                           \n");
    printf("  11. Remove out of order ports                        \n");
    printf("  12. Remove customer                                  \n");
    printf("  13. Close station                                    \n");
    printf("                                                        \n");
    printf("  SYSTEM:                                              \n");
    printf("  0. Exit system                                       \n");
    printf("                                                        \n");
    printf("========================================================\n");
    printf("                                                        \n");
    printf("  Enter your choice (0-13): ");
}

int returnToMenu()
{
    char choice = 0;
    printf("\nDo you wish to go back to the menu? (Y/N):");
    while (choice != 'Y' && choice != 'N' && choice != 'y' && choice != 'n')
    {
    fseek(stdin, 0, SEEK_END);
    scanf("%c", &choice);
	if (choice == 'Y' || choice == 'y') return 1; // If user chooses 'Y' or 'y', return to menu
	if (choice == 'N' || choice == 'n') return 0; // If user chooses 'N' or 'n', exit the program
    else
    {
        displayError(ERROR_INVALID_INPUT);
		printf("Please enter 'Y' to return to the menu or 'N' to exit: ");
    }
    }
    
}


int loadFiles(StationBST* stationsBST, carBST* carsBST) {
    if (!stationsBST || !carsBST) {
        displayError(ERROR_NULL_POINTER);
        return 0;
    }

    int status = 1;

    printf("Loading data files...\n");
    

	if (!loadStationsFromFile(STATIONS_FILE, stationsBST)) { // Loads stations from file
        displayError(ERROR_LOADING_STATIONS);
        status = 0;
    }

	if (!loadCarsFromFile(CARS_FILE, carsBST)) { // Loads cars from file
        displayError(ERROR_LOADING_CARS);
        status = 0;
    }

	if (!loadPortsFromFile(PORTS_FILE, stationsBST, carsBST)) { // Loads ports from file
        displayError(ERROR_LOADING_PORTS);
        status = 0;
    }

	if (!loadLineOfCarsFromFile(LINEOFCARS_FILE, stationsBST, carsBST)) { // Loads line of cars from file
        displayError(ERROR_LOADING_LINE);
        status = 0;
    }

    if (status) {
		printf("All files loaded successfully!\n"); // If all files loaded successfully, print success message
    }

    return status;
}

int loadStationsFromFile(const char* filename, StationBST* stationsBST) {
    FILE* file = fopen(filename, "r"); //Opens file in read mode
    if (!file) return 0;

    char line[256];
    fgets(line, sizeof(line), file);//Skips the first line

    while (fgets(line, sizeof(line), file)) {

        line[strcspn(line, "\n")] = '\0';

        int id, numPorts;
        char name[MAX_NAME_LENGTH];
        double x, y;

        if (sscanf(line, "%d,%99[^,],%d,%lf,%lf", &id, name, &numPorts, &x, &y) == 5) {
			Station* station = initStation(id, name, numPorts, x, y); // Initializes a new station

            if (station) {
				stationsBST->root = insertStation(stationsBST->root, station); // Inserts the station into the BST
            }
        }
        else displayError(ERROR_FILE_READ_FAILED);
    }

    fclose(file);
    return 1;
}

int loadCarsFromFile(const char* filename, carBST* carsBST) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;

    char line[256];
	fgets(line, sizeof(line), file);// Skips the first line

    while (fgets(line, sizeof(line), file)) {

        line[strcspn(line, "\n")] = '\0';

        char license[MAX_LICENSE_LENGTH];
        char portTypeStr[10];
        double totalPayed;
        int stationId, portNumber, inQueue;

        if (sscanf(line, "%9[^,],%9[^,],%lf,%d,%d,%d",
            license, portTypeStr, &totalPayed, &stationId, &portNumber, &inQueue) == 6) {

            portSpeed type;
            if (strcmp(portTypeStr, "FAST") == 0) {
                type = FAST;
            }
            else if (strcmp(portTypeStr, "MID") == 0) {
                type = MID;
            }
            else {
                type = SLOW;
            }

			Car* car = initCar(license, type, totalPayed, inQueue); // Initializes a new car
            if (car) {
				carsBST->root = insertCar(carsBST->root, car); // Inserts the car into the BST
            }
        }
    }

    fclose(file);
    return 1;
}

int loadPortsFromFile(const char* filename, StationBST* stationsBST, carBST* carsBST) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return 0;
    }

    char line[256];
    fgets(line, sizeof(line), file);// Skips the first line

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        int stationId, portNum, status;
        int year, month, day, hour, min;
        char portTypeStr[10];
        char carLicense[MAX_LICENSE_LENGTH];

        if (sscanf(line, "%d,%d,%9[^,],%d,%d,%d,%d,%d,%d,%9s",
            &stationId, &portNum, portTypeStr, &status,
            &year, &month, &day, &hour, &min, carLicense) == 10) {

			Date portDate = initDate(year, month, day, hour, min); // Initializes a new date

			Port* newPort = initPort(portNum, portTypeStr, status, portDate); // Initializes a new port

			Station* station = searchStation(stationsBST->root, stationId, compareById); // Searches for the station by ID

            if (!station)
            {
                displayError(ERROR_STATION_NOT_FOUND);
                return 0;
            }

			station->portsList = addPortToStation(station->portsList, newPort); // Adds the port to the station's port list

			if (status == OCCUPIED) { // If the port is occupied, find the car and link it
                Car* ptr2Car = searchCar(carsBST->root, carLicense);
                if (ptr2Car) {
                    newPort->car2p = ptr2Car;
                    ptr2Car->pPort = newPort;
                }
            }
        }
    }
    fclose(file);
    return 1;
}



int loadLineOfCarsFromFile(const char* filename, StationBST* stationsBST, carBST* carsBST) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return 0;
    }

    char line[256];
    fgets(line, sizeof(line), file);// Skips the first line

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        char carLicense[MAX_LICENSE_LENGTH];
        int stationId;

        if (sscanf(line, "%9[^,],%d", carLicense, &stationId) == 2) {

			Station* station = searchStation(stationsBST->root, stationId, compareById); // Searches for the station by ID
            if (station) {

				Car* car = searchCar(carsBST->root, carLicense); // Searches for the car by license number
                if (car) {
					if (enqueueCarNode(&station->carQueue, car))station->nCars++; // Enqueues the car to the station's queue and increments the car count
                    else {
                        displayError(ERROR_CANNOT_ADD_CAR_TO_CHARGE);
                        return 0; // If enqueue fails, return error
					}
                }
            }
        }
    }

    fclose(file);
    return 1;
}

// Function to update all files
int updateFiles(StationBST* stationsBST, carBST* carsBST)
{
    if (!stationsBST || !carsBST) {
        displayError(ERROR_NULL_POINTER);
        return 0;
    }

    int success = 1;

    if (!updateStationsFile(stationsBST->root)) {
        displayError(ERROR_UPDATING_STATIONS);
        success = 0;
    }

    if (!updateCarsFile(carsBST->root, stationsBST->root)) {
        displayError(ERROR_UPDATING_CARS);
        success = 0;
    }

    if (!updatePortsFile(stationsBST->root)) {
        displayError(ERROR_UPDATING_PORTS);
        success = 0;
    }

    if (!updateLineOfCarsFile(stationsBST->root)) {
        displayError(ERROR_UPDATING_LINE);
        success = 0;
    }

    if (!success) {
        displayError(ERROR_FILE_UPDATE_FAILED);
        return 0;
    }
    return 1;
}

// Function to update stations.txt
int updateStationsFile(Station* root)
{
    FILE* file = fopen(STATIONS_FILE, "w");
    if (!file) {
        displayError(ERROR_FILE_WRITE_FAILED);
        return 0;
    }

    // Write header
    fprintf(file, "ID,StationName,NumOfPorts,CoordX,CoordY\n");

    // Write stations in order
    writeStationsInOrder(file, root);

    fclose(file);
    return 1;
}

// Recursive helper function to write stations in order
void writeStationsInOrder(FILE* file, Station* root)
{
    if (!root) return;

    writeStationsInOrder(file, root->left);

    fprintf(file, "%d,%s,%d,%.2f,%.2f\n",
        root->id, root->name, root->nPorts,
        root->coord.x, root->coord.y);

    writeStationsInOrder(file, root->right);
}

// Function to update Cars.txt
static int updateCarsFile(tCar* root, Station* stationsRoot)
{
    FILE* file = fopen(CARS_FILE, "w");
    if (!file) {
        displayError(ERROR_FILE_WRITE_FAILED);
        return 0;
    }

    // Write header
    fprintf(file, "License,PortType,TotalPayed,StationID,PortNumber,InQueue\n");

    // Write cars data in order
    writeCarsInOrder(file, root, stationsRoot);

    fclose(file);
    return 1;
}

// Recursive helper function to write cars in order
void writeCarsInOrder(FILE* file, tCar* root, Station* stationsRoot)
{
    if (!root) return;

    writeCarsInOrder(file, root->left, stationsRoot);

    Car* car = root->car;

    // Determine station ID and port number
    int stationId = 0;
    int portNumber = 0;

	// If car is connected to a port or in queue, get the station ID and port number
    if (car->pPort || car->inqueue) {
        Station* station = searchStation(stationsRoot, car->nLicense, compareByLicense);
        if (!station) displayError(ERROR_STATION_NOT_FOUND);
        stationId = station->id;
        if(car->pPort) portNumber = car->pPort->num; // If car is connected to a port, get the port number
    }

    fprintf(file, "%s,%s,%.2f,%d,%d,%d\n",
        car->nLicense,
        car->portType == FAST ? "FAST" : (car->portType == MID ? "MID" : "SLOW"),
        car->totalPayed,
        stationId,
        portNumber,
        car->inqueue);

    writeCarsInOrder(file, root->right, stationsRoot);
}

// Function to update Ports.txt
int updatePortsFile(Station* root)
{
    FILE* file = fopen(PORTS_FILE, "w");
    if (!file) {
        displayError(ERROR_FILE_WRITE_FAILED);
        return 0;
    }

    // Write header
    fprintf(file, "StationID,PortNumber,PortType,Status,Year,Month,Day,Hour,Min,CarLicense\n");

    // Write ports data for all stations
    writePortsForAllStations(file, root);

    fclose(file);
    return 1;
}

// Recursive helper function to write ports for all stations
void writePortsForAllStations(FILE* file, Station* root)
{
    if (!root) return;

    writePortsForAllStations(file, root->left);

    // Write all ports for current station
    Port* currentPort = root->portsList;
    while (currentPort) {
        char* portTypeStr = currentPort->portType == FAST ? "FAST" :
            (currentPort->portType == MID ? "MID" : "SLOW");

        char carLicense[MAX_LICENSE_LENGTH] = "-1"; // Empty port
        Date portTime = currentPort->tin;

        if (currentPort->status == OCCUPIED && currentPort->car2p) {
            strcpy(carLicense, currentPort->car2p->nLicense);
        }
        else {
            // For free/out of order ports, use default time
            portTime = initDate(0, 0, 0, 0, 0);
        }

        fprintf(file, "%d,%d,%s,%d,%d,%d,%d,%d,%d,%s\n",
            root->id,
            currentPort->num,
            portTypeStr,
            currentPort->status,
            portTime.year,
            portTime.month,
            portTime.day,
            portTime.hour,
            portTime.min,
            carLicense);

        currentPort = currentPort->next;
    }

    writePortsForAllStations(file, root->right);
}

// Function to update LineOfCars.txt
int updateLineOfCarsFile(Station* root)
{
    FILE* file = fopen(LINEOFCARS_FILE, "w");
    if (!file) {
        displayError(ERROR_FILE_WRITE_FAILED);
        return 0;
    }

    // Write header
    fprintf(file, "License,StationID\n");

    // Write queue data for all stations
    writeQueuesForAllStations(file, root);

    fclose(file);
    return 1;
}

// Recursive helper function to write queues for all stations
void writeQueuesForAllStations(FILE* file, Station* root)
{
    if (!root) return;

    writeQueuesForAllStations(file, root->left);

    // Write all cars in queue for current station
    carNode* currentCar = root->carQueue.front;
    while (currentCar) {
        fprintf(file, "%s,%d\n", currentCar->car->nLicense, root->id);
        currentCar = currentCar->next;
    }

    writeQueuesForAllStations(file, root->right);
}

// Main function to free all system memory
void freeAllMemory(StationBST* stationsBST, carBST* carsBST)
{
    if (stationsBST) {
        freeStationsBST(stationsBST->root);
        free(stationsBST);
    }

    if (carsBST) {
        freeCarsBST(carsBST->root);
        free(carsBST);
    }

}

// Free all stations in BST
void freeStationsBST(Station* root)
{
    if (!root) return;

	// Post order to free children first
    freeStationsBST(root->left);
    freeStationsBST(root->right);

    // Free this station
    freeStation(root);
}

// Free a single station and all its components
void freeStation(Station* station)
{
    if (!station) return;

    // Free station's name
    free(station->name);

    // Free all ports in the linked list
    freePortsList(station->portsList);

    // Free car queue
    freeCarQueue(&station->carQueue);

    // Free the station itself
    free(station);
}

// Free all ports in linked list
void freePortsList(Port* head)
{
    Port* current = head;
    while (current) {
        Port* next = current->next;
        free(current);
        current = next;
    }
}

// Free car queue
void freeCarQueue(qCar* queue)
{
    if (!queue) return;
	// Free all car nodes in the queue
    carNode* current = queue->front;
    while (current) {
        carNode* next = current->next;
        free(current);
        current = next;
    }

    // Reset queue pointers
    queue->front = NULL;
    queue->rear = NULL;
}

// Free all cars in BST
void freeCarsBST(tCar* root)
{
    if (!root) return;

    // Post order to free children first
    freeCarsBST(root->left);
    freeCarsBST(root->right);

    // Free the car and the BST node
    if (root->car) {
        free(root->car);
    }
    free(root);
}

void exitSys(StationBST* stationsBST, carBST* carsBST)
{
    printf("\n");
    printf("========================================================\n");
    printf("                    SYSTEM SHUTDOWN                    \n");
    printf("========================================================\n");
    printf("Preparing to close EasyCharge system...\n\n");

    // Save all current data to files
    printf("Step 1: Saving all data to files...\n");
    if (updateFiles(stationsBST, carsBST)) {
        printf("Data successfully saved to files.\n");
    }
    else {
        printf("Warning: Some data may not have been saved properly.\n");
        printf("  Please check file permissions and disk space.\n");
    }
    printf("\n");

    // Free all allocated memory
    printf("Step 2: Cleaning up memory...\n");
    freeAllMemory(stationsBST, carsBST);
    printf("All memory has been freed.\n");
    printf("\n");

    // Shutdown message
    printf("========================================================\n");
    printf("     Thank you for using EasyCharge services!         \n");
    printf("              System closed successfully.              \n");
    printf("                   See you next time!                 \n");
    printf("========================================================\n");
}