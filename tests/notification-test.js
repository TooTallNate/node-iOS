var iOS = require('../');

iOS.createNotification({
  message: 'This message will have NO buttons!',
  defaultButton: null,
  timeout: 3
}, function(err, res) {
  console.log(arguments);

  iOS.createNotification({
    header: 'test header',
    message: 'This is a message!!',
    defaultButton: 'first',
    alternateButton: 'second'
  }, console.log);
});

