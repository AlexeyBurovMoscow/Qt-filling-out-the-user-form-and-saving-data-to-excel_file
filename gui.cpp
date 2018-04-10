#include "gui.h"
#include "xlslogger.h"

//конструктор
GUI::GUI(QWidget *parent): QWidget(parent)
{
    createGUI();

    //создал таймер для отображения даты/времени
    timer = new QTimer;
    timer->start(1000);

    //получил текущую дату
    current_date = QDate::currentDate().toString("dddd, d, MMMM yy,");

    //Создал экземпляр XLS-логгера
    xlslog = XLSlogger::getInstance();
    //название таблицы и установка модели
    report_tbv->setModel(xlslog->getModel());
    xlslog->setHeader("MyHeader");


    qDebug() << "read from json:";
    //расстановка данных в поля формы из JSON
    json_saver.read_from_json();
    if(json_saver.exists()) //
    type_fields_from_json();


    //подсоединил слот логгера к сигналу с формы для отсылки сформированного опросника
    //QObject::connect(this,&GUI::data_Ready, xlslog, &XLSlogger::report_Received);

    //кнопка ввода
    QObject::connect(input_btn, &QPushButton::clicked, this, &GUI::on_Input_pressed);
    //выбор времени суток
    QObject::connect(times_of_day_qcb, static_cast<void (QComboBox::*)(int)>(QComboBox::currentIndexChanged),this,&GUI::on_Time_of_day_select);
    //выбор метеоусловий
    QObject::connect(meteoconditions_qcb, static_cast<void (QComboBox::*)(int)>(QComboBox::currentIndexChanged),this,&GUI::on_Conditions_select);
    //осадки
    QObject::connect(precipitation_qcb,static_cast<void (QComboBox::*)(int)>(QComboBox::currentIndexChanged),this,&GUI::on_Precipitation_select);
    //облачность
    QObject::connect(cloudcover_qcb, static_cast<void (QComboBox::*)(int)>(QComboBox::currentIndexChanged),this,&GUI::on_Cloudcover_select);
    //дальность видимости
    QObject::connect(range_visibility_qcb, static_cast<void (QComboBox::*)(int)>(QComboBox::currentIndexChanged),this,&GUI::on_RangeVisibility_select);
    //температура
    QObject::connect<void(QSpinBox::*)(int)>(temperature_spb, &QSpinBox::valueChanged, this, &GUI::on_Temperature_select);
    //выбор даты в календаре
    QObject::connect(calendar, &QCalendarWidget::selectionChanged, this, &GUI::on_CalendarDate_select);
    //отметка CheckBoxa реальное время
    QObject::connect(realtime_chb, &QCheckBox::clicked, this, &GUI::on_Real_Checked);
    //таймер для вывода текущего времени
    QObject::connect(timer, &QTimer::timeout, this, &GUI::on_TimerTick);


}
//деструктор
GUI::~GUI()
{

}
//методы

