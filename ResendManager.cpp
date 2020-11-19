#include "ResendManager.h"
#include <QtDebug>
#include <QMap>

ResendManager::ResendManager(int vIntervalMax, int vRepeatMax) {
    init(vIntervalMax, vRepeatMax);
}

ResendManager::~ResendManager() {

}

void ResendManager::init(int vIntervalMax, int vRepeatMax) {
    mEnabled = false;
    mIntervalMax = vIntervalMax;
    mRepeatMax = vRepeatMax;
    mIntervalCount = 0;
    mRepeatCount = 0;
}

void ResendManager::start() {
    mEnabled = true;
    mIntervalCount = 0;
    mRepeatCount = 0;
}

void ResendManager::stop() {
    mEnabled = false;
    mIntervalCount = 0;
    mRepeatCount = 0;
}

bool ResendManager::enabled() const {
    return mEnabled;
}

AutoResendBase::AutoResendBase(QObject *parent) : QObject(parent) {
    mBaseTimer = new QTimer();
    connect(mBaseTimer, &QTimer::timeout,
            this, &AutoResendBase::timeout, Qt::QueuedConnection);
    mBaseInterval = 100; // 100ms
    mBaseTimer->start(mBaseInterval);
}

AutoResendBase::~AutoResendBase() {
    mBaseTimer->stop();
    mItems.clear();

    if (mBaseTimer) {
        delete mBaseTimer;
        mBaseTimer = nullptr;
    }
}

QMap<int, ResendManager *> AutoResendBase::items() const {
    return mItems;
}

int AutoResendBase::baseInterval() const {
    return mBaseInterval;
}

void AutoResendBase::startItem(int vItemId) {
    if (!mItems[vItemId]) {
        qInfo() << "Pointer is nullptr in start.";
        return;
    }
    mItems[vItemId]->start();
}

void AutoResendBase::stopItem(int vItemId) {
    if (!mItems[vItemId]) {
        qInfo() << __FUNCTION__ << " Pointer is nullptr in stop.";
        return;
    }
    mItems[vItemId]->stop();
}

bool AutoResendBase::isItemRunning(int vItemId) {
    if (!mItems[vItemId]) {
        qInfo() << "Pointer is nullptr in isRunning.";
        return false;
    }
    return mItems[vItemId]->enabled();
}

void AutoResendBase::appendItem(int vKey, ResendManager *vResend) {
    mItems[vKey] = vResend;
}

void AutoResendBase::timeout() {
    QList<int> keys = mItems.keys();
    for (int i = 0; i < keys.size(); ++i) {
        ResendManager *itemResend = mItems[keys[i]];
        if (itemResend && itemResend->enabled()) {
            if (itemResend->mIntervalCount < itemResend->mIntervalMax) {
                itemResend->mIntervalCount += mBaseInterval;
                continue;
            }
            // timeout
            itemResend->mIntervalCount = 0;
            // timeout times is small than repeat number
            if (itemResend->mRepeatCount < itemResend->mRepeatMax) {
                itemResend->mRepeatCount++;
                Q_EMIT sgItemTimeout(keys[i], itemResend->mRepeatCount);
                continue;
            }
            // more than reapeat number, signal fail
            itemResend->mRepeatCount = 0;
            itemResend->stop();
            Q_EMIT sgItemFailed(keys[i]);
        }
    }
}

///*****************************************
// * description:
//******************************************/
//AutoResendManager::AutoResendManager(QObject *parent)
//    : QObject(parent) {
//    mMcuStartReManager.init();
//    mMcuSncodeWriteReManager.init(sncodeWriteMcuInterval, defaultRepeatNum);
//    mMcuVoltageReManager.init(voltageMcuInterval, defaultRepeatNum);
//    mMcuInfraredReManager.init(infraredMcuInterval, defaultRepeatNum);
//    mMcuBluetoothReManager.init(bluetoothMcuInterval, defaultRepeatNum);
//    mMcuFlashReManager.init(flashMcuInterval, defaultRepeatNum);
//    mMcuImuReManager.init(imuMcuInterval, defaultRepeatNum);
//    mMcuMotorReManager.init(motorMcuInterval, defaultRepeatNum);
//    mMcuEndReManager.init();

//    mIpcStartReManager.init(startIpcInterval, 0); // repeat count set to 0, means timeout is failed.
//    mIpcSncodeWriteReManager.init(sncodeWriteInterval, defaultRepeatNum);
//    mIpcVoltageReManager.init(voltageInterval, defaultRepeatNum);
//    mIpcSdcardReManager.init(sdcardIpcInterval, defaultRepeatNum);
//    mIpcInfraredReManager.init(infraredIpcInterval, defaultRepeatNum);
//    mIpcWifiReManager.init(wifiInterval, defaultRepeatNum);
//    mIpcVoiceReManager.init(voiceInterval, defaultRepeatNum);
//    mIpcVideoReManager.init(videoIpcInterval, defaultRepeatNum);
//    mIpcEndReManager.init();
//    mIpcAfterHeartbeatManager.init(ipcWaitInterval, 1);

//    mIpcProStartReManager.init(startIpcInterval, 0);
//    mIpcProSncodeWriteReManager.init(sncodeWriteInterval, defaultRepeatNum);
//    mIpcProVoltageReManager.init(voltageInterval, defaultRepeatNum);
//    mIpcProWifiReManager.init(wifiInterval, defaultRepeatNum);
//    mIpcProVoiceReManager.init(voiceInterval, defaultRepeatNum);
//    mIpcProEndReManager.init();
//    mIpcProAfterHeartbeatManager.init(ipcWaitInterval, 1);

//    mChargePileStartReManager.init();
//    mChargePileSncodeWriteReManager.init(sncodeWriteChargePileInterval, defaultRepeatNum);
//    mChargePileVoltageReManager.init(voltageChargePileInterval, defaultRepeatNum);
//    mChargePileInfraredReManager.init(infraredChargePileInterval, defaultRepeatNum);
//    mChargePileKeyReManager.init(keyChargePileInterval, defaultRepeatNum);
//    mChargePileEndReManager.init();

