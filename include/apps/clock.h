#pragma once

#include <memory>
#include <vector>
#include "app.h"
#include "display.h"
#include "configservice.h"
#include "coordinate.h"
#include "transitions/transition.h"

namespace qlocktoo {
class Clock : public App {
   private:
    static constexpr const char* LOG_TAG = "clock";

    const std::vector<Coordinate> HETIS = {{0, 9}, {1, 9}, {2, 9}, {4, 9}, {5, 9}};
    const std::vector<Coordinate> VIJF = {{7, 9}, {8, 9}, {9, 9}, {10, 9}};
    const std::vector<Coordinate> TIEN = {{0, 8}, {1, 8}, {2, 8}, {3, 8}};
    const std::vector<Coordinate> KWART = {{6, 7}, {7, 7}, {8, 7}, {9, 7}, {10, 7}};
    const std::vector<Coordinate> VOOR1 = {{7, 8}, {8, 8}, {9, 8}, {10, 8}};
    const std::vector<Coordinate> OVER1 = {{0, 7}, {1, 7}, {2, 7}, {3, 7}};
    const std::vector<Coordinate> HALF = {{0, 6}, {1, 6}, {2, 6}, {3, 6}};
    const std::vector<Coordinate> UUR = {{8, 0}, {9, 0}, {10, 0}};
    const std::vector<Coordinate> VOOR2 = {{0, 5}, {1, 5}, {2, 5}, {3, 5}};
    const std::vector<Coordinate> OVER2 = {{7, 6}, {8, 6}, {9, 6}, {10, 6}};
    const std::vector<std::vector<Coordinate>> GETAL = {
        {{}},
        {{7, 5}, {8, 5}, {9, 5}},
        {{0, 4}, {1, 4}, {2, 4}, {3, 4}},
        {{7, 4}, {8, 4}, {9, 4}, {10, 4}},
        {{0, 3}, {1, 3}, {2, 3}, {3, 3}},
        {{4, 3}, {5, 3}, {6, 3}, {7, 3}},
        {{8, 3}, {9, 3}, {10, 3}},
        {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}},
        {{0, 1}, {1, 1}, {2, 1}, {3, 1}},
        {{6, 2}, {7, 2}, {8, 2}, {9, 2}, {10, 2}},
        {{4, 1}, {5, 1}, {6, 1}, {7, 1}},
        {{8, 1}, {9, 1}, {10, 1}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}}
    };

    struct tm previousTime, currentTime;
    uint8_t timeBrightness();
    void setColor(Image &image, const std::vector<Coordinate> &coordinates, HsbColor color);
    std::unique_ptr<Transition> transition;
    Image getImageFromTime(const tm &time);

   protected:
    void setup() override;
    void loop() override;

   public:
    Clock() : App() {};
    std::string name() override;
};
}