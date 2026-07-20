#define _CRT_SECURE_NO_WARNINGS
#include "General.h"

StationBST* initStationBST() {
    StationBST* bst = (StationBST*)malloc(sizeof(StationBST));
    if (!bst) {
        displayError(ERROR_MEMORY_ALLOCATION);
        return NULL;
    }
    bst->root = NULL;
    return bst;
}

Station* initStation(int id, const char* name, int numPorts, double x, double y)
{
    Station* station = (Station*)malloc(sizeof(Station));
    if (!station) {
        displayError(ERROR_MEMORY_ALLOCATION);
        return NULL;
    }
    station->name = (char*)malloc(strlen(name) + 1); //Dynamic allocation to station name
    if (!station->name) {
        displayError(ERROR_MEMORY_ALLOCATION);
        free(station);
        return NULL;
    }
    strcpy(station->name, name);
    station->id = id;
    station->coord.x = x;
    station->coord.y = y;
    station->nPorts = numPorts;
    station->left = NULL;
    station->right = NULL;
    station->portsList = NULL;
    station->nCars = 0;
    station->carQueue.front = NULL;
    station->carQueue.rear = NULL;

    return station;
}

Station* insertStation(Station* root, Station* station)
{
    if (root == NULL) return station;

    if (station->id < root->id) {
        root->left = insertStation(root->left, station);
    }
    else if (station->id > root->id) {
        root->right = insertStation(root->right, station);
    }
    return root;
}

void locateNearestStation(Station* root)
{
    if (!root) {
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }

    double userX, userY;

    printf("\n");
    printf("========================================================\n");
    printf("                 LOCATE NEAREST STATION                \n");
    printf("========================================================\n");
    printf("\n");
    printf("  Please enter your current coordinates:\n");
    printf("\n");

    while (1) {
        printf("  Latitude (X):  ");
        fseek(stdin, 0, SEEK_END);
        if (scanf(" %lf", &userX) != 1) {
            printf("  Invalid input! Please enter a valid number.\n");
            displayError(ERROR_INVALID_INPUT);
            if (returnToMenu()) return;
            continue;
        }

        printf("  Longitude (Y): ");
        fseek(stdin, 0, SEEK_END);
        if (scanf(" %lf", &userY) != 1) {
            printf("  Invalid input! Please enter a valid number.\n");
            displayError(ERROR_INVALID_INPUT);
            continue;
        }
        break;
    }

    printf("\n  Searching for nearest station...\n\n");

    Station* nearest = locNearSt(root, userX, userY);
    if (!nearest) {
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }

    double distance = calculateDistance(userX, userY, nearest->coord.x, nearest->coord.y);

    // Clean results display
    printf("--------------------------------------------------------\n");
    printf("                    NEAREST STATION                     \n");
    printf("--------------------------------------------------------\n");
    printf("\n");
    printf("  Station: %s\n", nearest->name);
    printf("  ID: %d\n", nearest->id);
    printf("  Location: (%.2f, %.2f)\n", nearest->coord.x, nearest->coord.y);
    printf("  Distance from you: %.2f km\n", distance);
    printf("\n");

    // Station details
    int activePorts = countActivePortsInStation(nearest);
    int occupiedPorts = countPortsByStatus(nearest->portsList, OCCUPIED);
    printf("  STATION DETAILS:\n");
    printf("  ----------------\n");
    printf("  Active ports:     %d\n", activePorts);
    printf("  Available ports:  %d\n", activePorts - occupiedPorts);
    printf("  Currently occupied: %d\n", occupiedPorts);
    printf("\n");
    printf("  PORT TYPES:\n");
    printf("  FAST: %d  |  MID: %d  |  SLOW: %d\n", countPortsByType(nearest->portsList, FAST),
        countPortsByType(nearest->portsList, MID),
        countPortsByType(nearest->portsList, SLOW));
    printf("\n");

    // Queue information
    if (isCarQueueEmpty(&nearest->carQueue)) {
        printf("  QUEUE STATUS: No cars waiting\n");
    }
    else {
        printf("  QUEUE STATUS: %d cars waiting\n", nearest->nCars);
    }
}

