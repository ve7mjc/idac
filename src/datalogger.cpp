/*
 * DataLogger
 *
 * Matthew Currie 2010
 *
 * Log data to disk for use by other applications live or
 * later replay
 *
 * Features:
 * - Enable/Disable
 * - ASCII, XML, etc
 * - Set Path or default to runtime folder
 * - Set Prefix / Suffix
 * - Log Rotation: see below
 * - Logger Slaving
 *   eg. need to write straight ascii (RAW) for
 *       another application to use but also log with
 *       date/timestamping line by line
 *
 * Rotating:
 * We can simply rely on the date/time string causing a new
 * file to be created, which would allow logs by any time
 * increment
 */

#include "datalogger.h"

#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

DataLogger::DataLogger(QObject *parent) :
    QObject(parent)
{
    //Defaults
    displayOutput = true;
    enabled = true;

    setFilenameExtension(".log");
    enableLineTimestamps(defaultLineTimestampsEnabled);
    setLineTimestampFormat(defaultLineTimestampFormat);

    //Default to Simple logging mode
    setFormat(SIMPLE);

    //Default to writing logs into 'logs' subfolder
    savePath = "logs";

    //Default to daily log file with naming convention
    //suitable for good sorting
    filenameDateTimeFormat = "yyyy-MM-dd";

}

void DataLogger::setDisplayOutput(bool value)
{
    displayOutput = value;
}

/**
  * enable()
  * Enable logging
  */
void DataLogger::enable()
{
    this->enabled = true;
}

/**
  * disable()
  * Disable logging
  */
void DataLogger::disable()
{
    this->enabled = false;
}

/**
  * setSavePath(path)
  * Set file system path where log file will be written
  */
int DataLogger::setSavePath(QString path)
{
    this->savePath = path;
    return false;
}

/**
  * setFilenamePrefix(prefix)
  *
  * Set log file name prefix
  */
int DataLogger::setFilenamePrefix(QString prefix)
{
    this->filenamePrefix = prefix;
    return false;
}

/**
  * setFilenameSuffix
  *
  * Set log filename suffix
  */
int DataLogger::setFilenameSuffix(QString suffix)
{
    this->filenameSuffix = suffix;
    return false;
}

/**
  * setFilenameExtension(extension)
  */
int DataLogger::setFilenameExtension(QString extension)
{
    this->filenameExtension = extension;
    return false;
}

/**
  * setFilenameDateTimeFormat
  * Set filename DateTime format
  * uses QDateTime format string
  */
int DataLogger::setFilenameDateTimeFormat(QString datetimeFormat)
{
    this->filenameDateTimeFormat = datetimeFormat;
    return false;
}

/**
  * setLineDateTimeFormat
  * Set QDateTime format which prefixes each line in raw mode
  */
int DataLogger::setLineTimestampFormat(QString datetimeFormat)
{
    this->lineTimestampFormat = datetimeFormat;
    return false;
}

/**
  * setFormat
  *
  * Configure logging format
  */
int DataLogger::setFormat(LogFormat logFormat)
{
    switch (logFormat) {
    case RAW:
        break;
    case SIMPLE:
        break;
    case XML:
        break;
    default:
        //Format not supported
        return true;
    }

    this->format = logFormat;
    return false;
}

/**
  * isLogFileWriteable()
  * Check if Log file can be written to
  */
bool DataLogger::isLogFileWriteable()
{
    /* Determine NMEA Log Filename */
    QDateTime dt = QDateTime::currentDateTime();
    QString nmeaFileName = QString("NMEA ").append(dt.toString("yyyy-MMM-dd")).append(".log");
    QFile nmeaLogFile( nmeaFileName );

    /* Write to file if opened successfully */
    if( !nmeaLogFile.open( QFile::WriteOnly | QFile::Append ) )
    {
        return false;
    }
    else nmeaLogFile.close();

    return true;
}

/**
  * buildFilename()
  * Build log filename (e.g. 2010-11-24_AIS.log)
  */
