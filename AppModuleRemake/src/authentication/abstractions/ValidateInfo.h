#ifndef VALIDATE_INFO_H
#define VALIDATE_INFO_H

namespace Auth {
   struct ValidateInfo {
        public:
            ValidateInfo(int statusCode, bool&& isValid) :
            m_statusCode(statusCode), m_isValid(isValid) {}
            int getStatusCode() { return m_statusCode; }
            bool isValid() { return m_isValid; }
        private:
            int m_statusCode;
            bool m_isValid;
    };
}

#endif