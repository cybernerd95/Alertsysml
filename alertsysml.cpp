#include "alertsysml.h"
#include <Arduino.h>
#include "FS.h"
#include "SPIFFS.h"
#include <vector>

alertsysml::alertsysml (){}
void alertsysml::begin (float baseval, float minNormal, float maxNormal, float minLearnable, float maxLearnable, int dur){
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed");
        return;
    }
    loadLearnedPatterns();
    this->baseval = baseval;
    this->minNormal = minNormal;
    this->maxNormal = maxNormal;
    this->minLearnable = minLearnable;
    this->maxLearnable = maxLearnable;
    this->dur = dur;
}

String alertsysml::dataprocessing(float val, int min, int hour, int day) {
    // Clear previous sentence
    sentence = "";
    // Handle invalid input
    if (isnan(val)) {
        sentence = "❌ Invalid val input (Day " + String(day) + ", Min " + String(min) + ", Hour " + String(hour) + ")";
        return sentence;
    }

    // Check if reading is valid
    bool isNormal = isInNormalRange(val);
    bool isLearnedVal = isLearned(val, min, hour);
    bool isOK = isNormal || isLearnedVal;

    // Handle human intervention
    if (humanIntervention) {
        humanIntervention = false;
        errorCount = 0;
        sentence = "🧍 [Day " + String(day) + ", Hour " + String(hour) + ", Min " + String(min) + "] Human: " + String(val, 2) + "°C";
        return sentence;
    }

    // Handle abnormal reading
    if (!isOK && !cleaning) {
        errorCount++;
        sentence = "⚠️ ERROR [Day " + String(day) + ", Hour " + String(hour) + ", Min " + String(min) + "]: "+String(val, 2) + "°C (count=" + String(errorCount) + ")";

        if (isInLearnableRange(val)) {
            updateConsecutiveReadings(val, min, hour, day);
        }

        if (errorCount >= 3) {
            errorCount = 0;
            errorfh++;
            return "🚨 ALERT: 3 consecutive abnormal readings!";
        }

        return sentence;
    }
    if (min % dur == 0 && min != last) {
        last = min;
        if (errorCount == 0 || cleaning) {
            sentence = "🟢 [Day " + String(day) + ", Hour " + String(hour) +", Min " + String(min) + "] OK (errors: " + String(errorfh) + ")";
            errorfh = 0;
            return sentence;
        } else {
            sentence = "🟡 [Day " + String(day) + ", Hour " + String(hour) +", Min " + String(min) + "] Current error count: " + String(errorCount);
            errorCount = 0;
            return sentence;
        }
        
    }
    return "";
}



String alertsysml::loadLearnedPatterns (){
    learnedPatterns.clear();

    File file = SPIFFS.open("/data.txt");
    if (!file) return "❌ Failed to open file";

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.length() == 0) continue;
        int comma1 = line.indexOf(',');
        int comma2 = line.indexOf(',', comma1 + 1);
        int comma3 = line.indexOf(',', comma2 + 1);
        int comma4 = line.indexOf(',', comma3 + 1);

        if (comma1 > 0 && comma2 > comma1 && comma3 > comma2 && comma4 > comma3) {
            float val = line.substring(0, comma1).toFloat();
            int min = line.substring(comma1 + 1, comma2).toInt();
            int hour = line.substring(comma2 + 1, comma3).toInt();
            float minRange = line.substring(comma3 + 1, comma4).toFloat();
            float maxRange = line.substring(comma4 + 1).toFloat();
            if (!isnan(val) && hour >= 0 && hour < 24 && min >=0 && min <= 59 ) {
                learnedPatterns.push_back({val, min, hour, minRange, maxRange});
            }
        }
    }
    file.close();
    return "✅ Loaded learned patterns";
}

String alertsysml::saveLearnedPattern (float val, int min, int hour){
    float minRange = val - 0.5;
    float maxRange = val + 0.5;
    File file = SPIFFS.open("/data.txt", FILE_APPEND);
    if (file) {
        file.printf("%.2f,%d,%d,%.2f,%.2f\n", val, min, hour, minRange, maxRange);
        file.close();
        learnedPatterns.push_back({val, min, hour, minRange, maxRange});
        sentence = "🤖 Learned: " + String(val) + "°C at min "+ min +" Hour " + hour;
        errorCount=0;
        return sentence;
    }else {
        return "❌ Failed to open file for writing";
    }
}


String alertsysml::clearLearnedPatterns (){
  if (!SPIFFS.begin(true)) return "❌ SPIFFS mount failed";
  SPIFFS.remove("/data.txt");
  learnedPatterns.clear();
  consecutiveReadings.clear();
  sentence = "🧹 All patterns cleared";
  return sentence;
}


String alertsysml::showLearnedPatterns (){
    sentence = "🤖 Learned Patterns:\n";
  for (auto& e : learnedPatterns) {
    sentence += "✓ Min minute" + String(e.min - 10) +": Maximum Minute  " + String(e.min + 10) +": Hour " + String(e.hour) +": " + String(e.val, 2) + "°C" +" (range: " + String(e.minRange, 2) + "–" + String(e.maxRange, 2) + ")\n";
  }
  return sentence;
  
}


String alertsysml::addManualPattern (float val, int min, int hour){
    if (min < 0 || min > 60 || isnan(val) || hour < 0 || hour > 23 ) {
    return ("❌ Invalid input");
    }
    saveLearnedPattern(val, min, hour);
    return ("Pattern Added manually ") ;
}

void alertsysml::updateConsecutiveReadings (float val, int min, int hour, int day){
    for (auto& r : consecutiveReadings) {
    if(r.min > min-10 && r.min < min+10){
      if (r.hour == hour) {
        if (day == r.day + 1 && abs(val - r.val) <= 0.5) {
          r.count++; r.day = day; r.val = val; 
          if (r.count >= 3) {
            saveLearnedPattern(val, min, hour);
            r.count = 0; r.day = -1;
          }
          return;
        } else if (day == r.day + 1 || day > r.day + 1) {
          r.val = val; r.day = day; r.count = 1;
          return;
        }
      }
    }    
  }
  consecutiveReadings.push_back({val, min, hour, day, 1});
}

void alertsysml::setcleaning(bool cleaning) {
    this->cleaning = cleaning;
}

bool alertsysml::iscleaning() {
    return cleaning;
}

bool alertsysml::isInNormalRange(float val) {
  return (val >= minNormal && val <= maxNormal);
}

bool alertsysml::isInLearnableRange(float val) {
  return (val >= minLearnable && val <= maxLearnable && !isInNormalRange(val));
}

bool alertsysml::isLearned(float val, int min, int hour) {
  for (auto& entry : learnedPatterns) {
    if (min > (entry.min-10) && min < (entry.min+10) && hour == entry.hour && val >= entry.minRange && val <= entry.maxRange) {
      return true;
    }
  }
  return false;
}

void alertsysml::setHumanIntervention(bool humanIntervention) {
    this->humanIntervention = humanIntervention;
}

bool alertsysml::isHumanIntervention() {
    return humanIntervention;
}
