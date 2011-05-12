#import <UIKit/UIKit.h>

// Add newer API stuff. This shouldn't be needed, unfortunately
// it seems as though the 'gcc' from Cydia for iOS (my version at least)
// doesn't support either the 'IPHONEOS_DEPLOYMENT_TARGET' env var nor
// the '-miphoneos-version-min' compiler flag... lame...
typedef enum {
    UIDeviceBatteryStateUnknown,
    UIDeviceBatteryStateUnplugged,   // on battery, discharging
    UIDeviceBatteryStateCharging,    // plugged in, less than 100%
    UIDeviceBatteryStateFull,        // plugged in, at 100%
} UIDeviceBatteryState;              // available in iPhone 3.0

@interface UIDevice ()
  - (void) setOrientation:(UIInterfaceOrientation)orientation;
  @property(getter=isBatteryMonitoringEnabled) BOOL batteryMonitoringEnabled;
  @property(readonly) UIDeviceBatteryState          batteryState;
  @property(readonly) float                         batteryLevel;
@end
