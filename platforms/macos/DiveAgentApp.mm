#import <Foundation/Foundation.h>
#import <AppKit/NSRunningApplication.h>

#include "../../source/DiveAgentApp.h"
#include "LaunchAtLoginController.h"

void DiveAgentApp::SureProcessToForeground()
{
  NSRunningApplication* thisApp = [NSRunningApplication currentApplication];
  [thisApp activateWithOptions: NSApplicationActivateIgnoringOtherApps];
}

void DiveAgentApp::InitStartOnLogin()
{
LaunchAtLoginController *launchController = [[LaunchAtLoginController alloc] init];
[launchController setLaunchAtLogin:YES];
[launchController release];
}
