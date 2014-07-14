
//
//  HoldOnConfig.h
//  HoldOnDemo
//
//  Created by ken on 14/7/3.
//
//

#ifndef __HOLDON_CONFIG_H__
#define __HOLDON_CONFIG_H__

#define IsPad (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)    //是否为pad判断

#define KLabelFontArial         "Arial"
#define KLabelFontStd           "Std"
#define KLabelFontHelvetica     "Helvetica"


#define PTM_RATIO  (32.0)
#define KInitialVelocity        (6)         //障碍物初始速度
#define KScoreOffTime           (3)         //得分动画时间
#define KBallOffset             (8)         //小球矩形偏差

#define KUserDefaultSetOpenVoice             @"default_open_voice"
#define KUserDefaultSetBestScore             @"default_best_score"

#define KLeaderBoardId          @"com.teamqi.holdon"

#define KADIphoneId             @"98f383ea220943db98dfb1bb2fb35d6c"
#define KADIpadId               @"d5794d2bd6994e63ac0beca13f45833f"

#endif // __HOLDON_CONFIG_H__