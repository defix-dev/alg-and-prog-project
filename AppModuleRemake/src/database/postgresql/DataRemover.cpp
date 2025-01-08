#include "../abstractions/DataRemover.h"

namespace Database {
    namespace Abstraction {
        void DataRemover<pqxx::connection>::remove(const std::string& id) {
            pqxx::connection* c = m_db.get();
            pqxx::work w(*c);

            std::stringstream ss;
            ss << "DELETE FROM " << m_tableNm << " WHERE id=" << id;

            w.exec(ss.str());
            w.commit();
        }
    }
}