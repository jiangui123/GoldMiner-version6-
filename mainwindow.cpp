#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<QDebug>
#include<QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(700, 600);
    setWindowTitle(tr("黄金矿工"));

    QPixmap pixmap(":/images/iii/ccc.png");
    QCursor cursor= QCursor(pixmap,-1,-1);
    setCursor(cursor);

    interval_angle=15;
    interval_miner=200;

    miner1=new Miner;
    miner2=new Miner;
    map=new Map;

    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::yellow);
    ui->label_final->setPalette(pal);
    //ui->lcdNumber->setPalette(pal);
    //ui->lcdNumber_sum->setPalette(pal);
   // ui->lcdNumber_speed->setPalette(pal);

    this->setStyleSheet("#MainWindow{border-image:url(:/images/iii/background.jpg);}");//设置背景图片
    pix1.load("://images//iii//hook2.png");//设置钩子，60*50
    pix2.load("://images//iii//hook2.png");

    timer_angle.setInterval(interval_angle);//设置触发间隔为15ms
    connect(&timer_angle,&QTimer::timeout,this,&MainWindow::rotate1);
    connect(&timer_angle,&QTimer::timeout,this,&MainWindow::rotate2);
    connect(&timer_angle,&QTimer::timeout,this,&MainWindow::ss1);
    connect(&timer_angle,&QTimer::timeout,this,&MainWindow::ss2);

    timer_s.setInterval(interval_miner);//s控制矿工的运动，，达到类似动画效果
    connect(&timer_s,&QTimer::timeout,this,&MainWindow::mm1);
    connect(&timer_s,&QTimer::timeout,this,&MainWindow::mm2);

    timer_clock.setInterval(1000);//触发间隔为1s，时间减少一类操作
    connect(&timer_clock,&QTimer::timeout,this,&MainWindow::timereduce);

    rad=180/3.1415926;//弧度
    angle_change_1=1;//角度转变量为1°
    angle_change_2=1;

    lable[0]=ui->label_1;   //小矿石
    lable[1]=ui->label_2;   //小矿石
    lable[2]=ui->label_3;   //岩石
    lable[3]=ui->label_4;   //大矿石
    lable[4]=ui->label_5;   //大矿石
    lable[5]=ui->label_6;   //小矿石
    lable[6]=ui->label_7;   //大矿石
    lable[7]=ui->label_8;   //小矿石
    lable[8]=ui->label_9;   //岩石
    lable[9]=ui->label_10;  //钻石
    lable[10]=ui->label_11; //加速道具  仅放、拉绳子速度翻3倍
    lable[11]=ui->label_12; //减速道具  所有运动(放绳拉绳角度变化)速度减半
    lable[12]=ui->label_13; //膜法时间
    lable[13]=ui->label_14; //随机物品

    init();
    do_stop();//暂停，跳出帮助菜单
    ui->label_pause->setVisible(false);//暂停图案
    ui->toolButton_ppause->setVisible(false);

    ui->label_begin->setVisible(true);
    ui->toolButton_start->setVisible(true);
}
void MainWindow::init()
{
    ui->label_pause->setVisible(false);//暂停图案
    ui->toolButton_ppause->setVisible(false);

    ui->label_finish->setVisible(false);//结束图案
    ui->toolButton_restart->setVisible(false);
    ui->toolButton_cancel->setVisible(false);

    ui->label_final->setVisible(false);
    ui->lcdNumber_speed->setVisible(false);

    miner1->init();//矿工初始化设置
    miner2->init();
    map->init();//地图初始化设置

    time_left=60;//剩余时间为60s
    time=time_up=time_down=0;
    k=1;
    for(int i=0;i<6;i++)
    {
        lable[i]->setVisible(true);
        lable[i]->move(map->get_point(i));
        if(i<2)
            lable[i]->resize(140,120);
        else
            lable[i]->resize(60,50);
    }
    for(int i=6;i<14;i++)//让随机矿石不可见
        lable[i]->setVisible(false);

    ui->toolButton1->setStyleSheet("#toolButton1{border-image:url(:/images/iii/1.png);}");//设置矿工
    ui->toolButton2->setStyleSheet("#toolButton2{border-image:url(:/images/iii/1.png);}");

    ui->lcdNumber->display(time_left);
    ui->lcdNumber_sum->display(miner1->get_sum()+miner2->get_sum());
    timer_angle.start();
    timer_clock.start();
    timer_s.start();
}

