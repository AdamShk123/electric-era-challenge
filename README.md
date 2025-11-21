# Electric Era Challenge

A C++23 application for calculating EV charging station uptime percentages from availability reports.

## Overview

This program processes charging station availability data and calculates uptime percentages for each station. It handles:
- Multiple chargers per station
- Overlapping availability reports
- Time gaps between reports (counted as downtime)
- Different states (up/down) within the same time period

## Dependencies

- **C++ Compiler**: GCC 15.2 or higher (C++23 support required)
- **CMake**: 4.0.2 or higher
- **Google Test**: For unit tests (automatically fetched by CMake)

## Building

### Compile the project:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Run tests:
```bash
cd build/test
ctest --output-on-failure
```

## Usage

```bash
./build/src/challenge <input_file>
```

Example:
```bash
./build/src/challenge /examples/input_1.txt
```

## Input Format

```
[Stations]
<station_id> <charger_id_1> <charger_id_2> ...

[Charger Availability Reports]
<charger_id> <start_time> <end_time> <true|false>
```

## Output Format

```
<station_id> <uptime_percentage>
```

Sorted by station ID, with uptime as an integer percentage (0-100).

## Code Structure

### Core Functions

- **`parseStationChargers`**: Parses station-to-charger mappings
- **`parseAvailabilityReports`**: Parses charger availability time intervals
- **`mergeIntervals`**: Merges overlapping time intervals, splitting when states differ
- **`resolvePercentage`**: Calculates uptime percentage (gaps count as downtime)
- **`produceUptimeResults`**: Orchestrates the calculation pipeline

### Algorithm

1. **Parse Input**: Extract station mappings and availability reports
2. **Merge Intervals**: For each station, combine all charger intervals:
   - Sort by start time
   - Merge consecutive intervals with same state
   - Split overlapping intervals with different states
3. **Calculate Percentage**:
   - Total time = all interval durations + gaps between them
   - Available time = sum of all "up" interval durations
   - Percentage = (available / total) × 100