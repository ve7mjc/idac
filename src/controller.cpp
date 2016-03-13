#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{

    // Upstream MQTT Client
    mqttUpstreamClient = new QMQTT::Client();
    logger = new MqttLogger(mqttUpstreamClient, "controller");
    connect(mqttUpstreamClient, SIGNAL(connected()),
            this, SLOT(onUpstreamMqttConnect()));
    connect(mqttUpstreamClient, SIGNAL(received(QMQTT::Message)),
            this, SLOT(onUpstreamMqttMessageReceived(QMQTT::Message)));

    // Local MQTT Client
    mqttLocalClient = new QMQTT::Client();
    connect(mqttLocalClient, SIGNAL(connected()),
            this, SLOT(onLocalMqttConnect()));
    connect(mqttLocalClient, SIGNAL(received(QMQTT::Message)),
            this, SLOT(onLocalMqttMessageReceived(QMQTT::Message)));

    QCA::Initializer init;

    // We need to ensure that we have certificate handling support
    if ( !QCA::isSupported( "cert" ) )
        qDebug() << "NO PKI certificate support";
    else
        qDebug() << "PKI certificate supported";

}

int Controller::loadConfig(QString token, QString server)
{

    config.loadFromJsonFile("config.json");

    // bail if we do not have a good config to work from
    if (!config.isInitialized())
        return true;

    qDebug() << qPrintable("Configuration loaded");

    // move this to a start/run method maybe?
    // Upstream MQTT Client
    mqttUpstreamClient->setHost(config.mqttUpstreamHost);
    mqttUpstreamClient->setPort(config.mqttUpstreamPort);
    mqttUpstreamClient->setCleansess(true);
    mqttUpstreamClient->connect();

    // Local MQTT Client
    mqttLocalClient->setHost(config.mqttLocalHost);
    mqttLocalClient->setPort(config.mqttLocalPort);
    mqttLocalClient->setCleansess(true);
    mqttLocalClient->connect();

    // we process config after calling connection to MQTT
    // so we can log any exceptions remotely
    processConfig();

    // everything went ok
    return false;

}

// does not process basic mqtt settings at this point
// they are being processed in the Config class
int Controller::processConfig()
{

    QJsonArray jUsers = config.json["users"].toArray();
    for (int i = 0; i < jUsers.count(); i++) {
        User *user = new User(jUsers.at(i).toObject());
        users.insert(user->id(), user);
        qDebug() << qPrintable(QString("Added user %0 %1").arg(user->nameFirst()).arg(user->nameLast()));
    }

    QJsonArray jTokens = config.json["tokens"].toArray();
    for (int i = 0; i < jTokens.count(); i++) {
        Token *token = new Token(jTokens.at(i).toObject());
        tokens.insert(token->id(), token);
        qDebug() << qPrintable(QString("Added token %1").arg(QString::number(token->id())));
    }

    QJsonArray jPartitions = config.json["partitions"].toArray();
    for( int i = 0; i < jPartitions.count(); i++ ) {
        Partition *partition = new Partition(jPartitions.at(i).toObject());
        partitions.insert(partition->id(), partition);
        qDebug() << qPrintable(QString("Added partition %1").arg(QString::number(partition->id())));
    }

    QJsonArray jLocks = config.json["locks"].toArray();
    for( int i = 0; i < jLocks.count(); i++ ) {
        Lock *lock = new Lock(jLocks.at(i).toObject());
        locks.insert(lock->id(), lock);
        qDebug() << qPrintable(QString("Added lock %1").arg(QString::number(lock->id())));
    }

    logger->write("configured");

    return false;
}

void Controller::onUpstreamMqttConnect()
{
    mqttUpstreamClient->subscribe("controller/controller1", 0);
    qDebug() << qPrintable("connected to upstream mqtt broker");

    logger->write("connected to upstream MQTT", LogLevel::NOTICE);
}

void Controller::onLocalMqttConnect()
{
    //mqttLocalClient->subscribe("Controller/", 0);
    qDebug() << qPrintable("connected to local mqtt broker");

    logger->write("connected to local MQTT", LogLevel::NOTICE);
}

void Controller::onUpstreamMqttMessageReceived(QMQTT::Message message)
{
    logger->write(QString("Received MQTT %1: %2").arg(message.topic()).arg(QString(message.payload())));
    QJsonObject msg = QJsonDocument::fromJson(message.payload()).object();

//    processWind(msg["speed"].toString().toDouble(), msg["direction"].toString().toDouble());
}

void Controller::onLocalMqttMessageReceived(QMQTT::Message message)
{
//    QJsonObject msg = QJsonDocument::fromJson(message.payload()).object();
//    processWind(msg["speed"].toString().toDouble(), msg["direction"].toString().toDouble());
}
