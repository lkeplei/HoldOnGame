#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
//    RootViewController    *viewController;
}

@property (strong, nonatomic) RootViewController* viewController;

@end

