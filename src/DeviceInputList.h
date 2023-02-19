#ifndef DEVICEINPUTLIST_H
#define DEVICEINPUTLIST_H

#ifndef MAX_INPUT_LIST_ARRAY_SIZE
#define MAX_INPUT_LIST_ARRAY_SIZE 12
#endif

template <typename TReturn>
class DeviceInput;

template <typename TReturn = int>
using DeviceInputType = DeviceInput<TReturn>*;

template <typename TReturn = int>
class DeviceInputList { // TODO If the base class has no templates, then the derived classes that you create can have templates, and you can still use an array of pointers to the base class to hold the derived objects.
  public:
    using DeviceInputType = DeviceInput<TReturn>*;

    template <typename... DeviceInputs>
    DeviceInputList(int update_interval_ms = 0, DeviceInputs*... device_inputs);
    template <typename... DeviceInputs>
    DeviceInputList(DeviceInputs*... device_inputs);
    template <uint8_t Size, typename... CallbacksAndTypes>
    DeviceInputList(DeviceInputType (&device_input_array)[Size], int update_interval_ms = 0, CallbacksAndTypes... callbacks_and_types);
    template <uint8_t Size, typename... CallbacksAndTypes>
    DeviceInputList(int update_interval_ms, DeviceInputType (&device_input_array)[Size], CallbacksAndTypes... callbacks_and_types);
    template <uint8_t Size, typename... CallbacksAndTypes>
    DeviceInputList(DeviceInputType (&device_input_array)[Size], CallbacksAndTypes... callbacks_and_types);

    bool addDeviceInput(DeviceInputType device_input);
    template <typename... DeviceInputs>
    bool addDeviceInputs(DeviceInputType device_input, DeviceInputs*... device_inputs);
    template <uint8_t Size>
    bool addDeviceInputs(DeviceInputType (&device_input_array)[Size]);
    
    bool setDeviceInput(DeviceInputType device_input);
    template <typename... DeviceInputs>
    bool setDeviceInputs(DeviceInputs*... device_inputs);
    template <uint8_t Size>
    bool setDeviceInputs(DeviceInputType (&device_input_array)[Size]);

    bool clearDeviceInputs();

    void setUpdateInterval(int update_interval_ms = 0);
    int getUpdateInterval();

    DeviceInputType* getList();
    DeviceInputType getDeviceInput(uint8_t index);

    void updateReadingForAll();
    
    void updateDetectedForAll();
    
    bool updateAll();

    bool addCallbackForAll(CallbackType callback_type, DeviceInputCallback callback);
    template <typename... CallbacksAndTypes>
    bool addCallbacksForAll(CallbacksAndTypes... callbacks_and_types);
    
    bool setCallbackForAll(CallbackType callback_type, DeviceInputCallback callback);
    template <typename... CallbacksAndTypes>
    bool setCallbacksForAll(CallbacksAndTypes... callbacks_and_types);

    bool clearCallbacksForAll(CallbackType callback_type);
    bool clearCallbacksForAll();
    
  private:
    DeviceInputType device_input_list[MAX_INPUT_LIST_ARRAY_SIZE] = {nullptr};
    uint8_t input_list_size = 0;
    
    int update_interval_ms = 0;
    unsigned long last_update_time = 0;
    
    bool addDeviceInputs();
};

#include "DeviceInputList.tpp"

#endif