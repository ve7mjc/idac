#include "partition.h"

Partition::Partition()
{

}

Partition::Partition(const QJsonObject &json)
{
    read(json);
}


void Partition::read(const QJsonObject &json)
{
    mId = json["id"].toInt();
    mName = json["name"].toString();
}