//вывод на экран окошка пользовательского интерфейса
void GUI::createGUI()
{
    report_tbv = new QTableView();//tableview можно не добавлять на форму
    mainlayout = new QVBoxLayout();
    top_layout = new QHBoxLayout();
    mid_layout = new QHBoxLayout();
    down_layout = new QHBoxLayout();
    window_plot_layout = new QVBoxLayout();
    //--------добавляем GroupBoxes в слои
    top_layout->addWidget(createTopLeftGroup());
    window_plot_layout->addWidget(createTopMiddleSubGroup_meteo());
    window_plot_layout->addWidget(createTopMiddleSubGroup_window());
    top_layout->addLayout(window_plot_layout);
    //top_layout->addWidget(createTopMiddleSubGroup_meteo());

    top_layout->addWidget(createTopRightGroup());
    mid_layout->addWidget(createMiddleGroup());
    down_layout->addWidget(createDownGroup());
    //--------добавляем основные слои в самый большой слой
    mainlayout->addLayout(top_layout);
    mainlayout->addLayout(mid_layout);
    mainlayout->addLayout(down_layout);
    this->setLayout(mainlayout);
    //--------Текст в заголовке окна----------------------
    this->setWindowTitle("Окно пользовательского ввода");
}
//методы, создающие группы виджетов
//--------------формирование левого верхнего-----------
QGroupBox *GUI::createTopLeftGroup()
{
    //в этой группе размещается все, что связано с календарем, временем, датой

    current_date_and_time_lbl = new QLabel("Дата и время наблюдения: " + QDateTime::currentDateTime().toString("dddd, d, MMMM yy, hh:mm:ss"));
    header_layout = new QHBoxLayout();
    time_of_day_lbl = new QLabel("Время суток");
    top_left_grb = new QGroupBox(tr("Время наблюдения"));
    top_left_layout = new QVBoxLayout();
    calendar_layout = new QVBoxLayout();
    date_and_time_layout = new QVBoxLayout();
    calendar = new QCalendarWidget();
    times_of_day_qcb = new QComboBox();
    //выравнивание текста в заголовке GroupBox
    top_left_grb->setAlignment(Qt::RightEdge);
    //настройка пунктов ComboBox
    times_of_day_qcb->addItem("Ночь без луны");
    times_of_day_qcb->addItem("Ночь с неполной луной");
    times_of_day_qcb->addItem("Ночь-полнолуние");
    times_of_day_qcb->addItem("Сумерки перед рассветом");
    times_of_day_qcb->addItem("Сумерки перед закатом");
    times_of_day_qcb->addItem("День");

    //-----компоновка
    calendar_layout->addWidget(calendar);
    header_layout->addSpacing(100);
    header_layout->addWidget(time_of_day_lbl);
    date_and_time_layout->addLayout(header_layout);
    date_and_time_layout->addWidget(times_of_day_qcb);
    date_and_time_layout->addWidget(current_date_and_time_lbl);
    top_left_layout->addLayout(calendar_layout);
    top_left_layout->addLayout(date_and_time_layout);
    date_and_time_layout->addStretch(1);
    top_left_grb->setLayout(top_left_layout);
    return top_left_grb;
}

//------------формирование среднего верхнего-----------
QGroupBox *GUI::createTopMiddleSubGroup_meteo()
{
    temperature_layout = new QHBoxLayout();
    humidity_layout = new QHBoxLayout();
    pressure_layout = new QHBoxLayout();
    temperature_format_lbl = new QLabel("          T, C˚:");
    humidity_format_lbl = new QLabel("           ϕ, %:");
    pressure_format_lbl = new QLabel("p,мм/рт.ст.:");
    meteoconditions_lbl = new QLabel("Условия");
    top_middle_grb_meteo = new QGroupBox(tr("Метеоусловия"));
    top_middle_layout = new QHBoxLayout();
    precipitation_lbl = new QLabel("Осадки");
    cloudcover_lbl = new QLabel("Облачность");
    temperature_lbl = new QLabel("Температура");
    humidity_lbl = new QLabel("Влажность");
    atmosphere_pressure_lbl = new QLabel("Атмосферное давление");
    meteoconditions_qcb = new QComboBox();
    precipitation_qcb = new QComboBox();
    cloudcover_qcb = new QComboBox();
    temperature_spb = new QSpinBox();
    humidity_spb = new QSpinBox();
    atmosphere_pressure_spb = new QSpinBox();
    left_meteo_layout = new QVBoxLayout();
    right_meteo_layout = new QVBoxLayout();
    //настройка Spinboxes
    temperature_spb->setRange(-100, 100);   //диапазон и текущее значение температуры
    temperature_spb->setValue(0);
    humidity_spb->setRange(0,100);          //диапазон и значение влажности
    humidity_spb->setValue(50);
    atmosphere_pressure_spb->setRange(300,800);//диапазон атмосферного давления (он же не выходит за эти рамки?)
    atmosphere_pressure_spb->setValue(740);
    //компоновка левой части
    left_meteo_layout->addWidget(meteoconditions_lbl);
    left_meteo_layout->addWidget(meteoconditions_qcb);
    left_meteo_layout->addWidget(precipitation_lbl);
    left_meteo_layout->addWidget(precipitation_qcb);
    left_meteo_layout->addWidget(cloudcover_lbl);
    left_meteo_layout->addWidget(cloudcover_qcb);
    //компоновка правой части
    temperature_layout->addWidget(temperature_format_lbl);
    temperature_layout->addWidget(temperature_spb);
    humidity_layout->addWidget(humidity_format_lbl);
    humidity_layout->addWidget(humidity_spb);
    pressure_layout->addWidget(pressure_format_lbl);
    pressure_layout->addWidget(atmosphere_pressure_spb);
    right_meteo_layout->addWidget(temperature_lbl);
    right_meteo_layout->addLayout(temperature_layout);
    right_meteo_layout->addWidget(humidity_lbl);
    right_meteo_layout->addLayout(humidity_layout);
    right_meteo_layout->addWidget(atmosphere_pressure_lbl);
    right_meteo_layout->addLayout(pressure_layout);
    //выравнивание текста в заголовке GroupBox
    top_middle_grb_meteo->setAlignment(Qt::RightEdge);
    //настройка пунктов ComboBoxes
    meteoconditions_qcb->addItem("Ясно");
    meteoconditions_qcb->addItem("Солнечно");
    meteoconditions_qcb->addItem("Ясно-дымка");
    meteoconditions_qcb->addItem("Солнечно-дымка");
    meteoconditions_qcb->addItem("Пасмурно");
    precipitation_qcb->addItem("Нет");
    precipitation_qcb->addItem("Слабый туман");
    precipitation_qcb->addItem("Средний туман");
    precipitation_qcb->addItem("Сильный туман");
    precipitation_qcb->addItem("Слабый дождь");
    precipitation_qcb->addItem("Средний дождь");
    precipitation_qcb->addItem("Ливень");
    precipitation_qcb->addItem("Слабый снег");
    precipitation_qcb->addItem("Средний снег");
    precipitation_qcb->addItem("Сильный снег");
    precipitation_qcb->addItem("Град");
    cloudcover_qcb->addItem("Нет");
    cloudcover_qcb->addItem("Слабая");
    cloudcover_qcb->addItem("Сильная");
    cloudcover_qcb->addItem("Сплошная");
    //---------------------------
    left_meteo_layout->addStretch(1);
    right_meteo_layout->addStretch(1);
    top_middle_layout->addLayout(left_meteo_layout);
    top_middle_layout->addLayout(right_meteo_layout);
    top_middle_grb_meteo->setLayout(top_middle_layout);
    return top_middle_grb_meteo;
}

