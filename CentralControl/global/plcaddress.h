#ifndef PLCADDRESS_H
#define PLCADDRESS_H


//for tcon
#define ADDRESS_D600 0x1770 //D6000
#define ADDRESS_D302 0x0BBA //D3002
#define ADDRESS_D000 0x0000
#define ADDRESS_D050 0x0032
#define ADDRESS_D10000 0x2710
#define ADDRESS_D10010 0x271A
#define ADDRESS_D10011 0x271B
#define ADDRESS_D10020 0x2724
#define ADDRESS_D10030 0x272E
#define ADDRESS_D10040 0x2738
#define ADDRESS_D10041 0x2739
#define WRITE_PLC 0x02
#define READ_PLC 0x01
#define WRITE_VISION 0x01
//for tcon

//for data acquisition
#define ADDRESS_TCON_TEST_STATION_W300 0x012C
#define ADDRESS_TCON_TEST_STATION_W310 0x0136
#define ADDRESS_TCON_TEST_STATION_W320 0x0140
#define ADDRESS_TCON_TEST_STATION_W370 0x0172
#define ADDRESS_TCON_TEST_STATION_W420 0x01A4

#define ADDRESS_TCON_AOI_W300 0x012C
#define ADDRESS_TCON_AOI_W320 0x0140
#define ADDRESS_TCON_AOI_W420 0x01A4

#define ADDRESS_TCON_PACKAGE_W300 0x012C
#define ADDRESS_TCON_PACKAGE_W420 0x01A4

#define ADDRESS_TEST_AOI_D3200 0x0C80
#define ADDRESS_TEST_PACKAGE_D5100 0x13EC
//for data acquisition

//for tbox-screw-machine
#define ADDRESS_SCREW_MACHINE_D10100 0x2774
#define ADDRESS_SCREW_MACHINE_D10145 0x27A1
#define ADDRESS_SCREW_MACHINE_D10202 0x27DA
#define ADDRESS_SCREW_MACHINE_D10203 0x27DB
#define ADDRESS_SCREW_MACHINE_D10204 0x27DC

//for tbox-screw-machine

//
#endif // PLCADDRESS_H
