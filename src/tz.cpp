#include "tz.h"
#include "esp_log.h"
#include <string>

void setTimezone(const std::string &newZone) {
    for(const zoneinfo_t zoneinfo : TIMEZONES) {
        if (zoneinfo.zone.compare(newZone) == 0) {
            
        }
    }
}

std::string getTimezone() {
    return temp_fixed_tz;
}
