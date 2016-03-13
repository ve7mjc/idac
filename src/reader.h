#ifndef READER_H
#define READER_H

#include <QObject>

enum ReaderType {
    READER_RFID_TOKEN
};

class Reader : public QObject
{
    Q_OBJECT
public:
    explicit Reader(QObject *parent = 0);

signals:

public slots:

};

#endif // READER_H
