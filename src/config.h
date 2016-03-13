#ifndef CONFIG_H
#define CONFIG_H

#include <QFile>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

class Config
{
public:
    Config();

    QString token;

    int loadFromJsonFile(QString file);
    int processJson(const QJsonObject &json);

    QString mqttUpstreamHost;
    quint16 mqttUpstreamPort;

    QString mqttLocalHost;
    quint16 mqttLocalPort;

    QJsonObject json;

    bool isInitialized() { return initialized; }

private:

    bool initialized;

};

#endif // CONFIG_H
