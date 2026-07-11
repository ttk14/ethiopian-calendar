/*
    Ethiopian and Gregorian Date and Time Processor (command-line version)

    The date logic (conversion, weekday, leap checks) lives in calendar_core.h and is
    shared with the WebAssembly build. This file holds the display names and the
    console interface only.
*/

#include <iostream>
#include <ctime>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <limits>
#include "calendar_core.h"

using namespace std;

// ----------------------------- display names -----------------------------
string monthsGC[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

string monthsEC[13] = {
    "Meskerem", "Tikimt", "Hidar", "Tahsas", "Tir", "Yekatit",
    "Megabit", "Miyazia", "Ginbot", "Sene", "Hamle", "Nehase", "Pagume"
};

const string WEEKDAYS_EN[7] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

const string WEEKDAYS_AM[7] = {"Ehud", "Segno", "Maksegno", "Erob", "Hamus", "Arb", "Kidame"};

// ----------------------------- input helpers -----------------------------
void printMenu() {
    cout << "\n   Menu\n";
    cout << "    1. Show local date and time\n";
    cout << "    2. Convert a date (Ethiopian or Gregorian)\n";
    cout << "    3. Add a number of days to a date\n";
    cout << "    4. Days between two dates\n";
    cout << "    5. Show the Zemen (Ethiopian year name)\n";
    cout << "    6. Check if a year is a leap year\n";
    cout << "    7. Weekday for a given date\n";
    cout << "    8. Show an Ethiopian month with Gregorian dates\n";
    cout << "    9. Show a Gregorian month with Ethiopian dates\n";
    cout << "    10. Exit\n";
    cout << "\nChoice = ";
}
char getCalendarType() {
    char c;
    cout << "Select 'E' for Ethiopian or 'G' for Gregorian: ";
    cin >> c;
    while (!(c == 'E' || c == 'e' || c == 'G' || c == 'g')) {
        cout << "Invalid type, enter 'E' or 'G': ";
        cin >> c;
    }
    return c;
}
bool isEthiopian(char c) { return c == 'E' || c == 'e'; }

// ----------------------------- features -----------------------------
void convertDate() {
    char calendarType = getCalendarType();
    char sep;
    Date d;
    cout << "Enter date in dd/mm/yyyy format: ";
    cin >> d.day >> sep >> d.month >> sep >> d.year;
    if (sep != '/') { cout << "Invalid format.\n"; return; }

    if (isEthiopian(calendarType)) {
        if (d.month < 1 || d.month > 13 || d.day < 1 || d.day > daysInEthiopianMonth(d.year, d.month))
            { cout << "Invalid Ethiopian date.\n"; return; }
        Date g = ethiopianToGregorian(d.day, d.month, d.year);
        cout << d.day << "/" << d.month << "/" << d.year << " (E.C.) is "
             << g.day << "/" << g.month << "/" << g.year << " (Gregorian).\n";
    } else {
        if (d.month < 1 || d.month > 12 || d.day < 1 || d.day > daysInGregorianMonth(d.year, d.month))
            { cout << "Invalid Gregorian date.\n"; return; }
        Date e = gregorianToEthiopian(d.day, d.month, d.year);
        cout << d.day << "/" << d.month << "/" << d.year << " (Gregorian) is "
             << e.day << "/" << e.month << "/" << e.year << " (E.C.).\n";
    }
}

void addDaysToDate() {
    char calendarType = getCalendarType();
    char sep;
    int day, month, year, n;
    cout << "Enter date in dd/mm/yyyy format: ";
    cin >> day >> sep >> month >> sep >> year;
    cout << "How many days do you want to add? ";
    cin >> n;
    int nCopy = n;

    bool eth = isEthiopian(calendarType);
    Date r = addDaysCore(eth, Date{day, month, year}, n);
    cout << "After " << nCopy << " days, the date is: " << r.day << "/" << r.month << "/" << r.year << "\n";
}

void dateDifference() {
    char calendarType = getCalendarType();
    char sep;
    Date a, b;
    cout << "Enter first date in dd/mm/yyyy format: ";
    cin >> a.day >> sep >> a.month >> sep >> a.year;
    cout << "Enter second date in dd/mm/yyyy format: ";
    cin >> b.day >> sep >> b.month >> sep >> b.year;

    Date g1 = isEthiopian(calendarType) ? ethiopianToGregorian(a.day, a.month, a.year) : a;
    Date g2 = isEthiopian(calendarType) ? ethiopianToGregorian(b.day, b.month, b.year) : b;
    long diff = dayCountGregorian(g1) - dayCountGregorian(g2);
    cout << "Difference: " << labs(diff) << " days.\n";
}

void showZemen() {
    int year;
    cout << "Enter an Ethiopian year: ";
    cin >> year;
    string zemen;
    if (year % 4 == 0)      zemen = "Yohannes";
    else if (year % 4 == 1) zemen = "Matewos";
    else if (year % 4 == 2) zemen = "Markos";
    else                    zemen = "Lucas";
    cout << year << " is Zemene " << zemen << ".\n";
}

void checkLeapYear() {
    char calendarType = getCalendarType();
    int year;
    cout << "Enter a year: ";
    cin >> year;
    bool leap = isEthiopian(calendarType) ? isEthiopianLeap(year) : isGregorianLeap(year);
    cout << year << (leap ? " is a leap year.\n" : " is not a leap year.\n");
}

void weekdayOfDate() {
    char calendarType = getCalendarType();
    char sep;
    Date d;
    cout << "Enter date in dd/mm/yyyy format: ";
    cin >> d.day >> sep >> d.month >> sep >> d.year;
    Date g = isEthiopian(calendarType) ? ethiopianToGregorian(d.day, d.month, d.year) : d;
    int idx = gregorianWeekdayIndex(g.day, g.month, g.year);
    cout << "That date falls on " << (isEthiopian(calendarType) ? WEEKDAYS_AM[idx] : WEEKDAYS_EN[idx]) << ".\n";
}

void showEthiopianMonth() {
    int year, month;
    cout << "Ethiopian year: ";  cin >> year;
    cout << "Ethiopian month (1-13): "; cin >> month;
    if (month < 1 || month > 13 || year < 1) { cout << "Invalid month or year.\n"; return; }
    cout << "\n" << monthsEC[month - 1] << " " << year << " E.C.\n";
    int days = daysInEthiopianMonth(year, month);
    for (int d = 1; d <= days; d++) {
        Date g = ethiopianToGregorian(d, month, year);
        int idx = gregorianWeekdayIndex(g.day, g.month, g.year);
        cout << setw(2) << d << " " << monthsEC[month - 1] << "  ->  "
             << monthsGC[g.month - 1] << " " << g.day << ", " << g.year
             << "  (" << WEEKDAYS_EN[idx] << ")\n";
    }
}

void showGregorianMonth() {
    int year, month;
    cout << "Gregorian year: "; cin >> year;
    cout << "Gregorian month (1-12): "; cin >> month;
    if (month < 1 || month > 12 || year < 1) { cout << "Invalid month or year.\n"; return; }
    cout << "\n" << monthsGC[month - 1] << " " << year << "\n";
    int days = daysInGregorianMonth(year, month);
    for (int d = 1; d <= days; d++) {
        Date e = gregorianToEthiopian(d, month, year);
        int idx = gregorianWeekdayIndex(d, month, year);
        cout << setw(2) << d << " " << monthsGC[month - 1] << "  ->  "
             << e.day << " " << monthsEC[e.month - 1] << " " << e.year
             << " E.C.  (" << WEEKDAYS_EN[idx] << ")\n";
    }
}

void showLocalDateTime() {
    time_t t = time(nullptr);
    tm *lt = localtime(&t);
    Date g = {lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900};
    Date e = gregorianToEthiopian(g.day, g.month, g.year);
    int idx = gregorianWeekdayIndex(g.day, g.month, g.year);
    cout << "\nGregorian:  " << monthsGC[g.month - 1] << " " << g.day << ", " << g.year << "\n";
    cout << "Ethiopian:  " << e.day << " " << monthsEC[e.month - 1] << " " << e.year << " E.C.\n";
    cout << "Weekday:    " << WEEKDAYS_EN[idx] << "\n";
    cout << "Local time: " << setw(2) << setfill('0') << lt->tm_hour << ":"
         << setw(2) << setfill('0') << lt->tm_min << ":"
         << setw(2) << setfill('0') << lt->tm_sec << setfill(' ') << "\n";
}

// ----------------------------- main -----------------------------
int main() {
    int choice;
    do {
        printMenu();
        cin >> choice;
        while (choice < 1 || choice > 10) {
            cout << "Please enter a valid choice (1-10): ";
            cin >> choice;
        }
        switch (choice) {
            case 1: showLocalDateTime(); break;
            case 2: convertDate(); break;
            case 3: addDaysToDate(); break;
            case 4: dateDifference(); break;
            case 5: showZemen(); break;
            case 6: checkLeapYear(); break;
            case 7: weekdayOfDate(); break;
            case 8: showEthiopianMonth(); break;
            case 9: showGregorianMonth(); break;
            case 10: cout << "Goodbye.\n"; break;
        }
    } while (choice != 10);
    return 0;
}