QGroupBox *GUI::createTopMiddleSubGroup_window()
{
    subLayout = new QHBoxLayout();
    top_middle_grb_window = new QGroupBox(tr("Окно-сюжет"));
    top_middle_grb_window->setAlignment(Qt::RightEdge);
    window_layout = new QHBoxLayout();
    plot_layout = new QHBoxLayout();
    window_lbl = new QLabel("Окно:");
    window_spb = new QSpinBox();
    plot_lbl = new QLabel("Сюжет:");
    plot_spb = new QSpinBox();
    window_spb->setRange(1,2);
    plot_spb->setRange(1,10);
    window_layout->addWidget(window_lbl);
    window_layout->addWidget(window_spb);
    plot_layout->addWidget(plot_lbl);
    plot_layout->addWidget(plot_spb);
    /*window_plot_layout*/subLayout->addLayout(window_layout);
    /*window_plot_layout*/subLayout->addLayout(plot_layout);
    //top_middle_grb_window->setLayout(window_plot_layout);
    top_middle_grb_window->setLayout(subLayout);
    return top_middle_grb_window;
}
//-----------формирование правого верхнего------------
QGroupBox *GUI::createTopRightGroup()
{
    illuminate_layout = new QHBoxLayout();
    illuminate_info_lbl = new QLabel("E,Лк");
    top_right_grb = new QGroupBox(tr("Условия видимости"));
    top_right_layout = new QHBoxLayout();
    left_visibility_layout = new QVBoxLayout();
    right_visibility_layout = new QVBoxLayout();
    range_visibility_lbl = new QLabel("Дальность видимости");
    range_visibility_qcb = new QComboBox();
    illumination_level_lbl = new QLabel("Уровень освещенности");
    illumination_level_spb = new QSpinBox();
    sunrise_lbl = new QLabel("Восход");
    sunrise_ted = new QTimeEdit(QTime::currentTime());
    sunset_lbl = new QLabel("Закат");
    sunset_ted = new QTimeEdit(QTime::currentTime());
    half_of_day_lbl = new QLabel("Астрономический\nполдень");
    half_of_day_ted = new QTimeEdit(QTime::currentTime());
    meteo_visibility_range_layout = new QHBoxLayout();
    meteo_visibility_range_lbl = new QLabel("Метеоролоческая\nдальность видимости");
    meteo_visibility_format_lbl = new QLabel("S,Км");
    meteo_visibility_range_spb = new QDoubleSpinBox();
    //----------------настройка SpinBoxes
                                                //настройка уровня освещенности
    illumination_level_spb->setRange(0,100000);   //100 000 - Наибольшая солнечная освещенность при чистом небе, Wikipedia
    illumination_level_spb->setValue(0);
    //----------------настройка TimeEdits
    sunrise_ted->setTimeRange(QTime(0,0,0,0),QTime(23,59,0,0));
    sunset_ted->setTimeRange(QTime(0,0,0,0),QTime(23,59,0,0));
    half_of_day_ted->setTimeRange(QTime(0,0,0,0),QTime(23,59,0,0));
    //выравнивание текста в заголовке GroupBox
    top_right_grb->setAlignment(Qt::RightEdge);
    //----------------настройка пунктов ComboBoxes
    range_visibility_qcb->addItem("1-й отметки (100 м)");
    range_visibility_qcb->addItem("2-й отметки (200 м)");
    range_visibility_qcb->addItem("3-й отметки (500 м)");
    range_visibility_qcb->addItem("4-й отметки (1 км)");
    range_visibility_qcb->addItem("5-й отметки (10 км)");
    range_visibility_qcb->addItem("6-й отметки (20 км)");
    //----------------компоновка----------------------------
    meteo_visibility_range_layout->addWidget(meteo_visibility_format_lbl);
    meteo_visibility_range_layout->addWidget(meteo_visibility_range_spb);
    illuminate_layout->addWidget(illuminate_info_lbl);
    illuminate_layout->addWidget(illumination_level_spb);
    left_visibility_layout->addWidget(range_visibility_lbl);
    left_visibility_layout->addWidget(range_visibility_qcb);
    left_visibility_layout->addWidget(illumination_level_lbl);
    left_visibility_layout->addLayout(illuminate_layout);
    left_visibility_layout->addWidget(meteo_visibility_range_lbl);
    left_visibility_layout->addLayout(meteo_visibility_range_layout);

    right_visibility_layout->addWidget(sunrise_lbl);
    right_visibility_layout->addWidget(sunrise_ted);
    right_visibility_layout->addWidget(sunset_lbl);
    right_visibility_layout->addWidget(sunset_ted);
    right_visibility_layout->addWidget(half_of_day_lbl);
    right_visibility_layout->addWidget(half_of_day_ted);
    left_visibility_layout->addStretch(1);
    right_visibility_layout->addStretch(1);
    top_right_layout->addLayout(left_visibility_layout);
    top_right_layout->addLayout(right_visibility_layout);
    top_right_grb->setLayout(top_right_layout);
    return top_right_grb;
}

