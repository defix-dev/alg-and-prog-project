#include "../abstractions/DataModificator.h"

namespace Database {
    namespace Abstraction {
        void DataModificator<pqxx::connection>::modifyByMask(const std::string& mask, const Data& data) {
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
                v = (!std::all_of(v.begin(), v.end(), ::isdigit) && v.find("ARRAY[") == std::string::npos) ? "'" + v + "'" : v;
                ss << field.getName() << "=" << v;
            }
            std::string mmask = " WHERE " + mask;

            if(!mask.empty()) ss << mmask;

            std::string res = ss.str();
            std::stringstream ssC;
            ssC << "UPDATE " << m_tableNm << " SET";
            if(!mask.empty()) ssC << mmask;
            if(res == ssC.str()) throw std::runtime_error("No data to modify");

            w.exec(ss.str());
            w.commit();
        }

        void DataModificator<pqxx::connection>::modify(const std::string& id, const Data& data) {
            modifyByMask("id="+id, data);
        }
    }
}