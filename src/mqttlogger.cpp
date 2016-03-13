#include "mqttlogger.h"

MqttLogger::MqttLogger(QMQTT::Client *client, QString clientName) : QObject()
{
    this->client = client;
    this->clientName = clientName;
    logLevel = LogLevel::DEBUG;
    logTopicPathBase = QString("log/%0").arg(clientName);
    connect(&messageQueueTimer, SIGNAL(timeout()),
            this, SLOT(onMessageQueueTimerTimeout()));
}

int MqttLogger::write(QByteArray* data, LogLevel level)
{

    // set associated MQTT topic name for
    // log level
    QString logLevel = "debug";
    switch (level) {
    case DEBUG:
        break;
    case INFO:
        break;
    case NOTICE:
        break;
    case WARN:
        break;
    case ERROR:
        break;
    default:
        break;
    }

    // Assemble MQTT Message
    QMQTT::Message *message = new QMQTT::Message();
    message->setTopic(QString("%0/%1").arg(logTopicPathBase).arg(logLevel).toUtf8());
    message->setPayload(QString("%0 %1").arg(QDateTime::currentDateTime().toString()).arg(data->constData()).toUtf8());

    // Enqueue Message if MQTT client is not (yet) connected
    // so we can send when able
    if (client->isConnected()) {
        client->publish(*message);
        delete message;
    } else {
        qDebug() << "MqttLogger Message Queued; Reason: Not connected";
        messageQueue.enqueue(message);
        messageQueueTimer.start(100);
    }

}

void MqttLogger::onMessageQueueTimerTimeout()
{

    if (client->isConnected()) {
        while (messageQueue.count()) {
            QMQTT::Message* message = messageQueue.dequeue();
            client->publish(*message);
            delete message;
        }
    }

    messageQueueTimer.stop();

}
