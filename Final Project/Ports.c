#define _CRT_SECURE_NO_WARNINGS
#include "General.h"

Port* initPort(int num, char* type, int status, Date portDate)
{
	Port* newPort = (Port*)malloc(sizeof(Port));
	if (!newPort) {
		displayError(ERROR_MEMORY_ALLOCATION);
		return NULL;
	}
	if (strcmp(type, "FAST") == 0) {
		newPort->portType = FAST;
	}
	else if (strcmp(type, "MID") == 0) {
		newPort->portType = MID;
	}
	else {
		newPort->portType = SLOW;
	}
	newPort->num = num;
	newPort->status = status;
	newPort->tin = portDate;
	newPort->car2p = NULL;
	newPort->next = NULL;

	return newPort;
}

Port* addPortToStation(Port* head, Port* port)
{
	if (head == NULL) return port;

	Port* temp = head;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	temp->next = port;
	port->next = NULL;
	return head;
}

int assignCarToPort(Port* port, Car* car)
{
	if (!port || !car)
	{
		displayError(ERROR_NULL_POINTER);
		return 0;
	}
	if (port->status != FREE)
	{
		displayError(ERROR_PORT_NOT_AVAILABLE);
		return 0;
	}
	port->car2p = car;
	port->status = OCCUPIED;
	car->pPort = port;
	port->tin = getCurrentDate(); // Set the current date as the start time
	return 1;
}

Port* findAvailablePort(Port* head, portSpeed type)
{
	if (!head)
	{
		displayError(ERROR_NULL_POINTER);
		return NULL;
	}
	Port* temp = head;
	while (temp != NULL)
	{
		if (temp->portType == type && temp->status == FREE) return temp;
		temp = temp->next;
	}
	return NULL;
}

int countPortsByStatus(Port* head, portStatus status)
{
	if (!head)
	{
		displayError(ERROR_NULL_POINTER);
		return -1;
	}
	Port* temp = head;
	int statusCounter = 0;
	while (temp != NULL)
	{
		if (status == temp->status) statusCounter++;
		temp = temp->next;
	}
	return statusCounter;
}

int countPortsByType(Port* head, portSpeed type)
{
	if (!head)
	{
		displayError(ERROR_NULL_POINTER);
		return -1;
	}
	Port* temp = head;
	int typeCounter = 0;
	while (temp != NULL)
	{
		if (type == temp->portType && temp->status != OUT_OF_ORDER) typeCounter++;
		temp = temp->next;
	}
	return typeCounter;
}

portSpeed selectPortType() {
	int choice;
	portSpeed selectedType;
	// Display port type selection menu
	while (1) {
		printf("\nPlease select your car's port type:\n");
		printf("1. FAST charging\n");
		printf("2. MID charging\n");
		printf("3. SLOW charging\n");
		printf("Your choice (1-3): ");

		fseek(stdin, 0, SEEK_END);
		if (scanf("%d", &choice) != 1) {
			displayError(ERROR_INVALID_INPUT);
			if (returnToMenu()) return -1;
			continue;
		}

		switch (choice) {
		case 1:
			selectedType = FAST;
			printf("You selected: FAST charging\n");
			break;
		case 2:
			selectedType = MID;
			printf("You selected: MID charging\n");
			break;
		case 3:
			selectedType = SLOW;
			printf("You selected: SLOW charging\n");
			break;
		default:
			printf("Invalid choice! Please select 1, 2, or 3.\n");
			continue;
		}
		// Confirm selection
		char confirm;
		printf("Are you sure you want to register your car with %s charging? (Y/N): ",
			selectedType == FAST ? "FAST" : (selectedType == MID ? "MID" : "SLOW"));
		fseek(stdin, 0, SEEK_END);
		scanf(" %c", &confirm);

		if (confirm == 'Y' || confirm == 'y') {
			return selectedType;
		}
		else if (confirm == 'N' || confirm == 'n') {
			printf("Please select your port type again.\n");
			continue;
		}
		else {
			displayError(ERROR_INVALID_INPUT);
			if (returnToMenu()) return -1;
			continue;
		}
	}
}

