#include "abstractions/CourseRestController.h"
#include "Authentication.h"
#include "Server.h"
#include "Database.h"
#include "Jwt.h"

namespace Server {
    void CourseRestController<pqxx::connection>::initialize(const std::shared_ptr<crow::SimpleApp>& app) {
        CROW_ROUTE((*app.get()), "/api/courses/get_courses_list")([this](const crow::request& req) {
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }
            try {
                Database::Abstraction::DataAdapter<pqxx::connection> adapter(m_db, Database::Configuration::Postgresql::Tables::COURSES);
                auto courses = adapter.getDatasByMask("");
                auto clist = nlohmann::json::array();
                for (const auto& course : courses) {
                    if(!course.getFields().empty()) clist.push_back(course.toJson());
                }
                return crow::response(nlohmann::json{
                    { "courses", clist }
                }.dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/courses/get_course_info")([this](const crow::request& req) {
            const char* id = req.url_params.get("id");
            if(!id) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }
            try {
                auto courseInfo = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::COURSES)
                .getDatasById(std::string(id), "name, description, teacher_id, is_deleted");
                if(!(!courseInfo.empty() && !courseInfo[0].getFields().empty())) return crow::response(crow::status::NOT_FOUND);
                return crow::response(courseInfo[0].toJson().dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/courses/set_course_info").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req) {
            auto j = crow::json::load(req.body);
            int id = j["id"].i();
            std::string name = j.has("name") ? std::string(j["name"].s()) : "";
            std::string description = j.has("description") ? std::string(j["description"].s()) : "";
            if(id == 0) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Courses::SET_COURSE_INFO);
            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchCourseIdOwner<pqxx::connection>(id, req, m_db))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }
            try {
                Database::Abstraction::DataModificator<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::COURSES)
                .modify(std::to_string(id), Database::Abstraction::Data({Database::Abstraction::Field("name", name), Database::Abstraction::Field("description", description)}));
                return crow::response(crow::status::OK);
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/courses/get_tests_list").methods(crow::HTTPMethod::GET)([this](const crow::request& req) {
            const char* id = req.url_params.get("id");
            if(!id) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Courses::GET_TESTS_LIST);
            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN
                 && (Auth::AccessRequestValidator::matchCourseIdOwner<pqxx::connection>(std::stoi(std::string(id)), req, m_db) || 
                 Auth::AccessRequestValidator::matchCourseIdSubscriber<pqxx::connection>(std::stoi(std::string(id)), req, m_db)))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                auto jtests = nlohmann::json::array();
                auto tests = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::TESTS)
                .getDatasByMask("course_id="+std::string(id));
                if(tests.empty()) return crow::response(crow::status::NOT_FOUND);
                for (const auto& test : tests) {
                    if(!test.getFields().empty()) jtests.push_back(test.toJson());
                }
                return crow::response(nlohmann::json{
                    { "tests", jtests }
                }.dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/courses/get_test_status").methods(crow::HTTPMethod::GET)([this](const crow::request& req) {
            const char* testId = req.url_params.get("test_id");
            if(!testId) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Courses::GET_TEST_STATUS);

            std::string courseId = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
            Database::Configuration::Postgresql::Tables::TESTS).getDatasById(std::string(testId),"course_id")[0]["course_id"];

            if(courseId.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);

            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN
                 && (Auth::AccessRequestValidator::matchCourseIdOwner<pqxx::connection>(std::stoi(std::string(courseId)), req, m_db) || 
                 Auth::AccessRequestValidator::matchCourseIdSubscriber<pqxx::connection>(std::stoi(std::string(courseId)), req, m_db)))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                auto data = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::TESTS)
                .getDatasByMask("id="+std::string(testId), "is_active");
                if(!(!data.empty() && !data[0]["is_active"].empty())) return crow::response(crow::status::NOT_FOUND);
                return crow::response(nlohmann::json{
                    { "status", data[0]["is_active"] }
                }.dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/courses/set_test_status").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req) {
            const char* testId = req.url_params.get("id");
            const char* status = req.url_params.get("status");
            if(!testId || !status) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Courses::SET_TEST_STATUS);

            std::string courseId = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
            Database::Configuration::Postgresql::Tables::TESTS).getDatasById(std::string(testId),"course_id")[0]["course_id"];

            if(courseId.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);

            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchCourseIdOwner<pqxx::connection>(std::stoi(courseId), req, m_db))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                Database::Abstraction::DataModificator<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::TESTS)
                .modify(std::string(testId), Database::Abstraction::Data({Database::Abstraction::Field("is_active", std::string(status))}));
                if(status) return crow::response(crow::status::OK);
                auto attemptIds = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::ATTEMPTS)
                .getDatasByMask("test_id="+std::string(testId), "id");
                if(attemptIds.empty()) return crow::response(crow::status::NOT_FOUND);
                auto attMod = Database::Abstraction::DataModificator<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::ATTEMPTS);
                for(const auto& id : attemptIds) {
                    attMod.modify(id["id"], Database::Abstraction::Data({Database::Abstraction::Field("is_completed", "true")}));
                }
                return crow::response(crow::status::OK);
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/courses/add_test").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
            auto j = crow::json::load(req.body);
            int courseId = j["course_id"].i();
            std::string name = j["name"].s();
            std::string description = j["description"].s();
            if(courseId == 0 || name.empty() || description.empty()) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Courses::ADD_TEST);

            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchCourseIdOwner<pqxx::connection>(courseId, req, m_db))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                std::string id = Database::Abstraction::DataUploader<pqxx::connection>(m_db, 
                Database::Configuration::Postgresql::Tables::TESTS).uploadWithIdOutput(Database::Postgresql::TestWorkpiece
                ::get(name, description, std::to_string(courseId), "", ""));
                if(id.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
                return crow::response(nlohmann::json{
                    { "test_id", id }
                }.dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/courses/delete_test").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req) {
            const char* testId = req.url_params.get("test_id");
            if(!testId) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Courses::DELETE_TEST);

            std::string courseId = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
            Database::Configuration::Postgresql::Tables::TESTS).getDatasById(std::string(testId),"course_id")[0]["course_id"];

            if(courseId.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);

            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchCourseIdOwner<pqxx::connection>(std::stoi(courseId), req, m_db))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                Database::Abstraction::DataModificator<pqxx::connection>(m_db, 
                Database::Configuration::Postgresql::Tables::TESTS).modify(std::string(testId), Database::Abstraction
                ::Data({Database::Abstraction::Field("is_deleted", "true")}));
                return crow::response(crow::status::OK);
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/courses/get_users_list").methods(crow::HTTPMethod::GET)([this](const crow::request& req) {
            const char* id = req.url_params.get("course_id");
            if(!id) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Courses::GET_USERS_LIST);

            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchCourseIdOwner<pqxx::connection>(std::stoi(std::string(id)), req, m_db))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                auto users = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::USERS_COURSES)
                .getDatasByMask("course_id="+std::string(id), "user_id");
                Database::Abstraction::DataAdapter<pqxx::connection> usersAd(m_db,
                Database::Configuration::Postgresql::Tables::USERS);
                auto jusers = nlohmann::json::array();
                for(const auto& user : users) {
                    if(std::string userId = user["user_id"]; !userId.empty()) {
                        if(auto data = usersAd.getDatasById(userId)[0]; !data.getFields().empty())
                            jusers.push_back(data.toJson());
                    }
                }
                return crow::response(nlohmann::json{
                    { "users", jusers }
                }.dump());
            } catch(std::exception& e) { 
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/courses/add_user").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
            const char* courseId = req.url_params.get("course_id");
            const char* userId = req.url_params.get("user_id");
            if(!courseId || !userId) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Courses::ADD_USER);

            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchUserId(std::stoi(std::string(userId)), req))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                Database::Abstraction::DataUploader<pqxx::connection>(m_db,
                 Database::Configuration::Postgresql::Tables::USERS_COURSES).upload(
                    Database::Postgresql::UserCourseWorkpiece::get(std::string(userId), std::string(courseId))
                 );
                 return crow::response(crow::status::OK);
            } catch(std::exception& e) { 
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/courses/delete_user").methods(crow::HTTPMethod::Delete)([this](const crow::request& req) {
            const char* courseId = req.url_params.get("course_id");
            const char* userId = req.url_params.get("user_id");
            if(!courseId || !userId) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Courses::DELETE_USER);

            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchUserId(std::stoi(std::string(userId)), req))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                Database::Abstraction::DataRemover<pqxx::connection>(m_db,
                 Database::Configuration::Postgresql::Tables::USERS_COURSES)
                 .removeByMask("user_id="+std::string(userId)+" AND "+"course_id="+std::string(courseId));
                 return crow::response(crow::status::OK);
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/courses/add_course").methods(crow::HTTPMethod::Post)([this](const crow::request& req) {
            auto j = crow::json::load(req.body);
            std::string name = j["name"].s();
            std::string description = j["description"].s();
            int teacherId = j["teacher_id"].i();
            if(name.empty() || description.empty() || teacherId == 0) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Courses::ADD_COURSE);

            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }

            try {
                Database::Abstraction::DataUploader<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::COURSES)
                .upload(Database::Postgresql::CourseWorkpiece::get(
                    name, description, std::to_string(teacherId), ""
                ));
                return crow::response(crow::status::OK);
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/courses/delete_course").methods(crow::HTTPMethod::Patch)([this](const crow::request& req) {
            const char* courseId = req.url_params.get("id");
            if(!courseId) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Courses::ADD_COURSE);

            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchCourseIdOwner<pqxx::connection>(std::stoi(std::string(courseId)), req, m_db))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                Database::Abstraction::DataModificator<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::COURSES)
                .modify(std::string(courseId), Database::Abstraction::Data({
                    Database::Abstraction::Field("is_deleted", "true")
                }));
                return crow::response(crow::status::OK);
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });
    }
}