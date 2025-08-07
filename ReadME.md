# AlertSysML Library 

THE LIBRARY AUTHORED BY MALLADI ROHIT 
A smart Arduino library for ESP32 that provides intelligent alert system capabilities with machine learning-like pattern recognition for sensor monitoring applications.

## Overview

The AlertSysML library monitors sensor values (primarily temperature) and learns normal patterns over time. It can distinguish between normal readings, learned acceptable patterns, and truly abnormal values that require alerts. The library uses SPIFFS for persistent storage of learned patterns.

## Features

- **Intelligent Pattern Learning**: Automatically learns acceptable sensor patterns based on time and consecutive readings
- **Multiple Alert Levels**: Distinguishes between warnings and critical alerts
- **Persistent Storage**: Saves learned patterns to SPIFFS for persistence across reboots  
- **Human Intervention Support**: Handles manual interventions and cleaning modes
- **Configurable Ranges**: Customizable normal and learnable value ranges
- **Time-based Pattern Recognition**: Associates patterns with specific times (hour/minute)

## Installation

1. Copy `alertsysml.h` and `alertsysml.cpp` to your Arduino libraries folder
2. Include the library in your sketch: `#include "alertsysml.h"`
3. Ensure your ESP32 has SPIFFS enabled

## Basic Usage

Include the library and initialize with appropriate parameters. Always check return strings before processing them.

## Function Reference

### Constructor & Initialization

#### `alertsysml()`
Default constructor for the AlertSysML class.

#### `void begin(float baseval, float minNormal, float maxNormal, float minLearnable, float maxLearnable, int dur)`
Initializes the alert system with configuration parameters.

**Parameters:**
- `baseval`: Base reference value (currently unused in processing)
- `minNormal`: Minimum value considered normal
- `maxNormal`: Maximum value considered normal  
- `minLearnable`: Minimum value that can be learned as acceptable
- `maxLearnable`: Maximum value that can be learned as acceptable
- `dur`: Duration interval in minutes for status reports

### Core Processing

#### `String dataprocessing(float val, int min, int hour, int day)`
Main processing function that analyzes sensor readings and returns status messages.

**Parameters:**
- `val`: Sensor value to process
- `min`: Current minute (0-59)
- `hour`: Current hour (0-23)
- `day`: Current day number

**Returns:** Status string (empty string if no message to display)

**Status Messages:**
- `🟢` OK status (at intervals)
- `🟡` Current error count
- `⚠️` ERROR warning with count
- `🚨` ALERT for 3 consecutive errors
- `❌` Invalid input error
- `🧍` Human intervention logged
- `🤖` Pattern learned

### Pattern Management

#### `String loadLearnedPatterns()`
Loads previously saved patterns from SPIFFS storage.

**Returns:** Success/failure message

#### `String saveLearnedPattern(float val, int min, int hour)`
Manually saves a pattern as acceptable.

**Parameters:**
- `val`: Value to save
- `min`: Minute when pattern occurs
- `hour`: Hour when pattern occurs

**Returns:** Success/failure message

#### `String clearLearnedPatterns()`
Removes all learned patterns from memory and storage.

**Returns:** Confirmation message

#### `String showLearnedPatterns()`
Returns a formatted list of all currently learned patterns.

**Returns:** Formatted pattern list

#### `String addManualPattern(float val, int min, int hour)`
Adds a pattern manually with validation.

**Parameters:**
- `val`: Value to add
- `min`: Minute (0-60)  
- `hour`: Hour (0-23)

**Returns:** Success/failure message

### Utility Functions

#### `void updateConsecutiveReadings(float val, int min, int hour, int day)`
Internal function that tracks consecutive readings for automatic pattern learning.

#### `void setcleaning(bool cleaning)`
Sets cleaning mode on/off. During cleaning mode, abnormal readings are treated as normal.

#### `bool iscleaning()`
Returns current cleaning mode status.

#### `void setHumanIntervention(bool humanIntervention)`
Flags the next reading as a human intervention.

#### `bool isHumanIntervention()`
Returns current human intervention flag status.

### Range Checking Functions

#### `bool isInNormalRange(float val)`
Checks if value is within normal operating range.

#### `bool isInLearnableRange(float val)`
Checks if value is within learnable range (abnormal but potentially acceptable).

#### `bool isLearned(float val, int min, int hour)`
Checks if a value matches any learned pattern for the given time.

## Pattern Learning Logic

The library automatically learns patterns when:
1. A value is outside normal range but within learnable range
2. The same abnormal pattern occurs for 3+ consecutive days at similar times
3. Values are within ±0.5 units and ±10 minutes of each other

Manual patterns can also be added using `addManualPattern()`.

## Error Handling

- **3 Consecutive Errors**: Triggers critical alert
- **Invalid Inputs**: NaN values and out-of-range times are handled gracefully  
- **SPIFFS Failures**: Reported with appropriate error messages
- **File Operations**: All file operations include error checking

## Time Pattern Matching

Patterns are matched with:
- **Hour**: Exact match required
- **Minute**: ±10 minute window around learned time
- **Value**: ±0.5 unit range around learned value

## Storage Format

Learned patterns are stored in `/data.txt` on SPIFFS in CSV format with fields: value, minute, hour, minRange, maxRange.

## ⚠️ IMPORTANT DISCLAIMER

**When using this library, you MUST implement proper string handling in your main code. Always check if the returned string is empty before printing or processing it.**

**Failure to check for empty strings may cause:**
- Unnecessary serial output
- Performance issues
- Unintended behavior in your application

The library returns empty strings intentionally when no status message is needed. Your application should respect this design and only process non-empty return values.

## Example Applications

- Temperature monitoring systems
- Industrial sensor networks  
- Environmental monitoring
- Equipment health monitoring
- Any time-based sensor pattern recognition

## Requirements

- ESP32 microcontroller
- Arduino IDE with ESP32 support
- SPIFFS file system support
- Sufficient flash memory for pattern storage

## License

This library is provided as-is for educational and development purposes.

## Version History

- v1.0: Initial release with core functionality