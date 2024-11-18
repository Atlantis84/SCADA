#ifndef TCON_PSQL_DATA_ACQUISITION_H
#define TCON_PSQL_DATA_ACQUISITION_H

#include <QObject>
#include <dataacquisitiondefines.h>

class tcon_psql_data_acquisition : public QObject
{
    Q_OBJECT
public:
    explicit tcon_psql_data_acquisition(QObject *parent = nullptr);

signals:

public slots:
    void slot_insert_tcon_psql_data_acquisition(QMap<int,QString> tconSubStationDataTable);
};

#endif // TCON_PSQL_DATA_ACQUISITION_H
