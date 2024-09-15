#include "apps/imageviewer.h"
#include "display.h"

using namespace qlocktoo;

std::string Imageviewer::name() {
    return "Imageviewer";
}

void Imageviewer::setup() {
    Display::drawImage(image);
    Display::show();
}

void Imageviewer::loop() {
    delay(1000);
}