#import <UIKit/UIKit.h>

#import "AdMoGoDelegateProtocol.h"
#import "AdMoGoView.h"
#import "AdMoGoWebBrowserControllerUserDelegate.h"

#import "AdMoGoInterstitialDelegate.h"

@interface RootViewController : UIViewController <AdMoGoDelegate, AdMoGoWebBrowserControllerUserDelegate, AdMoGoInterstitialDelegate>{
    AdMoGoView* adView;
}
@property (nonatomic, strong) AdMoGoView* adView;

-(void)resetAd;
-(void)removeAd;

-(void)initFullMogo;
-(void)showFullAd;
-(void)cancelFullAd;
-(void)clearAllAd;

@end
