#ifndef _DATETIME_H_
#define _DATETIME_H_

#include <stdint.h>

struct ag_datetime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};

int gregorian_to_jdn(int year, int month, int day);
void jdn_to_gregorian(int jdn, int *year, int *month, int *day);

double time_to_fraction(int hour, int minute, int second);
void fraction_to_time(double fraction, int *hour, int *minute, int *second);

void fat_datetime_to_ag_datetime(uint16_t fat_date, uint16_t fat_time, struct ag_datetime *adt);
void ag_datetime_to_fat_datetime(const struct ag_datetime *adt, uint16_t *fat_date, uint16_t *fat_time);

double ag_datetime_to_double(const struct ag_datetime *adt);
void double_to_ag_datetime(double d, struct ag_datetime *adt);

#endif