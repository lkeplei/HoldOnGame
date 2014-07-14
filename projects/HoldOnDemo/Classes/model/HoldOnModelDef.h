//
//  HoldOnModelDef.h
//  HoldOnDemo
//
//  Created by 刘坤 on 14-7-5.
//
//

#ifndef  _HOLDONMODEL_DEF_H_
#define  _HOLDONMODEL_DEF_H_

typedef enum {
    KEffectTypeAnJian = 0,
    KEffectTypeBackground,
    KEffectTypeCollision,
    KEffectTypeGameOver,
    KEffectTypeNewRecorder,
    KEffectTypeNumberRoll,
    KEffectTypeTime,
    KEffectTypeUpgrade,
} HoldOnEffectType;

typedef enum {
    KBodyTypeRectangleVer = 1001,       //竖长方形
    KBodyTypeRectangleHor,              //横长方形
    KBodyTypeTriangle,                  //三角形
    KBodyTypeSquare,                    //正方形
} HoldOnBodyType;

typedef enum {
    HoldOnNetNotReachable = 0,
    HoldOnNetReachableViaWiFi,
    HoldOnNetReachableViaWWAN,
} HoldOnNetType;

#endif // _HOLDONMODEL_DEF_H_