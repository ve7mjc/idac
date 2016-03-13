#include "lock.h"

#include <QTimer>

Lock::Lock(QObject *parent) :
    QObject(parent)
{
    // default to generic unless directed otherwise
    m_type = TypeGeneric;
    m_status = StatusUnknown;
}

Lock::Lock(const QJsonObject &json)
{
    read(json);
}


LockType Lock::type()
{
    return m_type;
}

int Lock::setLockType(LockType type)
{
    if (type) {
        m_type = type;
        return false;
    } else return true;
}

int Lock::lock()
{
    // log->writeLine(QString("Requesting lock '%1' to lock").arg(m_name));
    return false;
}

int Lock::unlock(int durationSecs)
{
    if (durationSecs) QTimer::singleShot(durationSecs * 1000, this, SLOT());
    // log->writeLine(QString("Requesting lock '%1' to unlock").arg(m_name));
    return false;
}

void Lock::setUnlocked()
{
    emit statusUnlocked();
}

void Lock::setLocked()
{
    emit statusLocked(this);
}

bool Lock::isLocked()
{
    // Return true only if known to be locked
    // otherwise return false
    if (m_status == StatusLocked) return true;
    else return false;
}

int Lock::initialize()
{
    // log->writeLine(QString("Initializing lock '%1'").arg(m_name));
    return false;
}

void Lock::setName(QString *name)
{
    m_name = *name;
}

void Lock::setDescription(QString *description)
{
    m_description = *description;
}

// QTimer::singleShot QTimer has timed out
// it is time to lock the door again
void Lock::on_unlockTimerTimeout()
{
    // log->writeLine(QString("Unlock timer timed out.  Requesting lock '%1' to lock").arg(m_name));
    this->lock();
}

QString Lock::name() { return m_name; }

QString Lock::description() { return m_description; }

void Lock::read(const QJsonObject &json)
{
    m_id = json["id"].toInt();
    m_name = json["name"].toString();
}

