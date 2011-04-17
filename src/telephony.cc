#import "telephony.h"

using namespace node;
using namespace v8;

//extern id CTTelephonyCenterGetDefault();
//extern void CTTelephonyCenterAddObserver(void*,id,CFNotificationCallback,NSString*,void*,int);

//static void callback(CFNotificationCenterRef center, void *observer, CFStringRef name, const void *object, CFDictionaryRef userInfo) {
//  NSLog(@"Name: %@", name);
//}

//v8::Handle<Value> Telephony::Init(const Arguments& args) {
//  id ct = CTTelephonyCenterGetDefault();
//  CTTelephonyCenterAddObserver(ct, NULL, callback, NULL, NULL, CFNotificationSuspensionBehaviorHold);
//  return Undefined();
//}

// Sends an SMS. First argument should be the number, second arg should be the message.
// TODO: All support for a 'sent' callback or something... I'm not sure if this is possible...
v8::Handle<Value> Telephony::SendSMS(const Arguments& args) {
  HandleScope scope;
  if (args.Length() != 2) {
    return ThrowException(Exception::TypeError(
                  String::New("\"sendSMS\" requires two String arguments, a number and the message")));
  }
  NSString* number = [NSString stringWithUTF8String: *String::Utf8Value(args[0]->ToString()) ];
  NSString* message = [NSString stringWithUTF8String: *String::Utf8Value(args[1]->ToString()) ];
  BOOL success = [[CTMessageCenter sharedMessageCenter] sendSMSWithText:message serviceCenter:nil toAddress:number];
  return scope.Close(v8::Boolean::New(success));
}
