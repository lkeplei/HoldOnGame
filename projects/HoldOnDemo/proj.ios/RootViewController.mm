#import "RootViewController.h"

#import "AdMoGoInterstitialManager.h"

#include "HoldOnConfig.h"

@implementation RootViewController

@synthesize adView;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
 
*/
// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
//    return UIInterfaceOrientationMaskAllButUpsideDown;
    return UIInterfaceOrientationMaskPortrait;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

#pragma mark - for ad
#pragma mark - full admogo
-(void)initFullMogo{
    NSString* mogoId = KADIphoneId;
    if (IsPad) {
        mogoId = KADIpadId;
    }
    [AdMoGoInterstitialManager setAppKey:mogoId];
    //初始化(必须先设置默认的AppKey才能通过此方法初始化SDK)
    [[AdMoGoInterstitialManager shareInstance] initDefaultInterstitial];
    [AdMoGoInterstitialManager setRootViewController:self];
    
    [AdMoGoInterstitialManager setDefaultDelegate:self];
}

-(void)showFullAd{
    [[AdMoGoInterstitialManager shareInstance] interstitialShow:NO];
}

-(void)cancelFullAd{
    [[AdMoGoInterstitialManager shareInstance] interstitialCancel];
}

/*
 返回广告rootViewController
 */
- (UIViewController *)viewControllerForPresentingInterstitialModalView{
    return self;
}

/*
 全屏广告开始请求
 */
- (void)adsMoGoInterstitialAdDidStart{
    NSLog(@"MOGO Full Screen Start");
}

/*
 全屏广告准备完毕
 */
- (void)adsMoGoInterstitialAdIsReady{
    NSLog(@"MOGO Full Screen IsReady");
}

/*
 全屏广告接收成功
 */
- (void)adsMoGoInterstitialAdReceivedRequest{
    NSLog(@"MOGO Full Screen Received");
}

/*
 全屏广告将要展示
 */
- (void)adsMoGoInterstitialAdWillPresent{
    NSLog(@"MOGO Full Screen Will Present");
}

/*
 全屏广告接收失败
 */
- (void)adsMoGoInterstitialAdFailedWithError:(NSError *) error{
    NSLog(@"MOGO Full Screen Failed");
}

/*
 全屏广告消失
 */
- (void)adsMoGoInterstitialAdDidDismiss{
    NSLog(@"MOGO Full Screen Dismiss");
}

#pragma mark - AdMoGoDelegate delegate
-(void)clearAllAd{
    [self removeAd];
    [[AdMoGoInterstitialManager shareInstance] interstitialCancel];
}

-(void)removeAd{
    if (adView) {
        [adView removeFromSuperview];
        adView = nil;
    }
}

-(void)resetAd{
    [self removeAd];
    
    NSString* mogoId = KADIphoneId;
    if (IsPad) {
        mogoId = KADIpadId;
    }
    
    //    typedef enum {
    //        AdViewTypeUnknown = 0,          //error
    //        AdViewTypeNormalBanner = 1,     //e.g. 320 * 50 ; 320 * 48  iphone banner
    //        AdViewTypeLargeBanner = 2,      //e.g. 728 * 90 ; 768 * 110 ipad only
    //        AdViewTypeMediumBanner = 3,     //e.g. 468 * 60 ; 508 * 80  ipad only
    //        AdViewTypeRectangle = 4,        //e.g. 300 * 250; 320 * 270 ipad only
    //        AdViewTypeSky = 5,              //Don't support
    //        AdViewTypeFullScreen = 6,       //iphone full screen ad
    //        AdViewTypeVideo = 7,            //Don't support
    //        AdViewTypeiPadNormalBanner = 8, //ipad use iphone banner
    //    } AdViewType;
    
    if (IsPad) {
        adView = [[AdMoGoView alloc] initWithAppKey:mogoId adType:AdViewTypeLargeBanner adMoGoViewDelegate:self];
    } else {
        adView = [[AdMoGoView alloc] initWithAppKey:mogoId adType:AdViewTypeNormalBanner adMoGoViewDelegate:self];
        adView.frame = CGRectMake(0.0, self.view.frame.size.height - 50, 320.0, 50.0);
    }
    adView.adWebBrowswerDelegate = self;
    [self.view addSubview:adView];
    
    if ([[UIDevice currentDevice].systemVersion floatValue] >=7.0) {
        self.automaticallyAdjustsScrollViewInsets = NO;
    }
    
    [self.view bringSubviewToFront:adView];
}

/*
 返回广告rootViewController
 */
- (UIViewController *)viewControllerForPresentingModalView{
    return self;
}

/**
 * 广告开始请求回调
 */
- (void)adMoGoDidStartAd:(AdMoGoView *)adMoGoView{
    NSLog(@"广告开始请求回调");
}
/**
 * 广告接收成功回调
 */
- (void)adMoGoDidReceiveAd:(AdMoGoView *)adMoGoView{
    NSLog(@"广告接收成功回调");
    if (IsPad) {
        adView.frame = CGRectOffset(adView.frame,
                                    ([UIScreen mainScreen].bounds.size.width - adView.frame.size.width) / 2,
                                    [UIScreen mainScreen].bounds.size.height - adView.frame.size.height);
    }
}
/**
 * 广告接收失败回调
 */
- (void)adMoGoDidFailToReceiveAd:(AdMoGoView *)adMoGoView didFailWithError:(NSError *)error{
    NSLog(@"广告接收失败回调");
}
/**
 * 点击广告回调
 */
- (void)adMoGoClickAd:(AdMoGoView *)adMoGoView{
    NSLog(@"点击广告回调");
//    if ([_currentShowView viewType] == KENViewTypePaiZhen) {
//        [(KENViewPaiZhen*)_currentShowView dealWithAd];
//    }
}
/**
 *You can get notified when the user delete the ad
 广告关闭回调
 */
- (void)adMoGoDeleteAd:(AdMoGoView *)adMoGoView{
    NSLog(@"广告关闭回调");
}

#pragma mark -
#pragma mark AdMoGoWebBrowserControllerUserDelegate delegate

/*
 浏览器将要展示
 */
- (void)webBrowserWillAppear{
    NSLog(@"浏览器将要展示");
}

/*
 浏览器已经展示
 */
- (void)webBrowserDidAppear{
    NSLog(@"浏览器已经展示");
}

/*
 浏览器将要关闭
 */
- (void)webBrowserWillClosed{
    NSLog(@"浏览器将要关闭");
}

/*
 浏览器已经关闭
 */
- (void)webBrowserDidClosed{
    NSLog(@"浏览器已经关闭");
}
/**
 *直接下载类广告 是否弹出Alert确认
 */
-(BOOL)shouldAlertQAView:(UIAlertView *)alertView{
    return NO;
}

- (void)webBrowserShare:(NSString *)url{
    
}
@end
