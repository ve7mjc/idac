#ifndef TOKEN_H
#define TOKEN_H

#include <QObject>

#include "config.h"

enum TokenType {
    GENERIC = 0,
    RFID_125KHZ = 1,
    RFID_NFC = 2
};

enum TokenStatus {
    DISABLED = 0,
    ACTIVE = 1,
    LOST = 2
};

class Token : public QObject
{
    Q_OBJECT
public:

    explicit Token();
    Token(const QJsonObject &json);

    QString code() { return mCode; }
    quint32 id() { return m_id; }

    // read from JSonObject
    void read(const QJsonObject &json);

private:

    quint32 m_id;
    quint32 mUserId;
    QString mCode;
    TokenStatus mStatus;
    TokenType mType;

};

#endif // TOKEN_H
