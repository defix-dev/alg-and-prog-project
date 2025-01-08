#include "../abstractions/DataAdapter.h"

namespace Database {
    namespace Abstraction {
        std::vector<Data> DataAdapter<pqxx::connection>::getDatasByMask(const std::string& mask, const std::string& pattern) {
            pqxx::connection* c = m_db.get();
            pqxx::work w(*c);

            std::stringstream ssM{""};
            if(!mask.empty()) {
                ssM << "WHERE " << mask;
            }

            std::stringstream ss;
            ss << "SELECT " << pattern << " FROM " << m_tableNm << ssM.str();
            pqxx::result res = w.exec(ss.str());

            std::vector<Data> datas;
            for (const auto& row : res) {
                std::vector<Field> fields;
                for(size_t i = 0; i < row.size(); ++i) {
                    if(row[i].is_null()) {
                        continue;
                    }
                    fields.push_back(Field(res.column_name(i), row[i].as<std::string>()));
                }
                datas.push_back(Data(fields));
            }
            w.commit();

            return datas;
        }

        std::vector<Data> DataAdapter<pqxx::connection>::getDatasById(const std::string& id, const std::string& pattern) {
            return getDatasByMask("id="+id, pattern);
        }
    }
}