#import <v8.h>
#import <node.h>
#import <GraphicsServices/GraphicsServices.h>

/* Private, undocumented APIs */
//void GSEventLockDevice();
//void GSEventQuitTopApplication();

class GraphicServices {
  public:
    static void Init(v8::Handle<v8::Object> target);
    static v8::Handle<v8::Value> LockScreen(const v8::Arguments& args);
    static v8::Handle<v8::Value> QuitTopApplication(const v8::Arguments& args);
};
