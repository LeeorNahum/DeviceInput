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
    
    bool callbackActive(CallbackType callback_type);
    
    bool runCallbacks();
    
    virtual void updateReading() = 0;
    virtual bool updateDetected() = 0;
    virtual bool update() = 0;
    
  protected:
    virtual bool getRisingReading() = 0; // TODO ? update to return bool instead of returning a comparison every time
    virtual bool getFallingReading() = 0;
    
    virtual bool getDetected() = 0;
    virtual bool getUndetected() = 0;
    
    virtual bool getToggled() = 0;
    virtual bool getUntoggled() = 0;

  private:
    bool callbacks_disabled = false;
    
    const CallbackType callbackTypes[6] = {TOGGLE, UNTOGGLE, DETECTED, UNDETECTED, RISING_READING, FALLING_READING};
    
    struct CallbackArray {
      DeviceInputCallback callbacks[MAX_CALLBACK_ARRAY_SIZE] = {nullptr};
      uint8_t num_callbacks = 0;
    };

    CallbackArray toggle_callbacks;
    CallbackArray untoggle_callbacks;
    CallbackArray detected_callbacks;
    CallbackArray undetected_callbacks;
    CallbackArray rising_reading_callbacks;
    CallbackArray falling_reading_callbacks;
    
    CallbackArray* getCallbackArray(CallbackType callback_type);
    
    template <typename... CallbacksAndTypes>
    bool addCallbacks(CallbackType callback_type, CallbackType new_callback_type, CallbacksAndTypes... callbacks_and_types);
    bool addCallbacks(CallbackType callback_type);
    bool addCallbacks();
};

#include "DeviceInputCallbacks.tpp"

#endif