//
//  GameKitHelper.h
//  HoldOnDemo
//
//  Created by ken on 14/7/4.
//
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>
@interface GameKitHelper : NSObject <GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate, GKMatchmakerViewControllerDelegate, GKMatchDelegate>{
    BOOL gameCenterAvailable;
    BOOL userAuthenticated;
}

@property (assign, readonly) BOOL gameCenterAvailable;

+ (GameKitHelper *)sharedGameKitHelper;
- (void) authenticateLocalUser;
- (void) showLeaderboard;
- (void) reportScore:(uint32_t)score;    //上传得分
- (void) whetherHighestScores:(uint32_t)score;      //是否为最高得分
- (void) leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController;

@property (assign) BOOL isNewRecord;

@end
