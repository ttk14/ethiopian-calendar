# Ethiopian and Gregorian Calendar

A calendar tool that converts dates between the Ethiopian and Gregorian calendars, both directions, and also finds the weekday, adds days, measures the gap between two dates, checks leap years, names the Ethiopian evangelist year (ዘመን), and shows a full month grid in either calendar.

The web version runs the same C++ as the command-line version. The date logic is
compiled to WebAssembly with Emscripten, so the page calls the compiled C++ rather than a separate JavaScript copy.

**Live demo:** https://ttk14.github.io/ethiopian-calendar/

## How it works

- `calendar_core.h` holds the shared date logic: the Ethiopian conversion, the weekday
  method, leap-year checks, and an add-days month-rolling loop. No input or output.
- `calendar.cpp` is the command-line program.
- `calendar_wasm.cpp` is a thin wrapper that exposes the logic to JavaScript. Dates cross
  the boundary as packed integers (year times 10000 plus month times 100 plus day).
- `index.html` is the web app. It loads the compiled module and calls the C++ functions.
- `calendar.js` and `calendar.wasm` are the Emscripten build output.
- `test_convert.cpp` cross-checks the conversion against a reference across thousands of dates.

## The conversion algorithm

The Ethiopian conversion is an original month-by-month algorithm (first written by Tarik Tadese Kassa and Hayat Sultan Mohammed in 2020). It was refactored into functions and tested against an independent reference. That testing surfaced two real bugs in the original tables: an off-by-one in the months before a leap year, and a sign bug from C++ negative remainders. Both were fixed, and the conversion now matches the reference with zero mismatches across about 28,000 dates in each direction.

## Build the WebAssembly

Requires Emscripten (`emcc`).

```bash
emcc calendar_wasm.cpp -O2 -o calendar.js \
  -sEXPORTED_FUNCTIONS='["_g2e","_e2g","_weekday_g","_is_leap_g","_is_leap_e","_add_days","_diff_days"]' \
  -sEXPORTED_RUNTIME_METHODS='["cwrap"]' \
  -sMODULARIZE=1 -sEXPORT_NAME=CalendarModule \
  -sENVIRONMENT=web
```

## Build and run the command-line version

```bash
g++ -std=c++11 -o calendar calendar.cpp
./calendar
```

## Run the web app locally

A browser will not load a `.wasm` file opened directly from disk, so use a small server:

```bash
python3 -m http.server 8000
```

Then open http://localhost:8000

## Credits

Original 2020 program by Tarik Tadese Kassa and Hayat Sultan Mohammed. Refactor,
testing, bug fixes, and the WebAssembly and web build by Tarik Tadese Kassa.
