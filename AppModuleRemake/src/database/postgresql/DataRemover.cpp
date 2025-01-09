#include "../abstractions/DataRemover.h"

namespace Database {
    namespace Abstraction {
        void DataRemover<pqxx::connection>::removeByMask(const std::string& mask) {
            pqxx::connection* c = m_db.get();
            pqxx::work w(*c);

            std::stringstream ss;
            ss << "DELETE FROM " << m_tableNm << " WHERE " << mask;

            w.exec(ss.str());
            w.commit();
        }

        void DataRemover<pqxx::connection>::remove(const std::string& id) {
            removeByMask("id="+id);
        }
    }
}