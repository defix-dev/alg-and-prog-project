#ifndef DATA_H
#define DATA_H

#include<iostream>
#include<vector>
#include<nlohmann/json.hpp>

namespace Database {
    namespace Abstraction {
        struct Field
        {
        public:
            Field(const std::string& name, const std::string& value) :
            m_name(name), m_value(value) {}
            const std::string& getName() const { return m_name; }
            const std::string& getValue() const { return m_value; }
        private:
            std::string m_name;
            std::string m_value;
        };

        struct Data
        {
            public:
                Data(const std::vector<Field>& fields) : m_fields(fields) {}
                const std::vector<Field>& getFields() const {
                    return m_fields;
                }
                const std::string& operator[](const std::string& name) const {
                    for (const auto& field : m_fields) {
                        if (field.getName() == name) {
                            return field.getValue();
                        }
                    }
                    throw std::out_of_range("Field not found: " + name);
                }
                nlohmann::json toJson() const {
                    nlohmann::json j;
                    for (const auto& field : m_fields) {
                        j[field.getName()] = field.getValue();
                    }
                    return j;
                }
            private:
                std::vector<Field> m_fields;
        };
    }
}

#endif