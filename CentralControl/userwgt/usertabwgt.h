#ifndef USERTABWGT_H
#define USERTABWGT_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
class UserTabWgt : public QWidget
{
    Q_OBJECT
public:
    explicit UserTabWgt(QWidget *parent = nullptr);

private:
    QTableWidget* tableWidget;
    QWidget* createTableWidget();
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);

    QLineEdit* m_pLineEditName;
    QLineEdit* m_pLineEditPassword;
    void set_table_data();
    void insert_new_usr_to_db();
signals:

public slots:
    void slot_btn_query();
    void slot_btn_insert();
    void slot_btn_delete();
};

#endif // USERTABWGT_H
