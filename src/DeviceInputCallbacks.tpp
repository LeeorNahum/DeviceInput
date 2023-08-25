void DeviceInputCallbacks::disableCallbacks(bool disable) {
  this->callbacks_disabled = disable;
}

void DeviceInputCallbacks::enableCallbacks(bool enable) {
  this->callbacks_disabled = !enable;
}

bool DeviceInputCallbacks::callbacksDisabled() {
  return this->callbacks_disabled;
}

bool DeviceInputCallbacks::callbacksEnabled() {
  return !this->callbacks_disabled;
}

bool DeviceInputCallbacks::hasCallbacks() {
  for (CallbackType callback_type : callbackTypes) {
    if (this->hasCallbacks(callback_type)) {
      return true;
    }
  }
  return false;
}

bool DeviceInputCallbacks::hasCallbacks(CallbackType callback_type) {
  std::vector<DeviceInputCallback>* callback_vector = this->getCallbackVector(callback_type);
  return !callback_vector->empty();
}

void DeviceInputCallbacks::addCallback(CallbackType callback_type, DeviceInputCallback callback) {
  std::vector<DeviceInputCallback>* callback_vector = this->getCallbackVector(callback_type);
  callback_vector->push_back(callback);
}

template <typename... CallbacksAndTypes>
void DeviceInputCallbacks::addCallbacks(CallbackType callback_type, DeviceInputCallback callback, CallbacksAndTypes... callbacks_and_types) {
  this->addCallback(callback_type, callback);
  this->addCallbacks(callback_type, callbacks_and_types...);
}

template <typename... CallbacksAndTypes>
void DeviceInputCallbacks::addCallbacks(CallbackType callback_type, CallbackType new_callback_type, CallbacksAndTypes... callbacks_and_types) {
  this->addCallbacks(new_callback_type, callbacks_and_types...);
}

void DeviceInputCallbacks::addCallbacks(CallbackType callback_type) {}

void DeviceInputCallbacks::addCallbacks() {}

void DeviceInputCallbacks::setCallback(CallbackType callback_type, DeviceInputCallback callback) {
  this->clearCallbacks();
  this->addCallback(callback_type, callback);
}

template <typename... CallbacksAndTypes>
void DeviceInputCallbacks::setCallbacks(CallbacksAndTypes... callbacks_and_types) {
  this->clearCallbacks();
  this->addCallbacks(callbacks_and_types...);
}

bool DeviceInputCallbacks::clearCallbacks(CallbackType callback_type) {
  std::vector<DeviceInputCallback>* callback_vector = this->getCallbackVector(callback_type);
  bool had_callbacks = !callback_vector->empty();
  callback_vector->clear();
  return had_callbacks;
}

bool DeviceInputCallbacks::clearCallbacks() {
  bool success = false;
  for (CallbackType callback_type : callbackTypes) {
    success = success || this->clearCallbacks(callback_type);
  }
  return success;
}

bool DeviceInputCallbacks::callbackActive(CallbackType callback_type) {
  switch (callback_type) {
    case TOGGLE:
      return this->getToggled();
    case UNTOGGLE:
      return this->getUntoggled();
    case DETECTED:
      return this->getDetected();
    case UNDETECTED:
      return this->getUndetected();
    case RISING_READING:
      return this->getRisingReading();
    case FALLING_READING:
      return this->getFallingReading();
    default:
      return false;
  }
}

bool DeviceInputCallbacks::runCallbacks() {
  if (this->callbacksDisabled()) {
    return false;
  }

  bool callback_ran = false;

  for (CallbackType callback_type : callbackTypes) {
    std::vector<DeviceInputCallback>* callback_vector = this->getCallbackVector(callback_type);

    if (callback_vector->empty() || !this->callbackActive(callback_type)) {
      continue;
    }

    for (DeviceInputCallback& callback : *callback_vector) {
      callback();
      callback_ran = true;
    }
  }

  return callback_ran;
}

std::vector<DeviceInputCallback>* DeviceInputCallbacks::getCallbackVector(CallbackType callback_type) {
  switch (callback_type) {
    case TOGGLE:
      return &this->toggle_callbacks;
    case UNTOGGLE:
      return &this->untoggle_callbacks;
    case DETECTED:
      return &this->detected_callbacks;
    case UNDETECTED:
      return &this->undetected_callbacks;
    case RISING_READING:
      return &this->rising_reading_callbacks;
    case FALLING_READING:
      return &this->falling_reading_callbacks;
    default:
      return nullptr;
  }
}
