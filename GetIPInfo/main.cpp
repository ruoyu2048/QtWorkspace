#include <QCoreApplication>
#include <QNetworkInterface>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    int flag =0;
    if(argc==1)
    {
        printf("Instructions:\n1 Get MoNiZhiHui Net IP information\n2 Get FangZhenDuiKang Net IP information\n");
        return 0;
    }
    if(strcmp("1",argv[1])==0)
    {
        //printf("%s\n",argv[1]);
        flag=1;

    }else if(strcmp("2",argv[1])==0)
    {
        //printf("%s\n",argv[1]);
        flag=2;
    }
    else
    {
        printf("Parameter error:%s\n",argv[1]);
    }
    while(true)
    {
        QList<QNetworkInterface>ni=QNetworkInterface::allInterfaces();
        if(ni.size()>0)
        {
            foreach (QNetworkInterface niItem, ni)
            {
                //过滤掉没有MAC地址信息的适配器
                if(!niItem.hardwareAddress().isEmpty())
                {
                    if(niItem.flags().testFlag(QNetworkInterface::IsUp)&&niItem.flags().testFlag(QNetworkInterface::IsRunning))
                    {
                        QList<QNetworkAddressEntry>entry=niItem.addressEntries();
                        if(entry.size()>0)
                        {
                            foreach (QNetworkAddressEntry itEntry, entry)
                            {
                                QHostAddress hostAddres=itEntry.ip();
                                if(hostAddres.protocol()==QAbstractSocket::IPv4Protocol)
                                {
                                    QString strHostIP = hostAddres.toString();
                                    if(strHostIP.contains("172."))
                                    {
                                        if(flag == 1 && strHostIP.contains("172.15."))
                                        {
                                           printf("%s\n",strHostIP.toStdString().c_str());
                                           return 0;
                                        }
                                        if(flag == 2 && !strHostIP.contains("172.15."))
                                        {
                                            printf("%s\n",strHostIP.toStdString().c_str());
                                            return 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    printf("NoIP\n");
    return 0;
}
