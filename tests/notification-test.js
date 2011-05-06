var iOS = require('../');

iOS.createNotification({
  message: 'This message will have NO buttons!',
  defaultButton: null,
  timeout: 3
}, console.log);

iOS.createNotification({
  header: 'test header',
  message: 'This is a message!!',
  defaultButton: 'first',
  alternateButton: 'second'
}, console.log);
