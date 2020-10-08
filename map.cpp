#include"map.h"
Map::Map()
{
    for(int i=0;i<14;i++)//初始化item数组
        item[i]=new Item;
}
QPoint Map::get_point(const int &i)
{
    QPoint p(item[i]->get_x(),item[i]->get_y());
    return p;
}
int Map::get_kind(const int &i)
{
    return item[i]->get_kind();
}

bool Map::is_visible(const int &i)
{
    return item[i]->is_visible();
}

bool Map::detection(const int &x,const int &y)
{
    for(int i=0;i<14;i++){
        if(item[i]->detection(x,y))//如果碰到了任意一个物体
        {
            cur_item=i;
            return true;
        }
    }
}
void Map::invisible()
{
    item[cur_item]->set_invisible();
}
void Map::rrand()   //在此函数中完成所有标号6~13的矿石、道具的初始化，其中6~9随机生成1~3个，10~13随机生成0~2个并均设为可见
{
    srand(time(NULL));
    int times=1+rand()%3;//随机生成1-3个
    int count=0;
    while(count<times)
    {
        int i=6+rand()%4;
        if(item[i]->is_visible()==false)
        {
            item[i]->set_visible();
            int w=50+rand()%600;
            int h=120+rand()%400;
            item[i]->set_position(w,h);
            if(i==6)//大矿石 6
            {
                item[i]->set_size(140,120);
                item[i]->set_price(500);
                item[i]->set_speed(-1);
                item[i]->set_kind(0);
            }
            else if(i==7)  //小矿石   7
            {
                item[i]->set_size(60,50);
                item[i]->set_price(200);
                item[i]->set_speed(-4);
                item[i]->set_kind(0);
            }
            else if(i==8)   //岩石  8
            {
                item[i]->set_size(60,50);
                item[i]->set_price(20);
                item[i]->set_speed(-2);
                item[i]->set_kind(0);
            }
            else if(i==9)//钻石  9
            {
                item[i]->set_size(60,50);
                item[i]->set_price(700);
                item[i]->set_speed(-3);
                item[i]->set_kind(0);
            }
            count++;
        }
        //只要不是所有随机矿石都被占满，就继续进行循环
        int flag=1;
        for(int i=6;i<10;i++)
            if(item[i]->is_visible()==false)
                flag=0;
        if(flag)
            break;
    }
    //生成随机物质
    times=rand()%3;//随机生成0-2个随机道具
    count=0;
    while(count<times)
    {
        int i=10+rand()%4;
        if(item[i]->is_visible()==false)
        {
            item[i]->set_visible();
            int w=50+rand()%600;
            int h=120+rand()%400;
            item[i]->set_position(w,h);
            item[i]->set_size(60,50);
            item[i]->set_price(0);
            item[i]->set_speed(-4);
            if(i==13)
                item[i]->set_kind(1+rand()%3);
            else
                item[i]->set_kind(i-9);
            count++;
        }
        //只要不是所有随机矿石都被占满，就继续进行循环
        int flag=1;
        for(int i=10;i<14;i++)
            if(item[i]->is_visible()==false)
                flag=0;
        if(flag)
            break;
    }

}

void Map::init()    //在此函数中仅保留0~5的矿石为可见，其他所有矿石、道具均改为不可见
{
    srand(time(NULL));
    for(int i=0;i<6;i++)
    {
        item[i]->init();    //可见，坐标、价值等属性通通为0
        int w=50+rand()%600;
        int h=120+rand()%400;
        item[i]->set_position(w,h);
        if(i==2)             //岩石  2
        {
            item[i]->set_size(60,50);
            item[i]->set_price(20);
            item[i]->set_speed(-2);
        }
        if(i==3||i==4)             //大矿石 3，4
        {
            item[i]->set_size(140,120);
            item[i]->set_price(500);
            item[i]->set_speed(-1);
        }
        else                //小矿石 0,1,5
        {
            item[i]->set_size(60,50);
            item[i]->set_price(200);
            item[i]->set_speed(-4);
        }
    }
    for(int i=6;i<14;i++)//让随机矿石不可见
    {
        item[i]->set_invisible();
    }
}
