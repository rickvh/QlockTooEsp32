#include "transitions/snake.h"
#include "transitions/pixel.h"
#include "display.h"
#include <list>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cmath>

using namespace qlocktoo;

void Snake::move() {
    Display::clear();

    if (!currentTarget) {
        // pick next target
        currentTarget = &(*targets.begin());
        ESP_LOGD(LOG_TAG, "New target: %u, %u", currentTarget->coordinate.x, currentTarget->coordinate.y);
        calculatePathToNextTarget();
    }
    
    if (pathToNextTarget.empty()) {
        ESP_LOGE(LOG_TAG, "No path to target");
        return;
    }

    // move snake 1 step towards currentTarget
    Coordinate nextStep = pathToNextTarget.front();
    ESP_LOGD(LOG_TAG, "move to %u, %u", nextStep.x, nextStep.y);
    pathToNextTarget.pop_front();    
    body.push_front(Pixel(nextStep, bodyColor));
    body.pop_back();

    // check if snake hits _a_ target. This could also be a target that's not our current target!

// TODO: wanneer target wordt gehit, wordt deze hieronder niet daadwerkelijk gevonden en opgeruimd.

    auto found = std::find(targets.begin(), targets.end(), body.front());
    ESP_LOGD(LOG_TAG, "found != targets.end(): %s", found != targets.end() ? "true" : "false");
    if (found != targets.end()) {
        // check if target is our current target
        if (body.front() == *currentTarget) {
            currentTarget = nullptr;
        }
        targets.erase(found);
    }
    
    draw();
}

// Implements the A-star path-finding algorithm
void Snake::calculatePathToNextTarget() {
    // Create a priority queue for the nodes to be visited
    std::unordered_map<Coordinate, Coordinate> cameFrom;
    std::unordered_map<Coordinate, float> gScore;
    std::unordered_map<Coordinate, float> fScore;
    auto cmp = [&fScore](const Coordinate& a, const Coordinate& b) { return fScore[a] > fScore[b]; };
    std::priority_queue<Coordinate, std::vector<Coordinate>, decltype(cmp)> openList(cmp);
    
    // Add the current position of the snake to the open list
    Coordinate current = body.front().coordinate;
    gScore[current] = 0;
    fScore[current] = current.manhattanDistanceTo(currentTarget->coordinate);
    openList.push(current);

    while (!openList.empty()) {
        // Get the next node to visit (the one with the lowest fScore)
        current = openList.top();
        openList.pop();

        if (current == currentTarget->coordinate) {
            // We have reached the target, so we can reconstruct the path
            Coordinate current = currentTarget->coordinate;
            while (current != body.front().coordinate) {
                pathToNextTarget.push_front(current);
                current = cameFrom[current];
            }
            return;
        }

        // Generate the list of adjacents pixels
        std::vector<Coordinate> adjacents;
        adjacents.push_back(Coordinate(current.x - 1, current.y));
        adjacents.push_back(Coordinate(current.x + 1, current.y));
        adjacents.push_back(Coordinate(current.x, current.y - 1));
        adjacents.push_back(Coordinate(current.x, current.y + 1));

        for (auto& neighbor : adjacents) {
            if (neighbor.x < 0 || neighbor.x >= Display::WIDTH || neighbor.y < 0 || neighbor.y >= Display::HEIGHT) {
                continue;
            }
            // snake cannot cross it's own body
            if (std::find(body.begin(), body.end(), neighbor) != body.end()) {
                continue;
            }
            
            // compute the tentative gScore
            float penalty = 1;
            // obstacles shouldn't be passed through
            if (std::find(obstacles.begin(), obstacles.end(), neighbor) != obstacles.end()) {    
                penalty = 500;
            }
            float tentativeGScore = gScore[current] + current.manhattanDistanceTo(neighbor) * penalty;
            if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                fScore[neighbor] = gScore[neighbor] + neighbor.manhattanDistanceTo(currentTarget->coordinate);
                openList.push(neighbor);
            }
        }
    }
    // If we reach here, there's no path to the target
    pathToNextTarget.clear();
}

void Snake::draw() const {
    for (auto& coordinate : pathToNextTarget) {
        Display::drawPixel(coordinate.x, coordinate.y, HsbColor(0.0f, 1.0f, 0.01f));
    }
    for (auto& pixel : body) {
        Display::drawPixel(pixel.coordinate.x, pixel.coordinate.y, HsbColor(0.2f, 1.0f, 0.5f));
    }
    for (auto& pixel : obstacles) {
        Display::drawPixel(pixel.coordinate.x, pixel.coordinate.y, HsbColor(0.4f, 1.0f, 0.05f));
    }
    for (auto& pixel : targets) {
        Display::drawPixel(pixel.coordinate.x, pixel.coordinate.y, HsbColor(0.6f, 1.0f, 0.5f));
    }
    Display::show();
}