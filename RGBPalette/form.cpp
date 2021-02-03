#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    setWindowTitle("RGB调色器");
    //setMaximumSize(QSize(400,400));
    setMinimumSize(QSize(472,209));
    setWindowIcon(QIcon(":/img/bitbug_favicon.ico"));

    m_serial = new Serial;
    connect(m_serial,SIGNAL(readSignal()),this,SLOT(ReadSerialData()));

    m_timer = new QTimer;
    m_timer->setSingleShot(false);//设置定时器是否为单次触发。默认为 false 多次触发s
    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeOut()));//定时器触发信号槽
    m_timer->start(100);

   flag  = true;
   R = 0;
   G = 0;
   B = 0;
   Mode = 0x00;
   data.resize(6);
   data[0] = Start;;
   data[1] = Mode;
   data[2] = R;
   data[3] = G;
   data[4] = B;
   data[5] = Stop;


}

Form::~Form()
{
    delete ui;
}



