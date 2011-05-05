node-iOS
========
### Native [node][Node] bindings to iOS functionality (vibrate, acceleromoter, geoservices, etc.)


This module offers native node binding to iOS functionality, meant for node running on an
Apple iDevice. It exposes low-level functionality of the device, like vibrate, sounds,
digital compass, geolocation, the accelerometer, and a lot more!

Considering the fact that most jailbroken devices won't have a GCC toolchain set up on their
device, this repo includes a pre-build version of the module that hopefully will work on the
mojority of devices.


Silly Examples
--------------

``` javascript
var iOS = require('iOS');

iOS.vibrate();
// Quick vibrate, like when you receive text message

iOS.device();
// { model: 'iPhone',
//   localizedModel: 'iPhone',
//   name: 'Nathan Rajlich’s iPhone',
//   systemName: 'iPhone OS',
//   systemVersion: '4.3.1',
//   uniqueIdentifier: 'f1dfb3fa9f73fc9ffef4fcf3f61fff6f05ff1afb' }
```

See the `API` section below to learn it all!


API
---

### vibrate() -> undefined

Vibrates the iDevice shortly. This is the same as when a text message or email arrives, etc.
On devices that don't vibrate, this function does nothing (no error is thrown).

### device() -> Object

Returns an Object containing properties from the current [UIDevice][]. An example:

``` javascript
{ model: 'iPhone',
  localizedModel: 'iPhone',
  name: 'Nathan Rajlich\'s iPhone',
  systemName: 'iPhone OS',
  systemVersion: '4.3.1',
  uniqueIdentifier: 'f1dfb3fa9f73fc9ffef4fcf3f61fff6f05ff1afb' }
```

### sendSMS(number, message) -> Boolean

Sends an SMS with the specified `message` String to the specified `number`. Examples:

``` javascript
iOS.sendSMS('5555555555', 'this is a text message!');
iOS.sendSMS('555-555-5555', 'another text message!');
iOS.sendSMS('(555) 555-5555', 'and one more?!');
iOS.sendSMS(5555555555, 'you may just use a Number as well');
```

### lockScreen() -> Undefined

Locks the screen of the iDevice. Same effect as pressing the top "Lock" button.

### quitTopApplication() -> Undefined

Quits the currently visible application, going straight to the Home screen. On a
device with multitasking support, the app will still be running in the process list.
Same effect as pressing the "Home" button.


[Node]: http://nodejs.org
[UIDevice]: http://developer.apple.com/library/ios/#documentation/uikit/reference/UIDevice_Class/Reference/UIDevice.html
