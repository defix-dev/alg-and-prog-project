#include "../abstractions/DataModificator.h"
#include <pqxx/pqxx>

template<>
void DataModificator<pqxx::connection>::modify(const std::string& id, const Data& data) {
    pqxx::connection* c = m_db.get();
    pqxx::work w(*c);

    std::stringstream ss;
    ss << "UPDATE " << m_tableNm << " SET ";
    for(const auto& field : data.getFields()) {
        std::string v = field.getValue();
        v = !std::all_of(v.begin(), v.end(), ::isdigit) ? "'" + v + "'" : v;
        ss << field.getName() << "=" << v << " ";
    }
    ss << "WHERE id=" << id;

    w.exec(ss.str());
    w.commit();
}