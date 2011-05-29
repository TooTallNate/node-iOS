#import <node.h>
#import <AddressBook/AddressBook.h>

class Record : public node::ObjectWrap {
  public:
    ABRecordID recordId;
}; // class Record
