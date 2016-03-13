#include "token.h"

Token::Token()
{

}

Token::Token(const QJsonObject &json)
{
    read(json);
}

void Token::read(const QJsonObject &json)
{
    m_id = json["id"].toInt();
    mCode = json["code"].toString();
}


//Token::Token(const QString code, int id, int member_id, int status_id, const QString name)
//{
//    this->code = code;
//    this->id = id;
//    this->member_id = member_id;
//    this->status_id = status_id;
//    this->name = name;
//}
