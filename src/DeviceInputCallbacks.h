#ifndef DEVICEINPUTCALLBACKS_H
#define DEVICEINPUTCALLBACKS_H

#include <vector>

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
    
    void addCallback(CallbackType callback_type, DeviceInputCallback callback);
    template <typename... CallbacksAndTypes>
    void addCallbacks(CallbackType callback_type, DeviceInputCallback callback, CallbacksAndTypes... callbacks_and_types);
    
    void setCallback(CallbackType callback_type, DeviceInputCallback callback);
    template <typename... CallbacksAndTypes>
    void setCallbacks(CallbacksAndTypes... callbacks_and_types);

    bool clearCallbacks(CallbackType callback_type);
    bool clearCallbacks();
    
    bool callbackActive(CallbackType callback_type);
    
    bool runCallbacks();
    
    virtual void updateReading() = 0;
    virtual bool updateDetected() = 0;
    virtual bool update() = 0;
    
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
    
    std::vector<DeviceInputCallback> toggle_callbacks, untoggle_callbacks, detected_callbacks, undetected_callbacks, rising_reading_callbacks, falling_reading_callbacks;
    
    std::vector<DeviceInputCallback>* getCallbackVector(CallbackType callback_type);
    
    template <typename... CallbacksAndTypes>
    void addCallbacks(CallbackType callback_type, CallbackType new_callback_type, CallbacksAndTypes... callbacks_and_types);
    void addCallbacks(CallbackType callback_type);
    void addCallbacks();
};

#include "DeviceInputCallbacks.tpp"

#endif
