#include <stdio.h>
#include "ErrorHandler.h"

const char* getErrorMessage(ErrorCode errorCode) {
    switch (errorCode) {

        // File errors
    case ERROR_FILE_NOT_FOUND:
        return "File not found";
    case ERROR_FILE_READ_FAILED:
        return "Failed to read from file";
    case ERROR_FILE_WRITE_FAILED:
        return "Failed to write to file";
    case ERROR_LOADING_STATIONS:
        return "Failed to load Stations from file";
    case ERROR_LOADING_CARS:
        return "Failed to load Cars from file";
    case ERROR_LOADING_PORTS:
        return "Failed to load Ports from file";
    case ERROR_LOADING_LINE:
        return "Failed to load LineOfCars from file";

        // File update errors
    case ERROR_UPDATING_STATIONS:
        return "Failed to update Stations.txt file";
    case ERROR_UPDATING_CARS:
        return "Failed to update Cars.txt file";
    case ERROR_UPDATING_PORTS:
        return "Failed to update Ports.txt file";
    case ERROR_UPDATING_LINE:
        return "Failed to update LineOfCars.txt file";
    case ERROR_FILE_UPDATE_FAILED:
        return "Failed to update one or more data files";

        // Memory errors
    case ERROR_MEMORY_ALLOCATION:
        return "Memory allocation failed";

        // Input validation errors
    case ERROR_NULL_POINTER:
        return "Null pointer encountered";
    case ERROR_INVALID_INPUT:
        return "Invalid input provided";
    case ERROR_INVALID_LICENSE:
        return "Invalid license number (must be 8 digits)";

        // Not found errors
    case ERROR_CAR_NOT_FOUND:
        return "Car not found in system";
    case ERROR_STATION_NOT_FOUND:
        return "Station not found";

        // Business logic errors
    case ERROR_CAR_ALREADY_CHARGING:
        return "Car is already charging";
    case ERROR_CAR_NOT_CHARGING:
        return "Car is not currently charging";
    case ERROR_PORT_NOT_AVAILABLE:
        return "No available ports for this car type";
    case ERROR_CANNOT_REMOVE_CHARGING_CAR:
        return "Cannot remove car - currently charging or in queue";
    case ERROR_CANNOT_ADD_CAR_TO_CHARGE:
        return "Cannot add car to charge - port not available or car already charging";

        // General error
    case ERROR_OPERATION_FAILED:
        return "Operation failed";

    default:
        return "Unknown error occurred";
    }
}

void displayError(ErrorCode errorCode) {
    printf("\n ERROR! \n%s\n", getErrorMessage(errorCode));
}