#include "user.h"

User::User()
{

}

User::User(const QJsonObject &json)
{
    read(json);
}

QString User::name()
{
    return QString("%0 %1").arg(mNameFirst).arg(mNameLast);
}

void User::read(const QJsonObject &json)
{
    mId = json["id"].toInt();
    mNameFirst = json["name_first"].toString();
    mNameLast = json["name_last"].toString();
    mCodePin = json["code_pin"].toString();
}
