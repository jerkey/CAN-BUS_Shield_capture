#include <SPI.h>
#include "mcp_can.h" // cd libraries && git clone https://github.com/Seeed-Studio/CAN_BUS_Shield.git

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10; // keith discovered setting this to 9 does not work
long time_seconds = 0; // how many seconds since the last time we printed header

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail!   Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
    printHeader();
}

void loop()
{
    if (millis() / 1000 > time_seconds++) printHeader(); // print header once per second

    unsigned char len = 0;
    unsigned char buf[8];

    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned int canId = CAN.getCanId();
        
        Serial.print(canId, HEX);

        for(int i = 0; i<8; i++)    // print the data
        {
            Serial.print("\t"); // print a tab no matter what
            if (i < len) Serial.print(buf[i], HEX); // only print the number of bytes in packet
        }
        Serial.print("\t"); // print a tab
        Serial.println(millis() / 1000.0, 2); // print number of seconds to two decimal places
    }
}

void printHeader()
{
    Serial.print("CAN_ID\t");
    for(int i = 0; i<8; i++) {
        Serial.print("\t");
        Serial.print(i); // print data byte number
    }
    Serial.println("\ttime");
}
