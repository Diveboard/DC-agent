#import <Foundation/Foundation.h>
#import <AppKit/NSRunningApplication.h>

#include "../DiveAgentApp.h"


void DiveAgentTaskBarIcon::SureProcessToForeground()
{
  NSRunningApplication* thisApp = [NSRunningApplication currentApplication];
  [thisApp activateWithOptions: NSApplicationActivateIgnoringOtherApps];
};

void DiveAgentApp::SureProcessToForeground()
{
  NSRunningApplication* thisApp = [NSRunningApplication currentApplication];
  [thisApp activateWithOptions: NSApplicationActivateIgnoringOtherApps];
};
