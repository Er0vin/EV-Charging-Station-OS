#define _CRT_SECURE_NO_WARNINGS
#include "General.h"


carBST* initCarBST()
{
    carBST* bst = (carBST*)malloc(sizeof(carBST));
    if (!bst) {
        displayError(ERROR_MEMORY_ALLOCATION);
        return NULL;
    }
    bst->root = NULL;
    return bst;
}

Car* initCar(const char* license, portSpeed portType, int totalPayed, int inQueue)
{
    if (!license || !validateLicense(license)) {
        return NULL;
    }

    Car* newCar = (Car*)malloc(sizeof(Car));
    if (!newCar) {
        displayError(ERROR_MEMORY_ALLOCATION);
        return NULL;
    }

    strcpy(newCar->nLicense, license);
    newCar->portType = portType;
    newCar->totalPayed = totalPayed;
    newCar->pPort = NULL;
    newCar->inqueue = inQueue;

    return newCar;
}

int validateLicense(const char* license) {
    if (!license)
    {
        displayError(ERROR_NULL_POINTER);
        return 0;
    }

    int len = strlen(license);
    if (len != 8) return 0;

    for (int i = 0; i < len; i++) {
        if (!isdigit(license[i])) {
            displayError(ERROR_INVALID_LICENSE);
            return 0;
        }
    }

    return 1;
}

