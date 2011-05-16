var iOS = require('../');

// Get an Array of all the Contacts in the address book
iOS.AddressBook.getContacts(function(err, results) {
  if (err) throw err;
  console.log(results);
  console.log(results.length);
});
