#ifndef PARTITION_H
#define PARTITION_H

#include <QString>

#include "config.h"

class Partition
{

public:

    Partition();
    Partition(const QJsonObject &json);

    quint32 id() { return mId; }
    QString name() { return mName; }

    // read from JSonObject
    void read(const QJsonObject &json);

private:

    quint32 mId;
    QString mName;

};

#endif // PARTITION_H
