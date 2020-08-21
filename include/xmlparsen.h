//!
//! \file xmlparsen.h
//! \author Li Chen, Hongyu Zhao  Team004
//! \date 25-Jul-2018
//! \brief  a class for the parse of the XML file
//!
//! parse the XML file to get the orders and put it into the
//! container, which will be used later. Because the suborders
//! have many infomations, so constructs for each suborders are
//! needed.
//!
#ifndef XMLPRASEN_H
#define XMLPRASEN_H

#include <QString>
#include <QtXml>
#include <QUrl>
//#include <QXmlSchmaValidator>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QString>
#include <iostream>
#include <QList>
#include <QVector>

#define UNKNOWN_STR "UNKNOWN"
using namespace std;


struct Order  //struct that reserve information of each order
{
    enum StorageRack
    {
        UNKNOWN_RACK = -1,//default value
        RACK_RED,//!<get Red Order
        RACK_GREEN,//!<get Green Order
        RACK_YELLOW//!<get Yellow Order

    };

    enum WarehouseClerk
    {   UNKNOWN_CLERK = -1,//default value
        CLERK_A,//!<delivery to clerk A
        CLERK_B//!<delivery to clerk B
    };

    QString id = UNKNOWN_STR;
    QString description = UNKNOWN_STR;
    StorageRack storageRack = UNKNOWN_RACK;
    WarehouseClerk warehouseClerk = UNKNOWN_CLERK;
    static StorageRack storageRackParser(const QString& str);
    static WarehouseClerk clerkParser(const QString& str);
};

struct OrderList //Struct that reserve all the order information
{
    QString idOrderList = UNKNOWN_STR;
    QString descriptionOrderList = UNKNOWN_STR;
    QList<Order> order;

};


class Xmlprasen
{
public:
    Xmlprasen();
    ~Xmlprasen();
    //! \brief get the xml file with the obtained filepath.
    //!
    static OrderList getXmlFilePath(const QString& xmlFileName);

};

#endif // XMLPRASEN_H
