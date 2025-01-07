#include<pqxx/pqxx>
#include "../abstractions/DataAdapter.h"

namespace Database {
    namespace Abstraction {
        template<>
        Data DataAdapter<pqxx::connection>::getDataById(const std::string& id) {
            pqxx::connection* c = m_db.get();
            pqxx::work w(*c);

            std::stringstream ss;
            ss << "SELECT * FROM " << m_tableNm << " WHERE id=" << id;
            pqxx::result res = w.exec(ss.str());

            std::vector<Field> fields;
            for (const auto& row : res) {
                for(size_t i = 0; i < row.size(); ++i) {
                    if(row[i].is_null()) {
                        continue;
                    }
                    fields.push_back(Field(res.column_name(i), row[i].as<std::string>()));
                }
            }
            w.commit();

            return Data(fields);
        }
    }
}