Station* locNearSt(Station* root, double userX, double userY) {
    if (!root) return NULL;

    Station* bestStation = root;
    double minDistance = calculateDistance(userX, userY, root->coord.x, root->coord.y);

    locNearInBST(root->left, userX, userY, &bestStation, &minDistance);
    locNearInBST(root->right, userX, userY, &bestStation, &minDistance);
    return bestStation;
}

void locNearInBST(Station* root, double userX, double userY, Station** bestStation, double* minDistance) {
    if (!root) return;

	double currentDistance = calculateDistance(userX, userY, root->coord.x, root->coord.y); // Calculate distance from user to current station

	if (currentDistance < *minDistance) { // Update best station if current is closer
        *bestStation = root;
        *minDistance = currentDistance;
    }

	locNearInBST(root->left, userX, userY, bestStation, minDistance); // Search left subtree
    locNearInBST(root->right, userX, userY, bestStation, minDistance); //   
}

int countActivePortsInStation(const Station* station)
{
    return station->nPorts - countPortsByStatus(station->portsList, OUT_OF_ORDER);
}

int compareById(Station* station, void* searchKey) {
    int searchId = (int)searchKey;
    return (station->id == searchId);
}

int compareByName(Station* station, void* searchKey) {
    char* name = (char*)searchKey;
    size_t len = strlen(name);

    // Trim newline only for comparison, without modifying original
    if (len > 0 && name[len - 1] == '\n') {
        name[len - 1] = '\0';  // Temporarily remove newline
		int result = (_stricmp(station->name, name) == 0);  // Compare names case-insensitively
        name[len - 1] = '\n';  // Restore the newline
        return result;
    }

	return (_stricmp(station->name, name) == 0);  // Compare names case-insensitively
}

int compareByLicense(Station* station, void* searchKey) {
    char* license = (char*)searchKey;

    // Check in ports
    for (Port* port = station->portsList; port != NULL; port = port->next) {
        if (port->car2p && strcmp(port->car2p->nLicense, license) == 0)
            return 1;
    }
    
	// Check in car queue
    if (searchCarInQueue(&station->carQueue, license) != NULL)
        return 1;

    return 0;
}

Station* searchStation(Station* root, void* searchKey, CompareFunc compare) {
    if (!root) return NULL;

    if (compare(root, searchKey)) {
        return root;
    }

    Station* result = searchStation(root->left, searchKey, compare);
    if (result) return result;

    return searchStation(root->right, searchKey, compare);
}

Station* getStationFromUser(StationBST* stationsBST) {
    if (!stationsBST || !stationsBST->root) {
        displayError(ERROR_STATION_NOT_FOUND);
        return NULL;
    }

    char stationInput[MAX_NAME_LENGTH];
    char confirm;
    Station* station = NULL;

    do {
        printf("Please enter station ID or name: ");
        fseek(stdin, 0, SEEK_END);
        fgets(stationInput, sizeof(stationInput), stdin);

        // Remove trailing newline
        size_t len = strlen(stationInput);
        if (len > 0 && stationInput[len - 1] == '\n') {
            stationInput[len - 1] = '\0';
        }

        // Confirm input
        printf("You entered: \"%s\". Is this correct? (Y/N): ", stationInput);
        fseek(stdin, 0, SEEK_END);
        scanf(" %c", &confirm);

        if (confirm == 'N' || confirm == 'n') {
            printf("Please re-enter the station information.\n");
            continue;
        }
        else if (confirm != 'Y' && confirm != 'y') {
            displayError(ERROR_INVALID_INPUT);
            if (returnToMenu()) return NULL;
            continue;
        }

        // Try to find the station (ID first, then case-insensitive name)
        int stationId;
        if (sscanf(stationInput, "%d", &stationId) == 1) {
            station = searchStation(stationsBST->root, stationId, compareById);
        }
        else {
            // Now this will be case-insensitive because compareByName uses _stricmp()
            station = searchStation(stationsBST->root, stationInput, compareByName);
        }

        if (!station) {
            printf("\nStation not found!\n");
            printf("Would you like to:\n");
            printf("1. Try again\n");
            printf("2. Return to main menu\n");
            printf("Your choice (1-2): ");

            int choice;
            fseek(stdin, 0, SEEK_END);
            if (scanf("%d", &choice) == 1) {
                switch (choice) {
                case 1:
                    continue; // Try again
                case 2:
                    return NULL; // Return to menu
                default:
                    displayError(ERROR_INVALID_INPUT);
                    continue;
                }
            }
            else {
                displayError(ERROR_INVALID_INPUT);
                if (returnToMenu()) return NULL;
                continue;
            }
        }
        else {
            // Station found - show details and return
            return station;
        }

    } while (1);
}

