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
  DeviceInputCallback* callback_array;
  uint8_t* num_callbacks;
  this->getCallbackPointers(callback_type, &callback_array, &num_callbacks);
  
  return *num_callbacks > 0;
}

bool DeviceInputCallbacks::addCallback(CallbackType callback_type, DeviceInputCallback callback) {
  DeviceInputCallback* callback_array;
  uint8_t* num_callbacks;
  this->getCallbackPointers(callback_type, &callback_array, &num_callbacks);
  
  if (*num_callbacks >= MAX_CALLBACK_ARRAY_SIZE) {
    return false;
  }
  callback_array[(*num_callbacks)++] = callback;
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
  DeviceInputCallback* callback_array;
  uint8_t* num_callbacks;
  this->getCallbackPointers(callback_type, &callback_array, &num_callbacks);
  
  if (*num_callbacks == 0) {
    return false;
  }
  
  for (uint8_t i = 0; i < MAX_CALLBACK_ARRAY_SIZE; i++) {
    callback_array[i] = nullptr;
  }
  *num_callbacks = 0;
  return true;
}

bool DeviceInputCallbacks::clearCallbacks() {
  bool success = false;
  for (CallbackType callback_type : callbackTypes) {
    success = success || this->clearCallbacks(callback_type);
  }
  return success;
}

bool DeviceInputCallbacks::runCallbacks() {
  if (this->callbacksDisabled()) {
    return false;
  }
  
  bool callback_ran = false;

  for (CallbackType callback_type : callbackTypes) {
    DeviceInputCallback* callback_array;
    uint8_t* num_callbacks;
    bool can_run_callback = this->getCallbackPointers(callback_type, &callback_array, &num_callbacks);

    if (!can_run_callback || *num_callbacks == 0) {
      continue;
    }
    
    for (uint8_t i = 0; i < *num_callbacks; i++) {
      callback_array[i]();
      callback_ran = true;
    }
  }

  return callback_ran;
}

bool DeviceInputCallbacks::getCallbackPointers(CallbackType callback_type, DeviceInputCallback** callback_array, uint8_t** num_callbacks) {
  switch (callback_type) {
    case TOGGLE:
      *callback_array = this->toggle_callbacks;
      *num_callbacks = &this->num_toggle_callbacks;
      return this->getToggled();
    case UNTOGGLE:
      *callback_array = this->untoggle_callbacks;
      *num_callbacks = &this->num_untoggle_callbacks;
      return this->getUntoggled();
    case DETECTED:
      *callback_array = this->detected_callbacks;
      *num_callbacks = &this->num_detected_callbacks;
      return this->getDetected();
    case UNDETECTED:
      *callback_array = this->undetected_callbacks;
      *num_callbacks = &this->num_undetected_callbacks;
      return this->getUndetected();
    case RISING_READING:
      *callback_array = this->rising_reading_callbacks;
      *num_callbacks = &this->num_rising_reading_callbacks;
      return this->getRisingReading();
    case FALLING_READING:
      *callback_array = this->falling_reading_callbacks;
      *num_callbacks = &this->num_falling_reading_callbacks;
      return this->getFallingReading();
  }
}