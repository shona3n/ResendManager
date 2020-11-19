#ifndef RESENDMANAGER_H
#define RESENDMANAGER_H

#include <QTimer>
#include <functional>
#include <QMap>
//#include "JsonProtocol.h"

static const int defaultRepeatNum = 3;

static const int startIpcInterval = 30000;
static const int startEboInterval = 30000;
// After get heartbeat, wait for ipcWaitInterval to start detection
static const int ipcWaitInterval = 4000;

static const int sncodeWriteMcuInterval = 3000;
static const int sncodeWriteInterval = 3000;
static const int sncodeWriteChargePileInterval = 3000;
static const int sncodeWriteEboInterval = 3000;
static const int sncodeWriteCollarInterval = 3000;

static const int voltageMcuInterval = 3000;
static const int voltageInterval = 3000;
static const int voltageChargePileInterval = 3000;
static const int voltageCollarInterval = 300;

static const int infraredMcuInterval = 3000;
static const int infraredIpcInterval = 2000;
static const int infraredChargePileInterval = 2000;
static const int infraredInterval = 2000;

static const int bluetoothMcuInterval = 5000;
static const int bluetoothInterval = 5000;
static const int bluetoothCollarInterval = 15000;

static const int flashMcuInterval = 2000;

static const int imuMcuInterval = 5000;
static const int imuInterval = 5000;
static const int imuCollarInterval = 5000;

static const int motorMcuInterval = 7000;
static const int motorInterval = 7000;

static const int sdcardIpcInterval = 2000;

static const int wifiInterval = 5000;

static const int wifiConfigInterval = 30000; // 10000

static const int voiceInterval = 1000;

static const int videoIpcInterval = 2000;
static const int videoInterval = 2000;

static const int keyChargePileInterval = 1000;
static const int keyInterval = 7000;

static const int tofInterval = 2000;

static const int sdcardEboInterval = 3000;

static const int ledInterval = 1000;

static const int chargeEboInterval = 5000;

static const int uidWriteInterval = 2000;

static const int restoreFactoryInterval = 2000;

static const int powerCollarInterval = 5000;

static const int rightAckInterval = 1000;

class ResendManager {
public:
//    ResendManager();
    ResendManager(int vIntervalMax = 3000, int vRepeatMax = 3);
    ~ResendManager();
    void init(int vIntervalMax = 3000, int vRepeatMax = 3);
    void start();
    void stop();

    bool enabled() const; // read only

    int mRepeatMax; // max times to resend
    int mIntervalMax; // mesc, over time
    int mRepeatCount; // recoder repeat count until mRepeatMax
    int mIntervalCount; // recoder interval time until mIntervalMax

private:
    bool mEnabled;
};

using TimeoutSignalFunction = std::function<void(int)>;
using SendSignalFunction = std::function<void()>;

class AutoResendBase : public QObject {
    Q_OBJECT
public:
    explicit AutoResendBase(QObject *parent = nullptr);
    ~AutoResendBase();
    QMap<int, ResendManager *> items() const;
    int baseInterval() const;
    void timeout();
    bool isItemRunning(int vItemId);

public Q_SLOTS:
    void startItem(int vItemId);
    void stopItem(int vItemId);
    void appendItem(int vKey, ResendManager *vResend);

Q_SIGNALS:
    void sgItemTimeout(int vId, int vRepeatCount);
    void sgItemFailed(int vId);

private:
    QTimer *mBaseTimer;
    int mBaseInterval;
    QMap<int, ResendManager*> mItems;
}; // class AutoResendBase

//class AutoResendManager: public QObject {
//    Q_OBJECT
//public:
//    explicit AutoResendManager(QObject *parent = nullptr);
//    ~AutoResendManager();

//    void timeoutFunction();
//    int baseInterval() const;
//    void start(FactoryModuleUnit vUnit);
//    void stop(FactoryModuleUnit vUnit);
//    bool isRunning(FactoryModuleUnit vUnit);

