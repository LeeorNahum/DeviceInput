// DeviceInputList.tpp

template <typename... DeviceInputs>
DeviceInputList::DeviceInputList(int update_interval_ms, DeviceInputs*... device_inputs) {
  this->setUpdateInterval(update_interval_ms);
  this->setDeviceInputs(device_inputs...);
}

template <typename... DeviceInputs>
DeviceInputList::DeviceInputList(DeviceInputs*... device_inputs) {
  this->setDeviceInputs(device_inputs...);
}

template <uint8_t Size>
DeviceInputList::DeviceInputList(DeviceInputType (&device_input_array)[Size], int update_interval_ms) {
  this->setDeviceInputs(device_input_array);
  this->setUpdateInterval(update_interval_ms);
}

template <uint8_t Size>
DeviceInputList::DeviceInputList(int update_interval_ms, DeviceInputType (&device_input_array)[Size]) {
  this->setUpdateInterval(update_interval_ms);
  this->setDeviceInputs(device_input_array);
}

template <uint8_t Size>
DeviceInputList::DeviceInputList(DeviceInputType (&device_input_array)[Size]) {
  this->setDeviceInputs(device_input_array);
}

void DeviceInputList::addDeviceInput(DeviceInputType device_input) {
  this->device_input_list.push_back(device_input);
}

template <typename... DeviceInputs>
void DeviceInputList::addDeviceInputs(DeviceInputType device_input, DeviceInputs*... device_inputs) {
  this->device_input_list.push_back(device_input);
  this->addDeviceInputs(device_inputs...);
}

template <uint8_t Size>
void DeviceInputList::addDeviceInputs(DeviceInputType (&device_input_array)[Size]) {
  for (uint8_t i = 0; i < Size; i++) {
    this->addDeviceInput(device_input_array[i]);
  }
}

void DeviceInputList::setDeviceInput(DeviceInputType device_input) {
  this->clearDeviceInputs();
  this->addDeviceInput(device_input);
}

template <typename... DeviceInputs>
void DeviceInputList::setDeviceInputs(DeviceInputs*... device_inputs) {
  this->clearDeviceInputs();
  this->addDeviceInputs(device_inputs...);
}

template <uint8_t Size>
void DeviceInputList::setDeviceInputs(DeviceInputType (&device_input_array)[Size]) {
  this->clearDeviceInputs();
  this->addDeviceInputs(device_input_array);
}

void DeviceInputList::clearDeviceInputs() {
  device_input_list.clear();
}

void DeviceInputList::setUpdateInterval(int update_interval_ms) {
  this->update_interval_ms = update_interval_ms;
}

int DeviceInputList::getUpdateInterval() {
  return this->update_interval_ms;
}

DeviceInputType* DeviceInputList::getDeviceInputList() {
  return this->device_input_list.data();
}

DeviceInputType DeviceInputList::getDeviceInput(uint8_t index) {
  return this->device_input_list[index];
}

void DeviceInputList::updateReadingForAll() {
  for (DeviceInputType& device_input : this->device_input_list) {
    device_input->updateReading();
  }
}

void DeviceInputList::updateDetectedForAll() {
  for (DeviceInputType& device_input : this->device_input_list) {
    device_input->updateDetected();
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

  for (DeviceInputType& device_input : this->device_input_list) {
    device_input->update();
  }

  return true;
}

void DeviceInputList::addCallbackForAll(DeviceInputCallback callback) {
  for (DeviceInputType& device_input : this->device_input_list) {
    device_input->addCallback(callback);
  }
}

template <typename... Callbacks>
void DeviceInputList::addCallbacksForAll(Callbacks... callbacks) {
  for (DeviceInputType& device_input : this->device_input_list) {
    device_input->addCallbacks(callbacks...);
  }
}

void DeviceInputList::setCallbackForAll(DeviceInputCallback callback) {
  for (DeviceInputType& device_input : this->device_input_list) {
    device_input->setCallback(callback);
  }
}

template <typename... Callbacks>
void DeviceInputList::setCallbacksForAll(Callbacks... callbacks) {
  for (DeviceInputType& device_input : this->device_input_list) {
    device_input->setCallbacks(callbacks...);
  }
}

bool DeviceInputList::clearCallbacksForAll() {
  bool success = false;
  for (DeviceInputType& device_input : this->device_input_list) {
    success = success || device_input->clearCallbacks();
  }
  return success;
}
