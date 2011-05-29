#import <node.h>
#import <v8.h>
#import <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>
#import <AddressBook/AddressBook.h>
#import "addressBook-Record.h"
#import "addressBook-Contact.h"

int GetContacts_DoRequest (eio_req *);
int GetContacts_AfterResponse (eio_req *);

struct async_request {
  v8::Persistent<v8::Function> cb;
  bool hasCb;
  CFIndex resultsCount;
  // 'results' is an array of pointers to "Record" instances
  Record **results;
};

class AddressBook {
  public:
    static void Init(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> GetContacts(const v8::Arguments& args);
    static v8::Handle<v8::Value> GetGroups(const v8::Arguments& args);
    static v8::Handle<v8::Value> Save(const v8::Arguments& args);
};
