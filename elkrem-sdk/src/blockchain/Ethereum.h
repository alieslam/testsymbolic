#ifndef Ethereum_h
#define Ethereum_h
#include "Elkrem.h"

class EthereumClass
{
    friend class ElkremClass;

  public:
    void requestAccountBalance(const char *);
    void requestCurrentBlock();
    void onAccountBalanceResponse(void (*)(const char *, uint64_t));
    void trackAccount(const char *);
    void onNewTransactionReceived(void (*)(EthereumTrackAccountTransactionsCallBackResponse * ));

  private:
    void processData(SDKMessageType, uint8_t *, uint16_t);
    void (*balanceResponseCallBack)(const char *, uint64_t);
    void (*transactionReceivedCallback)(EthereumTrackAccountTransactionsCallBackResponse * );
};

extern EthereumClass Ethereum;
#endif
