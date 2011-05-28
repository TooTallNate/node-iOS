#import <node.h>
#import <v8.h>
#import <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>
#import <AddressBook/AddressBook.h>

int GetContacts_DoRequest (eio_req *);
int GetContacts_AfterResponse (eio_req *);

struct person_object {
  ABRecordID recordId;
  const char* firstName;
  const char* lastName;
};

struct async_request {
  v8::Persistent<v8::Function> cb;
  bool hasCb;
  CFIndex resultsCount;
  // 'results' is actually a malloc'd array of person_object's
  person_object *results;
};

/*struct class_object {

};*/

class AddressBook {
  public:
    static void Init(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> GetContacts(const v8::Arguments& args);
    static v8::Handle<v8::Value> GetGroups(const v8::Arguments& args);
    static v8::Handle<v8::Value> Save(const v8::Arguments& args);
};
