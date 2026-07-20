#define _CRT_SECURE_NO_WARNINGS

#include "General.h"


Date initDate(int year, int month, int day, int hour, int min) {
	Date NewDate = { .year = year, .month = month, .day = day, .hour = hour, .min = min };
	return NewDate;
}

int validateDate(const Date* date) {

	Date* CurrentDate = date;

	if (CurrentDate->year < 1900 || CurrentDate->year > 2025) {
		return 0;
	}
	if (CurrentDate->month < 1 || CurrentDate->month > 12) {
		return 0;
	}
	if (CurrentDate->day < 1 || CurrentDate->day > 31) {
		return 0;
	}
	if (CurrentDate->hour < 0 || CurrentDate->hour > 23) {
		return 0;
	}
	if (CurrentDate->min < 0 || CurrentDate->min > 59) {
		return 0;
	}
	return 1;
}

void printDate(const Date* date) {

	Date* CurrentDate = date;

	printf("current date is:\n %d/%1d/%1d time: %d:%d\n", CurrentDate->day, CurrentDate->month, CurrentDate->year, CurrentDate->hour, CurrentDate->min);
}

int dateDiffInMinutes(const Date* start, const Date* end) {
	struct tm tm_start = { 0 };
	tm_start.tm_year = start->year - 1900;
	tm_start.tm_mon = start->month - 1;
	tm_start.tm_mday = start->day;
	tm_start.tm_hour = start->hour;
	tm_start.tm_min = start->min;

	struct tm tm_end = { 0 };
	tm_end.tm_year = end->year - 1900;
	tm_end.tm_mon = end->month - 1;
	tm_end.tm_mday = end->day;
	tm_end.tm_hour = end->hour;
	tm_end.tm_min = end->min;

	time_t time_start = mktime(&tm_start);
	time_t time_end = mktime(&tm_end);

	return (int)(difftime(time_end, time_start) / 60);
}

Date getCurrentDate() {
	time_t now = time(NULL);
	struct tm* local = localtime(&now);

	return initDate(
		local->tm_year + 1900,  // tm_year is years since 1900
		local->tm_mon + 1,      // tm_mon is 0-11, we need 1-12
		local->tm_mday,         // day of month (1-31)
		local->tm_hour,         // hour (0-23)
		local->tm_min           // minutes (0-59)
	);
}
