template <typename TReturn>
DeviceInputList<TReturn>::DeviceInputList(int update_interval_ms) {
  this->clearDeviceInputs();
  this->setUpdateInterval(update_interval_ms);
}

template <typename TReturn>
template <typename... DeviceInputs>
DeviceInputList<TReturn>::DeviceInputList(int update_interval_ms, DeviceInputs*... device_inputs) {
  this->setUpdateInterval(update_interval_ms); 
  this->setDeviceInputs(device_inputs...);
}

template <typename TReturn>
template <typename... DeviceInputs>
DeviceInputList<TReturn>::DeviceInputList(DeviceInputs*... device_inputs) {
  this->setDeviceInputs(device_inputs...);
}

template <typename TReturn>
template <uint8_t Size>
DeviceInputList<TReturn>::DeviceInputList(DeviceInputType* (&device_input_array)[Size], int update_interval_ms) {
  this->setDeviceInputs(device_input_array);
  this->setUpdateInterval(update_interval_ms);
}

template <typename TReturn>
bool DeviceInputList<TReturn>::addDeviceInput(DeviceInputType* device_input) {
  if (this->input_list_size >= MAX_INPUT_LIST_ARRAY_SIZE) {
    return false;
  }

  this->device_input_list[this->input_list_size++] = device_input;
  return true;
}

template <typename TReturn>
template <typename... DeviceInputs>
bool DeviceInputList<TReturn>::addDeviceInputs(DeviceInputType* device_input, DeviceInputs*... device_inputs) {
  bool success = true;
  
  success = success && addDeviceInput(device_input);
  success = success && addDeviceInputs(device_inputs...);
  
  return success;
}

template <typename TReturn>
bool DeviceInputList<TReturn>::addDeviceInputs() {
  return true;
}

template <typename TReturn>
template <uint8_t Size>
bool DeviceInputList<TReturn>::addDeviceInputs(DeviceInputType* (&device_input_array)[Size]) {
  bool success = true;

  for (uint8_t i = 0; i < Size; i++) {
    success = success && this->addDeviceInput(device_input_array[i]);
  }

  return success;
}

template <typename TReturn>
bool DeviceInputList<TReturn>::setDeviceInput(DeviceInputType* device_input) {
  this->clearDeviceInputs();
  return this->addDeviceInput(device_input);
}

template <typename TReturn>
template <typename... DeviceInputs>
bool DeviceInputList<TReturn>::setDeviceInputs(DeviceInputs*... device_inputs) {
  this->clearDeviceInputs();
  return this->addDeviceInputs(device_inputs...);
}

template <typename TReturn>
template <uint8_t Size>
bool DeviceInputList<TReturn>::setDeviceInputs(DeviceInputType* (&device_input_array)[Size]) {
  this->clearDeviceInputs();
  return this->addDeviceInputs(device_input_array);
}

template <typename TReturn>
bool DeviceInputList<TReturn>::clearDeviceInputs() {
  if (!this->input_list_size > 0) {
    return false;
  }
  
  for (uint8_t i = 0; i < MAX_INPUT_LIST_ARRAY_SIZE; i++) {
    this->device_input_list[i] = nullptr;
  }
  
  this->input_list_size = 0;
  return true;
}

template <typename TReturn>
void DeviceInputList<TReturn>::setUpdateInterval(int update_interval_ms) {
  this->update_interval_ms = update_interval_ms;
}

template <typename TReturn>
int DeviceInputList<TReturn>::getUpdateInterval() {
  return this->update_interval_ms;
}

template <typename TReturn>
DeviceInput<TReturn>** DeviceInputList<TReturn>::getList() {
  return this->device_input_list;
}

template <typename TReturn>
DeviceInput<TReturn>* DeviceInputList<TReturn>::getDeviceInput(uint8_t index) {
  if (index >= this->input_list_size) {
    return nullptr;
  }

  return this->device_input_list[index];
}

template <typename TReturn>
bool DeviceInputList<TReturn>::updateAll() {
  if (this->getUpdateInterval() > 0) {
    unsigned long current_time = millis();
    if (current_time - this->last_update_time < this->getUpdateInterval()) {
      return false;
    }
    this->last_update_time = current_time;
  }
  
  for (uint8_t i = 0; i < this->input_list_size; i++) {
    this->device_input_list[i]->update();
  }
  
  return true;
}

