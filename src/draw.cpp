#include <Arduino.h>
#include "draw.h"
#include "display.h"

using namespace qlocktoo;

void Draw::setup() {
    image->show();
}

void Draw::loop() {
    Command command;
    if (xQueueReceive(xDrawQueue, &command, 0) == pdTRUE) {
        switch (command.operation) {
            case Operation::READ:
                image->readFile("/img/test.spr");
                break;
            case Operation::WRITE:
                image->writeFile("/img/test.spr");
                break;
            case Operation::DRAW:
                image->setColor(command.x, command.y, RgbwColor(command.r, command.g, command.b, command.w));
                break;
            case Operation::CLEAR:
                delete image;
                image = new Image(Debug);
                break;
            default:
                break;
        }

        image->show();
    }

    delay(100);
}