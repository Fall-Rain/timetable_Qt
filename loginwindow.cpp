#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include <QMessageBox>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QApplication>
#include <QLabel>
#include <QtCore5Compat/QTextCodec>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
//    setFixedSize(1000,400);
    ui->setupUi(this);
//    this->resize(QSize(1200,900));
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::replyFinished(QNetworkReply *arg){
    int statusCode=arg->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<QString::fromUtf8(arg->readAll());

    qDebug()<<"http状态码"<<statusCode;
    Mainwindow *main=new Mainwindow();
    main->show();
    qDebug()<<"跳转链接"<<arg->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
    QList list=arg->rawHeaderPairs().toList();
    for(int i=0;i<list.size();i++){
        qDebug()<<list[i].first<<":"<<list[i].second;
    }

}

QString LoginWindow::encodeInp(QString str){
    QString key="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    QString res="";
    int chr1=0, chr2=0, chr3=0,enc1=0, enc2=0, enc3=0, enc4=0;
    int i=0;
    do {
        chr1=str.at(i++).unicode();
        if(i<str.size()) chr2=str.at(i++).unicode();
        else chr2=0;
        if(i<str.size()) chr3=str.at(i++).unicode();
        else chr3=0;

        enc1=chr1 >> 2;
        enc2=((chr1 & 3) << 4)|(chr2>>4);
        enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
        enc4 = chr3 & 63;

        if(chr2==0){
            enc3=64;
            enc4=64;
        }else if(chr3==0){
            enc4=64;
        }
        res.append(key[enc1]);
        res.append(key[enc2]);
        res.append(key[enc3]);
        res.append(key[enc4]);
    } while (i < str.size());
    return res;
}

void LoginWindow::on_login_clicked()
{
    if(ui->username->toPlainText().length()==0 || ui->password->toPlainText().length()==0) {
        QMessageBox::warning(NULL,"警告","账户密码为空",QMessageBox::Ok,QMessageBox::Ok);
        return;
    }
    QString encoded=encodeInp(ui->username->toPlainText())+"%%%"+encodeInp(ui->password->toPlainText());
    QNetworkRequest request;
    QNetworkAccessManager *nManager=new QNetworkAccessManager(this);
    nManager->setRedirectPolicy(QNetworkRequest::ManualRedirectPolicy);
    QMetaObject::Connection connRet = QObject::connect(nManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    Q_ASSERT(connRet);
    request.setUrl(QUrl("http://jwky.jltc.edu.cn:89/jsxsd/xk/LoginToXk"));
    QString data;
    data.append("userAccount="+ui->username->toPlainText().toUtf8().toPercentEncoding());
    data.append("&userPassword="+ui->password->toPlainText().toUtf8().toPercentEncoding());
    data.append("&encoded="+encoded.toUtf8().toPercentEncoding());
    QByteArray by=data.toUtf8().toPercentEncoding();
    qDebug()<<data;
    nManager->post(request,data.toUtf8());
}

