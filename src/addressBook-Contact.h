#import "addressBook-Record.h"

class Contact : public Record {
  public:
    // Person
    const char *firstName;
    const char *middleName;
    const char *lastName;
    // Organization
    const char *organization;
    const char *jobTitle;
    const char *department;
    // Phone Numbers
    int numNumbers;
    const char **numbersNames;
    const char **numbersValues;
}; // class Contact
