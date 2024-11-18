#ifndef LIGHTBARNEWPRODUCTWGT_H
#define LIGHTBARNEWPRODUCTWGT_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QTableWidgetItem>
class LightBarNewProductWgt : public QWidget
{
    Q_OBJECT
public:
    explicit LightBarNewProductWgt(QWidget *parent = nullptr);

    QString select_id_from_db(const QString halfmaterialnumber);
private:
    QTableWidget* tableWidget;
    QWidget* createTableWidget();
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    QLineEdit* m_pLineEditID;
    QLineEdit* m_pLineEditSN;
    void insert_new_product_to_db();

    void set_table_data();

signals:

public slots:
    void slot_btn_query();
    void slot_btn_insert();
    void slot_btn_delete();
    void slot_select_item(QTableWidgetItem* item);
};

#endif // LIGHTBARNEWPRODUCTWGT_H
