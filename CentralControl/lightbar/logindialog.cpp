#include "logindialog.h"
#include "title/TitleBar.h"
#include <QRegExp>
#include <QValidator>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPainter>
#include "imessagebox.h"
#include "gdatafactory.h"
#include <QSqlQuery>
#include <QMessageBox>
LoginDialog::LoginDialog()
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setWindowIcon(QIcon(":/icon/24x24.png"));
    this->resize(300,200);

    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"超级管理员");
    pTitleBar->setFixedWidth(300);
    installEventFilter(pTitleBar);

    m_pLineAccount = new QLineEdit();
    m_pLineAccount->setStyleSheet("color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);background-color:rgb(240,242,245);font-family:KaiTi;font-size:20px;");
    m_pLineAccount->setAlignment(Qt::AlignCenter);
    m_pLineAccount->setText(u8"");
    QRegExp regx("[A-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,m_pLineAccount);
    m_pLineAccount->setValidator(validator);

    QGroupBox* grpBox = new QGroupBox(u8"请输入管理员密码");
    grpBox->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    QHBoxLayout* hBox = new QHBoxLayout();
    hBox->addWidget(m_pLineAccount);
    grpBox->setLayout(hBox);

    QHBoxLayout* hBox1 = new QHBoxLayout();
    QPushButton* btnOk = new QPushButton(u8"  确 定  ");
    btnOk->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                                    QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnOk,SIGNAL(clicked()),this,SLOT(slot_ok()));
    QPushButton* btnCancel = new QPushButton(u8"  取 消  ");
    btnCancel->setStyleSheet("QPushButton{border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                                    QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(slot_cancel()));
    hBox1->addWidget(btnOk);
    hBox1->addWidget(btnCancel);

    QVBoxLayout* vAll = new QVBoxLayout();
    vAll->addSpacing(pTitleBar->height());
    vAll->addWidget(grpBox);
    vAll->addLayout(hBox1);
    this->setLayout(vAll);
}

void LoginDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void LoginDialog::slot_ok()
{
    if(m_pLineAccount->text().trimmed() == "")
    {
        QMessageBox* msgBox = new QMessageBox();
        msgBox->setStyleSheet("QPushButton{border:solid 1px rgba(0,0,0,100);color:rgb(0,0,0);}QLabel{min-width:200px;min-height:100px;color:rgb(0,0,0);}QWidget{background-color:rgb(255,255,255);}");
        msgBox->setWindowTitle(u8"警告");
        msgBox->setText(u8"请输入管理员账号!");
        msgBox->exec();
        return;
    }

    QString strSql;
    if(this->m_pLineAccount->text().trimmed() != "")
        strSql = QString("select * from public.%1 where \"usrname\"='%2' and \"password\" like '\%%3\%'").
                arg(constUserInfoTable).arg("admin").arg(GDataFactory::get_factory()->EncodeStr(this->m_pLineAccount->text().trimmed()));

    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        if(queryResult.size() == 0)
        {
            QMessageBox* msgBox = new QMessageBox();
            msgBox->setStyleSheet("QPushButton{border:solid 1px rgba(0,0,0,100);color:rgb(0,0,0);}QLabel{min-width:200px;min-height:100px;color:rgb(0,0,0);}QWidget{background-color:rgb(255,255,255);}");
            msgBox->setWindowTitle(u8"警告");
            msgBox->setText(u8"密码错误，请重新输入!");
            msgBox->exec();
        }
        else
            this->accept();
    }
}

void LoginDialog::slot_cancel()
{
    this->close();
}
