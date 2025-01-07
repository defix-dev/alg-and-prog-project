#include "../abstractions/DataUploader.h"
#include <pqxx/pqxx>
#include<cctype>
#include<string>

namespace Database {
    namespace Abstraction {
        template<>
        void DataUploader<pqxx::connection>::upload(const Data& data) {
            pqxx::connection* c = m_db.get();
            pqxx::work w(*c);

            std::stringstream ssM, ssK, ssV;

            ssM << "INSERT INTO " << m_tableNm;
            ssK << "(";
            ssV << "(";

            bool first = true;
            for(const auto& field : data.getFields()) {
                std::string v = field.getValue();
                if(v.empty()) continue;
                
                if(!first) {
                    ssK << ",";
                    ssV << ",";
                }
                first = false;

                v = !std::all_of(v.begin(), v.end(), ::isdigit) ? "'" + v + "'" : v;

                ssK << field.getName();
                ssV << v;
            }

            ssK << ")";
            ssV << ")";

            ssM << ssK.str() << " VALUES " << ssV.str();

            w.exec(ssM.str());
            w.commit();
        }
    }
}