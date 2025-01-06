#include "../abstractions/DataRemover.h"
#include <pqxx/pqxx>

template<>
void DataRemover<pqxx::connection>::remove(const std::string& id) {
    pqxx::connection* c = m_db.get();
    pqxx::work w(*c);

    std::stringstream ss;
    ss << "DELETE FROM " << m_tableNm << " WHERE id=" << id;

    w.exec(ss.str());
    w.commit();
}