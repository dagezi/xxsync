/* XXsync */

#import <Cocoa/Cocoa.h>
#import "xdpy.h"

@interface XXsync : NSObject
{
  XDpy *xdpy;
  NSPasteboard *pasteboard;
}
- (IBAction)sync:(id)sender;
@end
