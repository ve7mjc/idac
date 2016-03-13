// Controller is the main class
// It is the top level daemon of the local
// access control controller
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#define QMQTT_LIBRARY
#include "../lib/qmqtt/qmqtt.h"

#include "mqttlogger.h"
#include "config.h"

#include <QObject>
#include <QDateTime>
#include <QDebug>

#include <QHash>

#include "partition.h"
#include "user.h"
#include "token.h"
#include "lock.h"

#include <QtCrypto>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

    // load config from remote server or fall back
    // to local cached configuration
    int loadConfig(QString token, QString server = "");
    int processConfig();

    int processTokenEvent();

    // MQTT Clients
    QMQTT::Client *mqttUpstreamClient;
    QMQTT::Client *mqttLocalClient;

    MqttLogger *logger;

    QHash<quint16, User *> users;
    QHash<quint16, Partition *> partitions;
    QHash<quint16, Token *> tokens;
    QHash<quint16, Lock *> locks;

private:
    Config config;

    QString mqttClientName;

    // Idac Particular Configs
    QString name;
    QString description;

signals:

public slots:
    void onUpstreamMqttConnect();
    void onLocalMqttConnect();
    void onUpstreamMqttMessageReceived(QMQTT::Message);
    void onLocalMqttMessageReceived(QMQTT::Message);
};

#endif // CONTROLLER_H
