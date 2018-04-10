#include "jsonsaver.h"

JSonSaver::JSonSaver()
{

}

bool JSonSaver::exists() const
{
    return QFile::exists(QApplication::applicationDirPath() + "/config/fields.json");
}


void JSonSaver::add_string_to_json(QString str)
{
    fields_of_form.append(str);
}

void JSonSaver::write_to_json()
{
    QDir dir;
    dir.mkdir(QApplication::applicationDirPath() + "/config");

    QFile file(QApplication::applicationDirPath() + "/config/fields.json");
    file.open(QIODevice::ReadWrite);


    QJsonDocument jsonDoc;
    QJsonArray fields;
    QJsonObject object;

    for (const auto& f : fields_of_form)    //проходимся по QStringList, в котором лежат данные взятые из формы
    {
        fields << f;
        qDebug() << "JSON LIST:" << f ;
    }
    object["Field"] = fields;
    jsonDoc.setObject(object);
    json = jsonDoc.toJson();

    //открываем и записываем в файл
    file.write(jsonDoc.toJson());
    file.close();
}

void JSonSaver::read_from_json()
{
    if (exists())
    {
        QFile file(QApplication::applicationDirPath() + "/config/fields.json");
        file.open(QIODevice::ReadOnly);
        //QJsonDocument jsondoc = QJsonDocument::fromJson(json);
        json = QJsonDocument::fromJson(file.readAll()).toJson();
        QJsonDocument jsondoc = QJsonDocument::fromJson(json);
        QJsonObject object = /*jsondoc.object();*/jsondoc.object();
        //qDebug() << "object" << object;
        QJsonArray array = object["Field"].toArray();
        for (int i = 0; i < array.size(); ++i)
        {
            //qDebug() << array[i];
            fields_of_form.append(array[i].toString());
        }
        //qDebug() << fields_of_form;
        file.close();
    }
}

const QStringList &JSonSaver::get_json()
{
    //qDebug() << "get_json:" << fields_of_form;
    return fields_of_form;
    fields_of_form.clear(); //чтение и запись осуществляются посредством одной и той же коллекции, поэтому чистить
}


