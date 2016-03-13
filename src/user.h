#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>

#include "config.h"
#include "token.h"

class User
{
public:

    User();
    User(const QJsonObject &json);

    quint32 id() { return mId; }

    // Contact Information
    QString name();
    QString nameFirst() { return mNameFirst; }
    QString nameLast() { return mNameLast; }

    // read from JSonObject
    void read(const QJsonObject &json);

private:

    quint32 mId;

    // Contact Information
    QString mNameFirst;
    QString mNameLast;
    QString mCodePin;

    Token fds;

    QList<Token*> tokens;

};

#endif // USER_H
