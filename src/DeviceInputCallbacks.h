#ifndef DEVICEINPUTCALLBACKS_H
#define DEVICEINPUTCALLBACKS_H

#ifndef MAX_CALLBACK_ARRAY_SIZE
#define MAX_CALLBACK_ARRAY_SIZE 3
#endif

enum CallbackType {
  TOGGLE,
  UNTOGGLE,
  DETECTED,
  UNDETECTED,
  RISING_READING,
  FALLING_READING
};

class DeviceInputCallbacks {
  public:
    using DeviceInputCallback = void (*)();
    
    void disableCallbacks(bool disable = true);
    void enableCallbacks(bool enable = true);
    bool callbacksDisabled();
    bool callbacksEnabled();

    bool hasCallbacks();
    bool hasCallbacks(CallbackType callback_type);
    
    bool addCallback(CallbackType callback_type, DeviceInputCallback callback);
    template <typename... Callbacks>
    bool addCallbacks(CallbackType callback_type, DeviceInputCallback callback, Callbacks... callbacks);
    template <uint8_t Size>
    bool addCallbacks(CallbackType callback_type, DeviceInputCallback (&callback_array)[Size]);
    
    bool setCallback(CallbackType callback_type, DeviceInputCallback callback);
    template <typename... Callbacks>
    bool setCallbacks(CallbackType callback_type, Callbacks... callbacks);
    template <uint8_t Size>
    bool setCallbacks(CallbackType callback_type, DeviceInputCallback (&callback_array)[Size]);

    bool clearCallbacks(CallbackType callback_type);
    bool clearCallbacks();
    
    bool addToggleCallbacks(DeviceInputCallback toggle_callback, DeviceInputCallback untoggle_callback = nullptr);
    bool setToggleCallbacks(DeviceInputCallback toggle_callback, DeviceInputCallback untoggle_callback = nullptr);
    
    bool addDetectedCallbacks(DeviceInputCallback detected_callback, DeviceInputCallback undetected_callback = nullptr);
    bool setDetectedCallbacks(DeviceInputCallback detected_callback, DeviceInputCallback undetected_callback = nullptr);
    
    bool addReadingCallbacks(DeviceInputCallback rising_reading_callback, DeviceInputCallback falling_reading_callback = nullptr);
    bool setReadingCallbacks(DeviceInputCallback rising_reading_callback, DeviceInputCallback falling_reading_callback = nullptr);
    
    //bool runCallback(CallbackType callback_type);
    bool runCallbacks();
    
  protected:
    virtual bool getRisingReading() = 0;
    virtual bool getFallingReading() = 0;
    
    virtual bool getDetected() = 0;
    virtual bool getUndetected() = 0;
    
    virtual bool getToggled() = 0;
    virtual bool getUntoggled() = 0;

  private:
    bool callbacks_disabled = false;
    
    const CallbackType callbackTypes[6] = {TOGGLE, UNTOGGLE, DETECTED, UNDETECTED, RISING_READING, FALLING_READING};
    
    bool getCallbackPointers(CallbackType callback_type, DeviceInputCallback** callbacks_array, uint8_t** num_callbacks);
    
    bool addCallbacks(CallbackType callback_type);

    DeviceInputCallback toggle_callbacks[MAX_CALLBACK_ARRAY_SIZE] = {nullptr};
    uint8_t num_toggle_callbacks = 0;

    DeviceInputCallback untoggle_callbacks[MAX_CALLBACK_ARRAY_SIZE] = {nullptr};
    uint8_t num_untoggle_callbacks = 0;

    DeviceInputCallback detected_callbacks[MAX_CALLBACK_ARRAY_SIZE] = {nullptr};
    uint8_t num_detected_callbacks = 0;

    DeviceInputCallback undetected_callbacks[MAX_CALLBACK_ARRAY_SIZE] = {nullptr};
    uint8_t num_undetected_callbacks = 0;

    DeviceInputCallback rising_reading_callbacks[MAX_CALLBACK_ARRAY_SIZE] = {nullptr};
    uint8_t num_rising_reading_callbacks = 0;

    DeviceInputCallback falling_reading_callbacks[MAX_CALLBACK_ARRAY_SIZE] = {nullptr};
    uint8_t num_falling_reading_callbacks = 0;
};

#include "DeviceInputCallbacks.tpp"

#endif