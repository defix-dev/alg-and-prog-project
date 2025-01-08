#ifndef SERVER_INITIALIZER_H
#define SERVER_INITIALIZER_H

#include <memory>
#include<vector>
#include "Controller.h"
#include "crow.h"

namespace Server {
    namespace Abstraction {
        class ServerInitializer {
            public:
                ServerInitializer(const std::vector<std::shared_ptr<Controller>>& controllers) 
                : m_controllers(controllers) {}
                virtual void initialize() = 0;
                virtual bool initialized() const = 0;
            protected:
                virtual void initializeControllers() = 0;
                std::shared_ptr<crow::SimpleApp> m_app;
                std::vector<std::shared_ptr<Controller>> m_controllers;
        };

        class DefaultServerInitializer : public ServerInitializer {
            public:
                DefaultServerInitializer(const std::vector<std::shared_ptr<Controller>>& controllers) 
                : ServerInitializer(controllers) {}
                void initialize() override;
                bool initialized() const override;
            private:
                void initializeControllers() override;
        };
    }
}

#endif