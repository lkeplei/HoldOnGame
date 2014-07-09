
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
#define KInitialVelocity        (5)         //障碍物初始速度
#define KScoreOffTime           (2)         //得分动画时间
#define KBallOffset             (5)         //小球矩形偏差

#define KUserDefaultSetOpenVoice             @"default_open_voice"

#define KLeaderBoardId          @"com.teamqi.holdon"

#endif // __HOLDON_CONFIG_H__