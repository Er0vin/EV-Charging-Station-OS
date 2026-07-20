#define _CRT_SECURE_NO_WARNINGS
#include "General.h"

int main() {

    int choice;
    StationBST* stationsBST = initStationBST();
    carBST* carBST = initCarBST();
    loadFiles(stationsBST, carBST);

    printf("\n");
    printf("========================================================\n");
    printf("                                                        \n");
    printf("                    EASYCHARGE                         \n");
    printf("            Electric Vehicle Charging System           \n");
    printf("                     Version 1.0                       \n");
    printf("                                                        \n");
    printf("========================================================\n");
    printf("\n");
    do {
        displayMenu();
        fseek(stdin, 0, SEEK_END);
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            locateNearestStation(stationsBST->root);
            break;
        case 2:
			chargeCar(stationsBST, carBST);
            updateFiles(stationsBST, carBST);
            break;
        case 3:
			checkCarStatus(stationsBST, carBST);
            break;
        case 4:
            stopCharge(stationsBST, carBST);
            updateFiles(stationsBST, carBST);
            break;
        case 5:
            dispAllSt(stationsBST);
            break;
        case 6:
			dispCarsAtSt(stationsBST);
			break;
        case 7:
			reportStStat(stationsBST);
			break;
		case 8:
            dispTopCustomers(carBST);
			break;
        case 9:
            addNewPort(stationsBST);
            updateFiles(stationsBST, carBST);
            break;
        case 10:
            releasePorts(stationsBST);
            updateFiles(stationsBST, carBST);
            break;
        case 11:
            remOutOrderPort(stationsBST);
            updateFiles(stationsBST, carBST);
            break;
        case 12:
			remCustomer(carBST);
            updateFiles(stationsBST, carBST);
            break;
        case 13:
            closeSt(stationsBST);
            updateFiles(stationsBST, carBST);
            break;
        case 0:
            exitSys(stationsBST, carBST);
            break;
        default:
            printf("Invalid choice! Try again!\n");
        }

        if(choice != 0) {
            printf("\nPress Enter to continue...");
            fseek(stdin, 0, SEEK_END); // Clear input buffer
            getchar(); // Wait for enter key
        }

    } while (choice != 0);

    return 0;
}
 