void dispAllSt(StationBST* stationsBST)
{
    if (!stationsBST || !stationsBST->root) {
        printf("\n");
        printf("========================================================\n");
        printf("                   DISPLAY ALL STATIONS                \n");
        printf("========================================================\n");
        printf("\n");
        printf("  No charging stations found in the system.\n");
        printf("\n");
        printf("========================================================\n");
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }

    printf("\n");
    printf("========================================================\n");
    printf("                   DISPLAY ALL STATIONS                \n");
    printf("========================================================\n");
    printf("\n");
    printf("  Complete list of all EasyCharge stations\n");
    printf("  with current operational status.\n");
    printf("\n");
    printf("--------------------------------------------------------\n");

    // Display all stations using existing function
    stationsInOrder(stationsBST->root);

    printf("\n");
    printf("========================================================\n");
    printf("                   END OF STATION LIST                 \n");
    printf("========================================================\n");
}

void printStation(Station* station)
{
    if (!station) {
        displayError(ERROR_NULL_POINTER);
        return;
    }

    int activePorts = countActivePortsInStation(station);
    int occupiedPorts = countPortsByStatus(station->portsList, OCCUPIED);
    int fastPorts = countPortsByType(station->portsList, FAST);
    int midPorts = countPortsByType(station->portsList, MID);
    int slowPorts = countPortsByType(station->portsList, SLOW);

    printf("\n");
    printf("  ----------------------------------------\n");
    printf("  STATION: %s\n", station->name);
    printf("  ----------------------------------------\n");
    printf("  ID: %d\n", station->id);
    printf("  Location: (%.2f, %.2f)\n", station->coord.x, station->coord.y);
    printf("\n");
    printf("  PORT SUMMARY:\n");
    printf("  Active ports: %d  |  Occupied: %d  |  Available: %d\n",
        activePorts, occupiedPorts, activePorts - occupiedPorts);
    printf("  FAST: %d  |  MID: %d  |  SLOW: %d\n", fastPorts, midPorts, slowPorts);
    printf("\n");
    printf("  QUEUE STATUS:\n");
    if (isCarQueueEmpty(&station->carQueue)) {
        printf("  No cars waiting\n");
    }
    else {
        printf("  Cars waiting: %d\n", station->nCars);
    }
    printf("  ----------------------------------------\n");
}

void stationsInOrder(Station* root)
{
    if (!root) return;
    stationsInOrder(root->left); // Traverse left subtree
    printStation(root);
	printf("\n"); // Print a new line for better readability
    stationsInOrder(root->right); // Traverse right subtree
}

void printChargingCarsOnStation(Station* station)
{
    if (!station || !station->portsList) {
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }

    int carNumber = 1;
    int found = 0;

    for (Port* port = station->portsList; port != NULL; port = port->next) {
        if (port->status == OCCUPIED && port->car2p) {
            printf("  %d. Port %d: ", carNumber, port->num);
            printCar(port->car2p);
            carNumber++;
            found = 1;
        }
    }

    if (!found) {
        printf("  No cars currently charging at this station.\n");
    }
}