QString DataLogger::buildFilename(bool withPath)
{
    QString logFileName;

    QDateTime dt = QDateTime::currentDateTime();

    //Append Prefix
    if (!this->filenamePrefix.isEmpty())
        logFileName = QString(this->filenamePrefix);

    //DateTime
    if (!this->filenameDateTimeFormat.isEmpty())
    {
        if (!this->filenamePrefix.isEmpty())
            logFileName.append("_");

        logFileName.append(dt.toString(this->filenameDateTimeFormat));
    }

    //Suffix
    if (!this->filenameSuffix.isEmpty())
    {
        //Append an underscore if necessary
        if (!this->filenameDateTimeFormat.isEmpty())
            logFileName.append("_");
        if ((!this->filenameDateTimeFormat.isEmpty()) &&
                (!this->filenamePrefix.isEmpty()))
            logFileName.append("_");

        logFileName.append(this->filenameSuffix);
    }

    //Append Suffix
    logFileName.append(this->filenameExtension);

    //Prepend File Path if requested
    if (withPath)
    {
        QString logFilePath = this->savePath;
        //Add trailing slash if necessary
        if ((!logFilePath.endsWith("/")) || (!logFilePath.endsWith("/")))
        {
            logFilePath += "/";

        }
        logFileName = logFilePath.append(logFileName);
    }

    //Complete the job
    return logFileName;
}


/**
  * filename(bool fullpath)
  * Return effective filename or fullpathname
  */
QString DataLogger::filename(bool withPath)
{
    return buildFilename(withPath);
}

/**
  * writeData
  * Write data to log file +
  * applying all relevent settigns
  * Default: No CR+LF
  */
int DataLogger::writeData(QByteArray* data, bool crlf)
{
    // Write data to console if enabled
    if (displayOutput) qDebug() << qPrintable(*data);

    //Exit if the logger is disabled
    if (!this->enabled) return false;

    QDateTime dt = QDateTime::currentDateTime();

    //Strip off trailing CR or CRLF if we are going to
    //add it anyways. Also converts UNIX to DOS/WIN.
    if (crlf) {
        if (data->endsWith("\r\n")||data->endsWith("\n\r"))
            data->truncate(data->count()-2);
        if (data->endsWith("\r")||data->endsWith("\n"))
            data->truncate(data->count()-1);
    }

    QString logFilePath = this->savePath;

    //Create file path if not exists
    if (!QDir().exists(logFilePath))
        QDir().mkpath(logFilePath);

    //Add trailing slash if necessary
    if ((!logFilePath.endsWith("/")) || (!logFilePath.endsWith("/")))
        logFilePath += "/";

    logFilePath = QString(logFilePath)
                  .append(buildFilename());

    QFile dataLogFile( logFilePath );

    //Write to file if opened successfully
    if( dataLogFile.open( QFile::WriteOnly | QFile::Append ) )
    {
        QTextStream ts( &dataLogFile );

        if (this->format == RAW) {
            //RAW format
            if (this->lineTimestampFormat.isEmpty()) ts << *data;
            ts << dt.toString(this->lineTimestampFormat) << *data;
            if (crlf) ts << "\r\n";
        }
        if (this->format == SIMPLE) {
            if (this->lineTimestampFormat.isEmpty())
                ts << *data;
            else
                //Append DateTime line header + space
                ts << dt.toString(this->lineTimestampFormat) << " " << *data;
            //Line Ending
            if (crlf) ts << "\r\n";
        }
        else if (this->format == XML) {
            //XML Format
            if (dataLogFile.size()==0)
            { // HEADER
                ts << "<header>\r\n";
                ts << "</header>\r\n";
            }
            ts << "<data datetime=\"" << dt.toString() << "\">";
            ts << data;
            ts << "</data>\r\n";
        }

        dataLogFile.close();
        return false;
    }
    else {
        return true;
    }
}

/**
  * writeLine
  * Convenience function to call writeData()
  * and add CR+LF
  * This may only be applicable to Simple & XML modes
  */
//int DataLogger::writeLine(QString* data)
//{
//    return writeData(data,true);
//}


/**
  * setDelimiter
  * For setting custom byte sequences/patterns
  */
void DataLogger::setDelimiter(QString delimiter)
{
    this->delimiterType_ = CUSTOM;
    this->delimiterCustom = delimiter;
}

/**
  * setDelimiter(enum delimiter)
  */
void DataLogger::setDelimiter(delimiter delimiterType)
{
    this->delimiterType_ = delimiterType;
}
