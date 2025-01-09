#include "../abstractions/DataUploader.h"
#include <pqxx/pqxx>
#include<cctype>
#include<string>

namespace Database {
    namespace Abstraction {
        std::unique_ptr<pqxx::result> DataUploader<pqxx::connection>::uploadWithResultOutput(const Data& data, bool returning) {
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

            if(returning) {
                ssM << " RETURNING id";
            }

            pqxx::result res = w.exec(ssM.str());
            w.commit();

            return std::make_unique<pqxx::result>(res);
        }

        void DataUploader<pqxx::connection>::upload(const Data& data) {
            uploadWithResultOutput(data, false);
        }

        std::string DataUploader<pqxx::connection>::uploadWithIdOutput(const Data& data) {
            return std::to_string((*uploadWithResultOutput(data).get())[0][0].as<int>());
        }
    }
}