//    mEboStartReManager.init(startEboInterval, 0);
//    mEboSncodeWriteReManager.init(sncodeWriteEboInterval, defaultRepeatNum);
//    mEboImuReManager.init(imuInterval, defaultRepeatNum);
//    mEboInfraredReManager.init(infraredInterval, defaultRepeatNum);
//    mEboMotorReManager.init(motorInterval, defaultRepeatNum);
//    mEboTofReManager.init(tofInterval, defaultRepeatNum);
//    mEboSdcardReManager.init(sdcardEboInterval, defaultRepeatNum);
//    mEboLedReManager.init(ledInterval, defaultRepeatNum);
//    mEboLedDownReManager.init(ledInterval, defaultRepeatNum);
//    mEboLedRedReManager.init(ledInterval, defaultRepeatNum);
//    mEboLedGreenReManager.init(ledInterval, defaultRepeatNum);
//    mEboLedBlueReManager.init(ledInterval, defaultRepeatNum);
//    mEboWifiReManager.init(wifiInterval, defaultRepeatNum);
//    mEboBluetoothReManager.init(bluetoothInterval, defaultRepeatNum);
//    mEboVoiceReManager.init(voiceInterval, defaultRepeatNum);
//    mEboVideoReManager.init(videoInterval, defaultRepeatNum);
//    mEboKeyReManager.init(keyInterval, defaultRepeatNum);
//    mEboChargeReManager.init(chargeEboInterval, defaultRepeatNum);
//    mEboUidWriteReManager.init(uidWriteInterval, defaultRepeatNum);
//    mEboWifiConfigReManager.init(wifiConfigInterval, defaultRepeatNum);
//    mEboRestoreFactoryReManager.init(restoreFactoryInterval, defaultRepeatNum);
//    mEboEndReManager.init();
//    mEboAfterHeartbeatManager.init(ipcWaitInterval, 1);

//    mEboProStartReManager.init(startEboInterval, 0);
//    mEboProAfterHeartbeatManager.init(ipcWaitInterval, 1);
//    mEboProSncodeWriteReManager.init(sncodeWriteInterval, defaultRepeatNum);
//    mEboProImuReManager.init(imuInterval, defaultRepeatNum);
//    mEboProInfraredReManager.init(infraredInterval, defaultRepeatNum);
//    mEboProMotorReManager.init(motorInterval, defaultRepeatNum);
//    mEboProTofReManager.init(tofInterval, defaultRepeatNum);
//    mEboProLedReManager.init(ledInterval, defaultRepeatNum);
//    mEboProLedDownReManager.init(ledInterval, defaultRepeatNum);
//    mEboProLedRedReManager.init(ledInterval, defaultRepeatNum);
//    mEboProLedGreenReManager.init(ledInterval, defaultRepeatNum);
//    mEboProLedBlueReManager.init(ledInterval, defaultRepeatNum);
//    mEboProWifiReManager.init(wifiInterval, defaultRepeatNum);
//    mEboProBluetoothReManager.init(bluetoothInterval, defaultRepeatNum);
//    mEboProVoiceReManager.init(voiceInterval, defaultRepeatNum);
//    mEboProVideoReManager.init(videoInterval, defaultRepeatNum);
//    mEboProKeyReManager.init(keyInterval, defaultRepeatNum);
//    mEboProUidWriteReManager.init(uidWriteInterval, defaultRepeatNum);
//    mEboProWifiConfigReManager.init(wifiConfigInterval, defaultRepeatNum);
//    mEboProRestoreFactoryReManager.init(restoreFactoryInterval, defaultRepeatNum);
//    mEboProEndReManager.init();

//    mCollarStartReManager.init();
//    mCollarSncodeWriteReManager.init(sncodeWriteCollarInterval, defaultRepeatNum);
//    mCollarBluetoothReManager.init(bluetoothCollarInterval, defaultRepeatNum);
//    mCollarPowerReManager.init(powerCollarInterval, defaultRepeatNum);
//    mCollarImuReManager.init(imuCollarInterval, defaultRepeatNum);
//    mCollarVoltageManager.init(voltageCollarInterval, 30);
//    mCollarKeyManager.init(keyInterval, defaultRepeatNum);
//    mCollarLedRgbManager.init(ledInterval, defaultRepeatNum);
//    mCollarStandbyCurrentManager.init(rightAckInterval, defaultRepeatNum);
//    mCollarConnectCurrentManager.init(rightAckInterval, defaultRepeatNum);
//    mCollarBroadcastCurrentManager.init(rightAckInterval, defaultRepeatNum);
//    mCollarEndReManager.init();

//    mEboLedDetectionManager.init(1000, 25);

//    createResendManagerMap();
//    creatTimeoutSignalMap();
//    createFailedSignalMap();

//    mBaseTimer = new QTimer();
//    connect(mBaseTimer, &QTimer::timeout,
//            this, &AutoResendManager::timeoutFunction, Qt::QueuedConnection);
//    mBaseInterval = 100; // 100ms
//    mBaseTimer->start(mBaseInterval);
//}

//AutoResendManager::~AutoResendManager() {
//    qDebug() << "resend end.";
//    mBaseTimer->stop();
//    mResendManagerMap.clear();
//    mTimeoutSignalMap.clear();
//    mFailedSignalMap.clear();

//    if (mBaseTimer) {
//        delete mBaseTimer;
//        mBaseTimer = nullptr;
//    }
//    qDebug() << "resend end done.";
//}

//int AutoResendManager::baseInterval() const {
//    return mBaseInterval;
//}

//void AutoResendManager::start(FactoryModuleUnit vUnit) {
//    if (!mResendManagerMap[vUnit]) {
//        qInfo() << "Pointer is nullptr in start.";
//        return;
//    }
//    mResendManagerMap[vUnit]->start();
//}

//void AutoResendManager::stop(FactoryModuleUnit vUnit) {
//    if (!mResendManagerMap[vUnit]) {
//        qInfo() << "Pointer is nullptr in stop.";
//        return;
//    }
//    mResendManagerMap[vUnit]->stop();
//}

//bool AutoResendManager::isRunning(FactoryModuleUnit vUnit) {
//    if (!mResendManagerMap[vUnit]) {
//        qInfo() << "Pointer is nullptr in isRunning.";
//        return false;
//    }
//    return mResendManagerMap[vUnit]->enabled();
//}

//void AutoResendManager::timeoutFunction() {
//    processResend(&mMcuStartReManager, FACTORY_MCU_START);
//    processResend(&mMcuSncodeWriteReManager, FACTORY_MCU_SNCODE_WRITE);
//    processResend(&mMcuVoltageReManager, FACTORY_MCU_VOLTAGE);
//    processResend(&mMcuInfraredReManager, FACTORY_MCU_INFRARED);
//    processResend(&mMcuBluetoothReManager, FACTORY_MCU_BLUETOOTH);
//    processResend(&mMcuFlashReManager, FACTORY_MCU_FLASH);
//    processResend(&mMcuImuReManager, FACTORY_MCU_IMU);
//    processResend(&mMcuMotorReManager, FACTORY_MCU_MOTOR);
//    processResend(&mMcuEndReManager, FACTORY_MCU_END);

