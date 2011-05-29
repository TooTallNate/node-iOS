#import "addressBook-Record.h"

class Contact : public Record {
  public:
    const char* firstName;
    const char* lastName;
    int numNumbers;
    const char ** numbersNames;
    const char ** numbersValues;
}; // class Contact
