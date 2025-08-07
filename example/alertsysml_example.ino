/*
  AlertSysML Library Test Example
  This example demonstrates all functions of the AlertSysML library
  
  Hardware Required: ESP32 with SPIFFS support
  
  This example will:
  1. Initialize the alert system
  2. Test all public functions
  3. Simulate various sensor scenarios
  4. Demonstrate pattern learning

  THE LIBRARY AUTHORED BY MALLADI ROHIT 
*/

#include "alertsysml.h"

alertsysml sensor;



int testDay = 1;
int testHour = 10;
int testMinute = 0;
int testPhase = 0;
unsigned long lastTest = 0;
const unsigned long TEST_INTERVAL = 5000; 

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("=== AlertSysML Library Test ===");
  Serial.println("Initializing...");
  


  sensor.begin(25.0, 20.0, 30.0, 15.0, 40.0, 5);
  
  Serial.println("Setup complete. Starting tests...\n");
}

void loop() {
  if (millis() - lastTest >= TEST_INTERVAL) {
    lastTest = millis();
    
    switch (testPhase) {
      case 0:
        testBasicFunctions();
        break;
      case 1:
        testNormalReadings();
        break;
      case 2:
        testAbnormalReadings();
        break;
      case 3:
        testPatternLearning();
        break;
      case 4:
        testManualPatterns();
        break;
      case 5:
        testHumanIntervention();
        break;
      case 6:
        testCleaningMode();
        break;
      case 7:
        testErrorScenarios();
        break;
      case 8:
        testPatternMatching();
        break;
      case 9:
        testPatternManagement();
        break;
      case 10:
        testRangeChecking();
        break;
      default:
        Serial.println("\n=== ALL TESTS COMPLETED ===");
        Serial.println("Restarting test cycle in 10 seconds...");
        delay(10000);
        testPhase = 0;
        testDay = 1;
        testHour = 10;
        testMinute = 0;
        return;
    }
    
    testPhase++;
    Serial.println("Press any key to continue to next test...");
    Serial.println("----------------------------------------");
  }
}

void testBasicFunctions() {
  Serial.println("TEST 1: Basic Functions");
  String result = sensor.loadLearnedPatterns();
  if (result != "") Serial.println("Load Patterns: " + result);
  result = sensor.showLearnedPatterns();
  if (result != "") Serial.println("Show Patterns: " + result);
  
  Serial.println("✅ Basic functions test complete");
}

void testNormalReadings() {
  Serial.println("TEST 2: Normal Readings");

  float normalValues[] = {22.5, 25.0, 28.3, 20.1, 29.9};
  
  for (int i = 0; i < 5; i++) {
    String result = sensor.dataprocessing(normalValues[i], testMinute, testHour, testDay);
    if (result != "") Serial.println("Normal Reading: " + result);
    
    testMinute += 5;
    if (testMinute >= 60) {
      testMinute = 0;
      testHour++;
    }
  }
  
  Serial.println("✅ Normal readings test complete");
}

void testAbnormalReadings() {
  Serial.println("TEST 3: Abnormal Readings");

  float abnormalValues[] = {35.0, 18.5, 32.1};
  
  for (int i = 0; i < 3; i++) {
    String result = sensor.dataprocessing(abnormalValues[i], testMinute, testHour, testDay);
    if (result != "") Serial.println("Abnormal Reading: " + result);
    
    testMinute += 1;
  }

  Serial.println("\nTesting consecutive abnormal readings:");
  for (int i = 0; i < 3; i++) {
    String result = sensor.dataprocessing(35.5, testMinute, testHour, testDay);
    if (result != "") Serial.println("Consecutive #" + String(i+1) + ": " + result);
    testMinute += 1;
  }
  
  Serial.println("✅ Abnormal readings test complete");
}

void testPatternLearning() {
  Serial.println("TEST 4: Pattern Learning");

  float learnableValue = 16.5; 
  int fixedMinute = 15;
  int fixedHour = 14;
  
  for (int day = 1; day <= 4; day++) {
    String result = sensor.dataprocessing(learnableValue, fixedMinute, fixedHour, day);
    if (result != "") Serial.println("Day " + String(day) + " Learning: " + result);
  }
  
  Serial.println("✅ Pattern learning test complete");
}