//    processResend(&mIpcStartReManager, FACTORY_IPC_START);
//    processResend(&mIpcSncodeWriteReManager, FACTORY_IPC_SNCODE_WRITE);
//    processResend(&mIpcVoltageReManager, FACTORY_IPC_VOLTAGE);
//    processResend(&mIpcSdcardReManager, FACTORY_IPC_SD_CARD);
//    processResend(&mIpcInfraredReManager, FACTORY_IPC_INFRARED);
//    processResend(&mIpcWifiReManager, FACTORY_IPC_WIFI);
//    processResend(&mIpcVoiceReManager, FACTORY_IPC_VOICE);
//    processResend(&mIpcVideoReManager, FACTORY_IPC_VIDEO);
//    processResend(&mIpcEndReManager, FACTORY_IPC_END);
//    processResend(&mIpcAfterHeartbeatManager, HEARTBEAT_AFTER_IPC);

//    processResend(&mIpcProStartReManager, FACTORY_IPC_PRO_START);
//    processResend(&mIpcProSncodeWriteReManager, FACTORY_IPC_PRO_SNCODE_WRITE);
//    processResend(&mIpcProVoltageReManager, FACTORY_IPC_PRO_VOLTAGE);
//    processResend(&mIpcProWifiReManager, FACTORY_IPC_PRO_WIFI);
//    processResend(&mIpcProVoiceReManager, FACTORY_IPC_PRO_VOICE);
//    processResend(&mIpcProEndReManager, FACTORY_IPC_PRO_END);
//    processResend(&mIpcProAfterHeartbeatManager, HEARTBEAT_AFTER_IPC_PRO);

//    processResend(&mChargePileStartReManager, FACTORY_CHARGE_PILE_START);
//    processResend(&mChargePileSncodeWriteReManager, FACTORY_CHARGE_PILE_SNCODE_WRITE);
//    processResend(&mChargePileVoltageReManager, FACTORY_CHARGE_PILE_VOLTAGE);
//    processResend(&mChargePileInfraredReManager, FACTORY_CHARGE_PILE_INFRARED);
//    processResend(&mChargePileKeyReManager, FACTORY_CHARGE_PILE_KEY);
//    processResend(&mChargePileEndReManager, FACTORY_CHARGE_PILE_END);

//    processResend(&mEboStartReManager, FACTORY_EBO_START);
//    processResend(&mEboSncodeWriteReManager, FACTORY_EBO_SNCODE_WRITE);
//    processResend(&mEboImuReManager, FACTORY_EBO_IMU);
//    processResend(&mEboInfraredReManager, FACTORY_EBO_INFRARED);
//    processResend(&mEboMotorReManager, FACTORY_EBO_MOTOR);
//    processResend(&mEboTofReManager, FACTORY_EBO_TOF);
//    processResend(&mEboSdcardReManager, FACTORY_EBO_SDCARD);
//    processResend(&mEboLedReManager, FACTORY_EBO_LED);
//    processResend(&mEboLedDownReManager, FACTORY_EBO_LED_DOWN);
//    processResend(&mEboLedRedReManager, FACTORY_EBO_LED_RED);
//    processResend(&mEboLedGreenReManager, FACTORY_EBO_LED_GREEN);
//    processResend(&mEboLedBlueReManager, FACTORY_EBO_LED_BLUE);
//    processResend(&mEboWifiReManager, FACTORY_EBO_WIFI);
//    processResend(&mEboBluetoothReManager, FACTORY_EBO_BLUETOOTH);
//    processResend(&mEboVoiceReManager, FACTORY_EBO_VOICE);
//    processResend(&mEboVideoReManager, FACTORY_EBO_VIDEO);
//    processResend(&mEboKeyReManager, FACTORY_EBO_KEY);
//    processResend(&mEboChargeReManager, FACTORY_EBO_CHARGE);
//    processResend(&mEboUidWriteReManager, FACTORY_EBO_UID_WRITE);
//    processResend(&mEboWifiConfigReManager, FACTORY_EBO_WIFI_CONFIG);
//    processResend(&mEboRestoreFactoryReManager, FACTORY_EBO_RESTORE_FACTORY);
//    processResend(&mEboEndReManager, FACTORY_EBO_END);
//    processResend(&mEboAfterHeartbeatManager, HEARTBEAT_AFTER_EBO);

//    processResend(&mEboProStartReManager, FACTORY_EBO_PRO_START);
//    processResend(&mEboProAfterHeartbeatManager, HEARTBEAT_AFTER_EBO_PRO);
//    processResend(&mEboProSncodeWriteReManager, FACTORY_EBO_PRO_SNCODE_WRITE);
//    processResend(&mEboProImuReManager, FACTORY_EBO_PRO_IMU);
//    processResend(&mEboProInfraredReManager, FACTORY_EBO_PRO_INFRARED);
//    processResend(&mEboProMotorReManager, FACTORY_EBO_PRO_MOTOR);
//    processResend(&mEboProTofReManager, FACTORY_EBO_PRO_TOF);
//    processResend(&mEboProLedReManager, FACTORY_EBO_PRO_LED);
//    processResend(&mEboProLedDownReManager, FACTORY_EBO_PRO_LED_DOWN);
//    processResend(&mEboProLedRedReManager, FACTORY_EBO_PRO_LED_RED);
//    processResend(&mEboProLedGreenReManager, FACTORY_EBO_PRO_LED_GREEN);
//    processResend(&mEboProLedBlueReManager, FACTORY_EBO_PRO_LED_BLUE);
//    processResend(&mEboProWifiReManager, FACTORY_EBO_PRO_WIFI);
//    processResend(&mEboProBluetoothReManager, FACTORY_EBO_PRO_BLUETOOTH);
//    processResend(&mEboProVoiceReManager, FACTORY_EBO_PRO_VOICE);
//    processResend(&mEboProVideoReManager, FACTORY_EBO_PRO_VIDEO);
//    processResend(&mEboProKeyReManager, FACTORY_EBO_PRO_KEY);
//    processResend(&mEboProUidWriteReManager, FACTORY_EBO_PRO_UID_WRITE);
//    processResend(&mEboProWifiConfigReManager, FACTORY_EBO_PRO_WIFI_CONFIG);
//    processResend(&mEboProRestoreFactoryReManager, FACTORY_EBO_PRO_RESTORE_FACTORY);
//    processResend(&mEboProEndReManager, FACTORY_EBO_PRO_END);

