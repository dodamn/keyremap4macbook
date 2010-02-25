#import "AppController.h"
#import "ConfigControl.h"

@implementation AppController

- (void) configItemSelected:(id)sender
{
  NSString* idx = [sender representedObject];
  [ConfigControl select:idx];
}

- (void) menuNeedsUpdate:(NSMenu*)menu
{
  // --------------------
  // clear
  for (;;) {
    NSMenuItem* item = [_statusMenu itemAtIndex:0];
    if (item == nil || [[item title] isEqualToString:@"endoflist"]) break;

    [_statusMenu removeItemAtIndex:0];
  }

  // --------------------
  // append
  NSArray* list = [ConfigControl getConfigList];
  int i = 0;
  for (id name in list) {
    if ([name length] == 0) continue;

    NSString* selected = [name substringToIndex:1];
    NSString* title = [name substringFromIndex:1];

    NSMenuItem* newItem = [[[NSMenuItem alloc] initWithTitle:title action:@selector(configItemSelected:)keyEquivalent:@""] autorelease];
    NSString* idx = [[[NSString alloc] initWithFormat:@"%d", i] autorelease];
    [newItem setRepresentedObject:idx];
    if ([selected isEqualToString:@"+"]) {
      [newItem setState:NSOnState];
    } else {
      [newItem setState:NSOffState];
    }

    [_statusMenu insertItem:newItem atIndex:i];
    ++i;
  }
}

- (void) applicationDidFinishLaunching:(NSNotification*)notification
{
  if (! [ConfigControl isStatusbarEnable]) {
    [NSApp terminate:self];

  } else {
    NSStatusBar* statusBar = [NSStatusBar systemStatusBar];

    _statusItem = [statusBar statusItemWithLength:24];
    [_statusItem retain];

    [_statusItem setTitle:@""];
    [_statusItem setImage:[NSImage imageNamed:@"icon.statusbar.0"]];
    [_statusItem setAlternateImage:[NSImage imageNamed:@"icon.statusbar.1"]];
    [_statusItem setHighlightMode:YES];

    [_statusItem setMenu:_statusMenu];
  }
}

- (IBAction) openPreferencePane:(id)sender
{
  [[NSWorkspace sharedWorkspace] openFile:@"/Library/PreferencePanes/KeyRemap4MacBook.prefPane"];
}

- (IBAction) launchEventViewer:(id)sender
{
  [[NSWorkspace sharedWorkspace] launchApplication:@"/Library/org.pqrs/KeyRemap4MacBook/app/KeyDump.app"];
}

@end
