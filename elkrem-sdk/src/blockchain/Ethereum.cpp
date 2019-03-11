#include "Ethereum.h"

void EthereumClass::requestAccountBalance(const char *address)
{
    EthereumGetBalanceRequest eab = EthereumGetBalanceRequest_init_zero;
    strcpy(eab.address, address);
    pb_ostream_t ostream = pb_ostream_from_buffer(ElkremClass::protobufBuffer, sizeof(ElkremClass::protobufBuffer));
    bool status = pb_encode(&ostream, EthereumGetBalanceRequest_fields, &eab);
    if (status)
    {
        Elkrem.sendMessage(SDKMessageType_REQUEST_ETHEREUM_GET_BALANCE, ElkremClass::protobufBuffer, ostream.bytes_written);
    }
}

void EthereumClass::requestCurrentBlock()
{
    Elkrem.sendMessage(SDKMessageType_REQUEST_ETHEREUM_GET_BLOCKNUMBER, 0, 0);
}

void EthereumClass::onAccountBalanceResponse(void (*_balanceResponseCallBack)(const char *, uint64_t))
{
    balanceResponseCallBack = _balanceResponseCallBack;
}

void EthereumClass::processData(SDKMessageType type, uint8_t *data, uint16_t length)
{
    if (type == SDKMessageType_RESPONSE_ETHEREUM_GET_BALANCE)
    {
        EthereumGetBalanceResponse eab = EthereumGetBalanceResponse_init_zero;
        pb_istream_t istream = pb_istream_from_buffer(data, length);
        bool status = pb_decode(&istream, EthereumGetBalanceResponse_fields, &eab);
        if (status && balanceResponseCallBack)
        {
            balanceResponseCallBack(eab.address, eab.balance);
        }
    }
    else if (type == SDKMessageType_RESPONSE_ETHEREUM_TRACK_ACCOUNT_CALLBACK)
    {
        EthereumTrackAccountTransactionsCallBackResponse tx = EthereumTrackAccountTransactionsCallBackResponse_init_zero;
        pb_istream_t istream = pb_istream_from_buffer(data, length);
        bool status = pb_decode(&istream, EthereumTrackAccountTransactionsCallBackResponse_fields, &tx);
        if (status && transactionReceivedCallback)
        {
            transactionReceivedCallback(&tx);
        }
    }
}

void EthereumClass::trackAccount(const char *toAddress)
{
    EthereumTrackAccountTransactionsRequest eas = EthereumGetBalanceRequest_init_zero;
    snprintf(eas.account_address,43, toAddress);
    pb_ostream_t ostream = pb_ostream_from_buffer(ElkremClass::protobufBuffer, sizeof(ElkremClass::protobufBuffer));
    bool status = pb_encode(&ostream, EthereumTrackAccountTransactionsRequest_fields, &eas);
    if (status)
    {
        Elkrem.sendMessage(SDKMessageType_REQUEST_ETHEREUM_TRACK_ACCOUNT_TRANSACTIONS, ElkremClass::protobufBuffer, ostream.bytes_written);
    }
}

void EthereumClass::onNewTransactionReceived(void (*_transactionReceivedCallback)(EthereumTrackAccountTransactionsCallBackResponse * tx))
{
    transactionReceivedCallback = _transactionReceivedCallback;
}

EthereumClass Ethereum;
