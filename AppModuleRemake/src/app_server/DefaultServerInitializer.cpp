#include "abstractions/ServerInitializer.h"
#include "ServerConfiguration.h"

namespace Server {
    class DefaultServerInitializer : public Abstraction::ServerInitializer {
        public:
            DefaultServerInitializer(const std::vector<std::shared_ptr<Abstraction::Controller>>& controllers) 
            : Abstraction::ServerInitializer(controllers) {}
            void initialize() override;
            bool initialized() const override;
        private:
            void initializeControllers() override;
    };

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