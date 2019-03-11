#include "Elkrem.h"

bool ElkremClass::_isInit = false;
uint8_t ElkremClass::protobufBuffer[MAX_PROTOBUF_LENGTH] = {0};
uint8_t ElkremClass::hdlcBuffer[MAX_HDLC_FRAME_LENGTH] = {0};
static Tiny::Packet inputPacket((char *)ElkremClass::hdlcBuffer, sizeof(ElkremClass::hdlcBuffer));

ElkremClass::ElkremClass()
{
    _isHostConnected = false;
    isHostConnectedCallBack = 0;
    _isInit = false;
}

void ElkremClass::setCommunicationPort(write_block_cb_t userWriteFunction, read_block_cb_t userReadFunction)
{
    hdlc.begin(userWriteFunction, userReadFunction);
    hdlc.enableCrc16();
    hdlc.enableUid();
}

void ElkremClass::init()
{
    _isInit = true;
    ::delay(10);
    sendMessage(SDKMessageType_PING, 0, 0);
}

void ElkremClass::waitForHostConnection()
{
    _isHostConnected = false;
    while (!_isHostConnected)
    {
        Elkrem.run();
        ::delay(0);
    }
}

void ElkremClass::write(uint8_t data)
{
    if (_isInit)
        Serial.write(data);
}

void ElkremClass::sendMessage(SDKMessageType type, uint8_t *data, uint16_t dataLength)
{
    /* Tiny protocol packet type message */
    uint8_t outputBuffer[MAX_HDLC_FRAME_LENGTH];
    Tiny::Packet outputPacket((char *)outputBuffer, sizeof(outputBuffer));
    outputPacket.putUid(static_cast<uint16_t>(type));
    for (int i = 0; i < dataLength; i++)
    {
        outputPacket.put(data[i]);
    }
    hdlc.write(outputPacket, TINY_FLAG_WAIT_FOREVER);
    memset(protobufBuffer, 0, MAX_PROTOBUF_LENGTH);
}

bool ElkremClass::isHostConnected()
{
    return _isHostConnected;
}

void ElkremClass::onHostConnection(void (*userFunction)(bool))
{
    isHostConnectedCallBack = userFunction;
}

void ElkremClass::run()
{
    // TODO send ping every 2 seconds
    int len;
    len = hdlc.read((char *)hdlcBuffer, sizeof(hdlcBuffer), TINY_FLAG_NO_WAIT);
    if (len > 0)
    {
        processFrame(hdlcBuffer, len);
    }
}

void ElkremClass::processFrame(uint8_t *buff, uint16_t len)
{
    // Byte shifting first two bytes to check on message ID.
    int16_t messageId = ((messageId) | (buff[1] << 8)) | ((messageId) | (buff[0]));
    SDKMessageType type = static_cast<SDKMessageType>(messageId);
    switch (type)
    {
    case SDKMessageType_PONG:
        Elkrem._isHostConnected = true;
        Elkrem.sendMessage(SDKMessageType_PING, 0, 0);
        break;
    case SDKMessageType_REQUEST_ETHEREUM_GET_BALANCE:
    case SDKMessageType_RESPONSE_ETHEREUM_TRACK_ACCOUNT_CALLBACK:
        Ethereum.processData(type, &buff[2], len - 2);
        break;
    default:
        break;
    }
}

void ElkremClass::delay(unsigned long time)
{
    unsigned long now = millis();
    while (millis() < (now + time))
    {
        run();
        ::delay(0);
    }
}
ElkremClass Elkrem;
