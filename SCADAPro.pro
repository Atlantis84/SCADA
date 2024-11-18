TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
         .\CentralControl \
         FinsUDPService \
         DMDBAccess \
         HttpBasedOnCurl \
         ModbusClientDll \
         QMelsecUdpClient \
         QMqServiceCommonUse \
         QsLog \
         TcpCommonUse\
         XmlRpc \
         QMqServiceSrc
