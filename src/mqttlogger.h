// MQTT Logging Class
//
// Write log messages to log/client_name/debug_level
// with DATE_TIME <message>
//
// TODO:
//  - Buffer messages to account for logging messages
//    prior to MQTT broker connection
//

#ifndef MQTTLOGGER_H
#define MQTTLOGGER_H

#include "../lib/qmqtt/qmqtt.h"

#include <QObject>
#include <QDateTime>
#include <QQueue>
#include <QTimer>

enum LogLevel {
    DEBUG,
    INFO,
    NOTICE,
    WARN,
    ERROR
};

class MqttLogger : public QObject
{
    Q_OBJECT
public:
    explicit MqttLogger(QMQTT::Client *client, QString clientName);

    int write(QByteArray*, LogLevel = LogLevel::NOTICE);
    int write(QString* data, LogLevel level = LogLevel::NOTICE) { return write(new QByteArray(data->toUtf8()), level); }
    int write(const char *data, LogLevel level = LogLevel::NOTICE) { return write(new QByteArray(data), level); }
    int write(QString data, LogLevel level = LogLevel::NOTICE) { return write(new QByteArray(data.toUtf8()), level); }

private:

    QMQTT::Client* client;
    QString clientName;
    LogLevel logLevel;
    QString logTopicPathBase;

    // Outbound Message Queue
    QQueue<QMQTT::Message*> messageQueue;
    QTimer messageQueueTimer;

signals:

public slots:

    void onMessageQueueTimerTimeout();




};

#endif // MQTTLOGGER_H
