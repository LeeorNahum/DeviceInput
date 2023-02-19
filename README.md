# DeviceInput Library

An Arduino library for creating hardware or software inputs with customizable callbacks, detection ranges, and update intervals.

## Table of Contents

- [Description](#description)
- [Library Capabilities](#library-capabilities)
  - [DeviceInput](#deviceinput)
    - [DeviceInputCallbacks](#deviceinputcallbacks)
  - [DeviceInputList](#deviceinputlist)
- [Examples](#examples)
  - [Simple examples](#simple-examples)
    - [Creating a DeviceInput with an input function, detection exact and update interval](#creating-a-deviceinput-with-an-input-function-detection-exact-and-update-interval)
    - [Creating a DeviceInput with an input function, detection range and update interval](#creating-a-deviceinput-with-an-input-function-detection-range-and-update-interval)
    - [Adding a callback for toggled state](#adding-a-callback-for-toggled-state)
    - [Clearing callbacks](#clearing-callbacks)
  - [Complex examples](#complex-examples)
    - [General example of all library capabilities](#general-example-of-all-library-capabilities)
    - [Example of callbacks](#example-of-callbacks)

## Description

The DeviceInput library provides an easy way to add hardware or software inputs to your project. It allows you to set callbacks for different input states and conditions. You can also set detection ranges and update intervals for the inputs. The library includes a DeviceInputList class for managing multiple DeviceInputs.

## Library Capabilities

## DeviceInput

Set an input function that returns a value
Set a detection range or an exact detection value
Set an update interval in milliseconds
Update the input reading
Get the current and last readings
Check if the input is detected or undetected, rising or falling
Invert the detection
Get the toggle and untoggle timestamps
Get the elapsed time for detection and undetection
Update the input state

- Set an input function that returns the state or value of the input
- Set an exact detection value or a detection range for the input
- Set an update interval for the input in milliseconds
- Invert the detected state of the input
- Update the reading and detected state of the input
- Get the reading, last reading, detected, undetected, last detected, toggled, untoggled, rising reading, and falling reading, states of the input
- Get the toggle and untoggle timestamps of the input
- Get the elapsed time since the input was detected or undetected

## DeviceInputCallbacks

- Enable or disable callbacks
- Check if callbacks are enabled or disabled
- Check if there are any callbacks or callbacks of a specific type
- Add one or multiple callbacks of a specific type
- Set a callback of a specific type
- Add and manage callbacks for different input states (toggle, untoggle, detected, undetected, rising reading, falling reading)

## DeviceInputList

- Create a list of multiple DeviceInput objects
- Access individual DeviceInput objects within the list
- Update the state of all inputs in the list at once
- Get the current and last readings for all inputs in the list
- Check if any input in the list is detected or undetected
- Get the toggle and untoggle timestamps for all inputs in the list
- Get the elapsed time for detection and undetection for all inputs in the list
- Add, remove, or retrieve DeviceInput objects from the list
- Sort the list based on various criteria such as detection state or elapsed time

## Examples

## Simple Examples

### Creating a DeviceInput with an input function, detection exact and update interval

``` C++
#include <DeviceInput.h>

int getButtonState() {
  // return the state of a button
}

DeviceInput<int> button(getButtonState, 1, 100);
```

### Creating a DeviceInput with an input function, detection range and update interval

``` C++
#include <DeviceInput.h>

int getSensorValue() {
  // return the value of a sensor
}

int range[2] = {10, 20};
DeviceInput<int> sensor(getSensorValue, range, 100);
```

### Adding a callback for toggled state

``` C++
#include <DeviceInput.h>

void toggleCallback() {
  // do something when the button is toggled
}

DeviceInput<int> button(getButtonState, 1, 100);
button.addCallback(TOGGLED, toggleCallback);
```

### Adding multiple callbacks for different states

``` C++
#include <DeviceInput.h>

void toggleCallback() {
  // do something when the button is toggled
}

void untoggleCallback() {
  // do something when the button is untoggled
}

DeviceInput<int> button(getButtonState, 1, 100);
button.addCallbacks(TOGGLED, toggleCallback, UNTOGGLED, untoggleCallback);
```

### Clearing callbacks

You can clear all callbacks for a button:

``` C++
Button button;
button.clearCallbacks();
```

### Running Callbacks

You can run all callbacks for a button:

``` C++
Button button;
button.runCallbacks();
```

### Disabling and Enabling Callbacks

You can temporarily disable callbacks for a button:

``` C++
Button button;
button.disableCallbacks();
button.runCallbacks(); // no callbacks will be executed
button.enableCallbacks();
button.runCallbacks(); // callbacks will be executed again
```

### Initializing a DeviceInputList

``` C++
// Initialize an empty DeviceInputList with an update interval of 100ms
DeviceInputList<> list1(100);

// Initialize a DeviceInputList with multiple DeviceInput objects and an update interval of 100ms
DeviceInput device1, device2, device3;
DeviceInputList<> list2(100, &device1, &device2, &device3);

// Initialize a DeviceInputList with multiple DeviceInput objects
DeviceInput device4, device5, device6;
DeviceInputList<> list3(&device4, &device5, &device6);

// Initialize a DeviceInputList with an array of DeviceInput objects and an update interval of 100ms
DeviceInput deviceArray[5];
DeviceInputList<> list4(deviceArray, 100);
```

### Adding and setting DeviceInput objects in a DeviceInputList

``` C++
// Adding a single DeviceInput object to a DeviceInputList
DeviceInput device7;
list1.addDeviceInput(&device7);

// Adding multiple DeviceInput objects to a DeviceInputList
DeviceInput device8, device9;
list1.addDeviceInputs(&device8, &device9);

// Setting a single DeviceInput object in a DeviceInputList
DeviceInput device10;
list1.setDeviceInput(&device10);

// Setting multiple DeviceInput objects in a DeviceInputList
DeviceInput device11, device12;
list1.setDeviceInputs(&device11, &device12);

// Setting multiple DeviceInput objects in a DeviceInputList using an array
DeviceInput deviceArray2[3];
list1.setDeviceInputs(deviceArray2);
```

### Updating the values of all DeviceInput objects in a DeviceInputList

``` C++
list1.updateAll();
```

### Setting update interval for a DeviceInputList

``` C++
list1.setUpdateInterval(50);
```

### Adding and setting callbacks for all DeviceInput objects in a DeviceInputList

``` C++
// Adding a single callback for all DeviceInput objects in a DeviceInputList
list1.addCallbackForAll(Toggle, toggleCallback1);

// Adding multiple callbacks for all DeviceInput objects in a DeviceInputList
list1.addCallbacksForAll(Toggle, toggleCallback2, toggleCallback3);

// Adding multiple callbacks for all DeviceInput objects in a DeviceInputList using an array
void toggleCallbackArray[2] = {toggleCallback4, toggleCallback5};
list1.addCallbacksForAll(Toggle, toggleCallbackArray);

// Setting a single callback for all DeviceInput objects in a DeviceInputList
list1.setCallbackForAll(Toggle, toggleCallback6);

// Setting multiple callbacks for all DeviceInput objects in a DeviceInputList
list1.setCallbacksForAll(Toggle, toggleCallback7, toggleCallback8);

// Setting multiple callbacks for all DeviceInput objects in a DeviceInputList using an array
void toggleCallbackArray2[2] = {toggleCallback9, toggleCallback10};
list1.setCallbacksForAll(Toggle, toggleCallbackArray2);
```

## Complex Examples

### General example of all library capabilities

``` C++
#include <DeviceInput.h>

int inputFunction() {
  return analogRead(A0);
}

DeviceInput<> input(inputFunction, 512);

void setup() {
  input.setDetectionRange({300, 700});
  input.setUpdateInterval(100);
}

void loop() {
  input.update();
  
  if (input.getDetected()) {
    Serial.println("Detected");
  }
}
```

### Example of callbacks

``` C++
#include <DeviceInput.h>

int inputFunction() {
  return analogRead(A0);
}

DeviceInput<> input(inputFunction, 512);

void setup() {
  input.setDetectionRange({300, 700});
  input.setUpdateInterval(100);
}

void loop() {
  input.update();
  
  if (input.getDetected()) {
    Serial.println("Detected");
  }
}
```