QGroupBox *GUI::createMiddleGroup()
{
    mid_grb = new QGroupBox(tr("Служебная информация"));
    service_info_lbl = new QLabel("Пункт плана");
    notes_lbl = new QLabel("Служебные отметки");
    mid_mid_layout = new QHBoxLayout();//такое название, т.к mid_layout уже есть
    left_service_layout = new QVBoxLayout();
    right_service_layout = new QVBoxLayout();
    clause_of_plan_spb = new QSpinBox();
    notes_qte = new QTextEdit();
    //настройка параметров QTextEdit
    notes_qte->setFixedSize(700,100);
    notes_qte->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    notes_qte->document()->setMaximumBlockCount(300);
    //настройка SpinBoxes
    clause_of_plan_spb->setMinimum(1);
    //выравнивание текста в заголовке groupBox
    mid_grb->setAlignment(Qt::RightEdge);
    //компоновка------------------------------

    left_service_layout->addWidget(service_info_lbl);
    left_service_layout->addWidget(clause_of_plan_spb);
    right_service_layout->addWidget(notes_lbl);
    right_service_layout->addWidget(notes_qte);
    mid_mid_layout->addLayout(left_service_layout);
    mid_mid_layout->addLayout(right_service_layout);
    mid_grb->setLayout(mid_mid_layout);
    left_service_layout->addStretch(1);
    return mid_grb;
}

