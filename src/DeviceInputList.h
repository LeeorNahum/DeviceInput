#ifndef DEVICEINPUTLIST_H
#define DEVICEINPUTLIST_H

#include <vector>

using DeviceInputType = DeviceInputCallbacks*;

class DeviceInputList {
  public:
    template <typename... DeviceInputs>
    DeviceInputList(int update_interval_ms = 0, DeviceInputs*... device_inputs);
    template <typename... DeviceInputs>
    DeviceInputList(DeviceInputs*... device_inputs);
    template <uint8_t Size>
    DeviceInputList(DeviceInputType (&device_input_array)[Size], int update_interval_ms = 0);
    template <uint8_t Size>
    DeviceInputList(int update_interval_ms, DeviceInputType (&device_input_array)[Size]);
    template <uint8_t Size>
    DeviceInputList(DeviceInputType (&device_input_array)[Size]);

    void addDeviceInput(DeviceInputType device_input);
    template <typename... DeviceInputs>
    void addDeviceInputs(DeviceInputType device_input, DeviceInputs*... device_inputs);
    template <uint8_t Size>
    void addDeviceInputs(DeviceInputType (&device_input_array)[Size]);
    
    void setDeviceInput(DeviceInputType device_input);
    template <typename... DeviceInputs>
    void setDeviceInputs(DeviceInputs*... device_inputs);
    template <uint8_t Size>
    void setDeviceInputs(DeviceInputType (&device_input_array)[Size]);

    void clearDeviceInputs();

    void setUpdateInterval(int update_interval_ms = 0);
    int getUpdateInterval();

    DeviceInputType* getDeviceInputList();
    DeviceInputType getDeviceInput(uint8_t index);

    void updateReadingForAll();
    void updateDetectedForAll();
    bool updateAll();

    void addCallbackForAll(DeviceInputCallback callback);
    template <typename... Callbacks>
    void addCallbacksForAll(Callbacks... callbacks);

    void setCallbackForAll(DeviceInputCallback callback);
    template <typename... Callbacks>
    void setCallbacksForAll(Callbacks... callbacks);

    bool clearCallbacksForAll();

  private:
    std::vector<DeviceInputType> device_input_list;
    int update_interval_ms = 0;
    unsigned long last_update_time = 0;

    void addDeviceInputs();
};

#include "DeviceInputList.tpp"

#endif