//    void processResend(ResendManager *vReManager, FactoryModuleUnit vUnit);
//    void processUnitTimeout(FactoryModuleUnit vUnit);

//private:
//    QMap<FactoryModuleUnit, TimeoutSignalFunction> mTimeoutSignalMap;
//    void creatTimeoutSignalMap();
//    QMap<FactoryModuleUnit, SendSignalFunction> mFailedSignalMap;
//    void createFailedSignalMap();
//    QMap<FactoryModuleUnit, ResendManager*> mResendManagerMap;
//    void createResendManagerMap();
//    QTimer *mBaseTimer;
//    int mBaseInterval;

//public:
//    /*****************************************
//     * description: MCU resend manager
//    ******************************************/
//    ResendManager mMcuStartReManager;
//    ResendManager mMcuSncodeWriteReManager;
//    ResendManager mMcuVoltageReManager;
//    ResendManager mMcuInfraredReManager;
//    ResendManager mMcuBluetoothReManager;
//    ResendManager mMcuFlashReManager;
//    ResendManager mMcuImuReManager;
//    ResendManager mMcuMotorReManager;
//    ResendManager mMcuEndReManager;

//Q_SIGNALS:
//    void sgMcuStartTimeout(int vRepeatCount);
//    void sgMcuStartFailed();
//    void sgMcuSncodeWriteTimeout(int vRepeatCount);
//    void sgMcuSncodeWriteFailed();
//    void sgMcuVoltageTimeout(int vRepeatCount);
//    void sgMcuVoltageFailed();
//    void sgMcuInfraredTimeout(int vRepeatCount);
//    void sgMcuInfraredFailed();
//    void sgMcuBluetoothTimeout(int vRepeatCount);
//    void sgMcuBluetoothFailed();
//    void sgMcuFlashTimeout(int vRepeatCount);
//    void sgMcuFlashFailed();
//    void sgMcuImuTimeout(int vRepeatCount);
//    void sgMcuImuFailed();
//    void sgMcuMotorTimeout(int vRepeatCount);
//    void sgMcuMotorFailed();
//    void sgMcuEndTimeout(int vRepeatCount);
//    void sgMcuEndFailed();

//    /*****************************************
//     * description: IPC resend manager
//    ******************************************/
//public:
//    ResendManager mIpcStartReManager;
//    ResendManager mIpcSncodeWriteReManager;
//    ResendManager mIpcVoltageReManager;
//    ResendManager mIpcSdcardReManager;
//    ResendManager mIpcInfraredReManager;
//    ResendManager mIpcWifiReManager;
//    ResendManager mIpcVoiceReManager;
//    ResendManager mIpcVideoReManager;
//    ResendManager mIpcEndReManager;
//    ResendManager mIpcAfterHeartbeatManager;
//Q_SIGNALS:
//    void sgIpcStartTimeout(int vRepeatCount);
//    void sgIpcSncodeWriteTimeout(int vRepeatCount);
//    void sgIpcVoltageTimeout(int vRepeatCount);
//    void sgIpcSdcardTimeout(int vRepeatCount);
//    void sgIpcInfraredTimeout(int vRepeatCount);
//    void sgIpcWifiTimeout(int vRepeatCount);
//    void sgIpcVoiceTimeout(int vRepeatCount);
//    void sgIpcVideoTimeout(int vRepeatCount);
//    void sgIpcEndTimeout(int vRepeatCount);

//    void sgIpcStartFailed();
//    void sgIpcSncodeWriteFailed();
//    void sgIpcVoltageFailed();
//    void sgIpcSdcardFailed();
//    void sgIpcInfraredFailed();
//    void sgIpcWifiFailed();
//    void sgIpcVoiceFailed();
//    void sgIpcVideoFailed();
//    void sgIpcEndFailed();

//    void sgIpcAfterHeartbeatTimeout(int vRepeatCount);
//    void sgIpcAfterHeartbeatFailed();

