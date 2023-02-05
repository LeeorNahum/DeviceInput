#ifndef DEVICEINPUTLIST_H
#define DEVICEINPUTLIST_H

#ifndef MAX_INPUT_LIST_ARRAY_SIZE
#define MAX_INPUT_LIST_ARRAY_SIZE 12
#endif

template <typename TReturn>
class DeviceInput;

template <typename TReturn = int>
class DeviceInputList {
  public:
    using DeviceInputType = DeviceInput<TReturn>;

    DeviceInputList(int update_interval_ms = 0);
    template <typename... DeviceInputs>
    DeviceInputList(int update_interval_ms, DeviceInputs*... device_inputs);
    template <typename... DeviceInputs>
    DeviceInputList(DeviceInputs*... device_inputs);
    template <uint8_t Size>
    DeviceInputList(DeviceInputType* (&device_input_array)[Size], int update_interval_ms = 0);

    bool addDeviceInput(DeviceInputType* device_input);
    template <typename... DeviceInputs>
    bool addDeviceInputs(DeviceInputType* device_input, DeviceInputs*... device_inputs);
    template <uint8_t Size>
    bool addDeviceInputs(DeviceInputType* (&device_input_array)[Size]);
    
    bool setDeviceInput(DeviceInputType* device_input);
    template <typename... DeviceInputs>
    bool setDeviceInputs(DeviceInputs*... device_inputs);
    template <uint8_t Size>
    bool setDeviceInputs(DeviceInputType* (&device_input_array)[Size]);

    bool clearDeviceInputs();

    void setUpdateInterval(int update_interval_ms = 0);
    int getUpdateInterval();

    DeviceInputType** getList();
    DeviceInputType* getDeviceInput(uint8_t index);

    bool updateAll();

    bool addCallbackForAll(CallbackType callback_type, DeviceInputCallback callback);
    template <typename... CallbacksAndTypes>
    bool addCallbacksForAll(CallbackType callback_type, DeviceInputCallback callback, CallbacksAndTypes... callbacks_and_types);
    
    bool setCallbackForAll(CallbackType callback_type, DeviceInputCallback callback);
    template <typename... CallbacksAndTypes>
    bool setCallbacksForAll(CallbackType callback_type, DeviceInputCallback callback, CallbacksAndTypes... callbacks_and_types);

    bool clearCallbacksForAll(CallbackType callback_type);
    bool clearCallbacksForAll();
    
  private:
    DeviceInputType* device_input_list[MAX_INPUT_LIST_ARRAY_SIZE];
    uint8_t input_list_size;
    
    int update_interval_ms = 0;
    unsigned long last_update_time = 0;
    
    bool addDeviceInputs();
};

#include "DeviceInputList.tpp"

#endif