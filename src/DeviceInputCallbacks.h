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

using DeviceInputCallback = void (*)();

class DeviceInputCallbacks {
  public:
    void disableCallbacks(bool disable = true);
    void enableCallbacks(bool enable = true);
    bool callbacksDisabled();
    bool callbacksEnabled();

    bool hasCallbacks();
    bool hasCallbacks(CallbackType callback_type);
    
    bool addCallback(CallbackType callback_type, DeviceInputCallback callback);
    template <typename... CallbacksAndTypes>
    bool addCallbacks(CallbackType callback_type, DeviceInputCallback callback, CallbacksAndTypes... callbacks_and_types);
    
    bool setCallback(CallbackType callback_type, DeviceInputCallback callback);
    template <typename... CallbacksAndTypes>
    bool setCallbacks(CallbacksAndTypes... callbacks_and_types);

    bool clearCallbacks(CallbackType callback_type);
    bool clearCallbacks();
    
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
    
    template <typename... CallbacksAndTypes>
    bool addCallbacks(CallbackType callback_type, CallbackType new_callback_type, CallbacksAndTypes... callbacks_and_types);
    bool addCallbacks(CallbackType callback_type);
    bool addCallbacks();

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