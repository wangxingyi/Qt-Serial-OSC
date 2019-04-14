#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include "qcustomplot.h"
#include <QVector>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void InitPort();
    void ReadPort();
    void GraphShow();
    void GetData(QString);
private slots:
    void on_pushButtonS_clicked();
    void on_pushButtonCW_clicked();

    void on_pushButtonCR_clicked();

    void on_horizontalAMax_sliderMoved(int position);

    void on_horizontalAMin_sliderMoved(int position);

    void on_horizontalVMin_sliderMoved(int position);

    void on_horizontalVMax_sliderPressed();

    void on_horizontalVMax_sliderMoved(int position);

private:
    Ui::Widget *ui;
    QTimer *Timer;
    QSerialPort SerialPort;
    QSerialPortInfo SerialPortInfo;
    int Clk ;

    QCustomPlot *Plot;
    QVector <double>    XData;
    QVector <double>    YData;
    QVector <double>    Y1Data;

    QCustomPlot *PlotV;
    QVector <double>    Y2Data;
    QVector <double>    Y3Data;

    qint32 Slither_aMax;
    qint32 Slither_aMin;
    qint32 Slither_vMax;
    qint32 Slither_vMin;
};

#endif // WIDGET_H










