#include "sorter.h"
#define red 0
#define green 1
#define yellow 2
#define A 0
#define B 1

Sorter::Sorter()
{

}
Sorter::~Sorter()
{

}
Order Sorter::getfirstlastOrder(OrderList orderx)
{
    orderAmount=orderx.order.size();
    for(int i=0;i<orderAmount;i++)
    {
        if(orderx.order.at(i).storageRack==yellow)
          yellowOrderList.order.append(orderx.order.at(i));
        else if(orderx.order.at(i).warehouseClerk==A)
          clerkAOrderList.order.append(orderx.order.at(i));
    }
    for(int j=0;j<orderAmount;j++)
    {
        if(orderx.order.at(j).storageRack==green)
          greenOrderList.order.append(orderx.order.at(j));
        else if(orderx.order.at(j).warehouseClerk==A)
          clerkAOrderList.order.append(orderx.order.at(j));
    }
    yellowAmount=yellowOrderList.order.size();
    AAmount=clerkAOrderList.order.size();
    greenAmount=greenOrderList.order.size();
    BAmount=clerkBOrderList.order.size();
    if(yellowAmount>AAmount)
    {firstOrder=yellowOrderList.order.at(0);
        sort=true;
        return firstOrder;}
   // else if(greenAmount>BAmount)
   // {firstOrder=green.order.at(1);
   //     sort=1;
    //    return firstOrder;}
   else if(AAmount>yellowAmount)
        {lastOrder=clerkAOrderList.order.at(0);
            sort=false;
        return lastOrder;}

    else
    {
        if(greenAmount>BAmount)
            {firstOrder=greenOrderList.order.at(0);
                sort=true;
               return firstOrder;}
        else if(BAmount>greenAmount)
                  {lastOrder=clerkBOrderList.order.at(0);
                      sort=false;
                    return lastOrder;}
        else
        {
            sort=true;
            firstOrder=orderx.order.at(0);
            return firstOrder;
        }
    }
  //  if(BAmount>greenAmount)
  //  {lastOrder=clerkB.order.at(1);
  //      sort=0;
    //    return lastOrder;}

}


OrderList Sorter::firstoptimalOrder(OrderList orderx)
{
    if(sort)
{
    diyiOrder=getfirstlastOrder(orderx);
    opt.order.append(diyiOrder);
    a_iter= orderx.order.begin();
/*    for(a_iter= orderx.order.begin(); a_iter!= orderx.order.end(); ++a_iter)
    {
        if((*a_iter->storageRack==diyiOrder.storageRack)&&\
             (*a_iter->warehouseClerk==diyiOrder.warehouseClerk))
            //std::cout << a_iter->storageRack << endl;
                orderx.order.erase(a_iter);
         restOrder=orderx;
         //break;

    }
*/
    for(int k=0;k<orderAmount;k++)
    {
        if((orderx.order.at(k).storageRack==diyiOrder.storageRack)&&\
                (orderx.order.at(k).warehouseClerk==diyiOrder.warehouseClerk))

            orderx.order.erase(a_iter+k);
        restOrder=orderx;
            break;
    }
          int restOrderAmount;
    orderAmount=orderx.order.size();
    for (int m=0;m<orderAmount;m++)
    {
      restOrderAmount=restOrder.order.size();
        if(opt.order.at(m).warehouseClerk==A)
        {

            for(int h=0;h<(restOrderAmount);h++)
            {
                if(restOrder.order.at(h).storageRack==yellow)
                { opt.order.append(restOrder.order.at(h));
                     restOrder.order.erase(a_iter+h);
                    break;
                }
           else if(restOrder.order.at(h).storageRack==red)
            { opt.order.append(restOrder.order.at(h));
                    restOrder.order.erase(a_iter+h);
                break;
            }
           else if(restOrder.order.at(h).storageRack==green)
            { opt.order.append(restOrder.order.at(h));
                    restOrder.order.erase(a_iter+h);
                break;
            }
            }
        }
       else if(opt.order.at(m).warehouseClerk==B)
        {
            for(int h=0;h<(restOrderAmount);h++)
            {
                if(restOrder.order.at(h).storageRack==green)
                { opt.order.append(restOrder.order.at(h));
                    restOrder.order.erase(a_iter+h);
                    break;
                }
           else if(restOrder.order.at(h).storageRack==red)
            { opt.order.append(restOrder.order.at(h));
                    restOrder.order.erase(a_iter+h);
                break;
            }
           else if(restOrder.order.at(h).storageRack==yellow)
            { opt.order.append(restOrder.order.at(h));
                    restOrder.order.erase(a_iter+h);
                break;
            }
            }
        }
    }
    }
    else
    {
        zuihouOrder=getfirstlastOrder(orderx);
        opt.order.append(zuihouOrder);
        for(int k=0;k<orderAmount;k++)
        {
            if((orderx.order.at(k).storageRack==zuihouOrder.storageRack)&&\
                  (orderx.order.at(k).warehouseClerk==zuihouOrder.warehouseClerk))
                orderx.order.erase(a_iter+k);
            restOrder=orderx;
                break;
        }
              int restOrderAmount;
        orderAmount=orderx.order.size();
        for (int m=0;m<orderAmount;m++)
        {
          restOrderAmount=restOrder.order.size();
            if(opt.order.at(m).storageRack==yellow)
            {

                for(int h=0;h<(restOrderAmount);h++)
                {
                    if(restOrder.order.at(h).warehouseClerk==A)
                    { opt.order.append(restOrder.order.at(h));
                         restOrder.order.erase(a_iter+h);
                        break;
                    }
               else if(restOrder.order.at(h).warehouseClerk==B)
                { opt.order.append(restOrder.order.at(h));
                        restOrder.order.erase(a_iter+h);
                    break;
                }
                }
            }
           else if(opt.order.at(m).storageRack==red)
            {

                for(int h=0;h<(restOrderAmount);h++)
                {
                    if(restOrder.order.at(h).warehouseClerk==A)
                    { opt.order.append(restOrder.order.at(h));
                         restOrder.order.erase(a_iter+h);
                        break;
                    }
               else if(restOrder.order.at(h).warehouseClerk==B)
                { opt.order.append(restOrder.order.at(h));
                        restOrder.order.erase(a_iter+h);
                    break;
                }
                }
            }
            else if(opt.order.at(m).storageRack==green)
             {

                 for(int h=0;h<(restOrderAmount);h++)
                 {
                     if(restOrder.order.at(h).warehouseClerk==B)
                     { opt.order.append(restOrder.order.at(h));
                          restOrder.order.erase(a_iter+h);
                         break;
                     }
                else if(restOrder.order.at(h).warehouseClerk==A)
                 { opt.order.append(restOrder.order.at(h));
                         restOrder.order.erase(a_iter+h);
                     break;
                 }
                 }
             }

        }
        std::reverse(opt.order.begin(),opt.order.end());
    }
    return opt;
}
