#import "notifications.h"

using namespace node;
using namespace v8;


// Initialize the notification-related exports
void Notifications::Init(v8::Handle<Object> target) {
  HandleScope scope;
  NODE_SET_METHOD(target, "createNotification", Notifications::createNotification);
}

// From v8's 'shell.cc'
const char* ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

// 'createNotification' begins the async notification process. The user should pass
// an "options" Object and an optional callback function (to examine the results of
// the notification).
v8::Handle<Value> Notifications::createNotification(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1) {
    return ThrowException(Exception::TypeError(String::New("An 'options' Object is required")));
  }

  Local<Object> options = args[0]->ToObject();

  // This is the struct that gets passed around EIO
  notification_request* nr = (notification_request*) malloc(sizeof(struct notification_request));

  CFOptionFlags flags = 0;
  flags |= kCFUserNotificationPlainAlertLevel;

  // 'dict' will contain the processed 'options', so the alert message is formatted the way the user asked
  CFMutableDictionaryRef dict = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

  // This is always set. It appears to be needed for daemon processes on iOS...
  CFDictionaryAddValue(dict, kCFUserNotificationAlertTopMostKey, kCFBooleanTrue);

  if (options->Has(String::NewSymbol("header"))) {
    String::Utf8Value headerStr(options->Get(String::NewSymbol("header")));
    CFDictionaryAddValue(dict, kCFUserNotificationAlertHeaderKey, CFStringCreateWithCString(NULL, ToCString(headerStr), kCFStringEncodingUTF8) );
  }
  if (options->Has(String::NewSymbol("message"))) {
    String::Utf8Value messageStr(options->Get(String::NewSymbol("message")));
    CFDictionaryAddValue(dict, kCFUserNotificationAlertMessageKey, CFStringCreateWithCString(NULL, ToCString(messageStr), kCFStringEncodingUTF8) );
  }
  if (options->Has(String::NewSymbol("defaultButton"))) {
    Local<Value> defaultButtonVal = options->Get(String::NewSymbol("defaultButton"));
    if (defaultButtonVal->IsNull()) {
      flags |= kCFUserNotificationNoDefaultButtonFlag;
    } else {
      String::Utf8Value firstButtonStr(defaultButtonVal);
      CFDictionaryAddValue(dict, kCFUserNotificationDefaultButtonTitleKey, CFStringCreateWithCString(NULL, ToCString(firstButtonStr), kCFStringEncodingUTF8) );
    }
  }
  if (options->Has(String::NewSymbol("alternateButton"))) {
    String::Utf8Value secondButtonStr(options->Get(String::NewSymbol("alternateButton")));
    CFDictionaryAddValue(dict, kCFUserNotificationAlternateButtonTitleKey, CFStringCreateWithCString(NULL, ToCString(secondButtonStr), kCFStringEncodingUTF8) );
  }
  if (options->Has(String::NewSymbol("otherButton"))) {
    String::Utf8Value thirdButtonStr(options->Get(String::NewSymbol("otherButton")));
    CFDictionaryAddValue(dict, kCFUserNotificationOtherButtonTitleKey, CFStringCreateWithCString(NULL, ToCString(thirdButtonStr), kCFStringEncodingUTF8) );
  }

  CFTimeInterval timeout = 0; // 0 = disabled, by default
  if (options->Has(String::NewSymbol("timeout"))) {
    timeout = options->Get(String::NewSymbol("timeout"))->Int32Value();
  }

  CFUserNotificationRef notif = CFUserNotificationCreate(NULL, timeout, flags, &nr->error, dict);
  nr->notif = notif;

  nr->hasCb = false;
  if (args.Length() >= 2) {
    Local<Function> cb = Local<Function>::Cast(args[1]);
    nr->cb = Persistent<Function>::New(cb);
    nr->hasCb = true;
  }

  eio_custom(CreateNotification_WaitForResponse, EIO_PRI_DEFAULT, CreateNotification_AfterResponse, nr);
  ev_ref(EV_DEFAULT_UC);

  return Undefined();
}

// This is the function that gets called on the thread pool. It blocks for as long as
// the notification is active on the iDevice.
int CreateNotification_WaitForResponse (eio_req * req) {
  struct notification_request * nr = (struct notification_request *)req->data;
  req->result = CFUserNotificationReceiveResponse(nr->notif, 0, &nr->options);
  return 0;
}

// This function gets called on node's main thread after the notification has been
// dismissed. This function collects the results of the dismissal into a 'results'
// Object that gets passed to the JS callback function.
int CreateNotification_AfterResponse (eio_req * req) {
  HandleScope scope;
  ev_unref(EV_DEFAULT_UC);

  struct notification_request * nr = (struct notification_request *)req->data;

  if (nr->hasCb) {
    // Prepare the callback arguments
    Local<Value> argv[2];
    argv[0] = Local<Value>::New(Null());

    Local<Object> results = Object::New();
    results->Set(String::NewSymbol("result"), Integer::New( req->result ));
    results->Set(String::NewSymbol("error"), Integer::New( nr->error ));
    results->Set(String::NewSymbol("options"), Integer::New( nr->options ));
    argv[1] = results;

    // Invoke 'le callback
    TryCatch try_catch;
    nr->cb->Call(Context::GetCurrent()->Global(), 2, argv);
    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }
    nr->cb.Dispose();
  }

  free(nr);
  return 0;
}
