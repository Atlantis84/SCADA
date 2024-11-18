#ifndef INSERTMACHINEERRORSWGT_H
#define INSERTMACHINEERRORSWGT_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QDateEdit>
#include <QComboBox>
class InsertMachineErrorsWgt : public QWidget
{
    Q_OBJECT
public:
    static InsertMachineErrorsWgt* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new InsertMachineErrorsWgt();
        return m_pInstance;
    }
private:
    explicit InsertMachineErrorsWgt(QWidget *parent = nullptr);

    void timerEvent(QTimerEvent *event);

    static InsertMachineErrorsWgt* m_pInstance;

private:
    QTableView* m_pTableView;
    QStandardItemModel* m_pItemModel;
    QDateEdit* m_pDateEdit;
    QComboBox* m_pCmbStationStyle;

signals:

public slots:
    void slot_comb_station_index_changed(int id);
    void slot_set_table_data(QList<QStringList> info);
    void slot_btn_import_click();
    void slot_btn_export_click();
};

#endif // INSERTMACHINEERRORSWGT_H
