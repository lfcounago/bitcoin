#ifndef BITCOIN_TX_LOGGER_H
#define BITCOIN_TX_LOGGER_H

#include <string>
#include <memory>
#include <primitives/transaction.h>

void LogIncomingTxToDB(const CTransactionRef& tx, const std::string& peer_ip, int peer_id);

#endif // BITCOIN_TX_LOGGER_H
