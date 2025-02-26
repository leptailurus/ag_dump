#include <datetime.h>

#include <math.h>

int gregorian_to_jdn(int year, int month, int day)
{
    if (month > 2)
    {
        month = month - 3;
    }
    else
    {
        month = month + 9;
        year = year - 1;
    }

    int c = year / 100;
    int ya = year - 100 * c;
    return (146097 * c) / 4 + (1461 * ya) / 4 + (153 * month + 2) / 5 + day + 1721119;
}

void jdn_to_gregorian(int jdn, int *year, int *month, int *day)
{
    int y, m, d;

    jdn = jdn - 1721119;

    y = (4 * jdn - 1) / 146097;
    jdn = 4 * jdn - 1 - 146097 * y;
    d = jdn / 4;

    jdn = (4 * d + 3) / 1461;
    d = 4 * d + 3 - 1461 * jdn;
    d = (d + 4) / 4;

    m = (5 * d - 3) / 153;
    d = 5 * d - 3 - 153 * m;
    d = (d + 5) / 5;

    y = 100 * y + jdn;

    if (m < 10)
    {
        m = m + 3;
    }
    else
    {
        m = m - 9;
        y = y + 1;
    }

    *year = y;
    *month = m;
    *day = d;
}

double time_to_fraction(int hour, int minute, int second)
{
    return ((hour * 3600) + (minute * 60) + second) / 86400.0;
}

void fraction_to_time(double fraction, int *hour, int *minute, int *second)
{
    *second = (int)(round(fraction * 86400));
    *minute = *second / 60;
    *second %= 60;
    *hour = *minute / 60;
    *minute %= 60;
}

void fat_datetime_to_ag_datetime(uint16_t fat_date, uint16_t fat_time, struct ag_datetime *adt)
{
    adt->year = (fat_date >> 11) + 1980;
    adt->month = (fat_date >> 5) & 0x000f;
    adt->day = fat_date & 0x001f;
    adt->hour = (fat_time >> 11);
    adt->minute = (fat_time >> 5) & 0x003f;
    adt->second = (fat_time & 0x001f) * 2;
}

void ag_datetime_to_fat_datetime(const struct ag_datetime *adt, uint16_t *fat_date, uint16_t *fat_time)
{
    *fat_date = (uint16_t)(((adt->year - 1980) << 11) + (adt->month << 5) + adt->day);
    *fat_time = (uint16_t)((adt->hour << 11) + (adt->minute << 5) + (adt->second / 2));
}

double ag_datetime_to_double(const struct ag_datetime *adt)
{
    int jdn = gregorian_to_jdn(adt->year, adt->month, adt->day);
    double time = time_to_fraction(adt->hour, adt->minute, adt->second);
    return (jdn - 2415019) + time;
}

void double_to_ag_datetime(double d, struct ag_datetime *adt)
{
    double di, df;
    int jdn;
    df = modf(d, &di);
    jdn = (int)di + 2415019;
    jdn_to_gregorian(jdn, &adt->year, &adt->month, &adt->day);
    fraction_to_time(df, &adt->hour, &adt->minute, &adt->second);
}
