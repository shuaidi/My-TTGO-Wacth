/****
 * QuickGLUI - Quick Graphical LVLGL-based User Interface development library
 * Copyright  2020  Skurydin Alexey
 * http://github.com/anakod
 * All QuickGLUI project files are provided under the MIT License.
 ****/

#ifndef JSONREQUEST_H
#define JSONREQUEST_H

#include "ArduinoJson.h"
#include "utils/json_psram_allocator.h"

#ifdef NATIVE_64BIT
    #include <string>
    using namespace std;
    #define String string
#else
    #ifdef M5PAPER
    #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 ) || defined( LILYGO_WATCH_2019_WITH_TOUCH )
    #endif
#endif

/**
 * @brief HTTP request wrapper with internal JSON parser.
 * Reponse should be in JSON format
 */
class JsonRequest : public SpiRamJsonDocument {
public:
    JsonRequest(size_t maxJsonBufferSize);
    ~JsonRequest();

    bool process(const char* url);

    int httpCode() { return httpcode; }
    DeserializationError getDeserializationError() { return dsError; }

    tm completedAt() { return timeStamp; }
    String formatCompletedAt(const char* format);
    String errorString();

protected:
    int httpcode = -1;
    time_t now;
    struct tm timeStamp;
    DeserializationError dsError;
};

#endif

