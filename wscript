def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")

def build(bld):
  bld.env["IPHONEOS_DEPLOYMENT_TARGET"] = "3.0"
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.cxxflags = ["-g", "-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE", "-Wall", "-ObjC++", "-F/System/Library/PrivateFrameworks"]
  obj.ldflags = ["-F/System/Library/PrivateFrameworks", "-mmacosx-version-min=10.5"]
  obj.framework = ['Foundation', 'AddressBook', 'AudioToolbox', 'UIKit', 'CoreFoundation', 'CoreTelephony', 'GraphicsServices']
  obj.target = "binding"
  obj.source = ["src/binding.cc", "src/addressBook.cc", "src/graphicServices.cc", "src/notifications.cc", "src/telephony.cc"]
