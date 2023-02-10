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
  CallbackArray* callback_array = this->getCallbackArray(callback_type);
  
  return callback_array->num_callbacks > 0;
}

bool DeviceInputCallbacks::addCallback(CallbackType callback_type, DeviceInputCallback callback) {
  CallbackArray* callback_array = this->getCallbackArray(callback_type);
  
  if (callback_array->num_callbacks >= MAX_CALLBACK_ARRAY_SIZE) {
    return false;
  }
  
  callback_array->callbacks[callback_array->num_callbacks++] = callback;
  
  return true;
}

template <typename... CallbacksAndTypes>
bool DeviceInputCallbacks::addCallbacks(CallbackType callback_type, DeviceInputCallback callback, CallbacksAndTypes... callbacks_and_types) {
  bool success = true;
  
  success = success && this->addCallback(callback_type, callback);
  success = success && this->addCallbacks(callback_type, callbacks_and_types...);
  
  return success;
}

template <typename... CallbacksAndTypes>
bool DeviceInputCallbacks::addCallbacks(CallbackType callback_type, CallbackType new_callback_type, CallbacksAndTypes... callbacks_and_types) {
  return this->addCallbacks(new_callback_type, callbacks_and_types...);
}

bool DeviceInputCallbacks::addCallbacks(CallbackType callback_type) {
  return true;
}

bool DeviceInputCallbacks::addCallbacks() {
  return true;
}

bool DeviceInputCallbacks::setCallback(CallbackType callback_type, DeviceInputCallback callback) {
  this->clearCallbacks();
  return this->addCallback(callback_type, callback);
}

template <typename... CallbacksAndTypes>
bool DeviceInputCallbacks::setCallbacks(CallbacksAndTypes... callbacks_and_types) {
  this->clearCallbacks();
  return this->addCallbacks(callbacks_and_types...);
}

bool DeviceInputCallbacks::clearCallbacks(CallbackType callback_type) {
  CallbackArray* callback_array = this->getCallbackArray(callback_type);
  
  if (callback_array->num_callbacks == 0) {
    return false;
  }
  
  for (uint8_t i = 0; i < MAX_CALLBACK_ARRAY_SIZE; i++) {
    callback_array->callbacks[i] = nullptr;
  }
  callback_array->num_callbacks = 0;
  return true;
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
    CallbackArray* callback_array = this->getCallbackArray(callback_type);

    if (callback_array->num_callbacks == 0 || !this->callbackActive(callback_type)) {
      continue;
    }

    for (uint8_t i = 0; i < callback_array->num_callbacks; i++) {
      callback_array->callbacks[i]();
      callback_ran = true;
    }
  }

  return callback_ran;
}

DeviceInputCallbacks::CallbackArray* DeviceInputCallbacks::getCallbackArray(CallbackType callback_type) {
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