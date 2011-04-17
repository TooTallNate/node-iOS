#import <node.h>
#import <v8.h>
#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>
#import <CoreTelephony/CoreTelephony.h>

class Telephony {
  public:
    static v8::Handle<v8::Value> Init(const v8::Arguments& args);
    static v8::Handle<v8::Value> SendSMS(const v8::Arguments& args);
};
