#ifndef Elkrem_h
#define Elkrem_h

#include "Arduino.h"
#include "TinyProtocol.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "proto/sdk.pb.h"
#include "blockchain/Ethereum.h"
#include "config.h"

class ElkremClass
{

  public:
    ElkremClass();
    void waitForHostConnection();
    bool isHostConnected();
    void onHostConnection(void (*)(bool));
    void run();
    void init();
    void delay(unsigned long);
    bool isInit();
    void sendMessage(SDKMessageType, uint8_t *, uint16_t);
    void setCommunicationPort(write_block_cb_t, read_block_cb_t);
    static uint8_t protobufBuffer[MAX_PROTOBUF_LENGTH];
    static uint8_t hdlcBuffer[MAX_HDLC_FRAME_LENGTH];

  private:
    Tiny::Proto hdlc;
    static bool _isInit;
    bool _isHostConnected;
     /**
     * Process incoming frames.
     * @param buff - data received
     *  @param len - length of the data in bytes
     * @return void
     */
    static void processFrame(uint8_t* buff, uint16_t len);
    void (*isHostConnectedCallBack)(bool);
    static void write(uint8_t data);
};

extern ElkremClass Elkrem;
#endif
