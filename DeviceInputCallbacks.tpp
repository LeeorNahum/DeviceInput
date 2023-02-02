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

template <typename... Callbacks>
bool DeviceInputCallbacks::addCallbacks(CallbackType callback_type, DeviceInputCallback callback, Callbacks... callbacks) {
  bool success = true;
  
  success = success && addCallback(callback_type, callback);
  success = success && addCallbacks(callback_type, callbacks...);
  
  return success;
}

bool DeviceInputCallbacks::addCallbacks(CallbackType callback_type) {
  return true;
}

template <uint8_t Size>
bool DeviceInputCallbacks::addCallbacks(CallbackType callback_type, DeviceInputCallback (&callback_array)[Size]) {
  for (uint8_t i = 0; i < Size; i++) {
    if (!this->addCallback(callback_type, callback_array[i])) {
      return false;
    }
  }
  return true;
}

bool DeviceInputCallbacks::setCallback(CallbackType callback_type, DeviceInputCallback callback) {
  this->clearCallbacks(callback_type);
  return this->addCallback(callback_type, callback);
}

template <typename... Callbacks>
bool DeviceInputCallbacks::setCallbacks(CallbackType callback_type, Callbacks... callbacks) {
  this->clearCallbacks(callback_type);
  return this->addCallbacks(callback_type, callbacks...);
}

template <uint8_t Size>
bool DeviceInputCallbacks::setCallbacks(CallbackType callback_type, DeviceInputCallback (&callback_array)[Size]) {
  this->clearCallbacks(callback_type);
  return this->addCallbacks(callback_type, callback_array);
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

bool DeviceInputCallbacks::addToggleCallbacks(DeviceInputCallback toggle_callback, DeviceInputCallback untoggle_callback) {
  if (!untoggle_callback) {
    untoggle_callback = toggle_callback;
  }
  bool add_toggle = this->addCallback(TOGGLE, toggle_callback);
  bool add_untoggle = this->addCallback(UNTOGGLE, untoggle_callback);
  return add_toggle && add_untoggle;
}

bool DeviceInputCallbacks::setToggleCallbacks(DeviceInputCallback toggle_callback, DeviceInputCallback untoggle_callback) {
  if (!untoggle_callback) {
    untoggle_callback = toggle_callback;
  }
  bool set_toggle = this->setCallback(TOGGLE, toggle_callback);
  bool set_untoggle = this->setCallback(UNTOGGLE, untoggle_callback);
  return set_toggle && set_untoggle;
}

bool DeviceInputCallbacks::addDetectedCallbacks(DeviceInputCallback detected_callback, DeviceInputCallback undetected_callback) {
  if (!undetected_callback) {
    undetected_callback = detected_callback;
  }
  bool add_detected = this->addCallback(DETECTED, detected_callback);
  bool add_undetected = this->addCallback(UNDETECTED, undetected_callback);
  return add_detected && add_undetected;
}

bool DeviceInputCallbacks::setDetectedCallbacks(DeviceInputCallback detected_callback, DeviceInputCallback undetected_callback) {
  if (!undetected_callback) {
    undetected_callback = detected_callback;
  }
  bool set_detected = this->setCallback(DETECTED, detected_callback);
  bool set_undetected = this->setCallback(UNDETECTED, undetected_callback);
  return set_detected && set_undetected;
}

bool DeviceInputCallbacks::addReadingCallbacks(DeviceInputCallback rising_reading_callback, DeviceInputCallback falling_reading_callback) {
  if (!falling_reading_callback) {
    falling_reading_callback = rising_reading_callback;
  }
  bool add_rising_reading = this->addCallback(RISING_READING, rising_reading_callback);
  bool add_falling_reading = this->addCallback(FALLING_READING, falling_reading_callback);
  return add_rising_reading && add_falling_reading;
}

bool DeviceInputCallbacks::setReadingCallbacks(DeviceInputCallback rising_reading_callback, DeviceInputCallback falling_reading_callback) {
  if (!falling_reading_callback) {
    falling_reading_callback = rising_reading_callback;
  }
  bool set_rising_reading = this->setCallback(RISING_READING, rising_reading_callback);
  bool set_falling_reading = this->setCallback(FALLING_READING, falling_reading_callback);
  return set_rising_reading && set_falling_reading;
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