//
//  KeyRemap4MacBookAppDelegate.h
//  KeyRemap4MacBook
//
//  Created by Takayama Fumihiko on 09/12/24.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Sparkle/SUUpdater.h"

@interface KeyRemap4MacBookAppDelegate : NSObject <NSApplicationDelegate> {
  NSWindow* window;
  IBOutlet SUUpdater* _suupdater;
}

@property (assign) IBOutlet NSWindow* window;

@end
