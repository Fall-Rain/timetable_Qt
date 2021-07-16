#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    QString encodeInp(QString str);

public slots:
    void replyFinished(QNetworkReply*);
private slots:
    void on_login_clicked();

private:
    Ui::LoginWindow *ui;
};
#endif // LOGINWINDOW_H
