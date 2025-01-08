#ifndef ACCESS_TOKEN_BODY_H
#define ACCESS_TOKEN_BODY_H

#include <iostream>
#include <vector>

namespace Jwt {
    struct AccessTokenBody {
            public:
                AccessTokenBody(int id, const std::vector<std::string>& permissions) :
                m_id(id), m_permissions(permissions) {}
                std::vector<std::string> getPermissions() const { return m_permissions; }
                int getId() const { return m_id; }
            private:
                std::vector<std::string> m_permissions;
                int m_id;
    };
}

#endif