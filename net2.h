#ifndef NET2_H
#define NET2_H
#include <QObject>
#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>


class Net2 : public QObject
{
    Q_OBJECT
      QNetworkAccessManager manager;
public:
    Net2();
    void PostData(QString url,QByteArray Value);
    void PutData(QString urlput,QByteArray Value);
};



#endif // NET2_H