void testManualPatterns() {
  Serial.println("TEST 5: Manual Pattern Management");

  String result = sensor.addManualPattern(17.5, 30, 12);
  if (result != "") Serial.println("Add Manual Pattern: " + result);

  result = sensor.addManualPattern(17.5, 70, 12);
  if (result != "") Serial.println("Invalid Pattern: " + result);

  result = sensor.saveLearnedPattern(19.0, 45, 16);
  if (result != "") Serial.println("Save Pattern: " + result);

  result = sensor.showLearnedPatterns();
  if (result != "") Serial.println("All Patterns:\n" + result);
  
  Serial.println("✅ Manual pattern management test complete");
}

void testHumanIntervention() {
  Serial.println("TEST 6: Human Intervention");

  sensor.setHumanIntervention(true);
  Serial.println("Human intervention flag set: " + String(sensor.isHumanIntervention()));

  String result = sensor.dataprocessing(35.0, testMinute, testHour, testDay);
  if (result != "") Serial.println("Human Intervention: " + result);

  Serial.println("Human intervention flag after processing: " + String(sensor.isHumanIntervention()));
  
  Serial.println("✅ Human intervention test complete");
}

void testCleaningMode() {
  Serial.println("TEST 7: Cleaning Mode");

  Serial.println("Cleaning mode initially: " + String(sensor.iscleaning()));

  sensor.setcleaning(true);
  Serial.println("Cleaning mode after setting: " + String(sensor.iscleaning()));

  String result = sensor.dataprocessing(35.0, testMinute, testHour, testDay);
  if (result != "") Serial.println("Cleaning Mode Reading: " + result);

  sensor.setcleaning(false);
  Serial.println("Cleaning mode after disabling: " + String(sensor.iscleaning()));
  
  Serial.println("✅ Cleaning mode test complete");
}

void testErrorScenarios() {
  Serial.println("TEST 8: Error Scenarios");

  String result = sensor.dataprocessing(NAN, testMinute, testHour, testDay);
  if (result != "") Serial.println("NaN Input: " + result);

  result = sensor.dataprocessing(50.0, testMinute, testHour, testDay);
  if (result != "") Serial.println("Extreme Value: " + result);
  
  result = sensor.dataprocessing(5.0, testMinute, testHour, testDay);
  if (result != "") Serial.println("Extreme Low Value: " + result);
  
  Serial.println("✅ Error scenarios test complete");
}

void testPatternMatching() {
  Serial.println("TEST 9: Pattern Matching");

  sensor.addManualPattern(18.5, 25, 11);

  bool isLearned = sensor.isLearned(18.3, 27, 11); 
  Serial.println("Pattern match test 1 (should be true): " + String(isLearned));

  isLearned = sensor.isLearned(18.3, 27, 12);
  Serial.println("Pattern match test 2 (should be false): " + String(isLearned));

  isLearned = sensor.isLearned(20.0, 27, 11);
  Serial.println("Pattern match test 3 (should be false): " + String(isLearned));
  
  Serial.println("✅ Pattern matching test complete");
}

void testPatternManagement() {
  Serial.println("TEST 10: Pattern Management");
  
  String result = sensor.showLearnedPatterns();
  if (result != "") Serial.println("Current Patterns:\n" + result);

  result = sensor.clearLearnedPatterns();
  if (result != "") Serial.println("Clear Patterns: " + result);

  result = sensor.showLearnedPatterns();
  if (result != "") Serial.println("Patterns After Clear:\n" + result);
  
  Serial.println("✅ Pattern management test complete");
}

void testRangeChecking() {
  Serial.println("TEST 11: Range Checking Functions");
  Serial.println("Normal range tests:");
  Serial.println("25.0°C in normal range: " + String(sensor.isInNormalRange(25.0)));
  Serial.println("35.0°C in normal range: " + String(sensor.isInNormalRange(35.0)));

  Serial.println("Learnable range tests:");
  Serial.println("35.0°C in learnable range: " + String(sensor.isInLearnableRange(35.0)));
  Serial.println("25.0°C in learnable range: " + String(sensor.isInLearnableRange(25.0))); 
  Serial.println("50.0°C in learnable range: " + String(sensor.isInLearnableRange(50.0))); 
  
  Serial.println("✅ Range checking test complete");
}

float simulateTemperature() {

  return 25.0 + random(-100, 100) / 100.0;
}