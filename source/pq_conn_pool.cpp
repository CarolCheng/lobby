#include "pq_conn_pool.h"
#include "db_config.h"

pq_conn_pool* pq_conn_pool::instance_ = nullptr;

pq_conn_pool::pq_conn_pool(std::string conn_string, size_t maxsize):
    cursize_(0), maxsize_(maxsize), conn_string_(conn_string)
{
    init_conn(maxsize_);
}
pq_conn_pool::~pq_conn_pool()
{
    this->release_pool();
}
void pq_conn_pool::init_conn(size_t size)
{
    std::lock_guard<std::mutex> locker(lock_);
    while( cursize_ < size ) {
        pqconnptr dbconn = std::make_shared<pqxx::connection>(conn_string_.c_str());
        if (dbconn->is_open()) {
            dbpool_.push_back(dbconn);
            ++cursize_;
        } else {
            perror("create connection failed!!");
        }
    }
}

pqconnptr pq_conn_pool::burrow()
{
    std::lock_guard<std::mutex> locker(lock_);
    if (!dbpool_.empty()) {
        pqconnptr dbconn= dbpool_.front();
        dbpool_.pop_front();
        if (!dbconn->is_open()) {
            dbconn.reset(new pqxx::connection(conn_string_.c_str()));
        }
        --cursize_; 
        return dbconn;
    }
    return nullptr;
}
bool pq_conn_pool::unburrow(pqconnptr dbconn)
{
    std::lock_guard<std::mutex> locker(lock_);
    if (dbconn) {
        dbpool_.push_back(dbconn);
        ++cursize_;
        return true;
    }
    return false;
}
void pq_conn_pool::release_pool()
{
    std::lock_guard<std::mutex> locker(lock_);
    for (auto iter = dbpool_.begin(); iter != dbpool_.end(); iter++) {
        this->release_connection(*iter);
    }
    cursize_ = 0;
    dbpool_.clear();
}
void pq_conn_pool::release_connection(pqconnptr conn)
{
    if (conn->is_open()) {
        try {
            conn->disconnect();
        } catch (const std::exception &ex) {
            perror(ex.what());
        }
        conn.reset();
    }
}
pq_conn_pool* pq_conn_pool::instance()
{
    if (instance_ == nullptr) {
        std::string connstr = "dbname="+db_config::dbname
        + " user=" + db_config::user 
        + " password=" + db_config::password
        + " hostaddr=" + db_config::hostname
        + " port=" + db_config::port;
        instance_ = new pq_conn_pool(connstr, 10);
    }
    return instance_;
}