//    processResend(&mCollarStartReManager, FACTORY_COLLAR_START);
//    processResend(&mCollarSncodeWriteReManager, FACTORY_COLLAR_SNCODE_WRITE);
//    processResend(&mCollarBluetoothReManager, FACTORY_COLLAR_BLUETOOTH);
//    processResend(&mCollarPowerReManager, FACTORY_COLLAR_POWER);
//    processResend(&mCollarImuReManager, FACTORY_COLLAR_IMU);
//    processResend(&mCollarVoltageManager, FACTORY_COLLAR_VOLTAGE);
//    processResend(&mCollarKeyManager, FACTORY_COLLAR_KEY);
//    processResend(&mCollarLedRgbManager, FACTORY_COLLAR_LED_RGB);
//    processResend(&mCollarStandbyCurrentManager, FACTORY_COLLAR_STANDBY_CURRENT);
//    processResend(&mCollarConnectCurrentManager, FACTORY_COLLAR_CONNECT_CURRENT);
//    processResend(&mCollarBroadcastCurrentManager, FACTORY_COLLAR_BROADCAST_CURRENT);
//    processResend(&mCollarEndReManager, FACTORY_COLLAR_END);

//    processResend(&mEboLedDetectionManager, DETEC_EBO_LED);
//}

//void AutoResendManager::processResend(ResendManager *vReManager, FactoryModuleUnit vUnit) {
//    if (!vReManager) {
//        return;
//    }
//    if (!vReManager->enabled()) {
//        return;
//    }

//    // count for timeout
//    if (vReManager->mIntervalCount < vReManager->mIntervalMax) {
//        vReManager->mIntervalCount += mBaseInterval;
//        return;
//    }
//    // timeout
//    vReManager->mIntervalCount = 0;

//    // timeout and small than repeat number
//    if (vReManager->mRepeatCount < vReManager->mRepeatMax) {
//        vReManager->mRepeatCount = vReManager->mRepeatCount + 1;
//        TimeoutSignalFunction timeoutHandler = mTimeoutSignalMap[vUnit];
//        timeoutHandler(vReManager->mRepeatCount);
//        return;
//    }
//    // more than reapeat number, set to failed
//    vReManager->mRepeatCount = 0;
//    vReManager->stop();
//    SendSignalFunction failedHandle = mFailedSignalMap[vUnit];
//    failedHandle();
//}

//void AutoResendManager::createResendManagerMap() {
//    mResendManagerMap.clear();
//    mResendManagerMap = {
//        {FACTORY_MCU_START, &mMcuStartReManager},
//        {FACTORY_MCU_SNCODE_WRITE, &mMcuSncodeWriteReManager},
//        {FACTORY_MCU_VOLTAGE, &mMcuVoltageReManager},
//        {FACTORY_MCU_INFRARED, &mMcuInfraredReManager},
//        {FACTORY_MCU_BLUETOOTH, &mMcuBluetoothReManager},
//        {FACTORY_MCU_FLASH, &mMcuFlashReManager},
//        {FACTORY_MCU_IMU, &mMcuImuReManager},
//        {FACTORY_MCU_MOTOR, &mMcuMotorReManager},
//        {FACTORY_MCU_END, &mMcuEndReManager},

//        {FACTORY_IPC_START, &mIpcStartReManager},
//        {FACTORY_IPC_SNCODE_WRITE, &mIpcSncodeWriteReManager},
//        {FACTORY_IPC_VOLTAGE, &mIpcVoltageReManager},
//        {FACTORY_IPC_SD_CARD, &mIpcSdcardReManager},
//        {FACTORY_IPC_INFRARED, &mIpcInfraredReManager},
//        {FACTORY_IPC_WIFI, &mIpcWifiReManager},
//        {FACTORY_IPC_VOICE, &mIpcVoiceReManager},
//        {FACTORY_IPC_VIDEO, &mIpcVideoReManager},
//        {FACTORY_IPC_END, &mIpcEndReManager},
//        {HEARTBEAT_AFTER_IPC, &mIpcAfterHeartbeatManager},

//        {FACTORY_IPC_PRO_START, &mIpcProStartReManager},
//        {FACTORY_IPC_PRO_SNCODE_WRITE, &mIpcProSncodeWriteReManager},
//        {FACTORY_IPC_PRO_VOLTAGE, &mIpcProVoltageReManager},
//        {FACTORY_IPC_PRO_WIFI, &mIpcProWifiReManager},
//        {FACTORY_IPC_PRO_VOICE, &mIpcProVoiceReManager},
//        {FACTORY_IPC_PRO_END, &mIpcProEndReManager},
//        {HEARTBEAT_AFTER_IPC_PRO, &mIpcProAfterHeartbeatManager},

//        {FACTORY_CHARGE_PILE_START, &mChargePileStartReManager},
//        {FACTORY_CHARGE_PILE_SNCODE_WRITE, &mChargePileSncodeWriteReManager},
//        {FACTORY_CHARGE_PILE_VOLTAGE, &mChargePileVoltageReManager},
//        {FACTORY_CHARGE_PILE_INFRARED, &mChargePileInfraredReManager},
//        {FACTORY_CHARGE_PILE_KEY, &mChargePileKeyReManager},
//        {FACTORY_CHARGE_PILE_END, &mChargePileEndReManager},

//        {FACTORY_EBO_START, &mEboStartReManager},
//        {FACTORY_EBO_SNCODE_WRITE, &mEboSncodeWriteReManager},
//        {FACTORY_EBO_IMU, &mEboImuReManager},
//        {FACTORY_EBO_INFRARED, &mEboInfraredReManager},
//        {FACTORY_EBO_MOTOR, &mEboMotorReManager},
//        {FACTORY_EBO_TOF, &mEboTofReManager},
//        {FACTORY_EBO_SDCARD, &mEboSdcardReManager},
//        {FACTORY_EBO_LED, &mEboLedReManager},
//        {FACTORY_EBO_LED_DOWN, &mEboLedDownReManager},
//        {FACTORY_EBO_LED_RED, &mEboLedRedReManager},
//        {FACTORY_EBO_LED_GREEN, &mEboLedGreenReManager},
//        {FACTORY_EBO_LED_BLUE, &mEboLedBlueReManager},
//        {FACTORY_EBO_WIFI, &mEboWifiReManager},
//        {FACTORY_EBO_BLUETOOTH, &mEboBluetoothReManager},
//        {FACTORY_EBO_VOICE, &mEboVoiceReManager},
//        {FACTORY_EBO_VIDEO, &mEboVideoReManager},
//        {FACTORY_EBO_KEY, &mEboKeyReManager},
//        {FACTORY_EBO_CHARGE, &mEboChargeReManager},
//        {FACTORY_EBO_UID_WRITE, &mEboUidWriteReManager},
//        {FACTORY_EBO_WIFI_CONFIG, &mEboWifiConfigReManager},
//        {FACTORY_EBO_RESTORE_FACTORY, &mEboRestoreFactoryReManager},
//        {FACTORY_EBO_END, &mEboEndReManager},
//        {HEARTBEAT_AFTER_EBO, &mEboAfterHeartbeatManager},

