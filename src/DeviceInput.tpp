template <typename TReturn>
template <typename... CallbacksAndTypes>
DeviceInput<TReturn>::DeviceInput(InputFunction input_function, TReturn detection_exact, int update_interval_ms, CallbacksAndTypes... callbacks_and_types) {
  this->setInputFunction(input_function);
  this->setDetectionExact(detection_exact);
  this->setUpdateInterval(update_interval_ms);
  this->setCallbacks(callbacks_and_types...);
}

template <typename TReturn>
template <typename... CallbacksAndTypes>
DeviceInput<TReturn>::DeviceInput(InputFunction input_function, TReturn detection_exact, CallbacksAndTypes... callbacks_and_types) {
  this->setInputFunction(input_function);
  this->setDetectionExact(detection_exact);
  this->setCallbacks(callbacks_and_types...);
}

template <typename TReturn>
template <typename... CallbacksAndTypes>
DeviceInput<TReturn>::DeviceInput(InputFunction input_function, TReturn detection_range[2], int update_interval_ms, CallbacksAndTypes... callbacks_and_types) {
  this->setInputFunction(input_function);
  this->setDetectionRange(detection_range);
  this->setUpdateInterval(update_interval_ms);
  this->setCallbacks(callbacks_and_types...);
}

template <typename TReturn>
template <typename... CallbacksAndTypes>
DeviceInput<TReturn>::DeviceInput(InputFunction input_function, TReturn detection_range[2], CallbacksAndTypes... callbacks_and_types) {
  this->setInputFunction(input_function);
  this->setDetectionRange(detection_range);
  this->setCallbacks(callbacks_and_types...);
}

template <typename TReturn>
void DeviceInput<TReturn>::setInputFunction(InputFunction input_function) {
  this->input_function = input_function;
  this->updateReading();
}

template <typename TReturn>
void DeviceInput<TReturn>::setDetectionExact(TReturn exact) {
  this->detection_exact = exact;
  this->is_detection_range = false;

  this->detection_range[0] = 0;
  this->detection_range[1] = 0;
  
  this->updateDetected();
  this->updateReading();
}

template <typename TReturn>
bool DeviceInput<TReturn>::setDetectionRange(TReturn range[2]) {
  if (sizeof(range)/sizeof(range[0]) < 2) {
    this->setDetectionExact(0);
    return false;
  }

  this->detection_range[0] = range[0];
  this->detection_range[1] = range[1];
  this->is_detection_range = true;

  this->detection_exact = 0;
  
  this->updateDetected();
  this->updateReading();

  return true;
}

template <typename TReturn>
TReturn DeviceInput<TReturn>::getDetectionExact() {
  return this->detection_exact;
}

template <typename TReturn>
TReturn* DeviceInput<TReturn>::getDetectionRange() {
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
    if (this->getReading() >= this->getDetectionRange()[0] && this->getReading() <= this->getDetectionRange()[1]) {
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
    this->toggle_timestamp = millis();
  }
  if (this->getUntoggled()) {
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