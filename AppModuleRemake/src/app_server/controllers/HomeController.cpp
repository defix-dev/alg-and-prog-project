#include "../abstractions/Controller.h"

namespace Server {
    class HomeController : public Abstraction::Controller {
        public:
            void initialize(const std::shared_ptr<crow::SimpleApp>& app) override {
                CROW_ROUTE((*app.get()),"/home")([]() {
                    return "Hello, world!";
                });
            }
    };
}