// We can require 'autoreleasepool' and the NSAutoreleasePool for the main
// (node's) thread will be automatically taken care of until the process exits
require('autoreleasepool');

// Ok, so the 'AddressBook' APIs look in "$HOME/Library/AddressBook" of the current
// user. The problem is that "/var/mobile" contains the REAL address book, and
// "/var/root" contains no address book, so when 'node' is run as the 'root' user,
// the address book bindings fail.... well that is unless we symlink to the real
// address book before any JS calls are made... dirty hack but I can't find a
// better way to do it (explicity setting the "HOME" var doesn't do it)...
var abPath = '/var/'+process.env.USER+'/Library/AddressBook';
var mobilePath = '/var/mobile/Library/AddressBook';
if (abPath !== mobilePath) {
  var fs = require('fs');
  if (!fs.lstatSync(abPath).isSymbolicLink()) {
    // Backup the original 'AddressBook' dir, just in case...
    fs.renameSync(abPath, abPath+'.bak');
    fs.symlinkSync(mobilePath, abPath);
  }
  fs = null;
}
abPath = mobilePath = null;

module.exports = require('./build/default/binding.node');
