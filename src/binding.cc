#import <v8.h>
#import <node.h>
#import <AudioToolbox/AudioToolbox.h>
#import <UIKit/UIKit.h>
#import "addressBook.h"
#import "graphicServices.h"
#import "notifications.h"
#import "telephony.h"
#import "compatibility.h" // ...meh

using namespace node;
using namespace v8;

class Binding {
  public:

  static void Init(v8::Handle<Object> target) {
    HandleScope scope;

    UIDevice *aDevice = [UIDevice currentDevice];
    [aDevice beginGeneratingDeviceOrientationNotifications];
    [aDevice setBatteryMonitoringEnabled:YES];

    NODE_SET_METHOD(target, "vibrate", Vibrate);
    NODE_SET_METHOD(target, "device", Device);
    NODE_SET_METHOD(target, "sendSMS", Telephony::SendSMS);
  }

  static v8::Handle<Value> Vibrate(const Arguments& args) {
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
    return Undefined();
  }

  static v8::Handle<Value> Device(const Arguments& args) {
    HandleScope scope;
    Local<Object> result = Object::New();

    UIDevice *aDevice = [UIDevice currentDevice];

    result->Set(String::NewSymbol("batteryLevel"), Number::New([aDevice batteryLevel]));
    result->Set(String::NewSymbol("batteryState"), Integer::New([aDevice batteryState]));
    result->Set(String::NewSymbol("model"), String::NewSymbol([[aDevice model] UTF8String]));
    result->Set(String::NewSymbol("localizedModel"), String::NewSymbol([[aDevice localizedModel] UTF8String]));
    result->Set(String::NewSymbol("orientation"), Integer::New([aDevice orientation]));
    result->Set(String::NewSymbol("name"), String::NewSymbol([[aDevice name] UTF8String]));
    result->Set(String::NewSymbol("systemName"), String::NewSymbol([[aDevice systemName] UTF8String]));
    result->Set(String::NewSymbol("systemVersion"), String::NewSymbol([[aDevice systemVersion] UTF8String]));
    result->Set(String::NewSymbol("uniqueIdentifier"), String::NewSymbol([[aDevice uniqueIdentifier] UTF8String]));

    return scope.Close(result);
  }

};

extern "C" {
  static void init (v8::Handle<Object> target) {
    Binding::Init(target);
    AddressBook::Init(target);
    GraphicServices::Init(target);
    Notifications::Init(target);
  }

  NODE_MODULE(binding, init);
}
