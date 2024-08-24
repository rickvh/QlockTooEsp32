#pragma once

#include <pgmspace.h>
#include <memory>
#include <array>
#include <string>
#include "app.h"
#include "display.h"
#include "image.h"

namespace qlocktoo {
class Imageviewer : public App {
   private:
    static constexpr const char* LOG_TAG = "imageviewer";
    std::unique_ptr<Image> image;

   protected:
    void setup() override;
    void loop() override;

   public:
    Imageviewer(std::unique_ptr<Image> image) : App(), image(std::move(image)) {}
    std::string name() override;
};
}