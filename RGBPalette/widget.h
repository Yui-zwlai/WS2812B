#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <serial.h>
#include <QDebug>
#include <stdint.h>
#include <QColorDialog.h>
#define cout qDebug() << "[" <<__FILE__ <<":"<<__LINE__ <<"]"

#define byte unsigned char
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void ReadSerialData();//读取从自定义串口类获得的数据
    void TimerTimeOut();

    void on_horizontalSlider_Mode_sliderMoved(int position);
    void on_horizontalSlider_R_sliderMoved(int position);
    void on_horizontalSlider_G_sliderMoved(int position);
    void on_horizontalSlider_B_sliderMoved(int position);

    void on_horizontalSlider_Mode_actionTriggered(int action);
    void on_horizontalSlider_R_actionTriggered(int action);
    void on_horizontalSlider_G_actionTriggered(int action);
    void on_horizontalSlider_B_actionTriggered(int action);

    void on_horizontalSlider_V_sliderMoved(int position);

    void on_pushButton_clicked();

    void on_SwitchButton_clicked();

private:
    Ui::Widget *ui;

    Serial *m_serial;
    QTimer * m_timer;

    bool flag;

    int R;
    int G;
    int B;

    char Mode;
    const char Start = 0x5B;
    const char Stop = 0x5D;
    QByteArray data;

    QStringList serialStrList;//用于存储串口列表

    float min(float r, float g, float b);
    float max(float r, float g, float b);
    void rgb2hsv(int r, int g, int b, float *h, float *s, float *v);
    void hsv2rgb(float h, float s, float v, int *r, int *g, int *b);
};

#endif // WIDGET_H
