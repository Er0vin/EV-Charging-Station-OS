#pragma once
#ifndef DATE_H
#define DATE_H


// Date structure for storing date and time information

typedef struct {
    int year;   // Year (4 digit)
    int month;  // Month (1-12)
    int day;    // Day (1-31, depending on month)
    int hour;   // Hour (0-23, 24-hour format)
    int min;    // Minutes (0-59)
} Date;

 
/*
 * Initialize a Date structure
 *  year - year value
 *  month - month value (1-12)
 *  day - day value
 *  hour - hour value (0-23)
 *  min - minute value (0-59)
 * return initialized Date structure
 */
Date initDate(int year, int month, int day, int hour, int min);

/*
 * Validate if a date is valid
 *  date - pointer to Date structure
 * return 1 if valid, 0 if invalid
 */
int validateDate(const Date* date);

/*
 * Print date in readable format
 *  date - pointer to Date structure
 */
void printDate(const Date* date);

/*
 * Calculate difference between two dates in minutes
 *  start - pointer to start date
 *  end - pointer to end date
 * return difference in minutes
 */
int dateDiffInMinutes(const Date* start, const Date* end);

/*
 * Get current date and time
 * return current Date structure
 */
Date getCurrentDate();

#endif