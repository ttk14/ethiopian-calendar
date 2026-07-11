#ifndef CALENDAR_CORE_H
#define CALENDAR_CORE_H

/*
    Shared date logic for the Ethiopian and Gregorian calendar tools.
    Used by both calendar.cpp (the command-line program) and
    calendar_wasm.cpp (the WebAssembly wrapper).
*/

typedef struct {
    int day;
    int month;
    int year;
} Date;

static const int CORE_monthLengthGC[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

inline bool isGregorianLeap(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}
inline bool isEthiopianLeap(int year) {
    return year % 4 == 3;
}
inline int daysInGregorianMonth(int year, int month) {
    if (month == 2) {
        if (isGregorianLeap(year)) { return 29; }
        else { return 28; }
    }
    return CORE_monthLengthGC[month - 1];
}
inline int daysInEthiopianMonth(int year, int month) {
    if (month == 13) {
        if (isEthiopianLeap(year)) { return 6; }
        else { return 5; }
    }
    return 30;
}

// Ethiopian -> Gregorian, month-by-month offsets.
inline Date ethiopianToGregorian(int dayEC, int monthEC, int yearEC) {
    int dayGC = 0, monthGC = 0, yearGC = 0;
    if (yearEC % 4 == 0) {                  // for a year after a leap year
        switch (monthEC) {
            case 1:
                if (dayEC <= 19) { dayGC = dayEC + 11; monthGC = 9;  yearGC = yearEC + 7; }
                else { dayGC = dayEC - 19; monthGC = 10; yearGC = yearEC + 7; }
                break;
            case 2:
                if (dayEC <= 20) { dayGC = dayEC + 11; monthGC = 10; yearGC = yearEC + 7; }
                else { dayGC = dayEC - 20; monthGC = 11; yearGC = yearEC + 7; }
                break;
            case 3:
                if (dayEC <= 20) { dayGC = dayEC + 10; monthGC = 11; yearGC = yearEC + 7; }
                else { dayGC = dayEC - 20; monthGC = 12; yearGC = yearEC + 7; }
                break;
            case 4:
                if (dayEC <= 21) { dayGC = dayEC + 10; monthGC = 12; yearGC = yearEC + 7; }
                else { dayGC = dayEC - 21; monthGC = 1;  yearGC = yearEC + 8; }
                break;
            case 5:
                if (dayEC <= 22) { dayGC = dayEC + 9;  monthGC = 1;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 22; monthGC = 2;  yearGC = yearEC + 8; }
                break;
            case 6:
                if (dayEC <= 21) { dayGC = dayEC + 8;  monthGC = 2;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 21; monthGC = 3;  yearGC = yearEC + 8; }
                break;
            case 7:
                if (dayEC <= 22) { dayGC = dayEC + 9;  monthGC = 3;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 22; monthGC = 4;  yearGC = yearEC + 8; }
                break;
            case 8:
                if (dayEC <= 22) { dayGC = dayEC + 8;  monthGC = 4;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 22; monthGC = 5;  yearGC = yearEC + 8; }
                break;
            case 9:
                if (dayEC <= 23) { dayGC = dayEC + 8;  monthGC = 5;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 23; monthGC = 6;  yearGC = yearEC + 8; }
                break;
            case 10:
                if (dayEC <= 23) { dayGC = dayEC + 7;  monthGC = 6;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 23; monthGC = 7;  yearGC = yearEC + 8; }
                break;
            case 11:
                if (dayEC <= 24) { dayGC = dayEC + 7;  monthGC = 7;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 24; monthGC = 8;  yearGC = yearEC + 8; }
                break;
            case 12:
                if (dayEC <= 25) { dayGC = dayEC + 6;  monthGC = 8;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 25; monthGC = 9;  yearGC = yearEC + 8; }
                break;
            case 13:
                dayGC = dayEC + 5; monthGC = 9; yearGC = yearEC + 8;
                break;
        }
    } else {                // for every other year
        switch (monthEC) {
            case 1:
                if (dayEC <= 20) { dayGC = dayEC + 10; monthGC = 9;  yearGC = yearEC + 7; }
                else { dayGC = dayEC - 20; monthGC = 10; yearGC = yearEC + 7; }
                break;
            case 2:
                if (dayEC <= 21) { dayGC = dayEC + 10; monthGC = 10; yearGC = yearEC + 7; }
                else { dayGC = dayEC - 21; monthGC = 11; yearGC = yearEC + 7; }
                break;
            case 3:
                if (dayEC <= 21) { dayGC = dayEC + 9;  monthGC = 11; yearGC = yearEC + 7; }
                else { dayGC = dayEC - 21; monthGC = 12; yearGC = yearEC + 7; }
                break;
            case 4:
                if (dayEC <= 22) { dayGC = dayEC + 9;  monthGC = 12; yearGC = yearEC + 7; }
                else { dayGC = dayEC - 22; monthGC = 1;  yearGC = yearEC + 8; }
                break;
            case 5:
                if (dayEC <= 23) { dayGC = dayEC + 8;  monthGC = 1;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 23; monthGC = 2;  yearGC = yearEC + 8; }
                break;
            case 6:
                if (dayEC <= 21) { dayGC = dayEC + 7;  monthGC = 2;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 21; monthGC = 3;  yearGC = yearEC + 8; }
                break;
            case 7:
                if (dayEC <= 22) { dayGC = dayEC + 9;  monthGC = 3;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 22; monthGC = 4;  yearGC = yearEC + 8; }
                break;
            case 8:
                if (dayEC <= 22) { dayGC = dayEC + 8;  monthGC = 4;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 22; monthGC = 5;  yearGC = yearEC + 8; }
                break;
            case 9:
                if (dayEC <= 23) { dayGC = dayEC + 8;  monthGC = 5;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 23; monthGC = 6;  yearGC = yearEC + 8; }
                break;
            case 10:
                if (dayEC <= 23) { dayGC = dayEC + 7;  monthGC = 6;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 23; monthGC = 7;  yearGC = yearEC + 8; }
                break;
            case 11:
                if (dayEC <= 24) { dayGC = dayEC + 7;  monthGC = 7;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 24; monthGC = 8;  yearGC = yearEC + 8; }
                break;
            case 12:
                if (dayEC <= 25) { dayGC = dayEC + 6;  monthGC = 8;  yearGC = yearEC + 8; }
                else { dayGC = dayEC - 25; monthGC = 9;  yearGC = yearEC + 8; }
                break;
            case 13:
                dayGC = dayEC + 5; monthGC = 9; yearGC = yearEC + 8;
                break;
        }
    }
    Date r = {dayGC, monthGC, yearGC};
    return r;
}

// Gregorian -> Ethiopian, month-by-month offsets.
inline Date gregorianToEthiopian(int dayGC, int monthGC, int yearGC) {
    int dayEC = 0, monthEC = 0, yearEC = 0;
    if (yearGC % 4 == 3) {                      // for years immediately before a leap year
        switch (monthGC) {
            case 1:
                if (dayGC <= 8)  { dayEC = dayGC + 22; monthEC = 4;  yearEC = yearGC - 8; }
                else { dayEC = dayGC - 8;  monthEC = 5;  yearEC = yearGC - 8; }
                break;
            case 2:
                if (dayGC <= 7)  { dayEC = dayGC + 23; monthEC = 5;  yearEC = yearGC - 8; }
                else { dayEC = dayGC - 7;  monthEC = 6;  yearEC = yearGC - 8; }
                break;
            case 3:
                if (dayGC <= 9)  { dayEC = dayGC + 21; monthEC = 6;  yearEC = yearGC - 8; }
                else { dayEC = dayGC - 9;  monthEC = 7;  yearEC = yearGC - 8; }
                break;
            case 4:
                if (dayGC <= 8)  { dayEC = dayGC + 22; monthEC = 7;  yearEC = yearGC - 8; }
                else { dayEC = dayGC - 8;  monthEC = 8;  yearEC = yearGC - 8; }
                break;
            case 5:
                if (dayGC <= 8)  { dayEC = dayGC + 22; monthEC = 8;  yearEC = yearGC - 8; }
                else { dayEC = dayGC - 8;  monthEC = 9;  yearEC = yearGC - 8; }
                break;
            case 6:
                if (dayGC <= 7)  { dayEC = dayGC + 23; monthEC = 9;  yearEC = yearGC - 8; }
                else { dayEC = dayGC - 7;  monthEC = 10; yearEC = yearGC - 8; }
                break;
            case 7:
                if (dayGC <= 7)  { dayEC = dayGC + 23; monthEC = 10; yearEC = yearGC - 8; }
                else { dayEC = dayGC - 7;  monthEC = 11; yearEC = yearGC - 8; }
                break;
            case 8:
                if (dayGC <= 6)  { dayEC = dayGC + 24; monthEC = 11; yearEC = yearGC - 8; }
                else { dayEC = dayGC - 6;  monthEC = 12; yearEC = yearGC - 8; }
                break;
            case 9:
                if (dayGC <= 5)  { dayEC = dayGC + 25; monthEC = 12; yearEC = yearGC - 8; }
                else if (dayGC <= 11) { dayEC = dayGC - 5; monthEC = 13; yearEC = yearGC - 8; }
                else { dayEC = dayGC - 11; monthEC = 1; yearEC = yearGC - 7; }
                break;
            case 10:
                if (dayGC <= 11) { dayEC = dayGC + 19; monthEC = 1;  yearEC = yearGC - 7; }
                else { dayEC = dayGC - 11; monthEC = 2;  yearEC = yearGC - 7; }
                break;
            case 11:
                if (dayGC <= 10) { dayEC = dayGC + 20; monthEC = 2;  yearEC = yearGC - 7; }
                else { dayEC = dayGC - 10; monthEC = 3;  yearEC = yearGC - 7; }
                break;
            case 12:
                if (dayGC <= 10) { dayEC = dayGC + 20; monthEC = 3;  yearEC = yearGC - 7; }
                else { dayEC = dayGC - 10; monthEC = 4;  yearEC = yearGC - 7; }
                break;
        }
    } else {                    // for every other year
        bool leap = isGregorianLeap(yearGC);
        switch (monthGC) {
            case 1:
                if (leap) {
                    if (dayGC <= 9) { dayEC = dayGC + 21; monthEC = 4; yearEC = yearGC - 8; }
                    else { dayEC = dayGC - 9; monthEC = 5; yearEC = yearGC - 8; }
                }
                else {
                    if (dayGC <= 8) { dayEC = dayGC + 22; monthEC = 4; yearEC = yearGC - 8; }
                    else { dayEC = dayGC - 8; monthEC = 5; yearEC = yearGC - 8; }
                }
                break;
            case 2:
                if (leap) {
                    if (dayGC <= 8) { dayEC = dayGC + 22; monthEC = 5; yearEC = yearGC - 8; }
                    else { dayEC = dayGC - 8; monthEC = 6; yearEC = yearGC - 8; }
                }
                else {
                    if (dayGC <= 7) { dayEC = dayGC + 23; monthEC = 5; yearEC = yearGC - 8; }
                    else { dayEC = dayGC - 7; monthEC = 6; yearEC = yearGC - 8; }
                }
                break;
            case 3:
                if (dayGC <= 9)  { dayEC = dayGC + 21; monthEC = 6;  yearEC = yearGC - 8; }
                else { dayEC = dayGC - 9;  monthEC = 7;  yearEC = yearGC - 8; }
                break;
            case 4:
                if (dayGC <= 8)  { dayEC = dayGC + 22; monthEC = 7;  yearEC = yearGC - 8; }
                else { dayEC = dayGC - 8;  monthEC = 8;  yearEC = yearGC - 8; }
                break;
            case 5:
                if (dayGC <= 8)  { dayEC = dayGC + 22; monthEC = 8;  yearEC = yearGC - 8; }
                else { dayEC = dayGC - 8;  monthEC = 9;  yearEC = yearGC - 8; }
                break;
            case 6:
                if (dayGC <= 7)  { dayEC = dayGC + 23; monthEC = 9;  yearEC = yearGC - 8; }
                else { dayEC = dayGC - 7;  monthEC = 10; yearEC = yearGC - 8; }
                break;
            case 7:
                if (dayGC <= 7)  { dayEC = dayGC + 23; monthEC = 10; yearEC = yearGC - 8; }
                else { dayEC = dayGC - 7;  monthEC = 11; yearEC = yearGC - 8; }
                break;
            case 8:
                if (dayGC <= 6)  { dayEC = dayGC + 24; monthEC = 11; yearEC = yearGC - 8; }
                else { dayEC = dayGC - 6;  monthEC = 12; yearEC = yearGC - 8; }
                break;
            case 9:
                if (dayGC <= 5)  { dayEC = dayGC + 25; monthEC = 12; yearEC = yearGC - 8; }
                else if (dayGC <= 10) { dayEC = dayGC - 5; monthEC = 13; yearEC = yearGC - 8; }
                else { dayEC = dayGC - 10; monthEC = 1; yearEC = yearGC - 7; }
                break;
            case 10:
                if (dayGC <= 10) { dayEC = dayGC + 20; monthEC = 1;  yearEC = yearGC - 7; }
                else { dayEC = dayGC - 10; monthEC = 2;  yearEC = yearGC - 7; }
                break;
            case 11:
                if (dayGC <= 9)  { dayEC = dayGC + 21; monthEC = 2;  yearEC = yearGC - 7; }
                else { dayEC = dayGC - 9;  monthEC = 3;  yearEC = yearGC - 7; }
                break;
            case 12:
                if (dayGC <= 9)  { dayEC = dayGC + 21; monthEC = 3;  yearEC = yearGC - 7; }
                else { dayEC = dayGC - 9;  monthEC = 4;  yearEC = yearGC - 7; }
                break;
        }
    }
    Date r = {dayEC, monthEC, yearEC};
    return r;
}

// Weekday for a Gregorian date, key-value method. Returns 0=Sunday .. 6=Saturday.
inline int gregorianWeekdayIndex(int dayGC, int monthGC, int yearGC) {
    int yearTwoDig = yearGC % 100;
    int yearCode = (yearTwoDig + (yearTwoDig / 4)) % 7;
    int monthCode = 0;

    switch (monthGC) {
        case 1:  monthCode = 0; break;
        case 2:  monthCode = 3; break;
        case 3:  monthCode = 3; break;
        case 4:  monthCode = 6; break;
        case 5:  monthCode = 1; break;
        case 6:  monthCode = 4; break;
        case 7:  monthCode = 6; break;
        case 8:  monthCode = 2; break;
        case 9:  monthCode = 5; break;
        case 10: monthCode = 0; break;
        case 11: monthCode = 3; break;
        case 12: monthCode = 5; break;
    }

    int centuryCode;
    if ((yearGC / 100) % 4 == 0)
        centuryCode = 6;
    else if ((yearGC / 100) % 4 == 1)
        centuryCode = 4;
    else if ((yearGC / 100) % 4 == 2)
        centuryCode = 2;
    else
        centuryCode = 0;

    int leapYearCode;
    if (isGregorianLeap(yearGC) && (monthGC == 1 || monthGC == 2)) { leapYearCode = -1; }
    else { leapYearCode = 0; }

    return (yearCode + monthCode + centuryCode + dayGC + leapYearCode) % 7;
}

// ---- add-days and day-count, using month-rolling approach ----
// Adds n days by rolling through months
// Handles negative n by rolling backward.
inline Date addDaysCore(bool eth, Date x, int n) {
    while (n > 0) {
        int dim;
        if (eth) { dim = daysInEthiopianMonth(x.year, x.month); }
        else { dim = daysInGregorianMonth(x.year, x.month); }

        int daysRemainMonth = dim - x.day;
        if (n > daysRemainMonth) {
            n = n - daysRemainMonth - 1;
            x.day = 1;

            int lastMonth = eth ? 13 : 12;

            if (x.month < lastMonth)
                x.month = x.month + 1;
            else {
                x.month = 1;
                x.year = x.year + 1;
            }
        }
        else {
            x.day = x.day + n;
            n = 0;
        }
    }

    while (n < 0) {
        if (x.day > 1) {
            x.day = x.day - 1;
        } else {
            int lastMonth = eth ? 13 : 12;

            if (x.month > 1)
                x.month = x.month - 1;
            else {
                x.month = lastMonth;
                x.year = x.year - 1;
            }

            int days;
            if (eth) { days = daysInEthiopianMonth(x.year, x.month); }
            else { days = daysInGregorianMonth(x.year, x.month); }
            x.day = days;
        }
        n = n + 1;
    }
    return x;
}

// Counts the days of a Gregorian date from year 1, by summing whole years and months.
// Used for the date-difference feature (subtract two counts). Not the Ethiopian conversion.
inline long dayCountGregorian(Date g) {
    long days = 0;

    for (int y = 1; y < g.year; y++) {
        if (isGregorianLeap(y)) { days += 366; }
        else { days += 365; }
    }

    for (int m = 1; m < g.month; m++)
        days += daysInGregorianMonth(g.year, m);

    return days + g.day;
}

#endif