void addNewPort(StationBST* stationsBST)
{
	if (!stationsBST || !stationsBST->root) {
		displayError(ERROR_NULL_POINTER);
		return;
	}

	printf("\n");
	printf("========================================================\n");
	printf("                      ADD NEW PORT                     \n");
	printf("========================================================\n");
	printf("\n");
	printf("  Add a new charging port to an existing station\n");
	printf("  to increase charging capacity.\n");
	printf("\n");

	// Get station from user
	printf("  STATION SELECTION\n");
	printf("  -----------------\n");
	Station* station = getStationFromUser(stationsBST);
	if (!station) {
		return; // User cancelled or station not found
	}

	printf("\n");
	printf("  SELECTED STATION\n");
	printf("  ----------------\n");
	printf("  Station: %s (ID: %d)\n", station->name, station->id);
	printf("  Current ports: %d\n", station->nPorts);
	printf("  Active ports: %d\n", countActivePortsInStation(station));
	printf("  Occupied ports: %d\n", countPortsByStatus(station->portsList, OCCUPIED));
	printf("\n");

	// Get port type from user
	printf("  PORT TYPE SELECTION\n");
	printf("  -------------------\n");

	portSpeed newPortType;
	int choice;

	while (1) {
		printf("  Select charging port type to add:\n");
		printf("  1. FAST charging port\n");
		printf("  2. MID charging port\n");
		printf("  3. SLOW charging port\n");
		printf("  Your choice (1-3): ");
		fseek(stdin, 0, SEEK_END);

		if (scanf("%d", &choice) != 1) {
			displayError(ERROR_INVALID_INPUT);
			if (returnToMenu()) return;
			continue;
		}

		switch (choice) {
		case 1:
			newPortType = FAST;
			printf("  Selected: FAST charging port\n");
			break;
		case 2:
			newPortType = MID;
			printf("  Selected: MID charging port\n");
			break;
		case 3:
			newPortType = SLOW;
			printf("  Selected: SLOW charging port\n");
			break;
		default:
			printf("  Invalid choice! Please select 1, 2, or 3.\n");
			continue;
		}
		break; // Exit loop if valid choice is made
	}

	// Find the next available port number
	int newPortNumber = 1;
	Port* current = station->portsList;
	while (current != NULL) {
		if (current->num >= newPortNumber) {
			newPortNumber = current->num + 1;
		}
		current = current->next;
	}

	// Confirm port addition with user
	char confirm;
	printf("\n");
	printf("  CONFIRM PORT ADDITION\n");
	printf("  ---------------------\n");
	printf("  Station: %s (ID: %d)\n", station->name, station->id);
	printf("  New port type: %s\n", newPortType == FAST ? "FAST" : (newPortType == MID ? "MID" : "SLOW"));
	printf("  Port number: %d\n", newPortNumber);
	printf("\n");

	while (1) {
		printf("  Are you sure you want to add this port? (Y/N): ");
		fseek(stdin, 0, SEEK_END);
		scanf(" %c", &confirm);

		if (confirm == 'Y' || confirm == 'y') {
			break; // User confirmed
		}
		else if (confirm == 'N' || confirm == 'n') {
			printf("\n");
			printf("  PORT ADDITION CANCELLED\n");
			printf("  -----------------------\n");
			printf("  Port addition has been cancelled.\n");
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
	printf("  ADDING PORT...\n");
	printf("  --------------\n");

	// Create the new port
	Date currentDate = getCurrentDate();
	char portTypeStr[10];
	strcpy(portTypeStr, newPortType == FAST ? "FAST" : (newPortType == MID ? "MID" : "SLOW"));

	Port* newPort = initPort(newPortNumber, portTypeStr, FREE, currentDate);
	if (!newPort) {
		displayError(ERROR_MEMORY_ALLOCATION);
		return;
	}

	// Add port to station
	station->portsList = addPortToStation(station->portsList, newPort);
	station->nPorts++; // Increment total port count

	printf("\n");
	printf("  PORT ADDITION SUCCESSFUL\n");
	printf("  ------------------------\n");
	printf("  New %s port (#%d) has been added to station %s\n",
		portTypeStr, newPortNumber, station->name);
	printf("  Station now has %d total ports\n", station->nPorts);
	printf("\n");

	// Check if there are cars waiting in the queue for this port type
	Car* waitingCar = searchCarInQueueByPortType(&station->carQueue, newPortType);
	if (waitingCar) {
		// Assign the car to the new port
		if (assignCarToPort(newPort, waitingCar)) {
			// Remove car from queue
			if (dequeueSpecificCarNode(&station->carQueue, waitingCar->nLicense)) {
				station->nCars--; // Decrement queue count

				printf("  QUEUE UPDATE\n");
				printf("  ------------\n");
				printf("  Car %s was moved from queue to the new port\n", waitingCar->nLicense);
				printf("  Car is now charging on port %d\n", newPortNumber);
				printf("  Charging started: %02d:%02d on %02d/%02d/%04d\n",
					newPort->tin.hour, newPort->tin.min,
					newPort->tin.day, newPort->tin.month, newPort->tin.year);
			}
		}
	}
	else {
		printf("  QUEUE STATUS\n");
		printf("  ------------\n");
		printf("  No cars waiting for %s charging\n", portTypeStr);
	}

	printf("\n");
	printf("  UPDATED STATION STATUS\n");
	printf("  ----------------------\n");
	printf("  Total ports: %d\n", station->nPorts);
	printf("  %s ports: %d\n", portTypeStr, countPortsByType(station->portsList, newPortType));
	printf("  Cars in queue: %d\n", station->nCars);
	printf("\n");
	printf("========================================================\n");
}

void releasePorts(StationBST* stationsBST)
{
	if (!stationsBST || !stationsBST->root) {
		displayError(ERROR_NULL_POINTER);
		return;
	}

	printf("\n");
	printf("========================================================\n");
	printf("                 RELEASE CHARGING PORTS                \n");
	printf("========================================================\n");
	printf("\n");
	printf("  Disconnect vehicles that have been charging\n");
	printf("  for more than %d hours and process the queue.\n", MAX_CHARGING_HOURS);
	printf("\n");

	// Get station from user
	printf("  STATION SELECTION\n");
	printf("  -----------------\n");
	Station* station = getStationFromUser(stationsBST);
	if (!station) {
		return; // User cancelled or station not found
	}

	printf("\n");
	printf("  SELECTED STATION\n");
	printf("  ----------------\n");
	printf("  Station: %s (ID: %d)\n", station->name, station->id);
	printf("  Checking for vehicles charging over %d hours...\n", MAX_CHARGING_HOURS);
	printf("\n");

	Date currentTime = getCurrentDate();
	int releasedCount = 0;
	int newConnectionsCount = 0;

	// Dynamically allocate arrays based on station port count
	char* releasedCars = (char*)malloc(station->nPorts * MAX_LICENSE_LENGTH * sizeof(char));
	char* connectedCars = (char*)malloc(station->nPorts * MAX_LICENSE_LENGTH * sizeof(char));

	if (!releasedCars || !connectedCars) {
		displayError(ERROR_MEMORY_ALLOCATION);
		if (releasedCars) free(releasedCars);
		if (connectedCars) free(connectedCars);
		return;
	}

	// Check all ports in the station
	Port* currentPort = station->portsList;
	while (currentPort != NULL) {
		if (currentPort->status == OCCUPIED && currentPort->car2p != NULL) {
			// Calculate charging time in minutes
			int chargingMinutes = dateDiffInMinutes(&currentPort->car2p->pPort->tin, &currentTime);

			// Check if charging time exceeds maximum allowed
			if (chargingMinutes > (MAX_CHARGING_HOURS * 60)) {
				Car* carToRelease = currentPort->car2p;

				// Store license number for display
				strcpy(releasedCars + (releasedCount * MAX_LICENSE_LENGTH), carToRelease->nLicense);

				// Calculate and update payment
				double chargingCost = chargingMinutes * CHARGING_RATE;
				carToRelease->totalPayed += chargingCost;

				printf("  Releasing car %s (charged %d hours, cost: %.2f shekels)\n",
					carToRelease->nLicense, chargingMinutes / 60, chargingCost);

				// Disconnect car from port
				currentPort->car2p = NULL;
				currentPort->status = FREE;
				carToRelease->pPort = NULL;
				carToRelease->inqueue = 0;

				releasedCount++;

				// Check if there are cars waiting in queue for this port type
				Car* waitingCar = searchCarInQueueByPortType(&station->carQueue, currentPort->portType);
				if (waitingCar) {
					// Assign the next car in queue to this port
					if (assignCarToPort(currentPort, waitingCar)) {
						// Remove car from queue
						if (dequeueSpecificCarNode(&station->carQueue, waitingCar->nLicense)) {
							// Store license number for new connection
							strcpy(connectedCars + (newConnectionsCount * MAX_LICENSE_LENGTH), waitingCar->nLicense);
							station->nCars--; // Decrement queue count
							newConnectionsCount++;
							printf("  -> Car %s moved from queue to port %d\n",
								waitingCar->nLicense, currentPort->num);
						}
					}
				}
			}
		}
		currentPort = currentPort->next;
	}

	printf("\n");
	printf("========================================================\n");
	printf("                   RELEASE SUMMARY                     \n");
	printf("========================================================\n");
	printf("\n");
	printf("  STATION: %s (ID: %d)\n", station->name, station->id);
	printf("--------------------------------------------------------\n");
	printf("\n");

	if (releasedCount > 0) {
		printf("  VEHICLES DISCONNECTED: %d\n", releasedCount);
		printf("  ----------------------\n");
		for (int i = 0; i < releasedCount; i++) {
			printf("  - %s\n",
				releasedCars + (i * MAX_LICENSE_LENGTH), MAX_CHARGING_HOURS);
		}
		printf("\n");

		if (newConnectionsCount > 0) {
			printf("  VEHICLES CONNECTED FROM QUEUE: %d\n", newConnectionsCount);
			printf("  ------------------------------\n");
			for (int i = 0; i < newConnectionsCount; i++) {
				printf("  - %s (moved from queue to charging)\n",
					connectedCars + (i * MAX_LICENSE_LENGTH));
			}
			printf("\n");
			printf("  Queue processing completed successfully.\n");
		}
		else {
			printf("  QUEUE STATUS\n");
			printf("  ------------\n");
			printf("  No cars were waiting for the released port types.\n");
		}
	}
	else {
		printf("  NO ACTION REQUIRED\n");
		printf("  ------------------\n");
		printf("  No vehicles found charging over %d hours.\n", MAX_CHARGING_HOURS);
		printf("  All cars are within acceptable charging time limits.\n");
	}

	printf("\n");
	printf("  UPDATED STATION STATUS\n");
	printf("  ----------------------\n");
	printf("  Occupied ports: %d\n", countPortsByStatus(station->portsList, OCCUPIED));
	printf("  Available ports: %d\n", countPortsByStatus(station->portsList, FREE));
	printf("  Cars in queue: %d\n", station->nCars);
	printf("\n");
	printf("========================================================\n");
	// Free dynamically allocated memory
	free(releasedCars);
	free(connectedCars);
}

void remOutOrderPort(StationBST* stationsBST) {
	if (!stationsBST || !stationsBST->root) {
		displayError(ERROR_STATION_NOT_FOUND);
		return;
	}

	printf("\n");
	printf("========================================================\n");
	printf("               REMOVE OUT OF ORDER PORTS               \n");
	printf("========================================================\n");
	printf("\n");
	printf("  Remove faulty or broken charging ports from stations.\n");
	printf("\n");

	// Get station from user
	printf("  STATION SELECTION\n");
	printf("  -----------------\n");
	Station* station = getStationFromUser(stationsBST);
	if (!station) return;

	printf("\n");
	printf("  STATION: %s (ID: %d)\n", station->name, station->id);
	printf("  ----------------------\n");

	// Count out of order ports
	Port* current = station->portsList;
	int outOfOrderCount = 0;

	while (current != NULL) {
		if (current->status == OUT_OF_ORDER) {
			outOfOrderCount++;
		}
		current = current->next;
	}

	if (outOfOrderCount == 0) {
		printf("  STATUS: All ports are operational\n");
		printf("  No out of order ports found at this station.\n");
		printf("\n");
		printf("========================================================\n");
		return;
	}

	// Dynamically allocate memory for out of order port numbers
	int* portNumbers = (int*)malloc(outOfOrderCount * sizeof(int));
	if (!portNumbers) {
		displayError(ERROR_MEMORY_ALLOCATION);
		return;
	}

	// Fill the portNumbers array with out of order port numbers
	current = station->portsList;
	int index = 0;
	while (current != NULL) {
		if (current->status == OUT_OF_ORDER) {
			portNumbers[index] = current->num;
			index++;
		}
		current = current->next;
	}

	// Display out of order ports
	printf("  OUT OF ORDER PORTS FOUND: %d\n", outOfOrderCount);
	printf("  ---------------------------\n");

	for (int i = 0; i < outOfOrderCount; i++) {
		// Find the port to display its details
		current = station->portsList;
		while (current != NULL) {
			if (current->status == OUT_OF_ORDER) {
				printf("  Port %d (%s charging) - OUT OF ORDER\n",
					current->num,
					current->portType == FAST ? "FAST" :
					(current->portType == MID ? "MID" : "SLOW"));
				break;
			}
			current = current->next;
		}
	}

	// Get port number to remove from user
	int portToRemove;
	int validPort = 0;

	printf("\n");
	printf("  PORT REMOVAL\n");
	printf("  ------------\n");

	while (!validPort) {
		printf("  Enter port number to remove: ");
		fseek(stdin, 0, SEEK_END);

		if (scanf("%d", &portToRemove) != 1) {
			displayError(ERROR_INVALID_INPUT);
			if (returnToMenu()) {
				free(portNumbers); // Free memory before returning
				return;
			}
			continue;
		}

		// Verify the port exists and is out of order
		validPort = 0;
		for (int i = 0; i < outOfOrderCount; i++) {
			if (portNumbers[i] == portToRemove) {
				validPort = 1;
				break;
			}
		}

		if (!validPort) {
			printf("\n");
			printf("  ERROR: Invalid port number\n");
			printf("  ---------------------------\n");
			printf("  Port %d is either:\n", portToRemove);
			printf("  - Not out of order\n");
			printf("  - Does not exist at this station\n");
			printf("  - Already removed\n");
			printf("\n");
			printf("  Please select from the out of order ports listed above.\n");
			if (returnToMenu()) {
				free(portNumbers); // Free memory before returning
				return;
			}
		}
	}

	// Confirm removal
	char confirm;
	printf("\n");
	printf("  CONFIRM REMOVAL\n");
	printf("  ---------------\n");
	printf("  You are about to permanently remove:\n");
	printf("  Port %d from station %s (ID: %d)\n", portToRemove, station->name, station->id);
	printf("\n");

	while (1) {
		printf("  Are you sure? (Y/N): ");
		fseek(stdin, 0, SEEK_END);
		scanf(" %c", &confirm);

		if (confirm == 'Y' || confirm == 'y') {
			// User confirmed - proceed with removal
			break;
		}
		else if (confirm == 'N' || confirm == 'n') {
			// User cancelled
			printf("\n");
			printf("  REMOVAL CANCELLED\n");
			printf("  -----------------\n");
			printf("  Port removal has been cancelled.\n");
			printf("  Port %d remains in the system.\n", portToRemove);
			printf("\n");
			printf("========================================================\n");

			// Free allocated memory before returning
			free(portNumbers);
			return;
		}
		else {
			// Invalid input - show error and continue loop
			displayError(ERROR_INVALID_INPUT);
			if (returnToMenu()) {
				free(portNumbers); // Free memory before returning
				return;
			}
			printf("  Please enter 'Y' for Yes or 'N' for No.\n");
		}
	}

	// Proceed with removal
	if (removePortFromStation(&station->portsList, portToRemove)) {
		station->nPorts--; // Decrease port count

		printf("\n");
		printf("  REMOVAL SUCCESSFUL\n");
		printf("  ------------------\n");
		printf("  Port %d has been successfully removed from station %s.\n",
			portToRemove, station->name);
		printf("  Station now has %d total ports.\n", station->nPorts);
		printf("\n");
	}
	else {
		displayError(ERROR_OPERATION_FAILED);
		printf("\n");
		printf("  REMOVAL FAILED\n");
		printf("  --------------\n");
		printf("  Could not remove port %d from the system.\n", portToRemove);
		printf("  Please try again or contact system administrator.\n");
	}

	printf("\n");
	printf("========================================================\n");

	// Free the dynamically allocated memory
	free(portNumbers);
}

int removePortFromStation(Port** head, int portNumber) {
	if (!head || !*head) return 0;

	Port* current = *head;
	Port* previous = NULL;

	// Search for the port to remove
	while (current != NULL) {
		if (current->num == portNumber) {
			// Found the port - remove it
			if (previous == NULL) {
				// Removing the first port in the list
				*head = current->next;
			}
			else {
				// Removing a port in the middle or end
				previous->next = current->next;
			}

			// Free the port memory
			free(current);
			return 1; // Success
		}

		previous = current;
		current = current->next;
	}

	return 0; // Port not found
}