#ifndef DEVICEINPUT_H
#define DEVICEINPUT_H

#include <Arduino.h>

#include "DeviceInputCallbacks.h"
#include "DeviceInputList.h"

template <typename T = int>
struct DetectionRange {
  DetectionRange(T min_val, T max_val) : min(min_val), max(max_val) {}
  T min;
  T max;
};

template <typename TReturn = int>
class DeviceInput: public DeviceInputCallbacks {
  public:
    using InputFunction = std::function<TReturn()>;
    
    template <typename... Callbacks>
    DeviceInput(const InputFunction& input_function, TReturn detection_exact = true, int update_interval_ms = 0, Callbacks... callbacks);
    template <typename... Callbacks>
    DeviceInput(const InputFunction& input_function, TReturn detection_exact, Callbacks... callbacks);
    template <typename T, typename... Callbacks>
    DeviceInput(const InputFunction& input_function, DetectionRange<T> detection_range, int update_interval_ms = 0, Callbacks... callbacks);
    template <typename T, typename... Callbacks>
    DeviceInput(const InputFunction& input_function, DetectionRange<T> detection_range, Callbacks... callbacks);

    void setInputFunction(const InputFunction& input_function);

    void setDetectionExact(TReturn exact);
    template <typename T>
    void setDetectionRange(DetectionRange<T> range);

    TReturn getDetectionExact();
    DetectionRange<TReturn> getDetectionRange();

    void setUpdateInterval(int update_interval_ms = 0);
    int getUpdateInterval();

    void updateReading() override;
    TReturn getReading();
    TReturn getLastReading();
    
    bool getRisingReading() override;
    bool getFallingReading() override;
    
    void invertDetected(bool invert = true);
    
    bool updateDetected() override;
    bool getDetected() override;
    bool getLastDetected();
    bool getUndetected() override;
    
    bool getToggled() override;
    bool getUntoggled() override;
    
    unsigned long getToggleTimestamp();
    unsigned long getUntoggleTimestamp();
    unsigned long getLastToggleTimestamp();
    unsigned long getLastUntoggleTimestamp();
    
    unsigned long getElapsedTimeDetected();
    unsigned long getElapsedTimeUndetected();
    
    bool update();
    
  private:
    InputFunction input_function = nullptr;

    TReturn detection_exact = true;
    DetectionRange<TReturn> detection_range = {0, 0};
    bool is_detection_range = false;
    
    int update_interval_ms = 0;
    unsigned long last_update_time = 0;
    
    bool invert_detected = false;

    TReturn reading = 0, last_reading = 0;
    
    bool detected = false, last_detected = false;

    unsigned long toggle_timestamp = 0, untoggle_timestamp = 0;
    unsigned long last_toggle_timestamp = 0, last_untoggle_timestamp = 0;
};

#include "DeviceInput.tpp"

#endif