QGroupBox *GUI::createDownGroup()
{
    down_grb = new QGroupBox(/*tr("Пользовательский ввод")*/);
    input_btn = new QPushButton("ОК");
    down_down_layout = new QHBoxLayout();//down_layout уже есть
    realtime_chb = new QCheckBox();
    frames_rate_lbl = new QLabel("Кадров/мин");
    realtime_lbl = new QLabel("Реальное время");
    frames_rate_spb = new QSpinBox();
    //----настройка SpinBox
    frames_rate_spb->setValue(60);
    frames_rate_spb->setMaximum(5000);
    //----выравнивание текста в заголовке GroupBox
    down_grb->setAlignment(Qt::RightEdge);
    //-----компоновка
    down_down_layout->addSpacing(330);
    down_down_layout->addWidget(input_btn);
    down_down_layout->addStretch(1);
    down_down_layout->addWidget(frames_rate_lbl);
    down_down_layout->addWidget(frames_rate_spb);
    down_down_layout->addWidget(realtime_lbl);
    down_down_layout->addWidget(realtime_chb);
    down_grb->setLayout(down_down_layout);
    return down_grb;
}

GUI::b_range GUI::get_range(int a)
{
    if (in_between(a, -30, -15))
    {
        qDebug() << "-30..-15";
        return B_RANGE_30_15_UNDER_ZERO;
    }

    else
        if (in_between(a, -15, 0))
        {
        qDebug() << "-15..0";
        return B_RANGE_15_0_UNDER_ZERO;
        }

        else
            if(in_between(a, 0, 10))
            {
            qDebug() << "0..+10";
            return B_RANGE_0_10_ABOVE_ZERO;
            }

            else
                if(in_between(a, 10, 20))
                {
                qDebug() << "10..20";
                return B_RANGE_10_20_ABOVE_ZERO;
                }

                else
                    if(in_between(a,20,40))
                    {
                    qDebug() << "20..40";
                    return B_RANGE_20_40_ABOVE_ZERO;
                    }

                    else
                        return B_RANGE_OUT_OF_RANGE;
}

QString GUI::parse_double(QString str)
{
    str.replace(".", ",");
    //qDebug() << "str" << str << "parsed";
    return str;
}

