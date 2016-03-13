#ifndef LOCK_H
#define LOCK_H

#include <QObject>
#include <QList>

#include "config.h"

enum LockType {
    TypeGeneric,
    TypeDeadbolt,
    TypeMagnetHold,
    TypeSolenoid,
    TypeMagneticStrike,
};

enum LockStatus {
    StatusLocked,
    StatusUnlocked,
    StatusLocking,
    StatusUnlocking,
    StatusFaulted,
    StatusUnknown
};

class Lock : public QObject
{
    Q_OBJECT
public:
    explicit Lock(QObject *parent = 0);
    Lock(const QJsonObject &json);

    quint16 id() { return m_id; }

    virtual int lock();
    virtual int unlock(int durationSecs = 0);

    void setLocked();
    void setUnlocked();

    virtual bool isLocked();

    virtual int initialize();

    LockType type();
    int setLockType(LockType type);
    LockStatus status();

    QString name();
    QString description();
    void setName(QString* name);
    void setDescription(QString* description);

    // read from JSonObject
    void read(const QJsonObject &json);

private:

    quint16 m_id;

protected:

    LockType m_type;
    LockStatus m_status;
    bool waitingForMovement; // maybe?
    QString m_name;
    QString m_description;

signals:

    void statusLocked(Lock* lock);
    void statusUnlocked();
    void statusFaulted();
    void lockButtonRequested();
    void statusLocking();

public slots:
    void on_unlockTimerTimeout();

};

#endif // LOCK_H
