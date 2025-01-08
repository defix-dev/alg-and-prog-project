#include "abstractions/ServerInitializer.h"
#include "ServerConfiguration.h"

namespace Server {
    namespace Abstraction {
        bool DefaultServerInitializer::initialized() const {
            return m_app.get() != nullptr;
        }

        void DefaultServerInitializer::initialize() {
            m_app = std::make_shared<crow::SimpleApp>();

            initializeControllers();

            m_app.get()->port(Configuration::Run::PORT)
            .multithreaded().run();
        }

        void DefaultServerInitializer::initializeControllers() {
            for(auto& controller : m_controllers) {
                controller.get()->initialize(m_app);
            }
        }
    }
}