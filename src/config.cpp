#include "config.h"

Config::Config()
{
    initialized = false;
}

// Load Controller Config from JSON File
int Config::loadFromJsonFile(QString fileName)
{

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << qPrintable(QString("could not open %0").arg(fileName));
        return true;
    }
    QByteArray data = file.readAll();

    QJsonParseError parseError;
    QJsonDocument doc(QJsonDocument::fromJson(data, &parseError));

    if (parseError.error == QJsonParseError::NoError)
        processJson(doc.object());
    else {
        qDebug() << qPrintable(QString("Config parse error at %0: %1").arg(parseError.offset).arg(parseError.errorString()));
        qDebug() << qPrintable(data.mid(parseError.offset-20,40));
        return true;
    }

    return false;

}

int Config::processJson(const QJsonObject &json)
{

    // Process Immediately Required Parameters
    //

    // MQTT Upstream
    if (json.contains("mqttUpstream")) {
        QJsonObject mqtt = json["mqttUpstream"].toObject();
        mqttUpstreamHost = mqtt["remote_host"].toString();
        mqttUpstreamPort = mqtt["remote_port"].toInt();
        if (mqttUpstreamPort == 0) mqttUpstreamPort = 1883;
    } else {
        qDebug() << qPrintable("MQTT Upstream not found in config file");
    }

    // MQTT Local
    if (json.contains("mqttLocal")) {
        QJsonObject mqtt = json["mqttLocal"].toObject();
        mqttLocalHost = mqtt["remote_host"].toString();
        mqttLocalPort = mqtt["remote_port"].toInt();
        if (mqttUpstreamPort == 0) mqttLocalPort = 1883;
    } else {
        qDebug() << qPrintable("MQTT Local not found in config file");
    }

    // Pass JSON object into config
    this->json = json;
    initialized = true;

}
