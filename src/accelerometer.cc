#import "accelerometer.h"

using namespace node;
using namespace v8;

// ObjC
@implementation AccelerometerDelegate

- (id)init {
  NSLog(@"Creating new 'AccelerometerDelegate'");
  return [super init];
}

- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {
    NSLog(@"(%.02f, %.02f, %.02f)", acceleration.x, acceleration.y, acceleration.z);
}

@end

// C++

Accelerometer::Accelerometer () {
  accelerometer = [UIAccelerometer sharedAccelerometer];
  del = [[AccelerometerDelegate alloc] init];
  accelerometer.updateInterval = 0.05f;
  accelerometer.delegate = del;
}

v8::Handle<Value> Accelerometer::Init(const Arguments& args) {
  //HandleScope scope;
  Accelerometer* a = new Accelerometer();
  //a->Wrap(args.This());
  //return args.This();
  return Undefined();
}

  /*static v8::Handle<Value> New(const Arguments& args) {
    HandleScope scope;
    Accelerometer* a = new Accelerometer();
    a->Wrap(args.This());
    return args.This();
  }*/
