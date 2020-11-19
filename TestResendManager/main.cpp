#include <QCoreApplication>
#include <QDebug>
#include "../ResendManager.h"

class TestResendManager : public QObject {

public:
    enum {
        ITEM1 = 1,
        ITEM2 = 2,
    } ItemIds;

    explicit TestResendManager(QObject *parent = nullptr) : QObject(parent) {
        initConnections();
        createItemsMap();
    }

    ~TestResendManager() {

    }

    void initConnections() {
        connect(&mResendManager, &AutoResendBase::sgItemTimeout,
                this, &TestResendManager::doTimeout, Qt::DirectConnection);
        connect (&mResendManager, &AutoResendBase::sgItemFailed,
                 this, &TestResendManager::doFailed, Qt::DirectConnection);
    }
    void createItemsMap() {
        ResendManager *resend = new ResendManager(1000, 3); // 1s timeout, repeat 3 to failed
        mResendManager.appendItem(ITEM1, resend);
        resend = new ResendManager(2000, 3); // 2s timout, repeat 3 if not call stop
        mResendManager.appendItem(ITEM2, resend);
    }

    void doSomething1() {
        mResendManager.startItem(ITEM1);
        qDebug() << "do something1";
    }
    void doSomething1Done() {
        mResendManager.stopItem(ITEM1);
        qDebug() << "do something1 done.";
    }

    void doSomething2() {
        mResendManager.startItem(ITEM2);
        qDebug() << "do something2";
    }
    void doSomething2Done() {
        mResendManager.stopItem(ITEM2);
        qDebug() << "do something2 done";
    }

public Q_SLOTS:
    void doTimeout(int vId, int vRepeadCount) {
        switch (vId) {
        case ITEM1:
            qDebug() << "ITEM1 timeout: " << vRepeadCount;
            // TODO timeout to do something
            break;
        case ITEM2:
            qDebug() << "ITEM2 timeout." << vRepeadCount;
            // TODO timeout to do something
            if (vRepeadCount == 2) {
                qDebug() << "get ITEM2 done to stop avoid into failed.";
                doSomething2Done();
            }
            break;
        default:
            qDebug() << "Error id: " << vId << vRepeadCount;
            break;
        }
    }

    void doFailed(int vId) {
        switch (vId) {
        case ITEM1:
            qDebug() << "ITEM1 failed.";
            // TODO failed to do something
            doSomething2();
            break;
        case ITEM2:
            qDebug() << "ITEM2 failed.";
            // TODO failed to do something
            break;
        default:
            qDebug() << "Error id: " << vId;
            break;
        }
    }

private:
    AutoResendBase mResendManager;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TestResendManager test;
    test.doSomething1();
    return a.exec();
}
