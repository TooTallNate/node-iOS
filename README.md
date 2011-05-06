node-iOS
========
### Native [node][Node] bindings to iOS functionality (vibrate, acceleromoter, geoservices, etc.)


This module offers native node binding to Apple iOS functionality, meant for `node`
running on an Apple iDevice. It exposes low-level functionality of the device, like
vibrate, sending SMS messages, displaying pop-up user notifications, simulating the
home and lock buttons, and a lot more planned!

Considering the fact that most jailbroken devices won't have a GCC toolchain set up on their
device, this repo includes a pre-built version of the module that hopefully will work on the
majority of devices.


API
---

### createNotification(options[, callback]) -> undefined

Creates and displays a pop-up "notification" onto the iDevice. The optional `callback`
function will be called after the notification has been dismissed (through user interaction
or otherwise cancelled).

The most recent call to this function will be the active notification. That is, if
there's already an active notification, and this function is called, then the newly
created notification will take precedence over any existing notifications. So if you
want to display a series of notifications, it's better invoke the next one in the
previous notification's callback.

``` javascript
iOS.createNotification({
  header: "Title",
  message: "Enter your name..."
}, function(err, response) {
  if (err) throw err;

  console.log(response);  
});
```

The `options` Object accepts the following parameters:

 * `header` - A String that will be used as the header of the notification. Defaults to `null`.
 * `message` - A String that will be used as the message body of the notification. Defaults to `null`.
 * `defaultButton` - The text of the default (primary) button. Defaults to `'OK'`. To disable the
                     default button (notification without any buttons), explicity pass `null` here.
 * `alternateButton` - The text of the secondary (alternate) button. Defaults to `null` (no second button).
 * `otherButton` - The text of the third (other) button. Defaults to `null` (no third button).
 * `timeout` - The timeout in seconds of the notification. Defaults to `0` for no timeout.

### vibrate() -> undefined

Vibrates the iDevice shortly. This is the same as when a text message or email arrives, etc.
On devices that don't vibrate, this function does nothing (no error is thrown).

### device() -> Object

Returns an Object containing properties from the current [UIDevice][]. An example:

``` javascript
{ model: 'iPhone',
  localizedModel: 'iPhone',
  name: 'Nathan Rajlich’s iPhone',
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
