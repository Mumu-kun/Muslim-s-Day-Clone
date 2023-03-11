#include <stdio.h>

typedef struct
{
    int year;
    int month;
    int day;
} Date;

typedef struct
{
    int hour;
    int min;
    int sec;
} tm_short;

typedef struct
{
    Date dateEn;
    Date dateArabic;
    int weekday;
    tm_short fajr;
    tm_short sunrise;
    tm_short dhuhr;
    tm_short asr;
    tm_short sunset;
    tm_short maghrib;
    tm_short isha;
} dayRoutine;

typedef struct
{
    char text[100];
    tm_short time;
} Reminder;

char *weekday[7] = {"Friday", "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday"};
char *monthEn[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
char *monthArabic[12] = {"Muharram", "Safar", "Rabi Al-Awwal", "Rabi Al-Thani", "Jamada Al-Awwal", "Jamada Al-Thani", "Rajab", "Sha’ban", "Ramadan", "Shawwal", "Dhul Qadah", "Dhul Hijjah"};

char *dateString(char *dest, Date *date, int type)
{
    char month[20];
    if (type == 0)
    {
        strcpy(month, monthEn[date->month - 1]);
    }
    else
    {
        strcpy(month, monthArabic[date->month - 1]);
    }
    sprintf(dest, "%02d %s %d", date->day, month, date->year);
    return dest;
}
char *timeString(char *dest, tm_short *tm)
{
    sprintf(dest, "%02d:%02d", tm->hour, tm->min);
    return dest;
}
char *timeStringSec(char *dest, tm_short *tm)
{
    sprintf(dest, "%02d:%02d:%02d", tm->hour, tm->min, tm->sec);
    return dest;
}

tm_short *addMinutes(tm_short *time, int min)
{
    time->min += min;
    if (time->min > 59)
    {
        time->hour += time->min / 60;
        time->min %= 60;
    }
    else if (time->min < 0)
    {
        time->hour += (time->min / 60 - 1);
        time->min %= 60;
        time->min += 60;
    }
    return time;
}
