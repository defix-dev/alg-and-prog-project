#include "../abstractions/DataModificator.h"

namespace Database {
    namespace Abstraction {
        void DataModificator<pqxx::connection>::modify(const std::string& id, const Data& data) {
            pqxx::connection* c = m_db.get();
            pqxx::work w(*c);

            std::stringstream ss;
            ss << "UPDATE " << m_tableNm << " SET ";
            bool first = true;
            for(const auto& field : data.getFields()) {
                std::string v = field.getValue();
                if(v.empty()) continue;
                if(!first) {
                    ss << ",";
                }
                first = false;
                v = !std::all_of(v.begin(), v.end(), ::isdigit) ? "'" + v + "'" : v;
                ss << field.getName() << "=" << v;
            }
            ss << " WHERE id=" << id;

            std::string res = ss.str();
            std::stringstream ssC;
            ssC << "UPDATE " << m_tableNm << " SET " << "WHERE id=" << id;
            if(res == ssC.str()) throw std::runtime_error("No data to modify");

            w.exec(ss.str());
            w.commit();
        }
    }
}