void GUI::type_fields_from_json()
{
    QString str;
    QStringList qsl = json_saver.get_json();
    //поля формы заполняются в посделовательности
//    1. ПОЛЕ "ВРЕМЯ ГОДА"
        //здесь ничего не делается, это поле завязано на виджет календаря
//    2. ПОЛЕ "ВРЕМЯ СУТОК"					times_of_day_qcb
    str = qsl[1];
    times_of_day_qcb->setCurrentText(str);

//    3. ПОЛЕ "УСЛОВИЯ"					meteoconditions_qcb
    str = qsl[2];
    meteoconditions_qcb->setCurrentText(str);
//    4. ПОЛЕ "ОСАДКИ"					precipitation_qcb
    str = qsl[3];
    precipitation_qcb->setCurrentText(str);
//    5. ПОЛЕ "ОБЛАЧНОСТЬ"					cloudcover_qcb
    str = qsl[4];
    cloudcover_qcb->setCurrentText(str);

//    6. ПОЛЕ "ДАЛЬНОСТЬ ВИДИМОСТИ" (ПО ОТМЕТКАМ)		range_visibility_qcb
    str = qsl[5];
    range_visibility_qcb->setCurrentText(str);

//    7. ПОЛЕ "МЕТЕОРОЛОГИЧЕСКАЯ ДАЛЬНОСТЬ ВИДИМОСТИ"		meteo_visibility_range_spb
    str = qsl[6];
    meteo_visibility_range_spb->setValue(str.toDouble());
//    8. ПОЛЕ "ДИАПАЗОН ТЕМПЕРАТУРЫ"
    //здесь ничего не делается

//    9. ПОЛЕ "ТЕМПЕРАТУРА":					temperature_spb
    str = qsl[8];
    temperature_spb->setValue(str.toInt());

//    10. ПОЛЕ "ВЛАЖНОСТЬ"					humidity_spb
    str = qsl[9];
    humidity_spb->setValue(str.toInt());

//    11. ПОЛЕ "АТМОСФЕРНОЕ ДАВЛЕНИЕ"				atmosphere_pressure_spb
    str = qsl[10];
    atmosphere_pressure_spb->setValue(str.toInt());

//    12. ПОЛЕ "УРОВЕНЬ ОСВЕЩЕННОСТИ"				illumination_level_spb
    str = qsl[11];
    illumination_level_spb->setValue(str.toInt());

//    13. ПОЛЕ "ВРЕМЯ ВОСХОДА"				sunrise_ted
    str = qsl[12];
    sunrise_ted->setTime(QTime::fromString(str));

//    14. ПОЛЕ "ВРЕМЯ ЗАКАТА"					sunset_ted
    str = qsl[13];
    sunset_ted->setTime(QTime::fromString(str));
//    15. ПОЛЕ "АСТРОНОМИЧЕСКИЙ ПОЛДЕНЬ"			half_of_day_ted
    str = qsl[14];
    half_of_day_ted->setTime(QTime::fromString(str));
    //16. ПОЛЕ "ОКНО-СЮЖЕТ"
    unparse_plot_num(qsl[15]);

//    17. ПОЛЕ "ПУНКТ ПЛАНА"					clause_of_plan_spb
    str = qsl[16];
    clause_of_plan_spb->setValue(str.toInt());
//    18. ПОЛЕ "СЛУЖЕБНЫЕ ОТМЕТКИ"				notes_qte
    str = qsl[17];
    notes_qte->setText(str);
    //отладка
//    for(int i = 0; i<qsl.length(); i++){
//        qDebug() << qsl.at(i);
    //    }
}

QString GUI::parse_plot_num()
{
    QString str = QString::number(window_spb->value());
    str.append(",");
    str.append(QString::number(plot_spb->value()));
    qDebug() << "plot-window str = " << str;
    return str;
}

void GUI::unparse_plot_num(QString str)
{
    QStringList list = str.split(",");
    QString str0 = list.at(0);
    QString str1 = list.at(1);
    window_spb->setValue(str0.toInt());
    plot_spb->setValue(str1.toInt());

}