template <typename TReturn>
bool DeviceInputList<TReturn>::addCallbackForAll(CallbackType callback_type, DeviceInputCallback callback) {
  bool success = true;
  for (int i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->addCallback(callback_type, callback);
  }
  return success;
}

template <typename TReturn>
template <typename... Callbacks>
bool DeviceInputList<TReturn>::addCallbacksForAll(CallbackType callback_type, Callbacks... callbacks) {
  bool success = true;
  for (int i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->addCallbacks(callback_type, callbacks...);
  }
  return success;
}

template <typename TReturn>
template <uint8_t Size>
bool DeviceInputList<TReturn>::addCallbacksForAll(CallbackType callback_type, DeviceInputCallback (&callback_array)[Size]) {
  bool success = true;
  for (int i = 0; i < Size; i++) {
    success = success && this->device_input_list[i]->addCallbacks(callback_type, callback_array);
  }
  return success;
}

template <typename TReturn>
bool DeviceInputList<TReturn>::setCallbackForAll(CallbackType callback_type, DeviceInputCallback callback) {
  bool success = true;
  for (int i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->setCallback(callback_type, callback);
  }
  return success;
}

template <typename TReturn>
template <typename... Callbacks>
bool DeviceInputList<TReturn>::setCallbacksForAll(CallbackType callback_type, Callbacks... callbacks) {
  bool success = true;
  for (int i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->setCallbacks(callback_type, callbacks...);
  }
  return success;
}

template <typename TReturn>
template <uint8_t Size>
bool DeviceInputList<TReturn>::setCallbacksForAll(CallbackType callback_type, DeviceInputCallback (&callback_array)[Size]) {
  bool success = true;
  for (int i = 0; i < Size; i++) {
    success = success && this->device_input_list[i]->setCallbacks(callback_type, callback_array);
  }
  return success;
}

template <typename TReturn>
bool DeviceInputList<TReturn>::clearCallbacksForAll(CallbackType callback_type) {
  bool success = false;
  for (uint8_t i = 0; i < this->input_list_size; i++) {
    success = success || this->device_input_list[i]->clearCallbacks(callback_type);
  }
  return success;
}

template <typename TReturn>
bool DeviceInputList<TReturn>::addToggleCallbacksForAll(DeviceInputCallback toggle_callback, DeviceInputCallback untoggle_callback) {
  bool success = false;
  for (uint8_t i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->addToggleCallbacks(toggle_callback, untoggle_callback);
  }
  return success;
}

template <typename TReturn>
bool DeviceInputList<TReturn>::clearCallbacksForAll() {
  bool success = false;
  for (uint8_t i = 0; i < this->input_list_size; i++) {
    success = success || this->device_input_list[i]->clearCallbacks();
  }
  return success;
}

template <typename TReturn>
bool DeviceInputList<TReturn>::setToggleCallbacksForAll(DeviceInputCallback toggle_callback, DeviceInputCallback untoggle_callback) {
  bool success = false;
  for (uint8_t i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->setToggleCallbacks(toggle_callback, untoggle_callback);
  }
  return success;
}

template <typename TReturn>
bool DeviceInputList<TReturn>::addDetectedCallbacksForAll(DeviceInputCallback detected_callback, DeviceInputCallback undetected_callback) {
  bool success = false;
  for (uint8_t i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->addDetectedCallbacks(detected_callback, undetected_callback);
  }
  return success;
}

template <typename TReturn>
bool DeviceInputList<TReturn>::setDetectedCallbacksForAll(DeviceInputCallback detected_callback, DeviceInputCallback undetected_callback) {
  bool success = false;
  for (uint8_t i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->setDetectedCallbacks(detected_callback, undetected_callback);
  }
  return success;
}

template <typename TReturn>
bool DeviceInputList<TReturn>::addReadingCallbacksForAll(DeviceInputCallback rising_reading_callback, DeviceInputCallback falling_reading_callback) {
  bool success = false;
  for (uint8_t i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->addReadingCallbacks(rising_reading_callback, falling_reading_callback);
  }
  return success;
}

template <typename TReturn>
bool DeviceInputList<TReturn>::setReadingCallbacksForAll(DeviceInputCallback rising_reading_callback, DeviceInputCallback falling_reading_callback) {
  bool success = false;
  for (uint8_t i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->setReadingCallbacks(rising_reading_callback, falling_reading_callback);
  }
  return success;
}