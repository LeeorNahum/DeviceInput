#ifndef DEVICEINPUT_H
#define DEVICEINPUT_H

#include <stdint.h>
#include "DeviceInputCallbacks.h"
#include "DeviceInputList.h"

template <typename TReturn = int>
class DeviceInput: public DeviceInputCallbacks {
  public:
    using InputFunction = TReturn (*)();
    
    template <typename... CallbacksAndTypes>
    DeviceInput(InputFunction input_function, TReturn detection_exact, int update_interval_ms = 0, CallbacksAndTypes... callbacks_and_types);
    template <typename... CallbacksAndTypes>
    DeviceInput(InputFunction input_function, TReturn detection_exact, CallbacksAndTypes... callbacks_and_types);
    template <typename... CallbacksAndTypes>
    DeviceInput(InputFunction input_function, TReturn detection_range[2], int update_interval_ms = 0, CallbacksAndTypes... callbacks_and_types);
    template <typename... CallbacksAndTypes>
    DeviceInput(InputFunction input_function, TReturn detection_range[2], CallbacksAndTypes... callbacks_and_types);

    void setInputFunction(InputFunction input_function);

    void setDetectionExact(TReturn exact);
    bool setDetectionRange(TReturn range[2]);

    TReturn getDetectionExact();
    TReturn* getDetectionRange();

    void setUpdateInterval(int update_interval_ms = 0);
    int getUpdateInterval();

    void updateReading();
    TReturn getReading();
    TReturn getLastReading();
    
    bool getRisingReading() override;
    bool getFallingReading() override;
    
    void invertDetected(bool invert = true);
    
    bool updateDetected();
    bool getDetected() override;
    bool getLastDetected();
    bool getUndetected() override;
    
    bool getToggled() override;
    bool getUntoggled() override;
    
    unsigned long getToggleTimestamp();
    unsigned long getUntoggleTimestamp();
    
    unsigned long getElapsedTimeDetected();
    unsigned long getElapsedTimeUndetected();
    
    bool update();
    
  private:
    InputFunction input_function = nullptr;

    TReturn detection_exact = 0;
    TReturn detection_range[2] = {0, 0};
    bool is_detection_range = false;
    
    int update_interval_ms = 0;
    unsigned long last_update_time = 0;
    
    bool invert_detected = false;

    TReturn reading = 0, last_reading = 0;
    
    bool detected = false, last_detected = false;

    unsigned long toggle_timestamp = 0, untoggle_timestamp = 0;
};

#include "DeviceInput.tpp"

#endif