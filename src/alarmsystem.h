#ifndef ALARMSYSTEM_H
#define ALARMSYSTEM_H

#include <QObject>

class AlarmSystem : public QObject
{
    Q_OBJECT
public:
    explicit AlarmSystem(QObject *parent = 0);

signals:

public slots:

};

#endif // ALARMSYSTEM_H
