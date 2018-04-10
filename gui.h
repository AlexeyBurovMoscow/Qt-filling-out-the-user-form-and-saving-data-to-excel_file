#ifndef GUI_H
#define GUI_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QCalendarWidget>
#include <QTimeEdit>
#include <QComboBox>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QSpinBox>
#include <QTextEdit>
#include <QTimer>
#include <QTableView>
#include "xlslogger.h"
#include "jsonsaver.h"
#include <QObject>
#include <QDebug>

class XLSlogger;
class GUI: public QWidget
{
    Q_OBJECT
public:
    explicit GUI(QWidget *parent = 0);
    ~GUI();

    void createGUI();

private:
    //Визуальные компоненты. Слои
    QVBoxLayout * mainlayout;       //основной слой, на который проецируется все окно
    //Слои поменьше
    QHBoxLayout * top_layout;       //в этом слое группы "Время наблюдения", "Метеоусловия", "Видимость"
    QHBoxLayout * mid_layout;       //в этом слое группа "Служебная информация"
    QHBoxLayout * down_layout;      //в этом слое группа "Ввод пользователя"...одна кнопка "Ввод"
    //Разбивка средней группы в top_layout`е для добавления группы с окном и сюжетом
    QVBoxLayout * window_plot_layout;

    //                  ТАЙМЕРs
    QTimer * timer;

    //-----------------ОСНОВНЫЕ ДАННЫЕ-----------------------

    //структура для хранения данных
    struct  Polling
    {
             QString time_of_year = "Лето";                             //время года
             QString time_of_day = "День";                              //время суток
             QString meteo_conditions = "Ясно";                         //условия (ясно/солнечно и т.д.)
             QString precipitation_level = "Нет";                       //осадки (туман/дождь/снег и т.д.)
             QString cloud_cover = "Слабая";                            //облачность(нет/слабая/сильная и т.д.)
             QString visibility_range_level = "6-й отметки (20 км)";    //дальность видимости
             /*double*/QString meteo_visibility_range_level = "20.5";                 //метеорологическая дальность видимости
             QString thermal_diapasone = "+20…+40";                     //температурный диапазон
             /*int*/QString temperature = "25";                                      //температура
             /*int*/QString humidity = "60";                                         //влажность
             /*int*/QString pressure = "746";                                        //давление
             /*int*/QString illumination = "500";                                    //уровень освещенности
             QString sunrise = "06:00";                                 //восход
             QString sunset = "23:00";                                  //закат
             QString noon = "12:00";                                    //полдень
             QString plot_num = "1.1";                                    //номер сюжета (складывается из значений формы "номер окна - номер сюжета")
             /*int*/QString clause = "1";                                          //пункт плана
             QString notes = "Какой прекрасный день!";                  //служебные отметки
    };
    //-----------------ПРОЧИЕ ДАННЫЕ----------------------
    //вспомогательные вектора, хранящие состояния ComboBoxes
    QVector<QString> time_of_day_vct = {"Ночь без луны", "Ночь с неполной луной", "Ночь-полнолуние", "Сумерки перед рассветом", "Сумерки перед закатом", "День"};
    QVector<QString> conditions_vct = {"Ясно", "Солнечно", "Ясно-дымка", "Солнечно-дымка", "Пасмурно"};
    QVector<QString> precipitation_vct = {"Нет", "Слабый туман", "Средний туман", "Сильный туман", "Слабый дождь", "Средний дождь", "Ливень", "Слабый снег", "Средний снег", "Сильный снег", "Град"};
    QVector<QString> cloud_cover_vct = {"Нет", "Слабая", "Сильная", "Сплошная"};
    QVector<QString> visibility_distance_vct = {"1-й отметки (100 м)", "2-й отметки (200 м)", "3-й отметки (500 м)", "4-й отметки (1 км)", "5-й отметки (10 км)", "6-й отметки (20 км)"};

    //объект- опросник
    Polling form;
    //таблица
    QTableView * report_tbv;
    //вспомогательные. участвуют в выводе на форму
    QString current_time;
    QString current_date;
    //excel - логгер
    XLSlogger * xlslog;
    //Reader-Writer JSON
    JSonSaver json_saver;

    //группы виджетов
    //                  ПАНЕЛИ С ЭЛЕМЕНТАМИ УПРАВЛЕНИЯ

    //методы, создающие группы виджетов c внутренними элементами
    QGroupBox *createTopLeftGroup();        //левый верхний GroupBox(Время наблюдения)
                //элементы внутри группы