//        {FACTORY_COLLAR_START, &mCollarStartReManager},
//        {FACTORY_COLLAR_SNCODE_WRITE, &mCollarSncodeWriteReManager},
//        {FACTORY_COLLAR_BLUETOOTH, &mCollarBluetoothReManager},
//        {FACTORY_COLLAR_POWER, &mCollarPowerReManager},
//        {FACTORY_COLLAR_IMU, &mCollarImuReManager},
//        {FACTORY_COLLAR_VOLTAGE, &mCollarVoltageManager},
//        {FACTORY_COLLAR_KEY, &mCollarKeyManager},
//        {FACTORY_COLLAR_LED_RGB, &mCollarLedRgbManager},
//        {FACTORY_COLLAR_STANDBY_CURRENT, &mCollarStandbyCurrentManager},
//        {FACTORY_COLLAR_CONNECT_CURRENT, &mCollarConnectCurrentManager},
//        {FACTORY_COLLAR_BROADCAST_CURRENT, &mCollarBroadcastCurrentManager},
//        {FACTORY_COLLAR_END, &mCollarEndReManager},

//        {FACTORY_EBO_PRO_START, &mEboProStartReManager},
//        {HEARTBEAT_AFTER_EBO_PRO, &mEboProAfterHeartbeatManager},
//        {FACTORY_EBO_PRO_SNCODE_WRITE, &mEboProSncodeWriteReManager},
//        {FACTORY_EBO_PRO_IMU, &mEboProImuReManager},
//        {FACTORY_EBO_PRO_INFRARED, &mEboProInfraredReManager},
//        {FACTORY_EBO_PRO_MOTOR, &mEboProMotorReManager},
//        {FACTORY_EBO_PRO_TOF, &mEboProTofReManager},
//        {FACTORY_EBO_PRO_LED, &mEboProLedReManager},
//        {FACTORY_EBO_PRO_LED_DOWN, &mEboProLedDownReManager},
//        {FACTORY_EBO_PRO_LED_RED, &mEboProLedRedReManager},
//        {FACTORY_EBO_PRO_LED_GREEN, &mEboProLedGreenReManager},
//        {FACTORY_EBO_PRO_LED_BLUE, &mEboProLedBlueReManager},
//        {FACTORY_EBO_PRO_WIFI, &mEboProWifiReManager},
//        {FACTORY_EBO_PRO_BLUETOOTH, &mEboProBluetoothReManager},
//        {FACTORY_EBO_PRO_VOICE, &mEboProVoiceReManager},
//        {FACTORY_EBO_PRO_VIDEO, &mEboProVideoReManager},
//        {FACTORY_EBO_PRO_KEY, &mEboProKeyReManager},
//        {FACTORY_EBO_PRO_UID_WRITE, &mEboProUidWriteReManager},
//        {FACTORY_EBO_PRO_WIFI_CONFIG, &mEboProWifiConfigReManager},
//        {FACTORY_EBO_PRO_RESTORE_FACTORY, &mEboProRestoreFactoryReManager},
//        {FACTORY_EBO_PRO_END, &mEboProEndReManager},

//        {DETEC_EBO_LED, &mEboLedDetectionManager},
//    };
//}

//void AutoResendManager::creatTimeoutSignalMap() {
//    mTimeoutSignalMap = {
//        {FACTORY_MCU_START, [this](int a){Q_EMIT this->sgMcuStartTimeout(a);}},
//        {FACTORY_MCU_SNCODE_WRITE, [this](int a){Q_EMIT this->sgMcuSncodeWriteTimeout(a);}},
//        {FACTORY_MCU_VOLTAGE, [this](int a){Q_EMIT this->sgMcuVoltageTimeout(a);}},
//        {FACTORY_MCU_INFRARED, [this](int a){Q_EMIT this->sgMcuInfraredTimeout(a);}},
//        {FACTORY_MCU_BLUETOOTH, [this](int a){Q_EMIT this->sgMcuBluetoothTimeout(a);}},
//        {FACTORY_MCU_FLASH, [this](int a){Q_EMIT this->sgMcuFlashTimeout(a);}},
//        {FACTORY_MCU_IMU, [this](int a){Q_EMIT this->sgMcuImuTimeout(a);}},
//        {FACTORY_MCU_MOTOR, [this](int a){Q_EMIT this->sgMcuMotorTimeout(a);}},
//        {FACTORY_MCU_END, [this](int a){Q_EMIT this->sgMcuEndTimeout(a);}},

//        {FACTORY_IPC_START, [this](int a){Q_EMIT this->sgIpcStartTimeout(a);}},
//        {FACTORY_IPC_SNCODE_WRITE, [this](int a){Q_EMIT this->sgIpcSncodeWriteTimeout(a);}},
//        {FACTORY_IPC_VOLTAGE, [this](int a){Q_EMIT this->sgIpcVoltageTimeout(a);}},
//        {FACTORY_IPC_SD_CARD, [this](int a){Q_EMIT this->sgIpcSdcardTimeout(a);}},
//        {FACTORY_IPC_INFRARED, [this](int a){Q_EMIT this->sgIpcInfraredTimeout(a);}},
//        {FACTORY_IPC_WIFI, [this](int a){Q_EMIT this->sgIpcWifiTimeout(a);}},
//        {FACTORY_IPC_VOICE, [this](int a){Q_EMIT this->sgIpcVoiceTimeout(a);}},
//        {FACTORY_IPC_VIDEO, [this](int a){Q_EMIT this->sgIpcVideoTimeout(a);}},
//        {FACTORY_IPC_END, [this](int a){Q_EMIT this->sgIpcEndTimeout(a);}},
//        {HEARTBEAT_AFTER_IPC, [this](int a){Q_EMIT this->sgIpcAfterHeartbeatTimeout(a);}},

//        {FACTORY_IPC_PRO_START, [this](int a){Q_EMIT this->sgIpcProStartTimeout(a);}},
//        {FACTORY_IPC_PRO_SNCODE_WRITE, [this](int a){Q_EMIT this->sgIpcProSncodeWriteTimeout(a);}},
//        {FACTORY_IPC_PRO_VOLTAGE, [this](int a){Q_EMIT this->sgIpcProVoltageTimeout(a);}},
//        {FACTORY_IPC_PRO_WIFI, [this](int a){Q_EMIT this->sgIpcProWifiTimeout(a);}},
//        {FACTORY_IPC_PRO_VOICE, [this](int a){Q_EMIT this->sgIpcProVoiceTimeout(a);}},
//        {FACTORY_IPC_PRO_END, [this](int a){Q_EMIT this->sgIpcProEndTimeout(a);}},
//        {HEARTBEAT_AFTER_IPC_PRO, [this](int a){Q_EMIT this->sgIpcProAfterHeartbeatTimeout(a);}},

