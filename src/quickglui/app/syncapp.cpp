/****
 * QuickGLUI - Quick Graphical LVLGL-based User Interface development library
 * Copyright  2020  Skurydin Alexey
 * http://github.com/anakod
 * All QuickGLUI project files are provided under the MIT License.
 ****/

#include "syncapp.h"
#include <config.h>

#ifdef NATIVE_64BIT
#else
    #ifdef M5PAPER
    #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 ) || defined( LILYGO_WATCH_2019_WITH_TOUCH )
    #endif
#endif

SynchronizedApplication::SynchronizedApplication() {
    #ifdef NATIVE_64BIT
    #else
        #ifdef M5PAPER
        #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 ) || defined( LILYGO_WATCH_2019_WITH_TOUCH )
        #endif
            syncEvent = xEventGroupCreate();
    #endif
}

Application& SynchronizedApplication::init(const char* name, const lv_img_dsc_t *iconImg, int userPageCount, int settingsPageCount) {
    this->init(name, iconImg, true, userPageCount, settingsPageCount);
    return *this;
}

SynchronizedApplication& SynchronizedApplication::init(const char* name, const lv_img_dsc_t *iconImg, bool addSyncButton, int userPageCount, int settingsPageCount) {
    Application::init(name, iconImg, userPageCount, settingsPageCount);
    title = name + String(" sync Task");

    if (addSyncButton)
    {
        mainPage().addRefreshButton([this](Widget btn) {
            startSynchronization(SyncRequestSource::ApplicationRequest);
        });
    }
    return *this;
}

SynchronizedApplication& SynchronizedApplication::synchronizeActionHandler(SynchronizeAction onSynchronizeHandler) {
    #ifdef NATIVE_64BIT

    #else
        #ifdef M5PAPER
        #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 ) || defined( LILYGO_WATCH_2019_WITH_TOUCH )
        #endif
        synchronize = onSynchronizeHandler;
    #endif
    
    return *this;
}

void SynchronizedApplication::startSynchronization(SyncRequestSource callSource) {
#ifdef NATIVE_64BIT
#else
    #ifdef M5PAPER
    #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 ) || defined( LILYGO_WATCH_2019_WITH_TOUCH )
    #endif
        if (xEventGroupGetBits(syncEvent) & SyncRequestSource::IsRequired)
        {
            log_i("Skip startSync() request, %s isn't completed yet", title.c_str());
            return;
        }

        xEventGroupSetBits(syncEvent, callSource);
        auto result = xTaskCreate(&SynchronizedApplication::SyncTaskHandler,      /* Function to implement the task */
                    title.c_str(),    /* Name of the task */
                    5000,                            /* Stack size in words */
                    (void*)this,                            /* Task input parameter */
                    1,                               /* Priority of the task */
                    &syncTask );  /* Task handle. */
        
        if (result == pdPASS)
            log_d("%s scheduled", title.c_str());
        else if (result == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
        {
            xEventGroupClearBits(syncEvent, callSource);
            log_e("No enough memory to start %s!", title.c_str());
        }
        else
            while (true); // Not possible!!!
#endif
}

void SynchronizedApplication::onSyncRequest() {
    #ifdef NATIVE_64BIT

    #else
        #ifdef M5PAPER
        #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 ) || defined( LILYGO_WATCH_2019_WITH_TOUCH )
        #endif
        log_i("start %s, heap: %d", title.c_str(), ESP.getFreeHeap());

        vTaskDelay(250);

        auto flags = (SyncRequestSource)xEventGroupGetBits(syncEvent);
        if (flags & SyncRequestSource::IsRequired)
        {   
            onStartSynchronization(flags);
            if (synchronize != nullptr)
                synchronize(flags);
        }
        
        xEventGroupClearBits(syncEvent, SyncRequestSource::AllFlagsValues);
        log_i("finsh %s, heap: %d", title.c_str(), ESP.getFreeHeap());
        vTaskDelete( NULL );
    #endif
}

void SynchronizedApplication::SyncTaskHandler(void* pvSelf) {
    #ifdef NATIVE_64BIT

    #else
        #ifdef M5PAPER
        #elif defined( LILYGO_WATCH_2020_V1 ) || defined( LILYGO_WATCH_2020_V2 ) || defined( LILYGO_WATCH_2020_V3 ) || defined( LILYGO_WATCH_2019_WITH_TOUCH )
        #endif
        log_i("SyncTaskHandler: %d", pvSelf);
        auto self = (SynchronizedApplication*)pvSelf;
        self->onSyncRequest();
    #endif
}