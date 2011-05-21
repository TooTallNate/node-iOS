#import "addressBook.h"
#include <stdio.h>

using namespace node;
using namespace v8;


// Set up the exports for AddressBook
void AddressBook::Init(v8::Handle<Object> target) {
  HandleScope scope;
  Local<Object> ab = Object::New();
  NODE_SET_METHOD(ab, "getContacts", AddressBook::GetContacts);
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
  struct async_request* ar = (struct async_request*)req->data;

  // 'error' is needed for ABAddressBookSave()
  //CFErrorRef error = NULL;
  ABAddressBookRef addressBook = ABAddressBookCreate();
  CFArrayRef people = ABAddressBookCopyArrayOfAllPeople(addressBook);
  // TODO: Sort by the user's current sort preference by default, or a configurable sort
  CFIndex count = CFArrayGetCount(people);
  ar->results = (struct person_object*) malloc(sizeof(struct person_object) * count);

  int buf_len;
  for (CFIndex i=0; i<count; i++) {
    struct person_object* p = (struct person_object*)(&ar->results + (sizeof(struct person_object)*i));
    ABRecordRef pRef = CFArrayGetValueAtIndex(people, i);

    CFStringRef str = (CFStringRef)ABRecordCopyValue(pRef, kABPersonFirstNameProperty);
//    CFShow(str);
    if (str != NULL) {
      buf_len = CFStringGetMaximumSizeForEncoding( CFStringGetLength(str), kCFStringEncodingUTF8 ) + 1;
      p->firstName = (char*)malloc(buf_len);
      if (!CFStringGetCString(str, p->firstName, buf_len, kCFStringEncodingUTF8)) {
        // TODO Throw error
      }
      CFRelease(str);
    } else {
      p->firstName = NULL;
    }

  }
  ar->resultsCount = count;

  //req->result = CFUserNotificationReceiveResponse(nr->notif, 0, &nr->options);
  CFRelease(people);
  CFRelease(addressBook);
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
      struct person_object* p = (struct person_object*)(&ar->results + (sizeof(struct person_object)*i));
      Local<Object> curPerson = Object::New();
      if (p->firstName != NULL) {
        curPerson->Set(String::NewSymbol("firstName"), String::NewSymbol( p->firstName ));
        // I don't think this is right..., I think we're supposed to free this
        // but I get a nasty error message when this gets uncommented...
        // somebody with some better memory manegement hax0rz needs to take
        // a look...
        //free(p->firstName);
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
