#import "XXsync.h"

@implementation XXsync

- (IBAction)sync:(id)sender
{
  char *r = retrieve_selection(xdpy, "PRIMARY");
  NSArray *types = [NSArray arrayWithObjects: NSStringPboardType, nil];
  NSString *nss = [NSString stringWithCString: r 
			    encoding: NSASCIIStringEncoding];
  [pasteboard declareTypes: types owner: self];
  [pasteboard setString: nss forType: NSStringPboardType];
  /* No need to delete nss and types? */
  free(r);
}

- (id)init
{
  xdpy = new_xdpy(":0.0");
  pasteboard = [NSPasteboard generalPasteboard];
  fputs("init\n", stderr);
  /* TODO: error handling */
  return [super init];
}
@end
