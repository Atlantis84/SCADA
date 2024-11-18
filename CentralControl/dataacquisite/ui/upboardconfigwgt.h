#ifndef UPBOARDCONFIGWGT_H
#define UPBOARDCONFIGWGT_H

#include <QObject>
#include <QWidget>

class UpBoardConfigWgt : public QWidget
{
    Q_OBJECT
public:
    static UpBoardConfigWgt* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new UpBoardConfigWgt();
        return m_pInstance;
    }

private:
    explicit UpBoardConfigWgt(QWidget *parent = nullptr);

    static UpBoardConfigWgt* m_pInstance;

signals:

public slots:
};

#endif // UPBOARDCONFIGWGT_H
