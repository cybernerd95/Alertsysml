#ifndef ALERTSYSML_H
#define ALERTSYSML_H


#include <Arduino.h>
#include "FS.h"
#include "SPIFFS.h"
#include <vector>

class alertsysml{
    public:
        alertsysml();
        void begin(float baseval, float minNormal, float maxNormal, float minLearnable, float maxLearnable, int dur );
        String dataprocessing(float val, int min, int hour, int day);
        String loadLearnedPatterns();
        String saveLearnedPattern(float val, int min, int hour);
        String clearLearnedPatterns();
        String showLearnedPatterns();
        String addManualPattern(float val, int min, int hour);
        void updateConsecutiveReadings(float val, int min, int hour, int day);
        void setcleaning(bool cleaning);
        bool iscleaning();
        bool isInNormalRange(float val);
        bool isInLearnableRange(float val);
        bool isLearned(float val, int min, int hour);
        void setHumanIntervention(bool humanIntervention);
        bool isHumanIntervention();
        
    private :
        float baseval;
        String sentence;
        float minNormal;
        float maxNormal;
        float minLearnable;
        float maxLearnable;
        int errorfh = 0;
        int errorCount = 0;
        bool humanIntervention = false;
        int last = -1; 
        int dur;
        bool cleaning=false;

        struct LearnedEntry {
        float val;
        int min;
        int hour;
        float minRange;
        float maxRange;
        };

        struct ConsecutiveReading {
        float val;
        int min;
        int hour;
        int day;
        int count;
        };

        std::vector<LearnedEntry> learnedPatterns;
        std::vector<ConsecutiveReading> consecutiveReadings;
};

#endif 
