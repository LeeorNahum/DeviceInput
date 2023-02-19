template <typename... DeviceInputs>
DeviceInputList::DeviceInputList(int update_interval_ms, DeviceInputs*... device_inputs) {
  this->setUpdateInterval(update_interval_ms);
  this->setDeviceInputs(device_inputs...);
}

template <typename... DeviceInputs>
DeviceInputList::DeviceInputList(DeviceInputs*... device_inputs) {
  this->setDeviceInputs(device_inputs...);
}

template <uint8_t Size, typename... CallbacksAndTypes>
DeviceInputList::DeviceInputList(DeviceInputType (&device_input_array)[Size], int update_interval_ms, CallbacksAndTypes... callbacks_and_types) {
  this->setDeviceInputs(device_input_array);
  this->setUpdateInterval(update_interval_ms);
  this->addCallbacksForAll(callbacks_and_types...);
}

template <uint8_t Size, typename... CallbacksAndTypes>
DeviceInputList::DeviceInputList(int update_interval_ms, DeviceInputType (&device_input_array)[Size], CallbacksAndTypes... callbacks_and_types) {
  this->setUpdateInterval(update_interval_ms);
  this->setDeviceInputs(device_input_array);
  this->addCallbacksForAll(callbacks_and_types...);
}

template <uint8_t Size, typename... CallbacksAndTypes>
DeviceInputList::DeviceInputList(DeviceInputType (&device_input_array)[Size], CallbacksAndTypes... callbacks_and_types) {
  this->setDeviceInputs(device_input_array);
  this->addCallbacksForAll(callbacks_and_types...);
}

bool DeviceInputList::addDeviceInput(DeviceInputType device_input) {
  if (this->input_list_size >= MAX_INPUT_LIST_ARRAY_SIZE) {
    return false;
  }

  this->device_input_list[this->input_list_size++] = device_input;
  return true;
}

template <typename... DeviceInputs>
bool DeviceInputList::addDeviceInputs(DeviceInputType device_input, DeviceInputs*... device_inputs) {
  bool success = true;
  
  success = success && addDeviceInput(device_input);
  success = success && addDeviceInputs(device_inputs...);
  
  return success;
}

bool DeviceInputList::addDeviceInputs() {
  return true;
}

template <uint8_t Size>
bool DeviceInputList::addDeviceInputs(DeviceInputType (&device_input_array)[Size]) {
  bool success = true;

  for (uint8_t i = 0; i < Size; i++) {
    success = success && this->addDeviceInput(device_input_array[i]);
  }

  return success;
}

bool DeviceInputList::setDeviceInput(DeviceInputType device_input) {
  this->clearDeviceInputs();
  return this->addDeviceInput(device_input);
}

template <typename... DeviceInputs>
bool DeviceInputList::setDeviceInputs(DeviceInputs*... device_inputs) {
  this->clearDeviceInputs();
  return this->addDeviceInputs(device_inputs...);
}

template <uint8_t Size>
bool DeviceInputList::setDeviceInputs(DeviceInputType (&device_input_array)[Size]) {
  this->clearDeviceInputs();
  return this->addDeviceInputs(device_input_array);
}

bool DeviceInputList::clearDeviceInputs() {
  if (!this->input_list_size > 0) {
    return false;
  }
  
  for (uint8_t i = 0; i < MAX_INPUT_LIST_ARRAY_SIZE; i++) {
    this->device_input_list[i] = nullptr;
  }
  
  this->input_list_size = 0;
  return true;
}

void DeviceInputList::setUpdateInterval(int update_interval_ms) {
  this->update_interval_ms = update_interval_ms;
}

int DeviceInputList::getUpdateInterval() {
  return this->update_interval_ms;
}

DeviceInputType* DeviceInputList::getDeviceInputList() {
  return this->device_input_list;
}

DeviceInputType DeviceInputList::getDeviceInput(uint8_t index) {
  if (index >= this->input_list_size) {
    return nullptr;
  }

  return this->device_input_list[index];
}

void DeviceInputList::updateReadingForAll() {
  for (uint8_t i = 0; i < this->input_list_size; i++) {
    this->device_input_list[i]->updateReading();
  }
}

void DeviceInputList::updateDetectedForAll() {
  for (uint8_t i = 0; i < this->input_list_size; i++) {
    this->device_input_list[i]->updateDetected();
  }
}

bool DeviceInputList::updateAll() {
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

bool DeviceInputList::addCallbackForAll(CallbackType callback_type, DeviceInputCallback callback) {
  bool success = true;
  for (int i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->addCallback(callback_type, callback);
  }
  return success;
}

template <typename... CallbacksAndTypes>
bool DeviceInputList::addCallbacksForAll(CallbacksAndTypes... callbacks_and_types) {
  bool success = true;
  for (int i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->addCallbacks(callbacks_and_types...);
  }
  return success;
}

bool DeviceInputList::setCallbackForAll(CallbackType callback_type, DeviceInputCallback callback) {
  bool success = true;
  for (int i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->setCallback(callback_type, callback);
  }
  return success;
}

template <typename... CallbacksAndTypes>
bool DeviceInputList::setCallbacksForAll(CallbacksAndTypes... callbacks_and_types) {
  bool success = true;
  for (int i = 0; i < this->input_list_size; i++) {
    success = success && this->device_input_list[i]->setCallbacks(callbacks_and_types...);
  }
  return success;
}

bool DeviceInputList::clearCallbacksForAll(CallbackType callback_type) {
  bool success = false;
  for (uint8_t i = 0; i < this->input_list_size; i++) {
    success = success || this->device_input_list[i]->clearCallbacks(callback_type);
  }
  return success;
}

bool DeviceInputList::clearCallbacksForAll() {
  bool success = false;
  for (uint8_t i = 0; i < this->input_list_size; i++) {
    success = success || this->device_input_list[i]->clearCallbacks();
  }
  return success;
}