//        {FACTORY_CHARGE_PILE_START, [this](int a){Q_EMIT this->sgChargePileStartTimeout(a);}},
//        {FACTORY_CHARGE_PILE_SNCODE_WRITE, [this](int a){Q_EMIT this->sgChargePileSncodeWriteTimeout(a);}},
//        {FACTORY_CHARGE_PILE_VOLTAGE, [this](int a){Q_EMIT this->sgChargePileVoltageTimeout(a);}},
//        {FACTORY_CHARGE_PILE_INFRARED, [this](int a){Q_EMIT this->sgChargePileInfraredTimeout(a);}},
//        {FACTORY_CHARGE_PILE_KEY, [this](int a){Q_EMIT this->sgChargePileKeyTimeout(a);}},
//        {FACTORY_CHARGE_PILE_END, [this](int a){Q_EMIT this->sgChargePileEndTimeout(a);}},

//        {FACTORY_EBO_START, [this](int a){Q_EMIT this->sgEboStartTimeout(a);}},
//        {FACTORY_EBO_SNCODE_WRITE, [this](int a){Q_EMIT this->sgEboSncodeWriteTimeout(a);}},
//        {FACTORY_EBO_IMU, [this](int a){Q_EMIT this->sgEboImuTimeout(a);}},
//        {FACTORY_EBO_INFRARED, [this](int a){Q_EMIT this->sgEboInfraredTimeout(a);}},
//        {FACTORY_EBO_MOTOR, [this](int a){Q_EMIT this->sgEboMotorTimeout(a);}},
//        {FACTORY_EBO_TOF, [this](int a){Q_EMIT this->sgEboTofTimeout(a);}},
//        {FACTORY_EBO_SDCARD, [this](int a){Q_EMIT this->sgEboSdcardTimeout(a);}},
//        {FACTORY_EBO_LED, [this](int a){Q_EMIT this->sgEboLedTimeout(a);}},
//        {FACTORY_EBO_LED_DOWN, [this](int a){Q_EMIT this->sgEboLedDownTimeout(a);}},
//        {FACTORY_EBO_LED_RED, [this](int a){Q_EMIT this->sgEboLedRedTimeout(a);}},
//        {FACTORY_EBO_LED_GREEN, [this](int a){Q_EMIT this->sgEboLedGreenTimeout(a);}},
//        {FACTORY_EBO_LED_BLUE, [this](int a){Q_EMIT this->sgEboLedBlueTimeout(a);}},
//        {FACTORY_EBO_WIFI, [this](int a){Q_EMIT this->sgEboWifiTimeout(a);}},
//        {FACTORY_EBO_BLUETOOTH, [this](int a){Q_EMIT this->sgEboBluetoothTimeout(a);}},
//        {FACTORY_EBO_VOICE, [this](int a){Q_EMIT this->sgEboVoiceTimeout(a);}},
//        {FACTORY_EBO_VIDEO, [this](int a){Q_EMIT this->sgEboVideoTimeout(a);}},
//        {FACTORY_EBO_KEY, [this](int a){Q_EMIT this->sgEboKeyTimeout(a);}},
//        {FACTORY_EBO_CHARGE, [this](int a){Q_EMIT this->sgEboChargeTimeout(a);}},
//        {FACTORY_EBO_UID_WRITE, [this](int a){Q_EMIT this->sgEboUidWriteTimeout(a);}},
//        {FACTORY_EBO_WIFI_CONFIG, [this](int a){Q_EMIT this->sgEboWifiConfigTimeout(a);}},
//        {FACTORY_EBO_RESTORE_FACTORY, [this](int a){Q_EMIT this->sgEboRestoreFactoryTimeout(a);}},
//        {FACTORY_EBO_END, [this](int a){Q_EMIT this->sgEboEndTimeout(a);}},
//        {HEARTBEAT_AFTER_EBO, [this](int a){Q_EMIT this->sgEboAfterHeartbeatTimeout(a);}},

//        {FACTORY_EBO_PRO_START, [this](int a){Q_EMIT this->sgEboProStartTimeout(a);}},
//        {HEARTBEAT_AFTER_EBO_PRO, [this](int a){Q_EMIT this->sgEboProAfterHeartbeatTimeout(a);}},
//        {FACTORY_EBO_PRO_SNCODE_WRITE, [this](int a){Q_EMIT this->sgEboProSncodeWriteTimeout(a);}},
//        {FACTORY_EBO_PRO_IMU, [this](int a){Q_EMIT this->sgEboProImuTimeout(a);}},
//        {FACTORY_EBO_PRO_INFRARED, [this](int a){Q_EMIT this->sgEboProInfraredTimeout(a);}},
//        {FACTORY_EBO_PRO_MOTOR, [this](int a){Q_EMIT this->sgEboProMotorTimeout(a);}},
//        {FACTORY_EBO_PRO_TOF, [this](int a){Q_EMIT this->sgEboProTofTimeout(a);}},
//        {FACTORY_EBO_PRO_LED, [this](int a){Q_EMIT this->sgEboProLedTimeout(a);}},
//        {FACTORY_EBO_PRO_LED_DOWN, [this](int a){Q_EMIT this->sgEboProLedDownTimeout(a);}},
//        {FACTORY_EBO_PRO_LED_RED, [this](int a){Q_EMIT this->sgEboProLedRedTimeout(a);}},
//        {FACTORY_EBO_PRO_LED_GREEN, [this](int a){Q_EMIT this->sgEboProLedGreenTimeout(a);}},
//        {FACTORY_EBO_PRO_LED_BLUE, [this](int a){Q_EMIT this->sgEboProLedBlueTimeout(a);}},
//        {FACTORY_EBO_PRO_WIFI, [this](int a){Q_EMIT this->sgEboProWifiTimeout(a);}},
//        {FACTORY_EBO_PRO_BLUETOOTH, [this](int a){Q_EMIT this->sgEboProBluetoothTimeout(a);}},
//        {FACTORY_EBO_PRO_VOICE, [this](int a){Q_EMIT this->sgEboProVoiceTimeout(a);}},
//        {FACTORY_EBO_PRO_VIDEO, [this](int a){Q_EMIT this->sgEboProVideoTimeout(a);}},
//        {FACTORY_EBO_PRO_KEY, [this](int a){Q_EMIT this->sgEboProKeyTimeout(a);}},
//        {FACTORY_EBO_PRO_UID_WRITE, [this](int a){Q_EMIT this->sgEboProUidWriteTimeout(a);}},
//        {FACTORY_EBO_PRO_WIFI_CONFIG, [this](int a){Q_EMIT this->sgEboProWifiConfigTimeout(a);}},
//        {FACTORY_EBO_PRO_RESTORE_FACTORY, [this](int a){Q_EMIT this->sgEboProResetTimeout(a);}},
//        {FACTORY_EBO_PRO_END, [this](int a){Q_EMIT this->sgEboProEndTimeout(a);}},

