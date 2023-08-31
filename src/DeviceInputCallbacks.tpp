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
  return !this->callbacks.empty();
}

void DeviceInputCallbacks::addCallback(DeviceInputCallback callback) {
  this->callbacks.push_back(callback);
}

template <typename... Callbacks>
void DeviceInputCallbacks::addCallbacks(DeviceInputCallback callback, Callbacks... callbacks) {
  this->addCallback(callback);
  this->addCallbacks(callbacks...);
}

void DeviceInputCallbacks::setCallback(DeviceInputCallback callback) {
  this->clearCallbacks();
  this->addCallback(callback);
}

template <typename... Callbacks>
void DeviceInputCallbacks::setCallbacks(Callbacks... callbacks) {
  this->clearCallbacks();
  this->addCallbacks(callbacks...);
}

bool DeviceInputCallbacks::clearCallbacks() {
  bool had_callbacks = !this->callbacks.empty();
  this->callbacks.clear();
  return had_callbacks;
}

bool DeviceInputCallbacks::runCallbacks() {
  if (this->callbacksDisabled()) {
    return false;
  }

  bool callback_ran = false;

  for (DeviceInputCallback& callback : this->callbacks) {
    callback();
    callback_ran = true;
  }

  return callback_ran;
}
