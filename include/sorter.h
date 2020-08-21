//!
//! \file sorter.h
//! \author Li Chen, Hongyu Zhao  Team004
//! \date 04-Jun-2018
//! \brief This Class is used to sort the logistics tasks, in order to get the optimal sort of the tasks.
//!
//! The logistics tasks consist of two main part: pick up, delivery.
//! Delivery time can not be optimized. But pick up time can be optimized. By using this
//! sorter class, we can get the shortest pick up time.
//!


#ifndef SORTER_H
#define SORTER_H
#include <QString>
#include <iostream>
#include <QVector>
#include <QVector2D>
#include <QList>
#include <QDebug>
#include<xmlparsen.h>
#include <iterator>


class Sorter
{
public:
    Sorter();
    ~Sorter();

    OrderList dataArray;

    OrderList optimalOrder;

    Order getfirstlastOrder(OrderList orderx);

    OrderList firstoptimalOrder(OrderList orderx);

    QList<Order>::iterator a_iter;
private:
    //Orderlist orderx;
    OrderList yellowOrderList;
    OrderList clerkAOrderList;
    OrderList greenOrderList;
    OrderList clerkBOrderList;
    OrderList opt;
    OrderList restOrder;
    Order firstOrder;
    Order lastOrder;
    Order diyiOrder;
    Order zuihouOrder;
    bool sort;
    int orderAmount;
    int yellowAmount;
    int AAmount;
    int greenAmount;
    int BAmount;
};

#endif // SORTER_H
