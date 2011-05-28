// http://developer.apple.com/library/ios/documentation/AddressBook/Reference/ABAddressBookRef_iPhoneOS/Reference/reference.html
// http://developer.apple.com/library/ios/documentation/AddressBook/Reference/ABRecordRef_iPhoneOS/Reference/reference.html
// http://developer.apple.com/library/ios/documentation/AddressBook/Reference/ABPersonRef_iPhoneOS/Reference/reference.html
// http://developer.apple.com/library/ios/documentation/AddressBook/Reference/ABGroupRef_iPhoneOS/Reference/reference.html

#import "addressBook.h"
#include <stdio.h>

using namespace node;
using namespace v8;


// Set up the exports for AddressBook
void AddressBook::Init(v8::Handle<Object> target) {
  HandleScope scope;
  Local<Object> ab = Object::New();
  NODE_SET_METHOD(ab, "getContacts", AddressBook::GetContacts);
  NODE_SET_METHOD(ab, "getGroups", AddressBook::GetGroups);
  target->Set(String::NewSymbol("AddressBook"), ab);
}

// From v8's 'shell.cc'
// TODO: Move this to some shared header file
static const char* ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

// 'createNotification' begins the async notification process. The user should pass
// an "options" Object and an optional callback function (to examine the results of
// the notification).
v8::Handle<Value> AddressBook::GetContacts(const Arguments& args) {
  HandleScope scope;

  // This is the struct that gets passed around EIO
  struct async_request* ar = (struct async_request*) malloc(sizeof(struct async_request));

  // TODO: Add support for the search predicate
  //Local<Object> options = args[0]->ToObject();

  // A callback function is optional (though why would you call
  // this if you didn't want the results?)
  ar->hasCb = false;
  int argsLen = args.Length();
  if (argsLen >= 1) {
    Local<Function> cb = Local<Function>::Cast(args[argsLen-1]);
    ar->cb = Persistent<Function>::New(cb);
    ar->hasCb = true;
  }

  eio_custom(GetContacts_DoRequest, EIO_PRI_DEFAULT, GetContacts_AfterResponse, ar);
  ev_ref(EV_DEFAULT_UC);

  return Undefined();
}

int GetContacts_DoRequest (eio_req * req) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  struct async_request* ar = (struct async_request*)req->data;

  ABAddressBookRef addressBook = ABAddressBookCreate();
  CFArrayRef people = ABAddressBookCopyArrayOfAllPeople(addressBook);
  // TODO: Sort by the user's current sort preference by default, or a configurable sort
  CFIndex count = CFArrayGetCount(people);
  struct person_object *results = (struct person_object *) malloc(sizeof(struct person_object) * count);

  for (CFIndex i=0; i<count; i++) {
    struct person_object *p = &(results[i]);
    ABRecordRef pRef = CFArrayGetValueAtIndex(people, i);

    p->recordId = ABRecordGetRecordID(pRef);

    NSString* firstNameStr = (NSString *)ABRecordCopyValue(pRef, kABPersonFirstNameProperty);
    if (firstNameStr != NULL) {
      //buf_len = CFStringGetMaximumSizeForEncoding( CFStringGetLength(firstNameStr), kCFStringEncodingUTF8 ) + 1;
      //p->firstName = (char*)malloc(buf_len);
      //CFStringGetCString(firstNameStr, p->firstName, buf_len, kCFStringEncodingUTF8);
      //CFRelease(firstNameStr);
      p->firstName = (const char*)[firstNameStr UTF8String];
    } else {
      p->firstName = NULL;
    }

    NSString *lastNameStr = (NSString *)ABRecordCopyValue(pRef, kABPersonLastNameProperty);
    if (lastNameStr != NULL) {
      p->lastName = (const char*)[lastNameStr UTF8String];
    } else {
      p->lastName = NULL;
    }



  }
  ar->results = results;
  ar->resultsCount = count;

  CFRelease(people);
  CFRelease(addressBook);
  [pool drain];
  return 0;
}

int GetContacts_AfterResponse (eio_req * req) {
  HandleScope scope;
  ev_unref(EV_DEFAULT_UC);

  struct async_request* ar = (struct async_request*)req->data;

  if (ar->hasCb) {
    // Prepare the callback arguments
    Local<Value> argv[2];
    argv[0] = Local<Value>::New(Null());

    Local<Array> resultsArray = Array::New(ar->resultsCount);
    for (CFIndex i=0; i < ar->resultsCount; i++) {
      //struct person_object* p = (struct person_object*)(&ar->results + (sizeof(struct person_object)*i));
      struct person_object* p = &ar->results[i];
      // TODO: These should be turned into a JavaScript "Contact" instance
      Local<Object> curPerson = Object::New();
      // TODO: This should be a member of the C++ Contact class, invisible to
      //       JS-land. For now this will do...
      curPerson->Set(String::NewSymbol("_id"), Integer::New(p->recordId));
      if (p->firstName != NULL) {
        curPerson->Set(String::NewSymbol("firstName"), String::NewSymbol( p->firstName ));
      }
      if (p->lastName != NULL) {
        curPerson->Set(String::NewSymbol("lastName"), String::NewSymbol( p->lastName ));
      }
      resultsArray->Set(Integer::New(i), curPerson);
    }
    argv[1] = resultsArray;

    // Invoke 'le callback
    TryCatch try_catch;
    ar->cb->Call(Context::GetCurrent()->Global(), 2, argv);
    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }
    ar->cb.Dispose();
  }

  free(ar->results);
  free(ar);
  return 0;
}

v8::Handle<Value> AddressBook::GetGroups(const Arguments& args) {
  HandleScope scope;

  return Undefined();
}
