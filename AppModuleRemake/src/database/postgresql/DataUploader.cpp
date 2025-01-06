#include "../abstractions/DataUploader.h"
#include <pqxx/pqxx>
#include<cctype>
#include<string>

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
        if(!first) {
            ssK << ",";
            ssV << ",";
        }
        first = false;

        ssK << field.getName();

        std::string v = field.getValue();
        v = !std::all_of(v.begin(), v.end(), ::isdigit) ? "'" + v + "'" : v;

        ssV << v;
    }

    ssK << ")";
    ssV << ")";

    ssM << ssK.str() << " VALUES " << ssV.str();

    w.exec(ssM.str());
    w.commit();
}