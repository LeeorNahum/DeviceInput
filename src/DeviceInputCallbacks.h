#ifndef DEVICEINPUTCALLBACKS_H
#define DEVICEINPUTCALLBACKS_H

#include <vector>
#include <functional>

using DeviceInputCallback = std::function<void()>;

class DeviceInputCallbacks {
  public:
    void disableCallbacks(bool disable = true);
    void enableCallbacks(bool enable = true);
    bool callbacksDisabled();
    bool callbacksEnabled();

    bool hasCallbacks();
    
    void addCallback(DeviceInputCallback callback);
    template <typename... Callbacks>
    void addCallbacks(DeviceInputCallback callback, Callbacks... callbacks);

    void setCallback(DeviceInputCallback callback);
    template <typename... Callbacks>
    void setCallbacks(Callbacks... callbacks);

    bool clearCallbacks();
    
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
    std::vector<DeviceInputCallback> callbacks;
};

#include "DeviceInputCallbacks.tpp"

#endif
