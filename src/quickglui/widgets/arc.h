/****
 * QuickGLUI - Quick Graphical LVLGL-based User Interface development library
 * Copyright  2020  Skurydin Alexey
 * http://github.com/anakod
 * Copyright  2021  Guilhem Bonnefille <guilhem.bonnefille@gmail.com>
 * All QuickGLUI project files are provided under the MIT License.
 ****/

#ifndef ARC_H_
#define ARC_H_

#include "widget.h"

#ifdef NATIVE_64BIT
    #include <string>
    using namespace std;
    #define String string
#else
        #include <Arduino.h>
    #ifdef M5PAPER
    #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 ) || defined( LILYGO_WATCH_2019_WITH_TOUCH )
    #endif
#endif

/**
 * @brief Represent Arc LVGL widget.
 * Documentation:
 * https://docs.lvgl.io/latest/en/html/widgets/arc.html
 */
class Arc : public Widget {

public:
    Arc(){};
    /** Create an arc from lvgl object */
    Arc(lv_obj_t* handle);
    /** Create an arc with text */
    Arc(const Widget* parent,  uint16_t start, uint16_t end);

    // Set arc values
    Arc& start(uint16_t start);
    Arc& end(uint16_t end);

    Arc& rotation(uint16_t rotation_angle);

    constexpr static const char* TypeName = "lv_arc";

protected:
  virtual void createObject(lv_obj_t* parent);
};

#endif