void reportStStat(StationBST* stationsBST)
{
    if (!stationsBST || !stationsBST->root) {
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }

    printf("\n");
    printf("========================================================\n");
    printf("                STATION STATISTICS REPORT              \n");
    printf("========================================================\n");
    printf("\n");

    // Get station using existing universal function
    printf("  STATION SELECTION\n");
    printf("  -----------------\n");
    Station* station = getStationFromUser(stationsBST);
    if (!station) {
        return;
    }

    // Calculate required statistics
    int totalPorts = station->nPorts;
    int occupiedPorts = countPortsByStatus(station->portsList, OCCUPIED);
    int outOfOrderPorts = countPortsByStatus(station->portsList, OUT_OF_ORDER);
    int carsCharging = occupiedPorts;
    int carsInQueue = station->nCars;

    // Port utilization percentage
    double utilizationPercentage = totalPorts > 0 ? (double)occupiedPorts / totalPorts * 100.0 : 0.0;

    // Out of order ports percentage
    double outOfOrderPercentage = totalPorts > 0 ? (double)outOfOrderPorts / totalPorts * 100.0 : 0.0;

    // Load level calculation
    double loadRatio = 0.0;
    char* loadStatus = "";

    if (carsInQueue == 0) {
        loadStatus = "No load (empty queue)";
    }
    else if (carsCharging == 0) {
        loadStatus = "Very heavy load";
    }
    else {
        loadRatio = (double)carsCharging / carsInQueue;

        if (loadRatio > 1.0) {
            loadStatus = "No load";
        }
        else if (loadRatio >= 0.8) {
            loadStatus = "Balanced (~1)";
        }
        else if (loadRatio >= 0.2) {
            loadStatus = "Loaded";
        }
        else {
            loadStatus = "Very heavy load";
        }
    }

    printf("\n");
    printf("  STATION: %s (ID: %d)\n", station->name, station->id);
    printf("--------------------------------------------------------\n");
    printf("\n");
    printf("  Port Utilization:     %.1f%% (%d occupied / %d total)\n",
        utilizationPercentage, occupiedPorts, totalPorts);
    printf("\n");
    printf("  Out of Order Ports:   %.1f%% (%d faulty / %d total)\n",
        outOfOrderPercentage, outOfOrderPorts, totalPorts);
    printf("\n");
    printf("  Load Level:           %s", loadStatus);
    if (carsCharging > 0 && carsInQueue > 0) {
        printf(" (ratio: %.2f)", loadRatio);
    }
    printf("\n");
    printf("                        %d charging, %d waiting\n", carsCharging, carsInQueue);
    printf("\n");
    printf("========================================================\n");
}

Station* findMinStation(Station* root) {
    while (root && root->left != NULL) {
        root = root->left;
    }
    return root;
}

Station* removeStation(Station* root, int stationId) {
    if (!root) return root;

    if (stationId < root->id) {
        root->left = removeStation(root->left, stationId);
    }
    else if (stationId > root->id) {
        root->right = removeStation(root->right, stationId);
    }
    else {
        // Found the station to delete

        // No children 
        if (root->left == NULL && root->right == NULL) {
            freeStation(root);
            return NULL;
        }

        // Only right child
        else if (root->left == NULL) {
            Station* temp = root->right;
            freeStation(root);
            return temp;
        }

        // Only left child
        else if (root->right == NULL) {
            Station* temp = root->left;
            freeStation(root);
            return temp;
        }

        // Two children
        else {
            // Find successor
            Station* successor = findMinStation(root->right);

            // Free the current node's data
            free(root->name);
            freePortsList(root->portsList);
            freeCarQueue(&root->carQueue);

            // Copy successor's data to this node
            root->id = successor->id;
            root->name = successor->name;
            root->nPorts = successor->nPorts;
            root->coord = successor->coord;
            root->portsList = successor->portsList;
            root->nCars = successor->nCars;
            root->carQueue = successor->carQueue;

            // Prevent double-free by nullifying successor's pointers
            successor->name = NULL;
            successor->portsList = NULL;
            successor->carQueue.front = NULL;
            successor->carQueue.rear = NULL;

            // Delete the successor
            root->right = removeStation(root->right, successor->id);
        }
    }

    return root;
}