//public:
//    ResendManager mIpcProStartReManager;
//    ResendManager mIpcProSncodeWriteReManager;
//    ResendManager mIpcProVoltageReManager;
//    ResendManager mIpcProWifiReManager;
//    ResendManager mIpcProVoiceReManager;
//    ResendManager mIpcProEndReManager;
//    ResendManager mIpcProAfterHeartbeatManager;

//Q_SIGNALS:
//    void sgIpcProStartTimeout(int vRepeatCount);
//    void sgIpcProSncodeWriteTimeout(int vRepeatCount);
//    void sgIpcProVoltageTimeout(int vRepeatCount);
//    void sgIpcProWifiTimeout(int vRepeatCount);
//    void sgIpcProVoiceTimeout(int vRepeatCount);
//    void sgIpcProEndTimeout(int vRepeatCount);
//    void sgIpcProAfterHeartbeatTimeout(int vRepeatCount);

//    void sgIpcProStartFailed();
//    void sgIpcProSncodeWriteFailed();
//    void sgIpcProVoltageFailed();
//    void sgIpcProWifiFailed();
//    void sgIpcProVoiceFailed();
//    void sgIpcProEndFailed();
//    void sgIpcProAfterHeartbeatFailed();

//    /*****************************************
//     * description: ChargePile resend manager
//    ******************************************/
//public:
//    ResendManager mChargePileStartReManager;
//    ResendManager mChargePileSncodeWriteReManager;
//    ResendManager mChargePileVoltageReManager;
//    ResendManager mChargePileInfraredReManager;
//    ResendManager mChargePileKeyReManager;
//    ResendManager mChargePileEndReManager;
//Q_SIGNALS:
//    void sgChargePileStartTimeout(int vRepeatCount);
//    void sgChargePileSncodeWriteTimeout(int vRepeatCount);
//    void sgChargePileVoltageTimeout(int vRepeatCount);
//    void sgChargePileInfraredTimeout(int vRepeatCount);
//    void sgChargePileKeyTimeout(int vRepeatCount);
//    void sgChargePileEndTimeout(int vRepeatCount);

//    void sgChargePileStartFailed();
//    void sgChargePileSncodeWriteFailed();
//    void sgChargePileVoltageFailed();
//    void sgChargePileInfraredFailed();
//    void sgChargePileKeyFailed();
//    void sgChargePileEndFailed();

//    /*****************************************
//     * description: Ebo resend manager
//    ******************************************/
//public:
//    ResendManager mEboStartReManager;
//    ResendManager mEboSncodeWriteReManager;
//    ResendManager mEboImuReManager;
//    ResendManager mEboInfraredReManager;
//    ResendManager mEboMotorReManager;
//    ResendManager mEboTofReManager;
//    ResendManager mEboSdcardReManager;
//    ResendManager mEboLedReManager;
//    ResendManager mEboLedDownReManager;
//    ResendManager mEboLedRedReManager;
//    ResendManager mEboLedGreenReManager;
//    ResendManager mEboLedBlueReManager;
//    ResendManager mEboWifiReManager;
//    ResendManager mEboBluetoothReManager;
//    ResendManager mEboVoiceReManager;
//    ResendManager mEboVideoReManager;
//    ResendManager mEboKeyReManager;
//    ResendManager mEboChargeReManager;
//    ResendManager mEboUidWriteReManager;
//    ResendManager mEboWifiConfigReManager;
//    ResendManager mEboRestoreFactoryReManager;
//    ResendManager mEboEndReManager;
//    ResendManager mEboAfterHeartbeatManager;

