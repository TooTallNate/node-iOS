#import "graphicServices.h"

using namespace node;
using namespace v8;

void GraphicServices::Init(v8::Handle<Object> target) {
  HandleScope scope;

  NODE_SET_METHOD(target, "lockScreen", LockScreen);
  NODE_SET_METHOD(target, "quitTopApplication", QuitTopApplication);
}

/* Same effect as pressing the top "Lock" button on your iDevice. No arguments required. */
v8::Handle<Value> GraphicServices::LockScreen(const Arguments& args) {
  HandleScope scope;
  GSEventLockDevice();
  GSEventLockDevice();
  return scope.Close(Undefined());
}

/* Quits the currently visible App. No arguments required. */
v8::Handle<Value> GraphicServices::QuitTopApplication(const Arguments& args) {
  HandleScope scope;
  GSEventQuitTopApplication();
  return scope.Close(Undefined());
}