void closeSt(StationBST* stationsBST) {
    if (!stationsBST || !stationsBST->root) {
        displayError(ERROR_STATION_NOT_FOUND);
        return;
    }

    printf("\n");
    printf("========================================================\n");
    printf("                     CLOSE STATION                     \n");
    printf("========================================================\n");
    printf("\n");
    printf("  WARNING: This will permanently close the station and\n");
    printf("  disconnect all cars currently charging or in queue.\n");
    printf("\n");

    // Get station from user
    printf("  STATION SELECTION\n");
    printf("  -----------------\n");
    Station* station = getStationFromUser(stationsBST);
    if (!station) return;

    printf("\n");
    printf("  STATION TO CLOSE\n");
    printf("  ----------------\n");
    printf("  Station: %s (ID: %d)\n", station->name, station->id);
    printf("  Location: (%.2f, %.2f)\n", station->coord.x, station->coord.y);
    printf("  Total ports: %d\n", station->nPorts);
    printf("  Cars currently charging: %d\n", countPortsByStatus(station->portsList, OCCUPIED));
    printf("  Cars in queue: %d\n", station->nCars);
    printf("\n");

    // Confirm closure with validation loop
    char confirm;
    while (1) {
        printf("  Are you sure you want to permanently close this station? (Y/N): ");
        fseek(stdin, 0, SEEK_END);
        scanf(" %c", &confirm);

        if (confirm == 'Y' || confirm == 'y') {
            break; // User confirmed, proceed with closure
        }
        else if (confirm == 'N' || confirm == 'n') {
            printf("\n");
            printf("  CLOSURE CANCELLED\n");
            printf("  -----------------\n");
            printf("  Station closure has been cancelled.\n");
            printf("  Station %s remains operational.\n", station->name);
            return;
        }
        else {
            displayError(ERROR_INVALID_INPUT);
            if (returnToMenu()) return;
            printf("  Please enter Y for Yes or N for No.\n");
            continue;
        }
    }

    printf("\n");
    printf("  CLOSING STATION...\n");
    printf("  ------------------\n");

    // Handle cars currently charging
    int chargingCars = 0;
    double totalCollected = 0.0;

    Port* currentPort = station->portsList;
    while (currentPort != NULL) {
        if (currentPort->status == OCCUPIED && currentPort->car2p) {
            Car* car = currentPort->car2p;

            // Calculate charging cost
            Date currentTime = getCurrentDate();
            int chargingMinutes = dateDiffInMinutes(&currentPort->tin, &currentTime);
            double chargingCost = chargingMinutes * CHARGING_RATE;

            // Update car's total payment
            car->totalPayed += chargingCost;
            totalCollected += chargingCost;

            // Disconnect car from port
            car->pPort = NULL;
            car->inqueue = 0;
            currentPort->car2p = NULL;

            printf("  - Disconnected car %s (charged %d min, cost: %.2f shekels)\n",
                car->nLicense, chargingMinutes, chargingCost);

            chargingCars++;
        }
        currentPort = currentPort->next;
    }

    // Handle cars in queue
    int queuedCars = station->nCars;
    carNode* queueNode = station->carQueue.front;
    while (queueNode != NULL) {
        Car* car = queueNode->car;
        car->inqueue = 0; // Remove from queue status
        printf("  - Removed car %s from queue\n", car->nLicense);
        queueNode = queueNode->next;
    }

    // Remove station from BST
    stationsBST->root = removeStation(stationsBST->root, station->id);

    // Display closure summary
    printf("\n");
    printf("  STATION CLOSURE COMPLETE\n");
    printf("  ------------------------\n");
    printf("  Station successfully closed and removed from system.\n");
    printf("  \n");
    printf("  CLOSURE SUMMARY:\n");
    printf("  - Cars disconnected from charging: %d\n", chargingCars);
    printf("  - Cars removed from queue: %d\n", queuedCars);
}