                QGroupBox * top_left_grb;
                QVBoxLayout * top_left_layout;
                QVBoxLayout * calendar_layout;
                QVBoxLayout * date_and_time_layout;
                QHBoxLayout * header_layout;
                QCalendarWidget * calendar;
                QComboBox * times_of_day_qcb;
                QLabel * current_date_and_time_lbl;
                QLabel * time_of_day_lbl;
    QGroupBox *createTopMiddleSubGroup_meteo();      //средний верхний GroupBox(Метеоусловия)(средняя группа теперь разделена на 2 подгруппы - низ и верх)
                QHBoxLayout * temperature_layout;
                QHBoxLayout * humidity_layout;
                QHBoxLayout * pressure_layout;
                QLabel * temperature_format_lbl;
                QLabel * humidity_format_lbl;
                QLabel * pressure_format_lbl;
                QGroupBox * top_middle_grb_meteo;
                QLabel * meteoconditions_lbl;
                QLabel * precipitation_lbl;
                QLabel * cloudcover_lbl;
                QLabel * temperature_lbl;
                QLabel * humidity_lbl;
                QLabel * atmosphere_pressure_lbl;
                QComboBox * meteoconditions_qcb;
                QComboBox * precipitation_qcb;
                QComboBox * cloudcover_qcb;
                QSpinBox * temperature_spb;
                QSpinBox * humidity_spb;
                QSpinBox * atmosphere_pressure_spb;
                QVBoxLayout * left_meteo_layout;
                QVBoxLayout * right_meteo_layout;
                QHBoxLayout * top_middle_layout;
    QGroupBox * createTopMiddleSubGroup_window();   //ввел подгруппу с двумя полями: номер окна и номер сюжета
                QGroupBox * top_middle_grb_window;
                QHBoxLayout * subLayout;
                QHBoxLayout * window_layout;
                QHBoxLayout * plot_layout;
                QLabel * window_lbl;
                QSpinBox * window_spb;
                QLabel * plot_lbl;
                QSpinBox * plot_spb;
    QGroupBox *createTopRightGroup();       //правый верхний GroupBox(Условия видимости)
                QHBoxLayout * illuminate_layout;
                QHBoxLayout * meteo_visibility_range_layout;
                QLabel * meteo_visibility_range_lbl;
                QLabel * meteo_visibility_format_lbl;
                QDoubleSpinBox * meteo_visibility_range_spb;
                QLabel * illuminate_info_lbl;
                QGroupBox * top_right_grb;
                QLabel * visibility_range_lbl;
                QHBoxLayout * top_right_layout;
                QVBoxLayout * left_visibility_layout;
                QVBoxLayout * right_visibility_layout;
                QLabel * range_visibility_lbl;
                QComboBox * range_visibility_qcb;
                QLabel * illumination_level_lbl;
                QSpinBox * illumination_level_spb;
                QLabel * sunrise_lbl;
                QTimeEdit * sunrise_ted;
                QLabel * sunset_lbl;
                QTimeEdit * sunset_ted;
                QLabel * half_of_day_lbl;
                QTimeEdit * half_of_day_ted;

    QGroupBox *createMiddleGroup();         //средний GroupBox(Служебная информация)
                QGroupBox * mid_grb;
                QLabel * service_info_lbl;
                QLabel * notes_lbl;
                QHBoxLayout * mid_mid_layout;//такое название, т.к mid_layout уже есть
                QVBoxLayout * left_service_layout;
                QVBoxLayout * right_service_layout;
                QSpinBox * clause_of_plan_spb;
                QTextEdit * notes_qte;

    QGroupBox *createDownGroup();           //нижний GroupBox (Пользовательский ввод)
                QGroupBox * down_grb;
                QPushButton * input_btn;
                QCheckBox * realtime_chb;
                QLabel * frames_rate_lbl;
                QLabel * realtime_lbl;
                QSpinBox * frames_rate_spb;
                QHBoxLayout * down_down_layout;//down_layout уже есть

private:
                //--------------эта конструкция для анализа значения, полученного из поля ввода температуры. Входит ли в заданный диапазон
                //  -30 и меньше
                //  -30…-15
                //  -15…0
                //  0…+10
                //  +10…+20
                //  +20…+40
                // + 40 и больше
                enum b_range
                {
                    B_RANGE_30_15_UNDER_ZERO,
                    B_RANGE_15_0_UNDER_ZERO,
                    B_RANGE_0_10_ABOVE_ZERO,
                    B_RANGE_10_20_ABOVE_ZERO,
                    B_RANGE_20_40_ABOVE_ZERO,
                    B_RANGE_OUT_OF_RANGE
                };
                bool in_between(int a, int min, int max);
                b_range get_range(int a);
                QString parse_double(QString str); //костыльный метод, нужен чтобы в excel double внезапно не становился датой (смысл в подмене разделителя "точка" разделителем "запятая")
                void type_fields_from_json();
                QString parse_plot_num();           //собирает данные из spinboxes "номер окна, номер сюжета", в одно значение для передачи в элемент структуры "окно-сюжет"
                void unparse_plot_num(QString str);


signals:
                void data_Ready();

public slots:
                void on_Input_pressed();    //обработка нажатия на ОК
                void on_Time_of_day_select();//выбор времени суток
                void on_Conditions_select();//выбор метеоусловий
                void on_Precipitation_select();//осадки
                void on_Cloudcover_select();//облачность
                void on_RangeVisibility_select();//дальность видимости
                void on_Temperature_select();//на выбор температуры произвести анализ диапазона
                void on_CalendarDate_select();//при выборе даты в календаре
                void on_TimerTick();        //на каждом срабатывании таймера изменяется строка с текущим временем/датой
                void on_Real_Checked();     //отметка CheckBox "Реальное время"
};

#endif // GUI_H
