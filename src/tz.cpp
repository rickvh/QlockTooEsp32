#include "tz.h"
#include "esp_log.h"
#include <string>

using namespace std;

void setTimezone(const string &newZone) {
    for(const zoneinfo_t zoneinfo : TIMEZONES) {
        if (zoneinfo.zone.compare(newZone) == 0) {
            
        }
    }
}

string getTimezone() {
    return temp_fixed_tz;
}
