#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QObject>
#include <QDebug>

const QString defaultLineTimestampFormat = "yyyy-MM-dd_HH:mm:ss.zzz";
const bool defaultLineTimestampsEnabled = true;

class DataLogger : public QObject
{
    Q_OBJECT
public:
    explicit DataLogger(QObject *parent = 0);

    //File Path & Naming
    int setSavePath(QString path);
    int setFilename(QString filename);
    int setFilenamePrefix(QString prefix);
    int setFilenameSuffix(QString suffix);
    int setFilenameExtension(QString extension);
    int setFilenameDateTimeFormat(QString datetimeFormat);
    bool isLogFileWriteable();
    QString filename(bool withPath = false);

    //Log Formatting
    enum LogFormat { RAW, SIMPLE, XML };
    int setFormat(LogFormat logFormat);
    enum delimiter { NONE, NEWLINE, CRLF, LF, CR, COMMA, CUSTOM };
    void setDelimiter(QString delimiter);
    void setDelimiter(delimiter delimiterType);
    int setLineTimestampFormat(QString datetimeFormat);
    void enableLineTimestamps(bool value = true) { this->lineTimestampsEnabled = value; }

    void setDisplayOutput(bool value = true);

    //Enable & Disable controls
    void enable();
    void disable();
    bool isEnabled() { return this->enabled; }

    //Write content to log
    // int writeData(QString* data, bool crlf = false);
    int writeData(QByteArray* data, bool crlf = false);

    int writeLine(QString* data) { return writeData(new QByteArray(data->toUtf8()),true); }
    int writeLine(const char *data) { return writeData(new QByteArray(data), true); }
    int writeLine(QString data) { return writeData(new QByteArray(data.toUtf8()),true); }
    // int writeLine(QByteArray msg) { return writeData(&msg); }



protected:
    LogFormat format;
    bool enabled;
    QString savePath;
    QString filename_; // no prefix/date/suffix
    QString filenamePrefix;
    QString filenameSuffix;
    QString filenameExtension;
    QString filenameDateTimeFormat;
    QString lineTimestampFormat;
    QString buildFilename(bool withPath = false);
    QString delimiterCustom;
    delimiter delimiterType_;

    bool lineTimestampsEnabled;

private:

    bool displayOutput; // display lines to qDebug()

signals:

public slots:

};

#endif // DATALOGGER_H
