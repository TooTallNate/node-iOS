#import <node.h>
#import <v8.h>
#import <unistd.h>
#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>
#import <CoreFoundation/CFUserNotification.h>

int CreateNotification_WaitForResponse (eio_req *);
int CreateNotification_AfterResponse (eio_req *);

struct notification_request {
  CFOptionFlags options;
  CFUserNotificationRef notif;
  SInt32 error;
  v8::Persistent<v8::Function> cb;
};

class Notifications {
  public:
    static void Init(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> createNotification(const v8::Arguments& args);
};