//        {FACTORY_COLLAR_START, [this](int a){Q_EMIT this->sgCollarStartTimeout(a);}},
//        {FACTORY_COLLAR_SNCODE_WRITE, [this](int a){Q_EMIT this->sgCollarSncodeWriteTimeout(a);}},
//        {FACTORY_COLLAR_BLUETOOTH, [this](int a){Q_EMIT this->sgCollarBluetoothTimeout(a);}},
//        {FACTORY_COLLAR_POWER, [this](int a){Q_EMIT this->sgCollarPowerTimeout(a);}},
//        {FACTORY_COLLAR_IMU, [this](int a){Q_EMIT this->sgCollarImuTimeout(a);}},
//        {FACTORY_COLLAR_VOLTAGE, [this](int a){Q_EMIT this->sgCollarVoltageTimeout(a);}},
//        {FACTORY_COLLAR_KEY, [this](int a){Q_EMIT this->sgCollarKeyTimeout(a);}},
//        {FACTORY_COLLAR_LED_RGB, [this](int a){Q_EMIT this->sgCollarLedRgbTimeout(a);}},
//        {FACTORY_COLLAR_STANDBY_CURRENT, [this](int a){Q_EMIT this->sgCollarStandbyCurrentTimeout(a);}},
//        {FACTORY_COLLAR_CONNECT_CURRENT, [this](int a){Q_EMIT this->sgCollarConnectCurrentTimeout(a);}},
//        {FACTORY_COLLAR_BROADCAST_CURRENT, [this](int a){Q_EMIT this->sgCollarBroadcastCurrentTimeout(a);}},
//        {FACTORY_COLLAR_END, [this](int a){Q_EMIT this->sgCollarEndTimeout(a);}},

//        {DETEC_EBO_LED, [this](int a){Q_EMIT this->sgEboLedDetectionTimeout(a);}},
//    };
//}

//void AutoResendManager::createFailedSignalMap() {
//    mFailedSignalMap = {
//        {FACTORY_MCU_START, [this](){Q_EMIT this->sgMcuStartFailed();}},
//        {FACTORY_MCU_SNCODE_WRITE, [this](){Q_EMIT this->sgMcuSncodeWriteFailed();}},
//        {FACTORY_MCU_VOLTAGE, [this](){Q_EMIT this->sgMcuVoltageFailed();}},
//        {FACTORY_MCU_INFRARED, [this](){Q_EMIT this->sgMcuInfraredFailed();}},
//        {FACTORY_MCU_BLUETOOTH, [this](){Q_EMIT this->sgMcuBluetoothFailed();}},
//        {FACTORY_MCU_FLASH, [this](){Q_EMIT this->sgMcuFlashFailed();}},
//        {FACTORY_MCU_IMU, [this](){Q_EMIT this->sgMcuImuFailed();}},
//        {FACTORY_MCU_MOTOR, [this](){Q_EMIT this->sgMcuMotorFailed();}},
//        {FACTORY_MCU_END, [this](){Q_EMIT this->sgMcuEndFailed();}},

//        {FACTORY_IPC_START, [this](){Q_EMIT this->sgIpcStartFailed();}},
//        {FACTORY_IPC_SNCODE_WRITE, [this](){Q_EMIT this->sgIpcSncodeWriteFailed();}},
//        {FACTORY_IPC_VOLTAGE, [this](){Q_EMIT this->sgIpcVoltageFailed();}},
//        {FACTORY_IPC_SD_CARD, [this](){Q_EMIT this->sgIpcSdcardFailed();}},
//        {FACTORY_IPC_INFRARED, [this](){Q_EMIT this->sgIpcInfraredFailed();}},
//        {FACTORY_IPC_WIFI, [this](){Q_EMIT this->sgIpcWifiFailed();}},
//        {FACTORY_IPC_VOICE, [this](){Q_EMIT this->sgIpcVoiceFailed();}},
//        {FACTORY_IPC_VIDEO, [this](){Q_EMIT this->sgIpcVideoFailed();}},
//        {FACTORY_IPC_END, [this](){Q_EMIT this->sgIpcEndFailed();}},

//        {FACTORY_IPC_PRO_START, [this](){Q_EMIT this->sgIpcProStartFailed();}},
//        {FACTORY_IPC_PRO_SNCODE_WRITE, [this](){Q_EMIT this->sgIpcProSncodeWriteFailed();}},
//        {FACTORY_IPC_PRO_VOLTAGE, [this](){Q_EMIT this->sgIpcProVoltageFailed();}},
//        {FACTORY_IPC_PRO_WIFI, [this](){Q_EMIT this->sgIpcProWifiFailed();}},
//        {FACTORY_IPC_PRO_VOICE, [this](){Q_EMIT this->sgIpcProVoiceFailed();}},
//        {FACTORY_IPC_PRO_END, [this](){Q_EMIT this->sgIpcProEndFailed();}},
//        {HEARTBEAT_AFTER_IPC_PRO, [this](){Q_EMIT this->sgIpcProAfterHeartbeatFailed();}},

//        {FACTORY_CHARGE_PILE_START, [this](){Q_EMIT this->sgChargePileStartFailed();}},
//        {FACTORY_CHARGE_PILE_SNCODE_WRITE, [this](){Q_EMIT this->sgChargePileSncodeWriteFailed();}},
//        {FACTORY_CHARGE_PILE_VOLTAGE, [this](){Q_EMIT this->sgChargePileVoltageFailed();}},
//        {FACTORY_CHARGE_PILE_INFRARED, [this](){Q_EMIT this->sgChargePileInfraredFailed();}},
//        {FACTORY_CHARGE_PILE_KEY, [this](){Q_EMIT this->sgChargePileKeyFailed();}},
//        {FACTORY_CHARGE_PILE_END, [this](){Q_EMIT this->sgChargePileEndFailed();}},

