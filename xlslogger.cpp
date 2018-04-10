#include "xlslogger.h"

XLSlogger * XLSlogger::pxls_instance;
XLSlogger::XLSlogger(QWidget *parent):QWidget(parent)
{
    qDebug() << "XLS_logger here";
    model = new QStandardItemModel();
}

XLSlogger *XLSlogger::getInstance()
{
    if(!pxls_instance)
        pxls_instance = new XLSlogger();
    return pxls_instance;
}

QStandardItemModel *XLSlogger::getModel()
{
    return model;
}

void XLSlogger::makeTable()
{
    //-----------------------------не трогать! Для случая с одиночными векторами работает нормально
//    QString str = "MyVector:";
//    item = new QStandardItem(str);
//    model->setItem(0,0,item);
//    for(int i = 0; i < basic_vector.length(); i++){
//        str = basic_vector.at(i).toString();
//        item = new QStandardItem(str);
//        model->setItem(0, i+1, item);
//    }
    //---------------------------------------------------------------------------------------------
     QString str = "MyVector:";
     int length = 0;
     if(block_vector.isEmpty())
         length = 1;
     else
         length = block_vector.length();

     //сначала пройтись по большой коллекции с целью определить самый длинный из вложенных векторов
     int max_length = 0;
     int local_max_length = 0;
     for(int y = 0; y < length; y++){
        local_max_length = block_vector.at(y).length();
        if(local_max_length >= max_length)
            max_length = local_max_length;
     }

     //как только это выполнено можно установить счетчик столбцов в модели, чтобы данные не терялись во время отображения
     model->setColumnCount(max_length);

     //затем извлечь значения векторов в Items модели

     for(int y = 0; y < length; y++){

             for(int i = 0; i < max_length; i++){
                 if(i==0)
                 {
                     QString str = "MyVector:" + QString::number(y);
                     item = new QStandardItem(str);
                     model->setItem(y+1,0,item);    //y+1 - т.к. строка заголовка занимает 0-строку
                 }
                 if(!block_vector.isEmpty())
                 {
                 str = block_vector.at(y).at(i).toString();
                 item = new QStandardItem(str);
                 model->setItem(y+1, i+1, item);    //i+1 - т.к. сторка "myVector" занимает 0-столбец (см. вывод таблицы на форму)
                 }
                 else
                 {
                     qDebug() << "first add your Vector to BIG-Vector";
                 }
             }
     }
}

void XLSlogger::setHeader(QString mystr)
{
    QString str = mystr;
    QString fpath;
    QString fname = "my_reports.xls";

    if(!QDir("ApplicationReports").exists())
    {
        QDir().mkdir("ApplicationReports");
    }
    fpath = QDir().absoluteFilePath("ApplicationReports").append("/").append(fname);
    QFile mFile(fpath);
    mFile.open(QIODevice::Append);

    item = new QStandardItem(str);
    model->setItem(0,0,item);//здесь конкретная ячейка заголовка (пишется в модель)
    //заголовок в excel-файл передается только один раз, поэтому придется на этой стадии открыть-дописать-закрыть файл
    QTextStream ts( &mFile );//файловый поток
    str = model->data(model->index(0,0),Qt::DisplayRole).toString();//и здесь конкретная ячейка заголовка(читается из модели в файл)
    ts << str;
    mFile.close();
}
void XLSlogger::set_BasicField(int i)
{
    int * p_int;
    double * p_double;
    float * p_float;

    basic_vector.clear();
    model->clear();
    switch (i) {
    case 0:
        //-------сформировать BASIC-вектор int`ов
        for(int i = 0; i<10; i++){
            p_int = new int(5);
            basic_vector.append(*p_int);
        }
        //--------------------------------
        break;
    case 1:
        //-------сформировать BASIC-вектор float`ов
        for(int i = 0; i<7; i++){
            p_float = new float(1.5);
            basic_vector.append(*p_float);
        }
        //-----------------------------------
        break;
    case 2:
        //-------сформировать BASIC-вектор doubl`ов
        for(int i = 0; i<5; i++){
            p_double = new double(0.19854);
            basic_vector.append(*p_double);
        }
        //----------------------------------
        break;
    default:
        break;
    }
    //--------------------------------------------------------------
    //только для проверки
//    switch (i) {
//    case 0:
//        for(int i = 0; i < basic_vector.length(); i++){
//            qDebug()<<basic_vector.at(i).toInt();
//        }
//        break;
//    case 1:
//        for(int i = 0; i < basic_vector.length(); i++){
//            qDebug()<<basic_vector.at(i).toFloat();
//        }
//        break;
//    case 2:
//        for(int i = 0; i < basic_vector.length(); i++){
//            qDebug()<<basic_vector.at(i).toDouble();
//        }
//        break;
//    default:
//        break;
//        }
    //досюда можно закомментить
    //----------------------------------------------------------------
}

void XLSlogger::set_BasicField(QString str)
{
    QVariant v;
    v = QVariant(str);
    basic_vector.append(v);
    //только для проверки
            for(int i = 0; i < basic_vector.length(); i++){
                qDebug()<<basic_vector.at(i).toString();
            }
}

void XLSlogger::writeBasicField_toDataLine()
{
    line_vector.append(basic_vector);
    //-------------------------------------------
    //----------только для проверки--------------
    qDebug() << "length of line = " << line_vector.length();
    for(int i = 0; i < line_vector.length(); i++){
        qDebug()<<line_vector.at(i).toString();
    }
    //досюда можно закомментить
    //-------------------------------------------
    basic_vector.clear();
}

void XLSlogger::writeDataLine_toBlock()
{
    block_vector.append(line_vector);
    line_vector.clear();
}

void XLSlogger::writeDataBlock_toFile()
{
    int i = 0;
    QString fpath;
    QString fname = "my_reports.xls";
    int block_rows = block_vector.length()+1;//в файл должен быть записан заголовок, т.е. +1 строка
    if(!QDir("ApplicationReports").exists())
    {
        QDir().mkdir("ApplicationReports");
    }
    fpath = QDir().absoluteFilePath("ApplicationReports").append("/").append(fname);
    qDebug()<<"path to file is"<<fpath;
    QFile mFile(fpath);
    mFile.open(QIODevice::Append);
    //--------------алгоритм по извлечению БЛОКА данных из модели в таблицу excel

    QTextStream ts( &mFile );//файловый поток
    QString str;

    for(int y = 0; y < /*block_vector.length()*/block_rows; y++)
    {
        while (i < model->columnCount())
        {
            str = model->data(model->index(y,i),Qt::DisplayRole).toString();
            ts<<str.append("\t");
            str.clear();
            i+=1;
        }
        ts << str.append("\n");
        str.clear();
        i = 0;
    }
    //---------------------------------------------------------------------
    mFile.close();
    block_vector.clear();
}

//void XLSlogger::report_Received()
//{
//    qDebug() << "Report from GUI received";
//}