tCar* createCarNode(Car* car) {
    if (!car) return NULL;

    tCar* newNode = (tCar*)malloc(sizeof(tCar));
    if (!newNode) {
        displayError(ERROR_MEMORY_ALLOCATION);
        return NULL;
    }

    newNode->car = car;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

tCar* insertCar(tCar* root, Car* car) {
    if (!car) return root;

    if (root == NULL) {
        return createCarNode(car);
    }

    int cmp = strcmp(car->nLicense, root->car->nLicense);

    if (cmp < 0) {
        root->left = insertCar(root->left, car);
    }
    else if (cmp > 0) {
        root->right = insertCar(root->right, car);
    }
    // If cmp == 0, car already exists - don't insert duplicate

    return root;
}

Car* searchCar(tCar* root, const char* license) {
    if (!root || !license) return NULL;
    int cmp = strcmp(license, root->car->nLicense);
    if (cmp < 0) {
        return searchCar(root->left, license);
    }
    else if (cmp > 0) {
        return searchCar(root->right, license);
    }
    else {
        return root->car; // Found the car
    }
}

int getLicenseFromUser(char* license) {
    if (!license) return 0;

    char confirm;

    printf("  Please enter your vehicle license number(8 digits only)\n");
    printf("\n");

    do {
        printf("  License number: ");
        fseek(stdin, 0, SEEK_END);
        scanf("%8s", license);

        // Validate license
        if (!validateLicense(license)) {
            printf("\n");
            printf("  INVALID LICENSE FORMAT\n");
            printf("  ----------------------\n");
            printf("  License numbers must be exactly 8 digits.\n");
            printf("\n");
            displayError(ERROR_INVALID_LICENSE);
            if (returnToMenu()) return 0;
            printf("  Please try again:\n");
            continue;
        }

        // Confirm license
        printf("\n");
        printf("  You entered: %s\n", license);
        printf("  Is this correct? (Y/N): ");
		fseek(stdin, 0, SEEK_END); // Clear input buffer
        scanf(" %c", &confirm);

        if (confirm == 'Y' || confirm == 'y') {
            return 1; // Success
        }
        else if (confirm == 'N' || confirm == 'n') {
            printf("\n  Please re-enter your license number:\n");
            continue;
        }
        else {
            printf("\n");
            displayError(ERROR_INVALID_INPUT);
            if (returnToMenu()) return 0;
            printf("  Please try again:\n");
            continue;
        }

    } while (1);
}

void printCar(Car* car)
{
    if (!car) {
        displayError(ERROR_NULL_POINTER);
        return;
    }

    printf("License: %s | Type: %s",
        car->nLicense,
        car->portType == FAST ? "FAST" : (car->portType == MID ? "MID" : "SLOW"));

    if (car->pPort) {
        Date startTime = car->pPort->tin;
        Date currentTime = getCurrentDate();
        int diffMinutes = dateDiffInMinutes(&startTime, &currentTime);
        printf(" | Charging time: %d minutes", diffMinutes);
    }
    printf("\n");
}

void chargeCar(StationBST* stationsBST, carBST* carsBST)
{
    char license[MAX_LICENSE_LENGTH];

    printf("\n");
    printf("========================================================\n");
    printf("                      CHARGE CAR                       \n");
    printf("========================================================\n");
    printf("\n");
    printf("  Welcome to EasyCharge! Let's get your car charging.\n");
    printf("\n");

    // Get license number
    printf("  STEP 1: Vehicle Registration\n");
    printf("  ----------------------------\n");
    if (!getLicenseFromUser(license)) {
        return; // User chose to return to menu
    }

    // Search for the car in the BST
    Car* car = searchCar(carsBST->root, license);

	// Register car if not found
    if (!car) {
        printf("\n");
        printf("  VEHICLE NOT FOUND\n");
        printf("  -----------------\n");
        printf("  Your car (%s) is not registered in our system.\n", license);
        printf("  Let's register it now to continue.\n");
        printf("\n");

        portSpeed portType = selectPortType();
        if (portType == -1) return; // User chose to return to menu

        car = initCar(license, portType, 0, 0);
        if (!car) {
            displayError(ERROR_MEMORY_ALLOCATION);
            return;
        }

        printf("\n");
        printf("  REGISTRATION SUCCESSFUL\n");
        printf("  -----------------------\n");
        printf("  Vehicle: %s\n", license);
        printf("  Port type: %s\n", portType == FAST ? "FAST" : (portType == MID ? "MID" : "SLOW"));
        printf("  Status: Ready for charging\n");
        printf("\n");

        carsBST->root = insertCar(carsBST->root, car);
    }
    else {
		// Car found - display details
        printf("\n");
        printf("  VEHICLE FOUND\n");
        printf("  -------------\n");
        printf("  Vehicle %s is registered in our system.\n", license);
        printf("  Port type: %s\n", car->portType == FAST ? "FAST" : (car->portType == MID ? "MID" : "SLOW"));
        printf("\n");

        // Check if car is already charging
        if (car->pPort != NULL) {
            // Find which station the car is at
             Station* station = searchStation(stationsBST->root, car->nLicense, compareByLicense);

            printf("  VEHICLE STATUS: Already Charging\n");
            printf("  --------------------------------\n");
            printf("  Vehicle %s is already charging!\n", license);

            if (station) {
                printf("  Current location: %s (ID: %d)\n", station->name, station->id);
                printf("  Port: %d\n", car->pPort->num);
            }
            return;
        }

        // Check if car is already in queue
        if (car->inqueue) {

            // Find which station the car is queued at
            Station* station = searchStation(stationsBST->root, car->nLicense, compareByLicense);

            printf("  VEHICLE STATUS: In Queue\n");
            printf("  ------------------------\n");
            printf("  Vehicle %s is already waiting in queue!\n", license);

            if (station) {
                int position = searchCarPositionInQueue(&station->carQueue, car->nLicense);
                printf("  Current location: %s (ID: %d)\n", station->name, station->id);
                if (position == 0) {
                    printf("  Queue position: Next in line!\n");
                }
                else {
                    printf("  Queue position: %d\n", position + 1);
                }
            }
            return;
        }

        // Car exists but is not charging or in queue
        printf("  Status: Available for charging\n");
        printf("\n");
    }

    // Get station
    printf("  STEP 2: Station Selection\n");
    printf("  -------------------------\n");
    Station* station = getStationFromUser(stationsBST);
    if (!station) return; // User chose to return to menu

    printf("\n");
    printf("  STATION SELECTED\n");
    printf("  ----------------\n");
    printf("  Station: %s (ID: %d)\n", station->name, station->id);
    printf("  Checking available ports...\n");
    printf("\n");

    // Check if the station has the right type of port available for charging
    Port* port = findAvailablePort(station->portsList, car->portType);

    if (!port) {
        // No available port - offer queue
        printf("  PORT STATUS: Not Available\n");
        printf("  ---------------------------\n");
        printf("  Unfortunately, there are no %s ports available right now.\n",
            car->portType == FAST ? "FAST" : (car->portType == MID ? "MID" : "SLOW"));
        printf("\n");

        char confirm;
        while (1) {
            printf("  Would you like to join the waiting queue? (Y/N): ");
            fseek(stdin, 0, SEEK_END);
            scanf(" %c", &confirm);

            if (confirm == 'N' || confirm == 'n') {
                printf("\n");
                printf("  CHARGING CANCELLED\n");
                printf("  ------------------\n");
                printf("  Thank you for using EasyCharge.\n");
                printf("  Please try again later or visit another station.\n");
                return;
            }
            else if (confirm == 'Y' || confirm == 'y') {
                if (enqueueCarNode(&station->carQueue, car)) {
                    station->nCars++;

                    printf("\n");
                    printf("  ADDED TO QUEUE\n");
                    printf("  --------------\n");
                    printf("  Station: %s (ID: %d)\n", station->name, station->id);
                    printf("  Your position: Added to %s charging queue\n",
                        car->portType == FAST ? "FAST" : (car->portType == MID ? "MID" : "SLOW"));
                    printf("  Status: Waiting for available port\n");
                    printf("\n");
                    printf("  You'll be automatically connected when a port becomes available\n");
                    break;
                }
                else {
                    displayError(ERROR_CANNOT_ADD_CAR_TO_CHARGE);
                    if (returnToMenu()) return;
                    continue;
                }
            }
            else {
                displayError(ERROR_INVALID_INPUT);
                if (returnToMenu()) return;
                continue;
            }
        }
    }
    else {
        // Port available - start charging
        if (assignCarToPort(port, car)) {
            station->nCars++;

            printf("  PORT AVAILABLE\n");
            printf("  --------------\n");
            printf("  Great news! A %s port is available.\n",
                car->portType == FAST ? "FAST" : (car->portType == MID ? "MID" : "SLOW"));
            printf("\n");
            printf("  CHARGING STARTED\n");
            printf("  ----------------\n");
            printf("  Vehicle: %s\n", car->nLicense);
            printf("  Station: %s (ID: %d)\n", station->name, station->id);
            printf("  Port: %d (%s charging)\n", port->num,
                car->portType == FAST ? "FAST" : (car->portType == MID ? "MID" : "SLOW"));
            printf("  Started: %02d:%02d on %02d/%02d/%04d\n",
                port->tin.hour, port->tin.min, port->tin.day, port->tin.month, port->tin.year);
            printf("  Rate: %.1f shekels per minute\n", CHARGING_RATE);

        }
        else {
            displayError(ERROR_CANNOT_ADD_CAR_TO_CHARGE);
            return;
        }
    }
}

void checkCarStatus(StationBST* stationsBST, carBST* carsBST)
{
    char license[MAX_LICENSE_LENGTH];

    printf("\n");
    printf("========================================================\n");
    printf("                   CHECK CAR STATUS                    \n");
    printf("========================================================\n");
    printf("\n");
    printf("  Check your vehicle's current charging status and\n");
    printf("  get detailed information about your session.\n");
    printf("\n");

    // Get license number
    printf("  VEHICLE IDENTIFICATION\n");
    printf("  ----------------------\n");
    if (!getLicenseFromUser(license)) {
        return;
    }
    // Check if car exists in system
    Car* car = searchCar(carsBST->root, license);
    if (!car) {
        printf("\n");
        printf("  VEHICLE NOT REGISTERED\n");
        printf("  ----------------------\n");
        printf("  Vehicle %s is not registered in our system.\n", license);
        printf("  Use menu option 2 (Charge Car) to register\n");
        printf("========================================================\n");
        return;
    }

	// Find the station where the car is located
    Station* station = searchStation(stationsBST->root, car->nLicense, compareByLicense);
    if (!station) {
        // Car exists but not at any station
        printf("\n");
        printf("  VEHICLE STATUS: Not Active\n");
        printf("  --------------------------\n");
        printf("  Vehicle: %s\n", car->nLicense);
        printf("  Port type: %s\n", car->portType == FAST ? "FAST" : (car->portType == MID ? "MID" : "SLOW"));
        printf("  Total paid to date: %.2f shekels\n", car->totalPayed);
        printf("  Current status: Not charging or queued\n");
        printf("  Use menu option 2 (Charge Car) to start charging\n");
        printf("========================================================\n");
        return;
    }

	// Car is at a station
    printf("\n");
    printf("  VEHICLE FOUND\n");
    printf("  -------------\n");
    printf("  Vehicle: %s\n", car->nLicense);
    printf("  Port type: %s\n", car->portType == FAST ? "FAST" : (car->portType == MID ? "MID" : "SLOW"));
    printf("  Station: %s (ID: %d)\n", station->name, station->id);
    printf("\n");

    if (car->pPort) {
        // Car is currently charging
        Date startTime = car->pPort->tin;
        Date currentTime = getCurrentDate();
        int chargingMinutes = dateDiffInMinutes(&startTime, &currentTime);
        double currentSessionCost = chargingMinutes * CHARGING_RATE;

        printf("  CHARGING STATUS: Active\n");
        printf("  -----------------------\n");
        printf("  Port number: %d\n", car->pPort->num);
        printf("  Charging since: %02d:%02d on %02d/%02d/%04d\n",
            startTime.hour, startTime.min, startTime.day, startTime.month, startTime.year);
        printf("  Duration: %d minutes\n", chargingMinutes);
        printf("  Current session cost: %.2f shekels\n", currentSessionCost);
        printf("  Rate: %.1f shekels per minute\n", CHARGING_RATE);
        printf("\n");
        printf("  ACCOUNT SUMMARY\n");
        printf("  ---------------\n");
        printf("  Total paid to date: %.2f shekels\n", car->totalPayed);
        printf("  Estimated total after session: %.2f shekels\n",
            car->totalPayed + currentSessionCost);

    }
    else if (car->inqueue) {
        // Car is in queue
        int queuePosition = searchCarPositionInQueue(&station->carQueue, car->nLicense);

        printf("  QUEUE STATUS: Waiting\n");
        printf("  ---------------------\n");
        printf("  Position in queue: ");

        if (queuePosition == 0) {
            printf("Next in line!\n");
            printf("  Status: You're next for %s charging\n",
                car->portType == FAST ? "FAST" : (car->portType == MID ? "MID" : "SLOW"));
            printf("  Expected wait: Very soon\n");
        }
        else {
            printf("%d\n", queuePosition + 1);
            printf("  Cars ahead of you: %d\n", queuePosition);
            printf("  Queue type: %s charging\n",
                car->portType == FAST ? "FAST" : (car->portType == MID ? "MID" : "SLOW"));
        }
    }
    else {
		// Unknown status, should not happen
        printf("  STATUS: Unknown\n");
        printf("  ---------------\n");
        printf("  There seems to be an issue with your car's status.\n");
        printf("  Please try using menu option 2 to start charging again.\n");
    }
    
}

void stopCharge(StationBST* stationsBST, carBST* carsBST)
{
    char license[MAX_LICENSE_LENGTH];

    printf("\n");
    printf("========================================================\n");
    printf("                      STOP CHARGE                      \n");
    printf("========================================================\n");
    printf("\n");
    printf("  Disconnect your vehicle from charging and finalize\n");
    printf("  your charging session with payment calculation.\n");
    printf("\n");

    // Get license number
    printf("  VEHICLE IDENTIFICATION\n");
    printf("  ----------------------\n");
    if (!getLicenseFromUser(license)) {
        return;
    }

    // Check if car exist in system
    Car* car = searchCar(carsBST->root, license);
    if (!car) {
        printf("\n");
        printf("  VEHICLE NOT REGISTERED\n");
        printf("  ----------------------\n");
        printf("  Vehicle %s is not registered in our system.\n", license);
        printf("\n");
        printf("  TO GET STARTED:\n");
        printf("  - Use menu option 2 (Charge Car) to register\n");
        printf("  - Registration is quick and free\n");
        printf("  - You can start charging immediately after registration\n");
        return;
    }

    // Find the station where the car is located
    Station* station = searchStation(stationsBST->root, car->nLicense, compareByLicense);
    if (!station) {
        printf("\n");
        printf("  VEHICLE STATUS: Not Active\n");
        printf("  --------------------------\n");
        printf("  Vehicle %s is not currently at any charging station.\n", license);
        printf("\n");
        printf("  TO START CHARGING:\n");
        printf("  - Use menu option 2 (Charge Car)\n");
        printf("  - Select your preferred station\n");
        return;
    }

    // Check if car is charging
    if (car->pPort == NULL) {
        printf("\n");
        printf("  CHARGING STATUS: Not Active\n");
        printf("  ---------------------------\n");
        printf("  Vehicle %s is not currently charging.\n", license);

        if (car->inqueue) {
            printf("  Current status: Waiting in queue at %s (ID: %d)\n",
                station->name, station->id);
            printf("\n");
            printf("  QUEUE INFORMATION:\n");
            int position = searchCarPositionInQueue(&station->carQueue, car->nLicense);
            if (position == 0) {
                printf("  - You're next in line for charging!\n");
            }
            else {
                printf("  - Position in queue: %d\n", position + 1);
            }
        }
        return;
    }

    // Display charging info
    Date startTime = car->pPort->tin;
    Date currentTime = getCurrentDate();
    int chargingMinutes = dateDiffInMinutes(&startTime, &currentTime);
    double sessionCost = chargingMinutes * CHARGING_RATE;

    printf("\n");
    printf("  CURRENT CHARGING SESSION\n");
    printf("  ------------------------\n");
    printf("  Vehicle: %s\n", car->nLicense);
    printf("  Station: %s (ID: %d)\n", station->name, station->id);
    printf("  Port: %d (%s charging)\n", car->pPort->num,
        car->portType == FAST ? "FAST" : (car->portType == MID ? "MID" : "SLOW"));
    printf("  Started: %02d:%02d on %02d/%02d/%04d\n",
        startTime.hour, startTime.min, startTime.day, startTime.month, startTime.year);
    printf("  Duration: %d minutes\n", chargingMinutes);
    printf("  Session cost: %.2f shekels\n", sessionCost);
    printf("\n");

    // Confirm stop charging with loop
    char confirm;
    while (1) {
        printf("  Are you sure you want to stop charging and finalize payment? (Y/N): ");
        fseek(stdin, 0, SEEK_END);
        scanf(" %c", &confirm);

        if (confirm == 'Y' || confirm == 'y') {
            break; // User confirmed
        }
        else if (confirm == 'N' || confirm == 'n') {
            printf("\n");
            printf("  CHARGING CONTINUES\n");
            printf("  ------------------\n");
            printf("  Your charging session will continue.\n");
            printf("  Vehicle %s remains connected to port %d.\n",
                car->nLicense, car->pPort->num);
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
    printf("  FINALIZING CHARGING SESSION...\n");
    printf("  ------------------------------\n");

    // Calculate final charging time and cost
    currentTime = getCurrentDate();
    chargingMinutes = dateDiffInMinutes(&car->pPort->tin, &currentTime);
    double chargingCost = chargingMinutes * CHARGING_RATE;

    // Update car total payment
    car->totalPayed += chargingCost;

    // Store port info before disconnecting
    Port* port = car->pPort;
    int portNumber = port->num;

    // Disconnect car from port
    port->car2p = NULL;
    port->status = FREE;
    car->pPort = NULL;
    car->inqueue = 0;

    printf("\n");
    printf("  CHARGING SESSION COMPLETED\n");
    printf("  --------------------------\n");
    printf("  Vehicle: %s\n", car->nLicense);
    printf("  Total charging time: %d minutes\n", chargingMinutes);
    printf("  Session cost: %.2f shekels\n", chargingCost);
    printf("  Total account balance: %.2f shekels\n", car->totalPayed);
    printf("  Successfully disconnected from port %d\n", portNumber);
    printf("\n");

    // Check if there are cars waiting in queue and assign next car
    if (!isCarQueueEmpty(&station->carQueue)) {
        Car* nextCar = searchCarInQueueByPortType(&station->carQueue, port->portType);
        if (nextCar) {
            if (assignCarToPort(port, nextCar)) {
                if (dequeueSpecificCarNode(&station->carQueue, nextCar->nLicense)) {
                    station->nCars--; // Decrement queue count

                    printf("  QUEUE UPDATE\n");
                    printf("  ------------\n");
                    printf("  Next vehicle in queue has been connected:\n");
                    printf("  - Vehicle: %s\n", nextCar->nLicense);
                    printf("  - Port: %d (%s charging)\n", port->num,
                        nextCar->portType == FAST ? "FAST" :
                        (nextCar->portType == MID ? "MID" : "SLOW"));
                    printf("  - Charging started automatically\n");
                }
            }
        }
    }
    else {
        printf("  PORT STATUS\n");
        printf("  -----------\n");
        printf("  Port %d is now available for new customers.\n", portNumber);
    }

    printf("\n");
    printf("  THANK YOU FOR USING EASYCHARGE!\n");
    printf("========================================================\n");
}

void dispCarsAtSt(StationBST* stationsBST)
{
    printf("\n");
    printf("========================================================\n");
    printf("                  DISPLAY CARS AT STATION              \n");
    printf("========================================================\n");
    printf("\n");

	// Get station from user
    printf("  STATION SELECTION\n");
    printf("  -----------------\n");
    Station* station = getStationFromUser(stationsBST);
    if (!station) return;

    printf("\n");
    printf("========================================================\n");
    printf("                    STATION OVERVIEW                   \n");
    printf("========================================================\n");
    printf("\n");
    printf("  STATION: %s (ID: %d)\n", station->name, station->id);
    printf("  Location: (%.2f, %.2f)\n", station->coord.x, station->coord.y);
    printf("  Active ports: %d  |  Occupied: %d\n",
        countActivePortsInStation(station),
        countPortsByStatus(station->portsList, OCCUPIED));
    printf("  Total cars at station: %d\n",
        countPortsByStatus(station->portsList, OCCUPIED) + station->nCars);
    printf("\n");
    printf("--------------------------------------------------------\n");

    // Display charging cars
    printf("\n");
    printf("  CURRENTLY CHARGING VEHICLES\n");
    printf("  ---------------------------\n");
    int chargingCount = countPortsByStatus(station->portsList, OCCUPIED);
    if (chargingCount == 0) {
        printf("  No vehicles currently charging.\n");
    }
    else {
        printf("  %d vehicle(s) currently charging:\n", chargingCount);
        printf("\n");
        printChargingCarsOnStation(station);
    }

    printf("\n");
    printf("--------------------------------------------------------\n");

    // Display queued cars
    printf("\n");
    printf("  VEHICLES WAITING IN QUEUE\n");
    printf("  -------------------------\n");
    if (isCarQueueEmpty(&station->carQueue)) {
        printf("  No vehicles waiting in queue.\n");
    }
    else {
        printf("  %d vehicle(s) waiting in queue:\n", station->nCars);
        printf("\n");
        printCarQueue(&station->carQueue);
    }

    printf("\n");
    printf("========================================================\n");
    printf("                      END OF REPORT                    \n");
    printf("========================================================\n");
}

void dispTopCustomers(carBST* carsBST) {
    if (!carsBST || !carsBST->root) {
        displayError(ERROR_NULL_POINTER);
        return;
    }

	// Count total cars in the BST
    int totalCars = countCarsInBST(carsBST->root);

    if (totalCars == 0) {
        printf("\nNo customers found in the system.\n");
        return;
    }

    // Allocate array based on number of cars
    Car** allCars = (Car**)malloc(totalCars * sizeof(Car*));
    if (!allCars) {
        displayError(ERROR_MEMORY_ALLOCATION);
        return;
    }

    // Collect all cars from BST
    int carCount = 0;
    collectCarsInOrder(carsBST->root, allCars, &carCount);

	// Sort cars by totalPayed from highest to lowest
    qsort(allCars, carCount, sizeof(Car*), compareCarsByPayment);

    // Display results
    printf("\n");
    printf("========================================================\n");
    printf("                    TOP 5 CUSTOMERS                    \n");
    printf("========================================================\n");
    printf("\n");

    for (int i = 0; i < TOP_CUSTOMERS_COUNT; i++) {
        printf("  %d. License: %s\n", i + 1, allCars[i]->nLicense);
        printf("     Total Paid: %.2f shekels\n", allCars[i]->totalPayed);
		printf("\n");
    }

	// Free allocated memory
    free(allCars);
}

int countCarsInBST(tCar* root) {
    if (!root) return 0;

	// Count current car and recursively count left and right subtrees
    return 1 + countCarsInBST(root->left) + countCarsInBST(root->right);
}

void collectCarsInOrder(tCar* root, Car** allCars, int* count) {
    if (!root || !allCars || !count) return;

    collectCarsInOrder(root->left, allCars, count);

    // Add current car to array
    allCars[*count] = root->car;
    (*count)++;

    collectCarsInOrder(root->right, allCars, count);
}

int compareCarsByPayment(const void* a, const void* b) {
    Car* carA = *(Car**)a;
    Car* carB = *(Car**)b;

	// Sort in descending order by totalPayed
    if (carB->totalPayed > carA->totalPayed) return 1;
    if (carB->totalPayed < carA->totalPayed) return -1;
    return 0; // Equal payments
}

tCar* findMinNode(tCar* root) {
    while (root && root->left != NULL) {
        root = root->left;
    }
    return root;
}

tCar* removeCar(tCar* root, const char* license) {
    if (!root) return root;

    if (strcmp(license, root->car->nLicense) < 0) {
        // License is in left subtree
        root->left = removeCar(root->left, license);
    }
    else if (strcmp(license, root->car->nLicense) > 0) {
        // License is in right subtree
        root->right = removeCar(root->right, license);
    }
    else {
        // Found the node to delete

        // Node has no children
        if (root->left == NULL && root->right == NULL) {
            free(root->car);  // Free the car data
            free(root);       // Free the node
            return NULL;
        }

        // Node has only right child
        else if (root->left == NULL) {
            tCar* temp = root->right;
            free(root->car);
            free(root);
            return temp;
        }

        // Node has only left child
        else if (root->right == NULL) {
            tCar* temp = root->left;
            free(root->car);
            free(root);
            return temp;
        }

        // Node has two children
        else {
            // Find smallest in right subtree
            tCar* successor = findMinNode(root->right);

            // Free the old car first
            free(root->car);
            root->car = successor->car;

            // Delete the successor
            successor->car = NULL;
            root->right = removeCar(root->right, root->car->nLicense);
        }
    }

    return root;
}

void remCustomer(carBST* carsBST) {
    if (!carsBST || !carsBST->root) {
        displayError(ERROR_NULL_POINTER);
        return 0;
    }

    char license[MAX_LICENSE_LENGTH];

    printf("\n");
    printf("========================================================\n");
    printf("                    REMOVE CUSTOMER                    \n");
    printf("========================================================\n");
    printf("\n");

    // Get license number
    if (!getLicenseFromUser(license)) {
        return 0;
    }

    // Find the car
    Car* car = searchCar(carsBST->root, license);
    if (!car) {
        printf("\n");
        printf("  CUSTOMER NOT FOUND\n");
        printf("  ------------------\n");
        printf("  Vehicle %s is not registered in our system.\n", license);
        return 0;
    }

    // Check if car is currently charging or in queue
    if (car->pPort != NULL) {
        printf("\n");
        printf("  REMOVAL NOT ALLOWED\n");
        printf("  -------------------\n");
        printf("  Vehicle %s is currently charging.\n", license);
        printf("  Please stop charging first before removing customer.\n");
        return 0;
    }

    if (car->inqueue) {
        printf("\n");
        printf("  REMOVAL NOT ALLOWED\n");
        printf("  -------------------\n");
        printf("  Vehicle %s is currently in queue for charging.\n", license);
        printf("  Please remove from queue first before removing customer.\n");
        return 0;
    }

    // Confirm removal
    char confirm;
    printf("\n");
    printf("  CUSTOMER DETAILS\n");
    printf("  ----------------\n");
    printf("  License: %s\n", car->nLicense);
    printf("  Port type: %s\n", car->portType == FAST ? "FAST" :
        (car->portType == MID ? "MID" : "SLOW"));
    printf("  Total paid: %.2f shekels\n", car->totalPayed);
    printf("\n");

    while (1) {
        printf("  Are you sure you want to remove this customer? (Y/N): ");
        fseek(stdin, 0, SEEK_END);
        scanf(" %c", &confirm);

        if (confirm == 'Y' || confirm == 'y') {
            break; // User confirmed, proceed with deletion
        }
        else if (confirm == 'N' || confirm == 'n') {
            printf("\n");
            printf("  REMOVAL CANCELLED\n");
            printf("  -----------------\n");
            printf("  Customer removal has been cancelled.\n");
            return;
        }
        else {
            displayError(ERROR_INVALID_INPUT);
            if (returnToMenu()) return; // If user wants to return to menu
            printf("  Please enter Y for Yes or N for No.\n");
            continue; // Ask again
        }
    }

    // Remove the car from BST
    carsBST->root = removeCar(carsBST->root, license);

    printf("\n");
    printf("  REMOVAL SUCCESSFUL\n");
    printf("  ------------------\n");
    printf("  Customer %s has been successfully removed from the system.\n", license);
    printf("  All associated data has been deleted.\n");
}
