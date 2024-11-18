#ifndef REDFLAGTESTCOMMANDWGT_H
#define REDFLAGTESTCOMMANDWGT_H

#include <QWidget>
#include <QTextEdit>
#include "pressstationserialcom.h"
class RedFlagTestCommandWgt : public QWidget
{
    Q_OBJECT

public:
    static RedFlagTestCommandWgt* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new RedFlagTestCommandWgt();
        return m_pInstance;
    }

private:
    explicit RedFlagTestCommandWgt(QWidget *parent = nullptr);

    void showEvent(QShowEvent* event);
    void closeEvent(QCloseEvent* event);

    static RedFlagTestCommandWgt* m_pInstance;
    QTextEdit* m_pLogTextEdit;

    QString get_current_time();
    PressStationSerialCom* m_pPressStationSerialCom;

signals:

public slots:
    void slot_btn_to_press();
    void slot_btn_to_read();

    void slot_from_press(int code);
    void slot_from_mano_meter(uint16_t value);
};

#endif // REDFLAGTESTCOMMANDWGT_H
