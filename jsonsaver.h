#ifndef JSONSAVER_H
#define JSONSAVER_H

#include <QFile>
#include <QApplication>
#include <QStringList>
#include <QString>
#include <QStringList>
#include <QDir>
#include <qjsondocument.h>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

class JSonSaver
{
    QStringList fields_of_form;//
    QByteArray json;

public:
    JSonSaver();
    bool exists() const;
    void add_string_to_json(QString str);
    void write_to_json();
    void read_from_json();
    const QStringList& get_json();
};

#endif // JSONSAVER_H
