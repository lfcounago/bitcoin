#include "tx_logger.h"
#include <sqlite3.h>
#include <chrono>
#include <iostream>

static sqlite3* db = nullptr;

static void InitDB() {
    if (db) return;
    int rc = sqlite3_open("/var/lib/bitcoind/tx_log.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open tx_log.db: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
        return;
    }
    const char* sql = "CREATE TABLE IF NOT EXISTS tx_log ("
                      "timestamp TEXT,"
                      "peer_id INTEGER,"
                      "ip TEXT,"
                      "txid TEXT,"
                      "wtxid TEXT"
                      ");";
    sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
}

void LogIncomingTxToDB(const CTransactionRef& tx, const std::string& peer_ip, int peer_id)
{
    InitDB();
    if (!db) return;

    std::string txid = tx->GetHash().ToString();
    std::string wtxid = tx->GetWitnessHash().ToString();

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    char timebuf[64];
    std::strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));

    std::string sql = "INSERT INTO tx_log (timestamp, peer_id, ip, txid, wtxid) VALUES ('" +
                      std::string(timebuf) + "', " +
                      std::to_string(peer_id) + ", '" +
                      peer_ip + "', '" +
                      txid + "', '" +
                      wtxid + "');";

    sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
}
