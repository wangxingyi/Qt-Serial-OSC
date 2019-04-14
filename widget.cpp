#include "widget.h"
#include "ui_widget.h"
#include "QDebug"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    Timer = new QTimer(this);

    ui->horizontalAMax->setRange(600,3000);
    ui->horizontalAMax->setValue(600);
    ui->horizontalAMin->setRange(0,600);
    ui->horizontalAMin->setValue(0);
    ui->horizontalVMax->setRange(500,800);
    ui->horizontalVMax->setValue(500);
    ui->horizontalVMin->setRange(0,500);
    ui->horizontalVMin->setValue(0);


    XData.resize(50);
    XData.value(0,1);

    YData.resize(50);
    Y1Data.resize(50);
    Y2Data.resize(50);
    Y3Data.resize(50);

    Plot = ui->widget;
    Plot->addGraph();
    Plot->addGraph();
    Plot->graph(0)->setPen(QPen(Qt::red));
    Plot->graph(1)->setPen(QPen(Qt::blue));
    Plot->xAxis->setLabel("mS");
    Plot->yAxis->setLabel("mA");
//    Plot->rescaleAxes(true);
    Plot->yAxis->setRange(0,700);
    Plot->xAxis->rescale(true);

    PlotV = ui->widget_C;
    PlotV->addGraph();
    PlotV->addGraph();
    PlotV->graph(0)->setPen(QPen(Qt::red));
    PlotV->graph(1)->setPen(QPen(Qt::blue));
    PlotV->xAxis->setLabel("mS");
    PlotV->yAxis->setLabel("Count");
    PlotV->yAxis->setRange(0,700);
    PlotV->xAxis->rescale(true);

    GraphShow();
    InitPort();
    QTimer::singleShot(80,this,&Widget::ReadPort);
    Clk = 0;

}

Widget::~Widget()
{
    delete ui;
}
/* 来自下位机的数据格式
 * "Cur_I:#%d#mA=Cur_I1:#%d#mA + Cur_I2:#%d#mA\r\n"
 * "PID_V_Input: #%f#V,Ratio=%f\r\n"
 * "Cur_I_Set=%f Control_Ratio=%f\r\n"
 * "CH1:#%d#,CH2:#%d#\r\n"
 */
void Widget::GetData(QString str)
{
    if(str.isEmpty())
        return;
    if(str.section("#",0,0) != "Cur_I:")
        return;
    double num = str.section("#",3,3).toDouble();
    double num1 = str.section("#",5,5).toDouble();
    if(num <= 0 || num1 <= 0)
        return ;
    XData.pop_front();
    XData.push_back(Clk++);

    YData.pop_front();
    YData.push_back(num);

    Y1Data.pop_front();
    Y1Data.push_back(num1);

    num = str.section("#",9,9).toDouble();
    num1 = str.section("#",11,11).toDouble();
    if(num <= 100 || num1 <= 100)
        return ;
    Y2Data.pop_front();
    Y2Data.push_back(num);

    Y3Data.pop_front();
    Y3Data.push_back(num1);
}

void Widget::GraphShow()
{
    Plot->graph(0)->setData(XData,YData);
    Plot->graph(1)->setData(XData,Y1Data);
    Plot->yAxis->setRange(Slither_aMin,Slither_aMax);
    Plot->xAxis->rescale(true);
    Plot->replot();

    PlotV->graph(0)->setData(XData,Y2Data);
    PlotV->graph(1)->setData(XData,Y3Data);
    PlotV->yAxis->setRange(Slither_vMin,Slither_vMax);
    PlotV->xAxis->rescale(true);
    PlotV->replot();
}

void Widget::InitPort()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name :"<<info.portName();
        qDebug() << "Description: "<<info.description();
        qDebug() << "Manufacturer: "<<info.manufacturer();
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->comboBoxCom->addItem(info.portName());
            serial.close();
        }
    }
}

void Widget::on_pushButtonS_clicked()
{
    if(ui->pushButtonS->text() == "开启串口")
    {
        SerialPort.setPortName(ui->comboBoxCom->currentText());
        if(SerialPort.open(QIODevice::ReadOnly))
        {
            SerialPort.setBaudRate(ui->comboBoxBaut->currentText().toInt());
            SerialPort.setDataBits(QSerialPort::Data8);
            SerialPort.setParity(QSerialPort::NoParity);
            SerialPort.setFlowControl(QSerialPort::NoFlowControl);
            SerialPort.setStopBits(QSerialPort::OneStop);

            ui->pushButtonS->setText("关闭串口");
        }
        else{
            qDebug() << "串口没有打开";
            return;
        }
    }else if(ui->pushButtonS->text() == "关闭串口")
        {
        SerialPort.close();
        ui->pushButtonS->setText("开启串口");
    }
    else{
        qDebug() <<"无效操作";
    }
}

void Widget::on_pushButtonCR_clicked()
{
    ui->comboBoxCom->clear();
    InitPort();

}
void Widget::ReadPort()
{
    QTimer::singleShot(40,this,&Widget::ReadPort);
    if(SerialPort.isOpen() == false)
        return;
    QByteArray temp = SerialPort.readAll();
    if(temp.isEmpty())
        return;
    ui->textEditR->append(QString(temp));
    GetData(QString(temp));
    GraphShow();
}

void Widget::on_horizontalVMin_sliderMoved(int position)
{
    Slither_vMin = position;
}

void Widget::on_horizontalVMax_sliderMoved(int position)
{
    Slither_vMax = position;
}


void Widget::on_horizontalAMax_sliderMoved(int position)
{
    Slither_aMax = position;
}
void Widget::on_horizontalAMin_sliderMoved(int position)
{
    Slither_aMin = position;
}
void Widget::on_pushButtonCW_clicked()
{
    ui->textEditR->clear();
    Clk = 0;
}























