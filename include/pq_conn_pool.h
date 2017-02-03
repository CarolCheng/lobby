#ifndef _PQ_CONN_POOL
#define _PQ_CONN_POOL
#include <memory>
#include <list>
#include <mutex>
#include <pqxx/pqxx> 

typedef std::shared_ptr<pqxx::connection> pqconnptr;

class pq_conn_pool 
{
public:
    static pq_conn_pool* instance();
    ~pq_conn_pool();
    void release_pool();
    pqconnptr burrow();
    bool unburrow(pqconnptr dbcon);
private:
    pq_conn_pool(const pq_conn_pool&) = delete;
    pq_conn_pool& operator=(const pq_conn_pool&) = delete;
private:
    pq_conn_pool(std::string conn_string, size_t maxsize);
    void init_conn(size_t size);
    void release_connection(pqconnptr conn);
private:
    size_t cursize_;
    size_t maxsize_;
    std::string conn_string_;
    std::list<pqconnptr> dbpool_;
    static pq_conn_pool *instance_;
    std::mutex lock_;
};

#endif //_PQ_CONN_POOL