void GUI::on_Input_pressed()
{
    QString str;
    form.temperature = QString::number(temperature_spb->value());
    form.humidity = QString::number(humidity_spb->value());
    form.pressure = QString::number(atmosphere_pressure_spb->value());
    form.illumination = QString::number(illumination_level_spb->value());
    form.sunrise = sunrise_ted->time().toString();
    form.sunset = sunset_ted->time().toString();
    form.noon = half_of_day_ted->time().toString();
    form.meteo_visibility_range_level = QString::number(meteo_visibility_range_spb->value());
    form.clause = QString::number(clause_of_plan_spb->value());

    form.notes = notes_qte->toPlainText();
    notes_qte->clear();
    //emit data_Ready();//в этом сигнале надо передать объект опросника..нужен ли???

    //последовательно переносится информация из полей структуры в xls файл
    //только не надо менять последовательность вызова методов
                                                //xlslog->set_BasicField(...);
                                                //xlslog->writeBasicField_toDataLine();
                                                //xlslog->writeDataLine_toBlock();
                                                //xlslog->makeTable();
                                                //report_tbv->setModel(xlslog->getModel());
                                                //xlslog->writeDataBlock_toFile();


    //--------------------------------------------ДАННЫЕ С ФОРМЫ ПОСЛЕДОВАТЕЛЬНО ПЕРЕНОСЯТСЯ В EXCEL-ФАЙЛ + В JSON
    //                              1. ПОЛЕ "ВРЕМЯ ГОДА"
    xlslog->set_BasicField(form.time_of_year);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.time_of_year);
    //                              2. ПОЛЕ "ВРЕМЯ СУТОК"
    xlslog->set_BasicField(form.time_of_day);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.time_of_day);
    //                              3. ПОЛЕ "УСЛОВИЯ"
    xlslog->set_BasicField(form.meteo_conditions);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.meteo_conditions);
    //                              4. ПОЛЕ "ОСАДКИ"
    xlslog->set_BasicField(form.precipitation_level);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.precipitation_level);
    //                              5. ПОЛЕ "ОБЛАЧНОСТЬ"
    xlslog->set_BasicField(form.cloud_cover);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.cloud_cover);
    //                              6. ПОЛЕ "ДАЛЬНОСТЬ ВИДИМОСТИ" (ПО ОТМЕТКАМ)
    xlslog->set_BasicField(form.visibility_range_level);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.visibility_range_level);
    //                              7. ПОЛЕ "МЕТЕОРОЛОГИЧЕСКАЯ ДАЛЬНОСТЬ ВИДИМОСТИ"
    //-------------------------сначала сделать подмену разделителя, потом добавлять в excel-------------
    str = parse_double(form.meteo_visibility_range_level);
    //--------------------------------------------------------------------------------------------------
    xlslog->set_BasicField(str);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.meteo_visibility_range_level);
    //                              8. ПОЛЕ "ДИАПАЗОН ТЕМПЕРАТУРЫ"
    xlslog->set_BasicField(form.thermal_diapasone);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.thermal_diapasone);
    //                              9. ПОЛЕ "ТЕМПЕРАТУРА"
    //xlslog->set_BasicField(QString::number(form.temperature));
    xlslog->set_BasicField(form.temperature);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.temperature);
    //                              10. ПОЛЕ "ВЛАЖНОСТЬ"
    //xlslog->set_BasicField(QString::number(form.humidity));
    xlslog->set_BasicField(form.humidity);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.humidity);
    //                              11. ПОЛЕ "АТМОСФЕРНОЕ ДАВЛЕНИЕ"
    //xlslog->set_BasicField(QString::number(form.pressure));
    xlslog->set_BasicField(form.pressure);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.pressure);
    //                              12. ПОЛЕ "УРОВЕНЬ ОСВЕЩЕННОСТИ"
    //xlslog->set_BasicField(QString::number(form.illumination));
    xlslog->set_BasicField(form.illumination);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.illumination);
    //                              13. ПОЛЕ "ВРЕМЯ ВОСХОДА"
    xlslog->set_BasicField(form.sunrise);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.sunrise);
    //                              14. ПОЛЕ "ВРЕМЯ ЗАКАТА"
    xlslog->set_BasicField(form.sunset);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.sunset);
    //                              15. ПОЛЕ "АСТРОНОМИЧЕСКИЙ ПОЛДЕНЬ"
    xlslog->set_BasicField(form.noon);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.noon);
    //                              16. ПОЛЕ "№ сюжета"
    xlslog->set_BasicField(parse_plot_num());
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(parse_plot_num());

    //                              17. ПОЛЕ "ПУНКТ ПЛАНА"
    //xlslog->set_BasicField(QString::number(form.clause));
    xlslog->set_BasicField(form.clause);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.clause);
    //                              18. ПОЛЕ "СЛУЖЕБНЫЕ ОТМЕТКИ"
    xlslog->set_BasicField(form.notes);
    xlslog->writeBasicField_toDataLine();
    json_saver.add_string_to_json(form.notes);

    //-------------------------------EXCEL-------------------
    xlslog->writeDataLine_toBlock();

    xlslog->makeTable();
    report_tbv->setModel(xlslog->getModel());

    xlslog->writeDataBlock_toFile();
    //-------------------------------JSON--------------------
    json_saver.write_to_json();

    //---------только для отладки------

