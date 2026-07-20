// Updated ErrorHandler.h
#pragma once
#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdio.h>

/*
 * Error codes
 */
typedef enum {
    // File errors
    ERROR_FILE_NOT_FOUND = 1,
    ERROR_FILE_READ_FAILED = 2,
    ERROR_FILE_WRITE_FAILED = 3,
    ERROR_LOADING_STATIONS = 4,
    ERROR_LOADING_CARS = 5,
    ERROR_LOADING_PORTS = 6,
    ERROR_LOADING_LINE = 7,

    // New file update errors
    ERROR_UPDATING_STATIONS = 8,
    ERROR_UPDATING_CARS = 9,
    ERROR_UPDATING_PORTS = 10,
    ERROR_UPDATING_LINE = 11,
    ERROR_FILE_UPDATE_FAILED = 12,

    // Memory errors (renumbered)
    ERROR_MEMORY_ALLOCATION = 13,

    // Input validation errors (renumbered)
    ERROR_NULL_POINTER = 14,
    ERROR_INVALID_INPUT = 15,
    ERROR_INVALID_LICENSE = 16,

    // Not found errors (renumbered)
    ERROR_CAR_NOT_FOUND = 17,
    ERROR_STATION_NOT_FOUND = 18,

    // Business logic errors (renumbered)
    ERROR_CAR_ALREADY_CHARGING = 19,
    ERROR_CAR_NOT_CHARGING = 20,
    ERROR_PORT_NOT_AVAILABLE = 21,
    ERROR_CANNOT_ADD_CAR_TO_CHARGE = 22,
    ERROR_CANNOT_REMOVE_CHARGING_CAR = 23,

    // General error (renumbered)
    ERROR_OPERATION_FAILED = 24
} ErrorCode;

const char* getErrorMessage(ErrorCode errorCode);

void displayError(int errorCode);

#endif