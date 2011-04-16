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

    var iOS = require('iOS');

    iOS.vibrate();
    // Quick vibrate, like when you receive text message

    iOS.device();
    // { model: 'iPhone',
    //   localizedModel: 'iPhone',
    //   name: 'Nathan Rajlich\'s iPhone',
    //   systemName: 'iPhone OS',
    //   systemVersion: '4.3.1',
    //   uniqueIdentifier: 'f1dfb3fa9f73fc9ffef4fcf3f61fff6f05ff1afb' }

    

See the `API` section below to learn it all!


API
---

### vibrate() -> undefined

Vibrates the iDevice shortly. This is the same as when a text message or email arrives, etc.
On devices that don't vibrate, this function does nothing (no error is thrown).

### device() -> Object

Returns an Object containing properties from the current [UIDevice][]. An example:

    { model: 'iPhone',
      localizedModel: 'iPhone',
      name: 'Nathan Rajlich\'s iPhone',
      systemName: 'iPhone OS',
      systemVersion: '4.3.1',
      uniqueIdentifier: 'f1dfb3fa9f73fc9ffef4fcf3f61fff6f05ff1afb' }



[Node]: http://nodejs.org
[UIDevice]: http://developer.apple.com/library/ios/#documentation/uikit/reference/UIDevice_Class/Reference/UIDevice.html
