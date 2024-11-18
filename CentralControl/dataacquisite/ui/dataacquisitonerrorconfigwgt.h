#ifndef DATAACQUISITONERRORCONFIGWGT_H
#define DATAACQUISITONERRORCONFIGWGT_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QComboBox>
#include <ActiveQt/QAxObject>
#include <QThread>
#include <QTableView>
#include <QStandardItemModel>
#include "waitcountwgt.h"
class ImportThread: public QThread
{
    Q_OBJECT
public:
    explicit ImportThread(QWidget *parent = nullptr);

private:
    void run();
signals:
    void signal_import_data(QList<QString> data);
    void signal_import_over();
};

class DataAcquisitonErrorConfigWgt : public QWidget
{
    Q_OBJECT
public:
    static DataAcquisitonErrorConfigWgt* get_instance()
    {
        if(m_pErrorWgt == nullptr)
            m_pErrorWgt = new DataAcquisitonErrorConfigWgt();
        return m_pErrorWgt;
    }
private:
    explicit DataAcquisitonErrorConfigWgt(QWidget *parent = nullptr);

    static DataAcquisitonErrorConfigWgt* m_pErrorWgt;

    QTableView* m_pTableView;
    QStandardItemModel* m_pItemModel;

    WaitCountWgt m_pWakeUp;

    void set_table_data(int id);
    ImportThread* m_pImportThread;

    QString open_excel_file();
    void insert_info_to_db(QList<QString> info);

signals:

public slots:
    void slot_import_data();
    void slot_import_data(QList<QString> data);
    void slot_comb_index_changed(int id);
    void slot_btn_import_click();
    void slot_import_over();
};

#endif // DATAACQUISITONERRORCONFIGWGT_H
