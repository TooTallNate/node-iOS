/* This code is PUBLIC DOMAIN, and is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND. See the accompanying 
 * LICENSE file.
 */

#include <v8.h>
#include <node.h>
#import <AudioToolbox/AudioToolbox.h>

using namespace node;
using namespace v8;

class Binding {
  public:

  //static Persistent<FunctionTemplate> s_ct;
  //static Persistent<FunctionTemplate> ft_vibrate;

  static void Init(v8::Handle<Object> target) {
    HandleScope scope;

    //Local<FunctionTemplate> t = FunctionTemplate::New(New);

    //s_ct = Persistent<FunctionTemplate>::New(t);
    //s_ct->InstanceTemplate()->SetInternalFieldCount(1);
    //s_ct->SetClassName(String::NewSymbol("iTunesApplication"));

    //NODE_SET_PROTOTYPE_METHOD(s_ct, "getVolume", GetVolume);

    //target->Set(String::NewSymbol("iTunesApplication"),
    //            s_ct->GetFunction());
    NODE_SET_METHOD(target, "vibrate", Vibrate);
  }

  /*static v8::Handle<Value> New(const Arguments& args) {
    HandleScope scope;
    node_iTunesApplication* hw = new node_iTunesApplication();
    hw->Wrap(args.This());
    return args.This();
  }*/

  static v8::Handle<Value> Vibrate(const Arguments& args) {
    //HandleScope scope;
    //node_iTunesApplication* hw = ObjectWrap::Unwrap<node_iTunesApplication>(args.This());
    //Local<Integer> result = Integer::New(getVolume((iTunesApplication*)hw->iTunesRef));
    //iTunesApplication* iTunes = hw->iTunesRef;
    //Local<Integer> result = Integer::New([iTunes soundVolume]);
    AudioServicesPlaySystemSound (kSystemSoundID_Vibrate);
    return Undefined();//scope.Close(result);
  }

};

//Persistent<FunctionTemplate> node_iTunesApplication::s_ct;

extern "C" {
  static void init (v8::Handle<Object> target)
  {
    Binding::Init(target);
  }

  NODE_MODULE(binding, init);
}
