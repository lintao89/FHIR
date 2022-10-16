#include "net2.h"
//https://stackoverflow.com/questions/25433706/qt-console-app-using-qnetworkaccessmanager
Net2::Net2()
{

}

/*void Net::requestFinished(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
      qDebug().noquote()<<"status code="+QString::number(statusCode.toInt());
    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
      qDebug().noquote()<<reason.toString();
    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
       qDebug().noquote()<<"Failed: " +reply->errorString();
     }
     else {
       // 獲取返回內容
      qDebug().noquote()<<reply->readAll()<<endl;
    }
}*/

void Net2::PostData(QString url,QByteArray Value)
{
    QUrl theUrl(url);
    QNetworkRequest request(theUrl);
    request.setRawHeader(QByteArray("Authorization"), QByteArray("Bearer 87944918-1f86-418e-8418-398e2e4ddee6"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json+fhir");
    request.setRawHeader("authorization","Bearer 87944918-1f86-418e-8418-398e2e4ddee6");
    //manager->post(request,Value.toUtf8());
    QEventLoop loop;
    //QNetworkReply* reply = manager.post(request, Value.toUtf8());
    QNetworkReply* reply = manager.post(request, Value);
    connect(reply , SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
      qDebug().noquote()<<"status code="+QString::number(statusCode.toInt());//201 Created
    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
      qDebug().noquote()<<reason.toString();
    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
       qDebug().noquote()<<"Failed: " +reply->errorString();
       qDebug().noquote()<<reply->readAll();
     }
     else {
       // 獲取返回內容
      qDebug().noquote()<<reply->readAll()<<"\n";
    }
}
void Net2::PutData(QString urlput,QByteArray Value)
{
    QUrl theUrl(urlput);
    QNetworkRequest request(theUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json+fhir");
    request.setRawHeader("authorization","Bearer 87944918-1f86-418e-8418-398e2e4ddee6");
    //manager->post(request,Value.toUtf8());
    QEventLoop loop;
    //QNetworkReply* reply = manager.post(request, Value.toUtf8());
    QNetworkReply* reply = manager.put(request, Value);
    connect(reply , SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
      qDebug().noquote()<<"status code="+QString::number(statusCode.toInt());//201 Created
    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
      qDebug().noquote()<<reason.toString();
    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
       qDebug().noquote()<<"Failed: " +reply->errorString();
       qDebug().noquote()<<reply->readAll();
     }
     else {
       // 獲取返回內容
      qDebug().noquote()<<reply->readAll()<<"\n";
    }
}
