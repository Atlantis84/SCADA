#include "modbusclientdll.h"
#include "modbus.h"
#include "QsLog.h"

ModbusClientDll::ModbusClientDll()
{
}

void ModbusClientDll::init_ip_port(const MODBUS_TYPE config,
                                   const QString RemoteIPOrPort,
                                   const int RemotPortOrBaud)
{
    modbus_mode = config;
    m_remote_ip_port = RemoteIPOrPort;
    m_remote_port_baud = RemotPortOrBaud;
}

MODBUS_STATUS ModbusClientDll::sendMessageToPLC(int rw_ctrl,
                                                int starting_address,
                                                int num_registers,
                                                uint16_t *value)
{
    modbus_t *ctx=nullptr;
    uint16_t data[16];

    if(modbus_mode == MODBUS_TYPE::MODBUS_TCP){
        QByteArray ba = m_remote_ip_port.toLatin1();
        const char *ip_address = ba.data();
        int port = m_remote_port_baud;
//        QLOG_WARN()<<"modbus connect ip:"<<m_remote_ip_port<<"modbus port:"<<m_remote_port_baud;

        // 创建Modbus TCP上下文
        ctx = modbus_new_tcp(ip_address, port);
        modbus_set_slave(ctx, 1);
        modbus_set_debug(ctx, TRUE);
    }else if(modbus_mode == MODBUS_TYPE::MODBUS_COM){
        QByteArray ba = m_remote_ip_port.toLatin1();
        const char *ip_address = ba.data();
        int port = m_remote_port_baud;
        ctx = modbus_new_rtu(ip_address,port,'N',8,1);
        modbus_set_slave(ctx, 1);
    }

    if (ctx == NULL) {
        fprintf(stderr, "无法创建Modbus上下文\n");
        return MODBUS_STATUS::CONNECT_ERR;
    }

    // 连接到Modbus服务器
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "无法连接到Modbus服务器\n");
        modbus_free(ctx);
//        QLOG_WARN()<<"modbus connect fail ip:"<<m_remote_ip_port<<"modbus port:"<<m_remote_port_baud;
        return MODBUS_STATUS::CONNECT_ERR;
    }

    if(rw_ctrl == MODBUS_READ_SIGN){
        // 读取保持寄存器的值
        if (modbus_read_registers(ctx, starting_address, num_registers, value) == -1) {
            fprintf(stderr, "读取寄存器失败\n");
            modbus_close(ctx);
            modbus_free(ctx);
            return MODBUS_STATUS::RES_TIMEOUT;
        }
    }
    else if(rw_ctrl == MODBUS_WRITE_SIGN){
        // 写入保持寄存器的值
        // 设置要写入的数据到data数组中
        if (modbus_write_registers(ctx, starting_address, num_registers, value) == -1) {
            fprintf(stderr, "写入寄存器失败\n");
            modbus_close(ctx);
            modbus_free(ctx);
            return MODBUS_STATUS::RES_TIMEOUT;
        }
    }

    // 断开与Modbus服务器的连接
    modbus_close(ctx);
    modbus_free(ctx);

    return RES_OK;
}

MODBUS_STATUS ModbusClientDll::sendMessageToPLC(int rw_ctrl, int starting_address, int num_registers)
{
    modbus_t *ctx=nullptr;
    uint16_t value[] = {0};

    if(modbus_mode == MODBUS_TYPE::MODBUS_TCP){
        QByteArray ba = m_remote_ip_port.toLatin1();
        const char *ip_address = ba.data();
        int port = m_remote_port_baud;
//        QLOG_WARN()<<"modbus connect ip:"<<m_remote_ip_port<<"modbus port:"<<m_remote_port_baud;

        // 创建Modbus TCP上下文
        ctx = modbus_new_tcp(ip_address, port);
        modbus_set_slave(ctx, 1);
        modbus_set_debug(ctx, TRUE);
    }else if(modbus_mode == MODBUS_TYPE::MODBUS_COM){
        QByteArray ba = m_remote_ip_port.toLatin1();
        const char *ip_address = ba.data();
        int port = m_remote_port_baud;
        ctx = modbus_new_rtu(ip_address,port,'N',8,1);
        modbus_set_slave(ctx, 1);
    }

    if (ctx == NULL) {
        fprintf(stderr, "无法创建Modbus上下文\n");
        return MODBUS_STATUS::CONNECT_ERR;
    }

    // 连接到Modbus服务器
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "无法连接到Modbus服务器\n");
        modbus_free(ctx);
//        QLOG_WARN()<<"modbus connect fail ip:"<<m_remote_ip_port<<"modbus port:"<<m_remote_port_baud;
        return MODBUS_STATUS::CONNECT_ERR;
    }

    if(rw_ctrl == MODBUS_READ_SIGN){
        // 读取保持寄存器的值
        if (modbus_read_registers(ctx, starting_address, num_registers, value) == -1) {
            fprintf(stderr, "读取寄存器失败\n");
            modbus_close(ctx);
            modbus_free(ctx);
            return MODBUS_STATUS::RES_TIMEOUT;
        }
        else
        {
            QVector<unsigned short> tmpData;
            for(int i=0;i<num_registers;i++)
                tmpData.push_back(value[i]);
            emit signal_to_business(tmpData);
        }
    }
    else if(rw_ctrl == MODBUS_WRITE_SIGN){
        // 写入保持寄存器的值
        // 设置要写入的数据到data数组中
        if (modbus_write_registers(ctx, starting_address, num_registers, value) == -1) {
            fprintf(stderr, "写入寄存器失败\n");
            modbus_close(ctx);
            modbus_free(ctx);
            return MODBUS_STATUS::RES_TIMEOUT;
        }
    }

    // 断开与Modbus服务器的连接
    modbus_close(ctx);
    modbus_free(ctx);

    return RES_OK;
}