//Q_SIGNALS:
//    void sgEboStartTimeout(int vRepeatCount);
//    void sgEboSncodeWriteTimeout(int vRepeatCount);
//    void sgEboImuTimeout(int vRepeatCount);
//    void sgEboInfraredTimeout(int vRepeatCount);
//    void sgEboMotorTimeout(int vRepeatCount);
//    void sgEboTofTimeout(int vRepeatCount);
//    void sgEboSdcardTimeout(int vRepeatCount);
//    void sgEboLedTimeout(int vRepeatCount);
//    void sgEboLedDownTimeout(int vRepeatCount);
//    void sgEboLedRedTimeout(int vRepeatCount);
//    void sgEboLedGreenTimeout(int vRepeatCount);
//    void sgEboLedBlueTimeout(int vRepeatCount);
//    void sgEboWifiTimeout(int vRepeatCount);
//    void sgEboBluetoothTimeout(int vRepeatCount);
//    void sgEboVoiceTimeout(int vRepeatCount);
//    void sgEboVideoTimeout(int vRepeatCount);
//    void sgEboKeyTimeout(int vRepeatCount);
//    void sgEboChargeTimeout(int vRepeatCount);
//    void sgEboUidWriteTimeout(int vRepeatCount);
//    void sgEboWifiConfigTimeout(int vRepeatCount);
//    void sgEboRestoreFactoryTimeout(int vRepeatCount);
//    void sgEboEndTimeout(int vRepeatCount);

//    void sgEboStartFailed();
//    void sgEboSncodeWriteFailed();
//    void sgEboImuFailed();
//    void sgEboInfraredFailed();
//    void sgEboMotorFailed();
//    void sgEboTofFailed();
//    void sgEboSdcardFailed();
//    void sgEboLedFailed();
//    void sgEboLedDownFailed();
//    void sgEboLedRedFailed();
//    void sgEboLedGreenFailed();
//    void sgEboLedBlueFailed();
//    void sgEboWifiFailed();
//    void sgEboBluetoothFailed();
//    void sgEboVoiceFailed();
//    void sgEboVideoFailed();
//    void sgEboKeyFailed();
//    void sgEboChargeFailed();
//    void sgEboUidWriteFailed();
//    void sgEboWifiConfigFailed();
//    void sgEboRestoreFactoryFailed();
//    void sgEboEndFailed();

//    void sgEboAfterHeartbeatTimeout(int vRepeatCount);
//    void sgEboAfterHeartbeatFailed();

//public:
//    ResendManager mEboProStartReManager;
//    ResendManager mEboProAfterHeartbeatManager;
//    ResendManager mEboProSncodeWriteReManager;
//    ResendManager mEboProImuReManager;
//    ResendManager mEboProInfraredReManager;
//    ResendManager mEboProMotorReManager;
//    ResendManager mEboProTofReManager;
//    ResendManager mEboProLedReManager;
//    ResendManager mEboProLedDownReManager;
//    ResendManager mEboProLedRedReManager;
//    ResendManager mEboProLedGreenReManager;
//    ResendManager mEboProLedBlueReManager;
//    ResendManager mEboProWifiReManager;
//    ResendManager mEboProBluetoothReManager;
//    ResendManager mEboProVoiceReManager;
//    ResendManager mEboProVideoReManager;
//    ResendManager mEboProKeyReManager;
//    ResendManager mEboProUidWriteReManager;
//    ResendManager mEboProWifiConfigReManager;
//    ResendManager mEboProRestoreFactoryReManager;
//    ResendManager mEboProEndReManager;

//Q_SIGNALS:
//    void sgEboProStartTimeout(int vRepeatCount);
//    void sgEboProAfterHeartbeatTimeout(int vRepeatCount);
//    void sgEboProSncodeWriteTimeout(int vRepeatCount);
//    void sgEboProImuTimeout(int vRepeatCount);
//    void sgEboProInfraredTimeout(int vRepeatCount);
//    void sgEboProMotorTimeout(int vRepeatCount);
//    void sgEboProTofTimeout(int vRepeatCount);
//    void sgEboProLedTimeout(int vRepeatCount);
//    void sgEboProLedDownTimeout(int vRepeatCount);
//    void sgEboProLedRedTimeout(int vRepeatCount);
//    void sgEboProLedGreenTimeout(int vRepeatCount);
//    void sgEboProLedBlueTimeout(int vRepeatCount);
//    void sgEboProWifiTimeout(int vRepeatCount);
//    void sgEboProBluetoothTimeout(int vRepeatCount);
//    void sgEboProVoiceTimeout(int vRepeatCount);
//    void sgEboProVideoTimeout(int vRepeatCount);
//    void sgEboProKeyTimeout(int vRepeatCount);
//    void sgEboProUidWriteTimeout(int vRepeatCount);
//    void sgEboProWifiConfigTimeout(int vRepeatCount);
//    void sgEboProResetTimeout(int vRepeatCount);
//    void sgEboProEndTimeout(int vRepeatCount);

