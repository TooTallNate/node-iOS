#include <node.h>
#include <v8.h>
#import <UIKit/UIKit.h>

@interface AccelerometerDelegate : NSObject<UIAccelerometerDelegate> {
}

@end

class Accelerometer : node::ObjectWrap {
  private:
    UIAccelerometer* accelerometer;
    AccelerometerDelegate* del;

  public:

    Accelerometer();
    static v8::Handle<v8::Value> Init(const v8::Arguments& args);
};
