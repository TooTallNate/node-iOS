node-iOS
========
### Native [node][Node] bindings to iOS functionality (vibrate, acceleromoter, geoservices, etc.)


This module offers native node binding to iOS functionality, meant for node running on an
Apple iDevice. It exposes low-level functionality of the device, like vibrate, sounds,
digital compass, geolocation, the accelerometer, and a lot more!

Considering the fact that most jailbroken devices won't have a GCC toolchain set up on their
device, this repo includes a pre-build version of the module that hopefully will work on all
devices.


Silly Example
-------------

    var iOS = require('iOS');

    iOS.vibrate();
    // Quick vibrate, like when you receive text message

    if (iOS.compass.available) { // true if the iDevice has a digital compass
      iOS.compass.on('heading', function(heading) {
        console.log(heading.magneticHeading); // Degrees relative to magnetic north
      });
    }

See the `API` section below to learn it all!


API
---

### vibrate() -> undefined

Vibrates the iDevice shortly. This is the same as when a text message or email arrives, etc.
On devices that don't vibrate, this function does nothing (no error is thrown).


[Node]: http://nodejs.org