//    void sgEboProStartFailed();
//    void sgEboProAfterHeartbeatFailed();
//    void sgEboProSncodeWriteFailed();
//    void sgEboProImuFailed();
//    void sgEboProInfraredFailed();
//    void sgEboProMotorFailed();
//    void sgEboProTofFailed();
//    void sgEboProLedFailed();
//    void sgEboProLedDownFailed();
//    void sgEboProLedRedFailed();
//    void sgEboProLedGreenFailed();
//    void sgEboProLedBlueFailed();
//    void sgEboProWifiFailed();
//    void sgEboProBluetoothFailed();
//    void sgEboProVoiceFailed();
//    void sgEboProVideoFailed();
//    void sgEboProKeyFailed();
//    void sgEboProUidWriteFailed();
//    void sgEboProWifiConfigFailed();
//    void sgEboProResetFailed();
//    void sgEboProEndFailed();

//    /*****************************************
//     * description: Collar resend manager
//    ******************************************/
//public:
//    ResendManager mCollarStartReManager;
//    ResendManager mCollarSncodeWriteReManager;
//    ResendManager mCollarBluetoothReManager;
//    ResendManager mCollarPowerReManager;
//    ResendManager mCollarImuReManager;
//    ResendManager mCollarVoltageManager;
//    ResendManager mCollarKeyManager;
//    ResendManager mCollarLedRgbManager;
//    ResendManager mCollarStandbyCurrentManager;
//    ResendManager mCollarConnectCurrentManager;
//    ResendManager mCollarBroadcastCurrentManager;
//    ResendManager mCollarEndReManager;

//Q_SIGNALS:
//    void sgCollarStartTimeout(int vRepeatCount);
//    void sgCollarSncodeWriteTimeout(int vRepeatCount);
//    void sgCollarBluetoothTimeout(int vRepeatCount);
//    void sgCollarPowerTimeout(int vRepeatCount);
//    void sgCollarImuTimeout(int vRepeatCount);
//    void sgCollarVoltageTimeout(int vRepeatCount);
//    void sgCollarKeyTimeout(int vRepeatCount);
//    void sgCollarLedRgbTimeout(int vRepeatCount);
//    void sgCollarStandbyCurrentTimeout(int vRepeatCount);
//    void sgCollarConnectCurrentTimeout(int vRepeatCount);
//    void sgCollarBroadcastCurrentTimeout(int vRepeatCount);
//    void sgCollarEndTimeout(int vRepeatCount);

//    void sgCollarStartFailed();
//    void sgCollarSncodeWriteFailed();
//    void sgCollarBluetoothFailed();
//    void sgCollarPowerFailed();
//    void sgCollarImuFailed();
//    void sgCollarVoltageFailed();
//    void sgCollarKeyFailed();
//    void sgCollarLedRgbFailed();
//    void sgCollarStandbyCurrentFailed();
//    void sgCollarConnectCurrentFailed();
//    void sgCollarBroadcastCurrentFailed();
//    void sgCollarEndFailed();

//public:


//Q_SIGNALS:

//public:
////    void processEboLedDetectTimer();
//    ResendManager mEboLedDetectionManager;
//Q_SIGNALS:
//    void sgEboLedDetectionTimeout(int vCount);
//    void sgEboLedDetectionFailed();
//};

#endif // RESENDMANAGER_H