//    qDebug() << "Время года:" << form.time_of_year;              //время года
//    qDebug() << "Время суток:" << form.time_of_day;               //время суток
//    qDebug() << "Метеоусловия:" << form.meteo_conditions;          //условия (ясно/солнечно и т.д.)
//    qDebug() << "Осадки:" << form.precipitation_level;       //осадки (туман/дождь/снег и т.д.)
//    qDebug() << "Облачность:" << form.cloud_cover;               //облачность(нет/слабая/сильная и т.д.)
//    qDebug() << "Дальность видимости" << form.visibility_range_level;    //дальность видимости
//    qDebug() << "Метеорологическая дальность видимости" << form.meteo_visibility_range_level    //Метеорологическая дальность видимости
//    qDebug() << "Температурный диапазон" << form.thermal_diapasone;         //температурный диапазон
//    qDebug() << "Температура" << form.temperature;                   //температура
//    qDebug() << "Влажность" << form.humidity;                      //влажность
//    qDebug() << "Давление" << form.pressure;                      //давление
//    qDebug() << "Освещенность" << form.illumination;                  //уровень освещенности
//    qDebug() << "Восход" << form.sunrise;                   //восход
//    qDebug() << "Закат" << form.sunset;                    //закат
//    qDebug() << "Полдень" << form.noon;                      //полдень
//    qDebug() << "Пункт плана" << form.clause;                    //пункт плана
//    qDebug() << "Отметки" << form.notes;                     //служебные отметки
}

void GUI::on_Time_of_day_select()
{
    form.time_of_day = time_of_day_vct.at(times_of_day_qcb->currentIndex());
    qDebug() << form.time_of_day;
}

void GUI::on_Conditions_select()
{
    form.meteo_conditions = conditions_vct.at(meteoconditions_qcb->currentIndex());
//    qDebug() << "Meteoconditions is" << form.meteo_conditions;
}

void GUI::on_Precipitation_select()
{
    form.precipitation_level = precipitation_vct.at(precipitation_qcb->currentIndex());
//    qDebug() << "Precipitation" << form.precipitation_level;
}

void GUI::on_Cloudcover_select()
{
    form.cloud_cover = cloud_cover_vct.at(cloudcover_qcb->currentIndex());
//    qDebug() << "Cloudcover" << form.cloud_cover;
}

void GUI::on_RangeVisibility_select()
{
    form.visibility_range_level = visibility_distance_vct.at(range_visibility_qcb->currentIndex());
//    qDebug() << "RangeVisibility" << form.visibility_range_level;
}

void GUI::on_Temperature_select()
{
    switch (this->get_range(temperature_spb->value())) {
    case B_RANGE_30_15_UNDER_ZERO:
        form.thermal_diapasone = "-30…-15";
        break;
    case B_RANGE_15_0_UNDER_ZERO:
        form.thermal_diapasone = "-15…0";
        break;
    case B_RANGE_0_10_ABOVE_ZERO:
        form.thermal_diapasone = "0…+10";
        break;
    case B_RANGE_10_20_ABOVE_ZERO:
        form.thermal_diapasone = "+10…+20";
        break;
    case B_RANGE_20_40_ABOVE_ZERO:
        form.thermal_diapasone = "+20…+40";
        break;
    case B_RANGE_OUT_OF_RANGE:
        form.thermal_diapasone = "Значение за пределами диапазона -30...+40";
        break;
    default:
        break;
    }
    form.temperature = temperature_spb->value();
}

void GUI::on_CalendarDate_select()
{
    int month = 0;
    qDebug() << "Date selected:"<<calendar->selectedDate();
    current_date = calendar->selectedDate().toString();
    month = calendar->monthShown();
    if((month==1)||(month==12)||(month == 2))
    {
        form.time_of_year = "Зима";
        qDebug() << "Winter";
    }

    if((month==3)||(month==4)||(month==5))
    {
        form.time_of_year = "Весна";
        qDebug() << "Spring";
    }

    if((month==6)||(month==7)||(month==8))
    {
        form.time_of_year = "Лето";
        qDebug() << "Summer";
    }

    if((month==9)||(month==10)||(month==11))
    {
        qDebug() << "Autumn";
        form.time_of_year = "Осень";
    }
}

void GUI::on_TimerTick()
{
    current_time = QDateTime::currentDateTime().toString("hh:mm:ss");//поменять формат - указать аргумент "hh:mm:ss"
    current_date_and_time_lbl->setText("Дата и время наблюдения: " + current_date + " " + current_time);
}

void GUI::on_Real_Checked()
{
    if(realtime_chb->isChecked()){
        frames_rate_spb->setValue(1500);
    }
    else{
        frames_rate_spb->setValue(60);
    }
}



//template<typename T>
bool GUI::in_between(int a, int min, int max)
{
    return a >= min && a <= max;
}
