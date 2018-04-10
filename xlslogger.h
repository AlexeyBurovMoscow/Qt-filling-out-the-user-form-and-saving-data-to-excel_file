#ifndef XLSLOGGER_H
#define XLSLOGGER_H
#include <QWidget>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDate>
#include <QVector>
#include <QVariant>
#include <QDebug>

class XLSlogger:public QWidget
{
    Q_OBJECT
private:
    static XLSlogger * pxls_instance;       //для инстанциирования экземпляра класса через Singletone
    explicit XLSlogger(QWidget *parent = 0);

    //абстрактная модель данных/для работы с таблицей
    QStandardItemModel * model;
    QStandardItem * item;

    //это самый мелкий (BASIC)размер поля, который из себя представляет одно из полей для заполнения
    QVector<QVariant> basic_vector;

    //это вектор (LINE), в который надо передавать базовые элементы (BASIC)
    QVector<QVariant> line_vector;

    //в этот вектор помещать все коллекции значений (BLOCK)
    QVector<QVector<QVariant>> block_vector;


public:

    static XLSlogger *getInstance();
    QStandardItemModel * getModel();//для использования в клиентском коде (см. GUI.cpp)
    void set_BasicField(int i);//чисто тестовая штука. Здесь формируются тестовые поля для заполнения LINE
    void set_BasicField(QString str);//практическая штука. вызывается из клиентского кода
    void makeTable();     //оформление данных в таблицу. Метод фактически обращается к объекту QStandartItemModel, который и управляет размещением данных. Т.е. не вызывать его нельзя
    void setHeader(QString mystr);
    void writeBasicField_toDataLine();//дописывает BASIC-FIELD в LINE
    void writeDataLine_toBlock();//дописывает LINE в BLOCK
    void writeDataBlock_toFile();//дописывает BLOCK в FILE
//public slots:
//    void report_Received();
};

#endif // XLSLOGGER_H