void MainWindow::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    int length1=miner1->get_length();
    int length2=miner2->get_length();
    if((miner1->isrotating()==false||miner1->isstopped())||(miner2->isrotating()==false||miner2->isstopped()))
    {
        if((miner1->isrotating()==false||miner1->isstopped())&&miner2->isrotating())
        {
            QPen pen1;
            pen1.setWidth(6);
            painter.setPen(pen1);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.drawLine(250,95,245-length1*sin(miner1->get_angle()/rad),105+length1*cos(miner1->get_angle()/rad));
            painter.drawPixmap(230-length1*sin(miner1->get_angle()/rad),95+length1*cos(miner1->get_angle()/rad),pix1);//绘图的起点

            painter.drawPixmap(430-20*sin(miner2->get_angle()/rad),95+20*cos(miner2->get_angle()/rad),pix2);//绘图的起点
            painter.drawLine(450,95,445-20*sin(miner2->get_angle()/rad),105+20*cos(miner2->get_angle()/rad));

        }

        if((miner2->isrotating()==false||miner2->isstopped())&&miner1->isrotating())
        {
            QPen pen2;
            pen2.setWidth(6);
            painter.setPen(pen2);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.drawLine(450,95,445-length2*sin(miner2->get_angle()/rad),105+length2*cos(miner2->get_angle()/rad));
            painter.drawPixmap(430-length2*sin(miner2->get_angle()/rad),95+length2*cos(miner2->get_angle()/rad),pix2);//绘图的起点
            painter.drawPixmap(230-20*sin(miner1->get_angle()/rad),95+20*cos(miner1->get_angle()/rad),pix2);//绘图的起点
            painter.drawLine(250,95,245-20*sin(miner1->get_angle()/rad),105+20*cos(miner1->get_angle()/rad));

        }
        if((miner1->isrotating()==false||miner1->isstopped())&&(miner2->isrotating()==false||miner2->isstopped()))
        {
            QPen pen2;
            pen2.setWidth(6);
            painter.setPen(pen2);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.drawLine(450,95,445-length2*sin(miner2->get_angle()/rad),105+length2*cos(miner2->get_angle()/rad));
            painter.drawPixmap(430-length2*sin(miner2->get_angle()/rad),95+length2*cos(miner2->get_angle()/rad),pix2);
            painter.drawLine(250,95,245-length1*sin(miner1->get_angle()/rad),105+length1*cos(miner1->get_angle()/rad));
            painter.drawPixmap(230-length1*sin(miner1->get_angle()/rad),95+length1*cos(miner1->get_angle()/rad),pix1);

        }
     }
    else
    {
       QPen pen2;
       pen2.setWidth(6);
       painter.setPen(pen2);
       painter.drawPixmap(230-20*sin(miner1->get_angle()/rad),95+20*cos(miner1->get_angle()/rad),pix1);//绘图的起点
       painter.drawLine(250,95,245-20*sin(miner1->get_angle()/rad),105+20*cos(miner1->get_angle()/rad));
       painter.drawPixmap(430-20*sin(miner2->get_angle()/rad),95+20*cos(miner2->get_angle()/rad),pix2);//绘图的起点
       painter.drawLine(450,95,445-20*sin(miner2->get_angle()/rad),105+20*cos(miner2->get_angle()/rad));


    }
    update();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    int key=e->key();
    if (!miner2->isstopped()&&key ==Qt::Key_Down) // 按向下方向键时
    {
        if(miner2->isrotating())//如果正在旋转
        {
            //停止旋转，进行下拉操作
            miner2->set_is_rotating(false);
        }
    }
    if (!miner1->isstopped()&&key ==Qt::Key_S) // 按向S键时
    {
        if(miner1->isrotating())//如果正在旋转
        {
            //停止旋转，进行下拉操作
            miner1->set_is_rotating(false);
        }
    }
    if(key==Qt::Key_Space)//暂停功能
    {
        if(miner1->isstopped()&&miner2->isstopped()&&time_left>0)
            resume();
        else
            do_stop();
    }
}
bool MainWindow::detection1()//碰撞和越界检测
{
    //定义临时变量angle1和length1
    int angle1=miner1->get_angle();
    int length1=miner1->get_length();

    if(fabs(angle1)>45){
        if(length1>(350/(sin(fabs(angle1/rad)))))
             return true;
    }
    else if(length1>(500/(cos(angle1/rad))))
             return true;
    if(map->detection(250-length1*sin(angle1/rad),95+length1*cos(angle1/rad)))//如果map.碰撞检测返回为真
    {
        miner1->set_is_grab(true);
        return true;
    }
    return false;
}
bool MainWindow::detection2()//碰撞和越界检测
{
    //定义临时变量angle2和length2
    int angle2=miner2->get_angle();
    int length2=miner2->get_length();

    if(fabs(angle2)>45){
        if(length2>(350/(sin(fabs(angle2/rad)))))
             return true;
    }
    else if(length2>(500/(cos(angle2/rad))))
             return true;
    if(map->detection(450-length2*sin(angle2/rad),95+length2*cos(angle2/rad)))//如果map.碰撞检测返回为真
    {
        miner2->set_is_grab(true);
        return true;
    }
    return false;
}
void MainWindow::do_stop()
{
    if(time_left>0){
        ui->label_pause->setVisible(true);//暂停图案
        ui->toolButton_ppause->setVisible(true);
    }
    miner1->set_is_rotating(!(miner1->isrotating()));
    miner2->set_is_rotating(!(miner1->isrotating()));
    timer_clock.stop();
    timer_angle.stop();
    timer_s.stop();
    miner1->set_stopped(true);
    miner2->set_stopped(true);
}
void MainWindow::resume()
{
    ui->label_pause->setVisible(false);//暂停图案
    ui->toolButton_ppause->setVisible(false);

    miner1->set_is_rotating(!(miner1->isrotating()));
    miner2->set_is_rotating(!(miner2->isrotating()));
    timer_clock.start();
    timer_angle.start();
    timer_s.start();
    miner1->set_stopped(false);
    miner2->set_stopped(false);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::timereduce()//计时器
{
    if(time_left>=0)
    {
        time++;
        ui->lcdNumber->display(time_left--);
        if(time_down==0)
        {
            timer_s.setInterval(interval_miner);
            timer_angle.setInterval(interval_angle);
        }
        if(time_up==0)
        {
            k=1;
        }
        if(time_down)
        {
            k=1;
            --time_down;
;
        }
        if(time_up)
        {
            --time_up;
            k=3;

        }
        if(time%8==0)//每8秒，随机生成矿石一次
        {
            map->rrand();
            for(int i=6;i<14;i++)
                if(map->is_visible(i))//如果该物体可见
                {
                    lable[i]->setVisible(true);
                    lable[i]->move(map->get_point(i));

                }
        }
    }
    if(time_left<0)//剩余时间为0，则停止游戏
    {
        do_stop();
        ui->label_finish->setVisible(true);
        ui->toolButton_cancel->setVisible(true);
        ui->toolButton_restart->setVisible(true);
        ui->label_final->setVisible(true);
        QString s = QString::number(miner1->get_sum()+miner2->get_sum(), 10);
        ui->label_final->setText(s);
    }
}
void MainWindow::rotate1()//控制旋转
{
    if(miner1->isrotating())//旋转时
    {
        miner1->change_angle(angle_change_1);
        int angle1=miner1->get_angle();
        if(angle1>=90||angle1<=-90)
            angle_change_1*=-1;
    }
    update();
}
void MainWindow::rotate2()//控制旋转
{
    if(miner2->isrotating())//旋转时
    {
        miner2->change_angle(angle_change_2);
        int angle2=miner2->get_angle();
        if(angle2>=90||angle2<=-90)
            angle_change_2*=-1;
    }
    update();
}
void MainWindow::mm1()//矿工动作设置
{
    if(!miner1->isrotating())
    {
        if(miner1->isfflag())//矿工的动作设置
            ui->toolButton1->setStyleSheet("#toolButton1{border-image:url(:/images/iii/2.png);}");
        else
            ui->toolButton1->setStyleSheet("#toolButton1{border-image:url(:/images/iii/3.png);}");
    }

}
void MainWindow::mm2()//矿工动作设置
{
    if(!miner2->isrotating())
    {
        if(miner2->isfflag())//矿工的动作设置
            ui->toolButton2->setStyleSheet("#toolButton2{border-image:url(:/images/iii/2.png);}");
        else
            ui->toolButton2->setStyleSheet("#toolButton2{border-image:url(:/images/iii/3.png);}");
    }

}

void MainWindow::ss1()//钩子动作
{
    if(!miner1->isrotating())
    {
        if(miner1->isgrab())//物体的移动
        {
            miner1->change_rlength(k*fabs(miner1->get_speed()));
            int rlength=miner1->get_rlength();
            int angle=miner1->get_angle();
            QPoint p=map->get_point(map->get_cur_item());//获取当前物体的坐标
            QPoint temp(rlength*sin(angle/rad),-rlength*cos(angle/rad));//temp为位移大小
            lable[map->get_cur_item()]->move((p+temp));
        }
        if(miner1->isgrab()||detection1())//如果已经抓到物体了或碰撞检测返回为true
        {
            if(miner1->isgrab())
                miner1->set_speed(map->get_cur_speed());
            else
                miner1->set_speed(-1*(miner1->get_speed()));//速度相反，往反向拉
        }

        miner1->change_length(k*miner1->get_speed());
        if(miner1->get_length()<=0)//如果钩子回到了起点
        {
            if(miner1->isgrab())//如果抓到了东西
            {
                map->invisible();//令当前物体不可见
                lable[map->get_cur_item()]->setVisible(false);//令当前物体不可见
                miner1->change_sum(map->get_cur_price());
                miner1->set_rlength(0);
                miner1->set_is_grab(false);
                if(map->get_cur_kind())//如果抓到了特殊物品
                {
                    int kind=map->get_cur_kind();
                    if(kind==1)
                    {
                        time_up=10;
                        time_down=0;//不能叠加，所以把另一个的时间归零
                        timer_s.setInterval(30);
                        k=3;
                    }else if(kind==2)
                    {
                        time_down=10;
                        time_up=0;//不能叠加，所以把另一个的时间归零
                        timer_s.setInterval(interval_miner*2);
                        timer_angle.setInterval(interval_angle*2);  //所有动作时间减半
                        k=1;
                    }
                    else
                        time_left+=15;
                }
            }
            miner1->set_is_rotating(true);
            miner1->set_speed(4);
        }
    }
    ui->lcdNumber_sum->display(miner1->get_sum()+miner2->get_sum());
    update();
}
void MainWindow::ss2()//钩子动作
{
    if(!miner2->isrotating())
    {
        if(miner2->isgrab())//物体的移动
        {
            miner2->change_rlength(k*fabs(miner2->get_speed()));
            int rlength=miner2->get_rlength();
            int angle=miner2->get_angle();
            QPoint p=map->get_point(map->get_cur_item());//获取当前物体的坐标
            QPoint temp(rlength*sin(angle/rad),-rlength*cos(angle/rad));//temp为位移大小
            lable[map->get_cur_item()]->move((p+temp));
        }
        if(miner2->isgrab()||detection2())//如果已经抓到物体了或碰撞检测返回为true
        {
            if(miner2->isgrab())
                miner2->set_speed(map->get_cur_speed());
            else
                miner2->set_speed(-1*(miner2->get_speed()));//速度相反，往反向拉
        }

        miner2->change_length(k*miner2->get_speed());
        if(miner2->get_length()<=0)//如果钩子回到了起点
        {
            if(miner2->isgrab())//如果抓到了东西
            {
                map->invisible();//令当前物体不可见
                lable[map->get_cur_item()]->setVisible(false);//令当前物体不可见
                miner2->change_sum(map->get_cur_price());
                miner2->set_rlength(0);
                miner2->set_is_grab(false);
                if(map->get_cur_kind())//如果抓到了特殊物品
                {
                    int kind=map->get_cur_kind();
                    if(kind==1)
                    {
                        time_up=10;
                        time_down=0;//不能叠加，所以把另一个的时间归零
                        timer_s.setInterval(30);
                        k=3;
                    }else if(kind==2)
                    {
                        time_down=10;
                        time_up=0;//不能叠加，所以把另一个的时间归零
                        timer_s.setInterval(interval_miner*2);
                        timer_angle.setInterval(interval_angle*2);  //所有动作时间减半
                        k=1;
                    }
                    else
                        time_left+=15;
                }
            }
            miner2->set_is_rotating(true);
            miner2->set_speed(4);
        }
    }
    ui->lcdNumber_sum->display(miner1->get_sum()+miner2->get_sum());
    update();
}
void MainWindow::on_toolButton_reset_clicked()
{
    init();
}

void MainWindow::on_toolButton_continue_clicked()
{
    if(miner1->isstopped()&&miner2->isstopped()&&time_left>0)
        resume();
    else
        do_stop();
}

void MainWindow::on_toolButton_start_clicked()
{
    ui->label_begin->setVisible(false);
    ui->toolButton_start->setVisible(false);
    resume();
}

void MainWindow::on_toolButton_ppause_clicked()
{
    resume();
}

void MainWindow::on_toolButton_restart_clicked()
{
    init();
}

void MainWindow::on_toolButton_cancel_clicked()
{
    qApp->quit();
//    ui->label_finish->setVisible(false);
//    ui->toolButton_cancel->setVisible(false);
//    ui->toolButton_restart->setVisible(false);
}
