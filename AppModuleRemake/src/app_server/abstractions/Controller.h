#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "crow.h"
#include <memory>

namespace Server {
    namespace Abstraction {
        class Controller {
            public:
                Controller() = default;
                virtual void initialize(const std::shared_ptr<crow::SimpleApp>& app) = 0;
        };
    }
}

#endif