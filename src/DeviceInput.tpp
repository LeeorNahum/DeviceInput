template <typename TReturn>
template <typename... Callbacks>
DeviceInput<TReturn>::DeviceInput(const InputFunction& input_function, TReturn detection_exact, int update_interval_ms, Callbacks... callbacks) {
  this->setInputFunction(input_function);
  this->setDetectionExact(detection_exact);
  this->setUpdateInterval(update_interval_ms);
  this->setCallbacks(callbacks...);
}

template <typename TReturn>
template <typename... Callbacks>
DeviceInput<TReturn>::DeviceInput(const InputFunction& input_function, TReturn detection_exact, Callbacks... callbacks) {
  this->setInputFunction(input_function);
  this->setDetectionExact(detection_exact);
  this->setCallbacks(callbacks...);
}

template <typename TReturn>
template <typename T, typename... Callbacks>
DeviceInput<TReturn>::DeviceInput(const InputFunction& input_function, DetectionRange<T> range, int update_interval_ms, Callbacks... callbacks) {
  this->setInputFunction(input_function);
  this->setDetectionRange(range);
  this->setUpdateInterval(update_interval_ms);
  this->setCallbacks(callbacks...);
}

template <typename TReturn>
template <typename T, typename... Callbacks>
DeviceInput<TReturn>::DeviceInput(const InputFunction& input_function, DetectionRange<T> range, Callbacks... callbacks) {
  this->setInputFunction(input_function);
  this->setDetectionRange(range);
  this->setCallbacks(callbacks...);
}

template <typename TReturn>
void DeviceInput<TReturn>::setInputFunction(const InputFunction& input_function) {
  this->input_function = input_function;
  this->updateReading();
}

template <typename TReturn>
void DeviceInput<TReturn>::setDetectionExact(TReturn exact) {
  this->detection_exact = exact;
  this->is_detection_range = false;

  this->detection_range.min = 0;
  this->detection_range.max = 0;
  
  this->updateDetected();
  this->updateReading();
}

template <typename TReturn>
template <typename T>
void DeviceInput<TReturn>::setDetectionRange(DetectionRange<T> range) {
  this->detection_range.min = range.min;
  this->detection_range.max = range.max;
  this->is_detection_range = true;

  this->detection_exact = true;

  this->updateReading();
  this->updateDetected();
}

template <typename TReturn>
TReturn DeviceInput<TReturn>::getDetectionExact() {
  return this->detection_exact;
}

template <typename TReturn>
DetectionRange<TReturn> DeviceInput<TReturn>::getDetectionRange() {
  return this->detection_range;
}

template <typename TReturn>
void DeviceInput<TReturn>::setUpdateInterval(int update_interval_ms) {
  this->update_interval_ms = update_interval_ms;
}

template <typename TReturn>
int DeviceInput<TReturn>::getUpdateInterval() {
  return this->update_interval_ms;
}

template <typename TReturn>
void DeviceInput<TReturn>::updateReading() {
  this->last_reading = this->getReading();
  this->reading = this->input_function();
}

template <typename TReturn>
TReturn DeviceInput<TReturn>::getReading() {
  return this->reading;
}

template <typename TReturn>
TReturn DeviceInput<TReturn>::getLastReading() {
  return this->last_reading;
}

template <typename TReturn>
bool DeviceInput<TReturn>::getRisingReading() {
  return this->getLastReading() < this->getReading();
}

template <typename TReturn>
bool DeviceInput<TReturn>::getFallingReading() {
  return this->getLastReading() > this->getReading();
}

template <typename TReturn>
void DeviceInput<TReturn>::invertDetected(bool invert) {
  this->invert_detected = invert;
}

template <typename TReturn>
bool DeviceInput<TReturn>::updateDetected() {
  this->last_detected = this->getDetected();

  if (this->getReading() == this->getLastReading()) {
    return false;
  }

  this->detected = false;
  if (this->is_detection_range) {
    if (this->getReading() >= this->getDetectionRange().min && this->getReading() <= this->getDetectionRange().max) {
      this->detected = true;
    }
  }
  else if (this->getReading() == this->getDetectionExact()) {
    this->detected = true;
  }
  
  if (this->invert_detected) {
    this->detected = !this->detected;
  }
  
  if (this->getToggled()) {
    this->last_toggle_timestamp = this->toggle_timestamp;
    this->toggle_timestamp = millis();
  }
  else if (this->getUntoggled()) {
    this->last_untoggle_timestamp = this->untoggle_timestamp;
    this->untoggle_timestamp = millis();
  }
  
  return true;
}

template <typename TReturn>
bool DeviceInput<TReturn>::getDetected() {
  return this->detected;
}

template <typename TReturn>
bool DeviceInput<TReturn>::getLastDetected() {
  return this->last_detected;
}

template <typename TReturn>
bool DeviceInput<TReturn>::getUndetected() {
  return !this->detected;
}

template <typename TReturn>
bool DeviceInput<TReturn>::getToggled() {
  return this->getLastDetected() < this->getDetected();
}

template <typename TReturn>
bool DeviceInput<TReturn>::getUntoggled() {
  return this->getLastDetected() > this->getDetected();
}

template <typename TReturn>
unsigned long DeviceInput<TReturn>::getToggleTimestamp() {
  return this->toggle_timestamp;
}

template <typename TReturn>
unsigned long DeviceInput<TReturn>::getUntoggleTimestamp() {
  return this->untoggle_timestamp;
}

template <typename TReturn>
unsigned long DeviceInput<TReturn>::getLastToggleTimestamp() {
  return this->last_toggle_timestamp;
}

template <typename TReturn>
unsigned long DeviceInput<TReturn>::getLastUntoggleTimestamp() {
  return this->last_untoggle_timestamp;
}

template <typename TReturn>
unsigned long DeviceInput<TReturn>::getElapsedTimeDetected() {
  return millis() - this->getToggleTimestamp();
}

template <typename TReturn>
unsigned long DeviceInput<TReturn>::getElapsedTimeUndetected() {
  return millis() - this->getUntoggleTimestamp();
}

template <typename TReturn>
bool DeviceInput<TReturn>::update() {
  if (this->getUpdateInterval() > 0) {
    unsigned long current_time = millis();
    if (current_time - this->last_update_time < this->getUpdateInterval()) {
      return false;
    }
    this->last_update_time = current_time;
  }
  
  this->updateReading();
  this->updateDetected();
  this->runCallbacks();
  return true;
}