#include <Codes/Time/intervals.h>

#include <Codes/Time/time.h>
#include <Codes/Debug/print.h>

void Intervals::updateIntervals() {
    for (Interval &interval: intervalList) {
        if (!interval.finished && Time::getCurrentTime() - interval.createdWhen > (interval.looped + 1) * interval.delay) {
            interval.looped++;
            interval.intervalFunction();
            if (interval.looped >= interval.numberOfLoops) {
                interval.finished = true;
            }
        }
    }
}

int Intervals::setInterval(float delay, int numberOfLoops, void (*intervalFunction)(void)) {
    int index = -1;
    for (std::size_t i = 0; i < intervalList.size(); i++) {
        if (intervalList[i].finished) {
            index = i;
            break;
        }
    }

    Interval interval;
    interval.createdWhen = Time::getCurrentTime();
    interval.delay = delay;
    interval.numberOfLoops = numberOfLoops;
    interval.intervalFunction = intervalFunction;
    interval.id = latestIntervalId;
    latestIntervalId++;
    if (latestIntervalId > 1000000) {
        PRINTLN("interval id pass 1000000");
    }

    if (index != -1) {
        intervalList[index] = interval;
    } else {
        intervalList.push_back(interval);
    }

    return interval.id;
}

void Intervals::stopInterval(int id) {
    for (Interval &interval: intervalList) {
        if (interval.id == id) {
            interval.finished = true;
        }
    }
}

int Intervals::wait(float delay, void (*waitFunction)(void)) {
    return setInterval(delay, 1, waitFunction);
}

void Intervals::stopWait(int id) {
    stopInterval(id);
}
