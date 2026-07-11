#include "calendar_core.h"

/*
    Thin C interface for WebAssembly. The web page calls these functions instead of
    a JavaScript copy of the logic, so the site runs the same C++ as the command-line
    program. Dates are passed and returned as packed integers: year*10000 + month*100 + day.
*/

static int pack(Date d) { return d.year * 10000 + d.month * 100 + d.day; }

extern "C" {

int g2e(int y, int m, int d)       { return pack(gregorianToEthiopian(d, m, y)); }
int e2g(int y, int m, int d)       { return pack(ethiopianToGregorian(d, m, y)); }
int weekday_g(int y, int m, int d) { return gregorianWeekdayIndex(d, m, y); }
int is_leap_g(int y)               { return isGregorianLeap(y) ? 1 : 0; }
int is_leap_e(int y)               { return isEthiopianLeap(y) ? 1 : 0; }

// Add n days (cal: 0 Gregorian, 1 Ethiopian) by rolling through months, the original
// add-days method. Handles negative n.
int add_days(int cal, int y, int m, int d, int n) {
    return pack(addDaysCore(cal == 1, Date{d, m, y}, n));
}

// Difference in days between two Gregorian dates. JavaScript converts Ethiopian
// inputs to Gregorian first, then calls this.
int diff_days(int y1, int m1, int d1, int y2, int m2, int d2) {
    long a = dayCountGregorian(Date{d1, m1, y1}), b = dayCountGregorian(Date{d2, m2, y2});
    return (int)(a > b ? a - b : b - a);
}

}
