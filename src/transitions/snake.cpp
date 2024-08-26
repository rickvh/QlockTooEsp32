#include "transitions/snake.h"
#include "display.h"
#include "image.h"

using namespace qlocktoo;

void Snake::setup() {
    /**
     * 
     * calculate which words don't exist in the new time
     * calculate which words don't exist in the old time
     * 
     * randomly pick one word that existed in the old time, but not in the new time -> that's our snake
     * all other words that no longer exist are targets the snake must eat.
     * 
     * Fade our snake to green
     * 
     * 
     * 
     * - each word must have a head and tail. The snake will target the tail and advances to the head.
     * - when the target is caught, the tail of the snake will not pass beyond the tail of the target (in case our snake is shorter than the total characters of target word)
     * - when our snake reaches the head of the target, the snake will shrink if nescessary to align with the target word 
     * 
     * 
    */
}

void Snake::update() {
    frame++;
    ESP_LOGD(LOG_TAG, "show frame %u", frame);
    Display::clear();

    switch (frame) {
        case 1:
            // from->show();
            break;
        case 2:
            // to->show();
            done = true;
            break;
        default:
            break;
    }

    delay(5000);
}
