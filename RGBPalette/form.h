#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QTimer>
#include <serial.h>
#include <QDebug>
#include <stdint.h>
#include <QColorDialog.h>
#define cout qDebug() << "[" <<__FILE__ <<":"<<__LINE__ <<"]"

#define byte unsigned char

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

private slots:





public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private:
    Ui::Form *ui;


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

#endif // FORM_H
