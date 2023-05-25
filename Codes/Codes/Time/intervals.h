#ifndef INTERVALS_H
#define INTERVALS_H

#include <vector>

class Intervals {
public:
    void updateIntervals();

    int setInterval(float delay, int numberOfLoops, void (*intervalFunction)(void));
    void stopInterval(int id);
    int wait(float delay, void (*waitFunction)(void));
    void stopWait(int id);
    
private:
    int latestIntervalId = 0;

    struct Interval {
        void (*intervalFunction)(void);
        float delay = 0;
        int looped = 0;
        int numberOfLoops = 0;
        float createdWhen = 0;
        bool finished = false;
        int id = -1;
    };
    std::vector<Interval> intervalList;
};

#endif
