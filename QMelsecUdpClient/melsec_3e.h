#ifndef MELSEC_3E_H
#define MELSEC_3E_H
#include <QTcpSocket>
#include <QByteArray>
#include <QBitArray>
#include <QVector>
#include <QUdpSocket>

class  Melsec_Comm_3E_ASCII
{
public:
    int CPUtype(QTcpSocket * socket, uint32_t timeout, QString &cpu_string, uint16_t &cpu_code);
    int run(QTcpSocket * socket, uint32_t timeout, int mode, int clear_mode);
    int stop(QTcpSocket * socket, uint32_t timeout, int mode);
    int reset(QTcpSocket * socket, uint32_t timeout, int mode);
    int pause(QTcpSocket * socket, uint32_t timeout, int mode);
    int loopback(QTcpSocket * socket, uint32_t timeout, const QByteArray &in_data, QByteArray &out_data);// 折返测试

    QBitArray readBits(QTcpSocket * socket, uint32_t timeout, char code[2], uint32_t addr, uint16_t N);
    QByteArray readBytes(QTcpSocket * socket, uint32_t timeout, uint16_t code, uint8_t addr, uint16_t N);
    QByteArray writeBytes(QTcpSocket * socket, uint32_t timeout, uint16_t code, uint8_t addr, uint16_t N, uint16_t values[]);
};

enum SoftCode {X, Y, M, L, S, B, F, TS, TC, TN, CS, CC, CN, D, W, R, ZR };
class  CommandFrame_MC_3E_ASCII
{
    QByteArray frame();
protected:
    QByteArray m_data;
protected:
    QByteArray textLength(QByteArray text);
    QByteArray address(uint32_t addr);
    QByteArray uint16ToHex(uint16_t x);
    QByteArray uint24ToHex(uint32_t x);
    QByteArray uint32ToHex(uint32_t x);
    QByteArray uint48ToHex(uint64_t x);
    QByteArray uint64ToHex(uint64_t x);
};

/**
 * @brief The CommandFrame_1001_ASCII class 远程 RUN 命令
 */
class  CommandFrame_1001_ASCII : public CommandFrame_MC_3E_ASCII
{
public:
    QByteArray build(int mode, int clear_mode);
};

/**
 * @brief The CommandFrame_1002_ASCII class 远程 STOP 命令
 */
class  CommandFrame_1002_ASCII : public CommandFrame_MC_3E_ASCII
{
public:
    QByteArray build(int mode);
};

/**
 * @brief The CommandFrame_1005_ASCII class 远程锁存清除 命令
 */
class  CommandFrame_1005_ASCII : public CommandFrame_MC_3E_ASCII
{
public:
    QByteArray build(int mode);
};

/**
 * @brief The CommandFrame_0101_ASCII class CPU 型号读出 命令
 */
class  CommandFrame_0101_ASCII : public CommandFrame_MC_3E_ASCII
{
public:
    QByteArray build();
};

/**
 * @brief The CommandFrame_1006_ASCII class 远程 RESET 命令
 */
class  CommandFrame_1006_ASCII : public CommandFrame_MC_3E_ASCII
{
public:
    QByteArray build(int mode);
};

/**
 * @brief The CommandFrame_1003_ASCII class 远程 PAUSE 命令
 */
class CommandFrame_1003_ASCII : public CommandFrame_MC_3E_ASCII
{
public:
    QByteArray build(int mode);
};

class CommandFrame_0619_ASCII : public CommandFrame_MC_3E_ASCII
{
public:
    QByteArray build(QByteArray t);
};

/**
 * @brief The CommandFrame_1401_ASCII class 成批写入命令
 *  用于将数据写入指定个数的连续软元件中。
 */
class  CommandFrame_1401_ASCII : public CommandFrame_MC_3E_ASCII
{
    /**
     * @brief build 以位为单位写入软元件。对应子命令 0001
     * @param code 指定软元件类型，比如 "M*", "D*"
     * @param addr 软元件的起始地址
     * @param array 数据
     * @return
     */
    QByteArray build(char code[2], uint32_t addr, QBitArray array);

    /**
     * @brief build 以字为单位写入软元件。对应子命令 0000
     * @param code 指定软元件类型，比如 "M*", "D*"
     * @param addr 软元件的起始地址
     * @param array 数据，每个uint16_t 对应 16 个线圈
     * @return
     */
    QByteArray build(char code[2], uint32_t addr, QVector<uint16_t> array);
private:
    QByteArray bitToArray(QBitArray array);
};


class CommandFrame_0401_ASCII : public CommandFrame_MC_3E_ASCII
{
public:
    QByteArray readBits(char code[2], uint32_t addr, uint16_t N);
    QByteArray readBytes(uint16_t code, uint8_t addr, uint16_t N);
    QByteArray writeBytes(uint16_t code, uint8_t addr, uint16_t N, uint16_t values[]);
};


class ResponseFrame_MC_3E_ASCII
{
public:
    ResponseFrame_MC_3E_ASCII(QByteArray frame);
    bool parser(QByteArray frame);
    uint16_t errorCode();
    QByteArray responseData();
    QByteArray errorData();
protected:
    QByteArray m_data;
};

class ErrorData_MC_3E_ASCII
{
    /**
     * @brief parser 用来接收 ResponseFrame_MC_3E_ASCII 的 errorData()
     * @param data ResponseFrame_MC_3E_ASCII 的 errorData()
     * @return
     */
    bool parser(QByteArray data);
    QByteArray netNumber();
    QByteArray PLCNumber();
    QByteArray IONumber();
    QByteArray command();
    QByteArray subCommand();
protected:
    QByteArray m_data;
};

class Melsec_3E
{
public:
    Melsec_3E();
};

#endif // MELSEC_3E_H
