/****
 * QuickGLUI - Quick Graphical LVLGL-based User Interface development library
 * Copyright  2020  Skurydin Alexey
 * http://github.com/anakod
 * All QuickGLUI project files are provided under the MIT License.
 ****/

#ifndef JSONOPTION_H_
#define JSONOPTION_H_

#include <config.h>
#include "ArduinoJson.h"
#include "../widgets/switch.h"
#include "../widgets/textarea.h"
#include "utils/alloc.h"

#ifdef NATIVE_64BIT
    #include "utils/logging.h"
    #include <string>
    using namespace std;
    #define String string
#else
        #include <Arduino.h>
    #ifdef M5PAPER
    #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 ) || defined( LILYGO_WATCH_2019_WITH_TOUCH )
    #endif
#endif

enum OptionDataType
{
  BoolOption,
  StringOption
};

struct JsonOption {
    JsonOption(const char* optionName, OptionDataType type) {
        strncpy(name, optionName, MAX_OPTION_NAME_LENGTH);
        optionDataType = type;
    }
    virtual ~JsonOption() {}

    virtual void applyFromUI() = 0;
    virtual void save(JsonDocument& document) = 0;
    virtual void load(JsonDocument& document) = 0;

    inline OptionDataType type() { return optionDataType; }

public:
    char name[MAX_OPTION_NAME_LENGTH];
private:
    OptionDataType optionDataType;
};

struct JsonBoolOption : public JsonOption {

    JsonBoolOption(const char* optionName, bool defValue = false) : JsonOption(optionName, OptionDataType::BoolOption) {
        value = defValue;
    }

    virtual ~JsonBoolOption() {
        source = nullptr;
        isControlAssigned = false;
    }

    virtual void applyFromUI() {
        if (isControlAssigned) {
            bool currentValue = control.value();
            value = currentValue;
            if (source != nullptr)
            *source = value;
        }
    }

    virtual void save(JsonDocument& document) {
        document[name] = value;
    }

    virtual void load(JsonDocument& document) {
    if (document.containsKey(name)) {
        value = document[name].as<bool>();
    }
    else {
        value = false;
    }

    if (source != nullptr) {
      *source = value;
    }

    if (isControlAssigned) {
      control.value(value);
    }
}

    /**
     * @brief Assign settings option to the variable
     */
    JsonBoolOption& assign(bool* sourceVariable) {
        source = sourceVariable;
        if (source != nullptr) {
            *source = value;
        }
        return *this;
    }
    /**
     * @brief Assign settings option to the UI widget
     */
    JsonBoolOption& assign(Switch& sourceControl) {
        isControlAssigned = true;
        control = sourceControl;
        control.value(value);
        return *this;
    }

public:
    bool value;
    bool* source = nullptr;
    bool isControlAssigned = false;
    Switch control;
};

struct JsonStringOption : public JsonOption {

    JsonStringOption(const char* optionName, int maxValueLength, const char* defValue = nullptr) : JsonOption(optionName, OptionDataType::StringOption) {
        maxLength = maxValueLength;
        value = (char*)MALLOC_ASSERT( maxLength, "JsonStringOption allocation failed" );

        if ( value == NULL ) {
            log_e("JsonStringOprtion alloc failed");
            while(1);
        }

        if ( defValue != nullptr ) {
            strncpy(value, defValue, maxLength);
        }
        else {
            memset(value, 0, maxLength);
        }
    }

    virtual ~JsonStringOption() {
        if ( value != NULL ) {
            free(value);
        }
        value = nullptr;
        source = nullptr;
        isControlAssigned = false;
    }

    virtual void applyFromUI() {
        if (isControlAssigned) {
            String currentValue = control.text();
            strncpy(value, currentValue.c_str(), maxLength);
            if (source != nullptr)
            *source = value;
        }
    }

    virtual void save(JsonDocument& document) {
        document[name] = value;
    }

    virtual void load(JsonDocument& document) {
        if ( document.containsKey( name ) ) {
            strncpy(value, document[name], maxLength);
        }
        else {
            value[0] = '\0';
        }

        if (source != nullptr) {
            *source = value;
        }

        if (isControlAssigned) {
            control.text(value);
        }
    }

    /**
     * @brief Assign digits mode
     */
    JsonStringOption& setDigitsMode(bool onlyDigits, const char* filterDigitsList) {
        this->onlyDigits = onlyDigits;
        this->filterDigitsList = filterDigitsList;
        return *this;
    }

    /**
     * @brief Assign settings option to the variable
     */
    JsonStringOption& assign(String* sourceVariable) {
        source = sourceVariable;
        if (source != nullptr) {
            *source = value;
        }
        return *this;
    }

    /**
     * @brief Assign settings option to the UI widget
     */
    JsonStringOption& assign(TextArea& sourceControl) {
        isControlAssigned = true;
        control = sourceControl;
        control.text(value);
        // Set digits mode
        sourceControl.digitsMode(true, filterDigitsList);
        return *this;
    }

public:
    char* value;
    int maxLength;
    String* source = nullptr;
    bool isControlAssigned = false;
    TextArea control;
    bool onlyDigits = false;
    const char *filterDigitsList = nullptr;
};

#endif
