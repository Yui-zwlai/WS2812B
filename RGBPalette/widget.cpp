#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
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

   ui->horizontalSlider_Mode->setEnabled(false);
   ui->horizontalSlider_R->setEnabled(false);
   ui->horizontalSlider_G->setEnabled(false);
   ui->horizontalSlider_B->setEnabled(false);
   ui->horizontalSlider_V->setEnabled(false);
   ui->pushButton->setEnabled(false);

}

void Widget::ReadSerialData()
{
     //ui->textEdit->append(QString::fromLocal8Bit(m_serial->GetReadBuf()));
}

void Widget::TimerTimeOut()
{
    if(m_timer->isActive())//判断定时器是否运行
            m_timer->stop();   //停止定时器

    QStringList currentSerialList = m_serial->scanSerial();//获取串口列表

    if (serialStrList != currentSerialList)
    {
        for(QString str:serialStrList)
        {
            if(!currentSerialList.contains(str))//查找消失的串口
            {
                if(ui->SwitchButton->text() == tr("断开") && str == ui->SerialcomboBoxl->currentText())//消失的串口正在被打开
                    emit on_SwitchButton_clicked();
                break;
            }
        }
        serialStrList = currentSerialList;

        ui->SerialcomboBoxl->clear();//清除串口列表
        for(int i = 0; i < serialStrList.size(); i++)//将串口列表进行显示
        {
            ui->SerialcomboBoxl->addItem(serialStrList[i]);
        }

    }
    m_timer->start(2000);
}

Widget::~Widget()
{
    delete ui;
}

float Widget::min(float r, float g, float b)
{
  float m;

  m = r < g ? r : g;
  return (m < b ? m : b);
}

float Widget::max(float r, float g, float b)
{
  float m;

  m = r > g ? r : g;
  return (m > b ? m : b);
}

void Widget::rgb2hsv(int r, int g, int b, float *h, float *s, float *v)
{
    float red, green ,blue;
      float cmax, cmin, delta;

      red = (float)r/255;
      green = (float)g/255 ;
      blue = (float)b/255;

      cmax = max(red, green, blue);
      cmin = min(red, green, blue);
      delta = cmax - cmin;

      /* H */
      if(delta == 0)
      {
        *h = 0;
      }
      else
      {
        if(cmax == red)
        {
          if(green >= blue)
          {
            *h = 60 * ((green - blue) / delta);
          }
          else
          {
            *h = 60 * ((green - blue) / delta) + 360;
          }
        }
        else if(cmax == green)
        {
          *h = 60 * ((blue - red) / delta + 2);
        }
        else if(cmax == blue)
        {
          *h = 60 * ((red - green) / delta + 4);
        }
      }

      /* S */
      if(cmax == 0)
      {
        *s = 0;
      }
      else
      {
        *s = delta / cmax;
      }

      /* V */
      *v = cmax;
}

void Widget::hsv2rgb(float h, float s, float v, int *r, int *g, int *b)
{
    int hi = ((int)h / 60) % 6;
        float f = h / 60 - hi;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1- (1 - f) * s);

        switch (hi){
            case 0:
                *r =  255 * v;
                *g =  255 * t;
                *b =  255 * p;
                break;
            case 1:
                *r =  255 * q;
                *g =  255 * v;
                *b =  255 * p;
                break;
            case 2:
                *r =  255 * p;
                *g =  255 * v;
                *b =  255 * t;
                break;
            case 3:
                *r =  255 * p;
                *g =  255 * q;
                *b =  255 * v;
                break;
            case 4:
                *r =  255 * t;
                *g =  255 * p;
                *b =  255 * v;
                break;
            case 5:
                *r =  255 * v;
                *g =  255 * p;
                *b =  255 * q;
                break;
        }
}

void Widget::on_horizontalSlider_Mode_sliderMoved(int position)
{
    Mode = position;
    ui->label_Mode->setText("模式:" + QString::number(Mode,10));
    data[1] = Mode;
    m_serial->SendData(data);

}

void Widget::on_horizontalSlider_R_sliderMoved(int position)
{
    R = position;
    ui->label_R->setText("R:" + QString::number(R,10));
    data[2] = R;
    m_serial->SendData(data);

    float h,s,v;
    rgb2hsv(R,G,B,&h,&s,&v);
    ui->horizontalSlider_V->setValue((int)(v*100));
    ui->label_V->setText("亮度:" + QString::number((int)(v*100),10));

}

void Widget::on_horizontalSlider_G_sliderMoved(int position)
{
    G = position;
    ui->label_G->setText("G:"+ QString::number(G,10));
    data[3] = G;
    m_serial->SendData(data);

    float h,s,v;
    rgb2hsv(R,G,B,&h,&s,&v);
    ui->horizontalSlider_V->setValue((int)(v*100));
    ui->label_V->setText("亮度:" + QString::number((int)(v*100),10));
}