//        {FACTORY_EBO_START, [this](){Q_EMIT this->sgEboStartFailed();}},
//        {FACTORY_EBO_SNCODE_WRITE, [this](){Q_EMIT this->sgEboSncodeWriteFailed();}},
//        {FACTORY_EBO_IMU, [this](){Q_EMIT this->sgEboImuFailed();}},
//        {FACTORY_EBO_INFRARED, [this](){Q_EMIT this->sgEboInfraredFailed();}},
//        {FACTORY_EBO_MOTOR, [this](){Q_EMIT this->sgEboMotorFailed();}},
//        {FACTORY_EBO_TOF, [this](){Q_EMIT this->sgEboTofFailed();}},
//        {FACTORY_EBO_SDCARD, [this](){Q_EMIT this->sgEboSdcardFailed();}},
//        {FACTORY_EBO_LED, [this](){Q_EMIT this->sgEboLedFailed();}},
//        {FACTORY_EBO_LED_DOWN, [this](){Q_EMIT this->sgEboLedDownFailed();}},
//        {FACTORY_EBO_LED_RED, [this](){Q_EMIT this->sgEboLedRedFailed();}},
//        {FACTORY_EBO_LED_GREEN, [this](){Q_EMIT this->sgEboLedGreenFailed();}},
//        {FACTORY_EBO_LED_BLUE, [this](){Q_EMIT this->sgEboLedBlueFailed();}},
//        {FACTORY_EBO_WIFI, [this](){Q_EMIT this->sgEboWifiFailed();}},
//        {FACTORY_EBO_BLUETOOTH, [this](){Q_EMIT this->sgEboBluetoothFailed();}},
//        {FACTORY_EBO_VOICE, [this](){Q_EMIT this->sgEboVoiceFailed();}},
//        {FACTORY_EBO_VIDEO, [this](){Q_EMIT this->sgEboVideoFailed();}},
//        {FACTORY_EBO_KEY, [this](){Q_EMIT this->sgEboKeyFailed();}},
//        {FACTORY_EBO_CHARGE, [this](){Q_EMIT this->sgEboChargeFailed();}},
//        {FACTORY_EBO_UID_WRITE, [this](){Q_EMIT this->sgEboUidWriteFailed();}},
//        {FACTORY_EBO_WIFI_CONFIG, [this](){Q_EMIT this->sgEboWifiConfigFailed();}},
//        {FACTORY_EBO_RESTORE_FACTORY, [this](){Q_EMIT this->sgEboRestoreFactoryFailed();}},
//        {FACTORY_EBO_END, [this](){Q_EMIT this->sgEboEndFailed();}},

//        {FACTORY_EBO_PRO_START, [this](){Q_EMIT this->sgEboProStartFailed();}},
//        {HEARTBEAT_AFTER_EBO_PRO, [this](){Q_EMIT this->sgEboProAfterHeartbeatFailed();}},
//        {FACTORY_EBO_PRO_SNCODE_WRITE, [this](){Q_EMIT this->sgEboProSncodeWriteFailed();}},
//        {FACTORY_EBO_PRO_IMU, [this](){Q_EMIT this->sgEboProImuFailed();}},
//        {FACTORY_EBO_PRO_INFRARED, [this](){Q_EMIT this->sgEboProInfraredFailed();}},
//        {FACTORY_EBO_PRO_MOTOR, [this](){Q_EMIT this->sgEboProMotorFailed();}},
//        {FACTORY_EBO_PRO_TOF, [this](){Q_EMIT this->sgEboProTofFailed();}},
//        {FACTORY_EBO_PRO_LED, [this](){Q_EMIT this->sgEboProLedFailed();}},
//        {FACTORY_EBO_PRO_LED_DOWN, [this](){Q_EMIT this->sgEboProLedDownFailed();}},
//        {FACTORY_EBO_PRO_LED_RED, [this](){Q_EMIT this->sgEboProLedRedFailed();}},
//        {FACTORY_EBO_PRO_LED_GREEN, [this](){Q_EMIT this->sgEboProLedGreenFailed();}},
//        {FACTORY_EBO_PRO_LED_BLUE, [this](){Q_EMIT this->sgEboProLedBlueFailed();}},
//        {FACTORY_EBO_PRO_WIFI, [this](){Q_EMIT this->sgEboProWifiFailed();}},
//        {FACTORY_EBO_PRO_BLUETOOTH, [this](){Q_EMIT this->sgEboProBluetoothFailed();}},
//        {FACTORY_EBO_PRO_VOICE, [this](){Q_EMIT this->sgEboProVoiceFailed();}},
//        {FACTORY_EBO_PRO_VIDEO, [this](){Q_EMIT this->sgEboProVideoFailed();}},
//        {FACTORY_EBO_PRO_KEY, [this](){Q_EMIT this->sgEboProKeyFailed();}},
//        {FACTORY_EBO_PRO_UID_WRITE, [this](){Q_EMIT this->sgEboProUidWriteFailed();}},
//        {FACTORY_EBO_PRO_WIFI_CONFIG, [this](){Q_EMIT this->sgEboProWifiConfigFailed();}},
//        {FACTORY_EBO_PRO_RESTORE_FACTORY, [this](){Q_EMIT this->sgEboProResetFailed();}},
//        {FACTORY_EBO_PRO_END, [this](){Q_EMIT this->sgEboProEndFailed();}},

//        {FACTORY_COLLAR_START, [this](){Q_EMIT this->sgCollarStartFailed();}},
//        {FACTORY_COLLAR_SNCODE_WRITE, [this](){Q_EMIT this->sgCollarSncodeWriteFailed();}},
//        {FACTORY_COLLAR_BLUETOOTH, [this](){Q_EMIT this->sgCollarBluetoothFailed();}},
//        {FACTORY_COLLAR_POWER, [this](){Q_EMIT this->sgCollarPowerFailed();}},
//        {FACTORY_COLLAR_IMU, [this](){Q_EMIT this->sgCollarImuFailed();}},
//        {FACTORY_COLLAR_VOLTAGE, [this](){Q_EMIT this->sgCollarVoltageFailed();}},
//        {FACTORY_COLLAR_KEY, [this](){Q_EMIT this->sgCollarKeyFailed();}},
//        {FACTORY_COLLAR_LED_RGB, [this](){Q_EMIT this->sgCollarLedRgbFailed();}},
//        {FACTORY_COLLAR_STANDBY_CURRENT, [this](){Q_EMIT this->sgCollarStandbyCurrentFailed();}},
//        {FACTORY_COLLAR_CONNECT_CURRENT, [this](){Q_EMIT this->sgCollarConnectCurrentFailed();}},
//        {FACTORY_COLLAR_BROADCAST_CURRENT, [this](){Q_EMIT this->sgCollarBroadcastCurrentFailed();}},
//        {FACTORY_COLLAR_END, [this](){Q_EMIT this->sgCollarEndFailed();}},
//        {HEARTBEAT_AFTER_IPC, [this](){Q_EMIT this->sgIpcAfterHeartbeatFailed();}},
//        {HEARTBEAT_AFTER_EBO, [this](){Q_EMIT this->sgEboAfterHeartbeatFailed();}},
//        {DETEC_EBO_LED, [this](){Q_EMIT this->sgEboLedDetectionFailed();}},
//    };
//}
