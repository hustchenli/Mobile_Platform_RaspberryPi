#include "xmlparsen.h"

Xmlprasen::Xmlprasen()
{

}


Xmlprasen::~Xmlprasen()
{

}

OrderList Xmlprasen::getXmlFilePath(const QString &xmlFileName)
{
    /*
     QUrl schemaUrl("file:///home/user/schema.xsd");

        QXmlSchema schema;
        schema.load(schemaUrl);

        if (schema.isValid()) {
            QFile file("test.xml");
            file.open(QIODevice::ReadOnly);

            QXmlSchemaValidator validator(schema);
            if (validator.validate(&file, QUrl::fromLocalFile(file.fileName())))
                qDebug() << "instance document is valid";
            else
                qDebug() << "instance document is invalid";
        }
    */
    OrderList DataArray;
    QFile xmlFile(xmlFileName);
    if(!xmlFile.exists() || !xmlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to open xml-file:"<<xmlFileName;
        return DataArray;
    }
    QDomDocument doc;
    doc.setContent(xmlFile.readAll()); // create DOM
    xmlFile.close();

    // get "holiday" attribute
    QDomElement orderlist = doc.firstChildElement("orderList");
    QDomElement idOrderList =orderlist.firstChildElement("id");
    QDomElement descriptionOrderList = orderlist.firstChildElement("description");
    DataArray.idOrderList = idOrderList.text();
    DataArray.descriptionOrderList = descriptionOrderList.text();

    QDomNodeList orderList = doc.elementsByTagName("order");
    for (int idx = 0; idx < orderList.size(); idx++)
    {
        Order currOrder;
        // get current "gift" element
        QDomNode currOrderNode = orderList.item(idx);
        // get "priority" attribute
        // get the other elements
        QDomElement id = currOrderNode.firstChildElement("id");
        QDomElement description = currOrderNode.firstChildElement("description");
        QDomElement storageRack = currOrderNode.firstChildElement("storageRack");
        QDomElement warehouseClerk = currOrderNode.firstChildElement("warehouseClerk");

        // print element names
       /*qDebug() << "   " << qPrintable(id.text())
                 << "   " << qPrintable(description.text())
                 << "   " << qPrintable(storageRack.text())
                 << "   " << qPrintable(warehouseClerk.text());
         qDebug();*/
        currOrder.id = id.text();
        currOrder.description = description.text();
        currOrder.storageRack    = Order::storageRackParser(storageRack.text());
        currOrder.warehouseClerk = Order::clerkParser(warehouseClerk.text());
        //qDebug() << "   " << currOrder.storageRack;

        DataArray.order.append(currOrder);
    }

    /*QList<QString>::iterator i;
    for(i=DataArray.order.begin();i=DataArray.order.end();i++)
        cout<<*i<<",";*/

    return DataArray;
}

Order::StorageRack Order::storageRackParser(const QString& str)
{
    StorageRack retValue = StorageRack::UNKNOWN_RACK;
    if (!str.compare("Red"))
    {retValue = StorageRack::RACK_RED;}
    else if (!str.compare("Green"))
    {retValue= StorageRack::RACK_GREEN;}
    else if (!str.compare("Yellow"))
    {retValue = StorageRack::RACK_YELLOW;}
    return retValue;
}

Order::WarehouseClerk Order::clerkParser(const QString &strclerk)
{
    WarehouseClerk retClerk = WarehouseClerk::UNKNOWN_CLERK;
    if (!strclerk.compare("ClerkA"))
    {retClerk = WarehouseClerk::CLERK_A;}
    else if (!strclerk.compare("ClerkB"))
    {retClerk = WarehouseClerk::CLERK_B;}
    return retClerk;
}