void Widget::on_horizontalSlider_B_sliderMoved(int position)
{
    B = position;
    ui->label_B->setText("B:" + QString::number(B,10));
    data[4] = B;
    m_serial->SendData(data);

    float h,s,v;
    rgb2hsv(R,G,B,&h,&s,&v);
    ui->horizontalSlider_V->setValue((int)(v*100));
    ui->label_V->setText("亮度:" + QString::number((int)(v*100),10));
}

void Widget::on_horizontalSlider_V_sliderMoved(int position)
{
    float h,s,v;

    rgb2hsv(R,G,B,&h,&s,&v);
    v = (float)position/100.0;
    hsv2rgb(h,s,v,&R,&G,&B);
    data[2]=R;
    data[3]=G;
    data[4]=B;

    ui->horizontalSlider_R->setValue(R);
    ui->label_R->setText("R:" + QString::number(R,10));
    ui->horizontalSlider_G->setValue(G);
    ui->label_G->setText("G:" + QString::number(G,10));
    ui->horizontalSlider_B->setValue(B);
    ui->label_B->setText("B:" + QString::number(B,10));

    ui->label_V->setText("亮度:" + QString::number(position,10));

    m_serial->SendData(data);

}


void Widget::on_horizontalSlider_Mode_actionTriggered(int action)
{
    if(action == 3)
        emit on_horizontalSlider_Mode_sliderMoved(ui->horizontalSlider_Mode->value()+1);
    else if (action == 4)
        emit on_horizontalSlider_Mode_sliderMoved(ui->horizontalSlider_Mode->value()-1);
}


void Widget::on_horizontalSlider_R_actionTriggered(int action)
{
    if(action == 3)
        emit on_horizontalSlider_R_sliderMoved(ui->horizontalSlider_R->value()+1);
    else if (action == 4)
        emit on_horizontalSlider_R_sliderMoved(ui->horizontalSlider_R->value()-1);
}

void Widget::on_horizontalSlider_G_actionTriggered(int action)
{
    if(action == 3)
        emit on_horizontalSlider_G_sliderMoved(ui->horizontalSlider_G->value()+1);
    else if (action == 4)
        emit on_horizontalSlider_R_sliderMoved(ui->horizontalSlider_G->value()-1);
}

void Widget::on_horizontalSlider_B_actionTriggered(int action)
{
    if(action == 3)
        emit on_horizontalSlider_B_sliderMoved(ui->horizontalSlider_B->value()+1);
    else if (action == 4)
        emit on_horizontalSlider_R_sliderMoved(ui->horizontalSlider_B->value()-1);
}


void Widget::on_pushButton_clicked()
{

    QColor c = QColorDialog::getColor(QColor(R,G,B));
    if(c.isValid())
    {
        R = c.red();
        G = c.green();
        B = c.blue();
    }
    ui->horizontalSlider_R->setValue(R);
    ui->label_R->setText("R:" + QString::number(R,10));
    ui->horizontalSlider_G->setValue(G);
    ui->label_G->setText("G:" + QString::number(G,10));
    ui->horizontalSlider_B->setValue(B);
    ui->label_B->setText("B:" + QString::number(B,10));

    float h,s,v;

    rgb2hsv(R,G,B,&h,&s,&v);
    ui->horizontalSlider_V->setValue((int)(v*100));
    ui->label_V->setText("亮度:" + QString::number((int)(v*100),10));
    data[2]=R;
    data[3]=G;
    data[4]=B;
    m_serial->SendData(data);
}

void Widget::on_SwitchButton_clicked()
{
    if(ui->SerialcomboBoxl->currentText().isEmpty())
    {
        QString dlgTitle="警告";
        QString strInfo="请选择串口";
        QMessageBox::information(this, dlgTitle, strInfo,QMessageBox::Ok,QMessageBox::NoButton);
        return ;
    }

    if(ui->SwitchButton->text() == tr("连接"))
    {
        if(m_serial->Open(ui->SerialcomboBoxl->currentText(),"115200",0,3,0,0))
        {
            ui->SwitchButton->setText(tr("断开"));
            ui->SerialcomboBoxl->setEnabled(false);
            ui->horizontalSlider_Mode->setEnabled(true);
            ui->horizontalSlider_R->setEnabled(true);
            ui->horizontalSlider_G->setEnabled(true);
            ui->horizontalSlider_B->setEnabled(true);
            ui->horizontalSlider_V->setEnabled(true);
            ui->pushButton->setEnabled(true);
        }
        else
        {
            QString dlgTitle="错误";
            QString strInfo="串口被占用";
            QMessageBox::critical(this, dlgTitle, strInfo);
            return ;
        }
    }
    else
    {
        m_serial->Close();
        ui->SwitchButton->setText(tr("连接"));
        ui->SerialcomboBoxl->setEnabled(true);
        ui->horizontalSlider_Mode->setEnabled(false);
        ui->horizontalSlider_R->setEnabled(false);
        ui->horizontalSlider_G->setEnabled(false);
        ui->horizontalSlider_B->setEnabled(false);
        ui->horizontalSlider_V->setEnabled(false);
        ui->pushButton->setEnabled(false);
    }
}
