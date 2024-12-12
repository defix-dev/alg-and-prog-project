#include <pqxx/pqxx>  // подключаем библиотеку libpqxx
#include <iostream>
#include <string>

int main() {
    try {
        // Подключаемся к базе данных PostgreSQL
        pqxx::connection c("dbname=sigma_squad_db user=postgres password=root host=localhost port=8080");

        // Проверка на успешное подключение
        if (!c.is_open()) {
            std::cerr << "Can't open database" << std::endl;
            return 1;
        }

        std::cout << "Opened database successfully: " << c.dbname() << std::endl;

        // Создаем транзакцию
        pqxx::work txn(c);

        // Выполняем SQL-запрос
        pqxx::result r = txn.exec("CREATE TABLE test_t(id int);");

        // Обрабатываем результаты
        for (auto row : r) {
            // Извлекаем значения из каждой строки результата
            int id = row["id"].as<int>();        // Преобразуем поле "id" в int
            std::string name = row["name"].as<std::string>();  // Преобразуем поле "name" в строку

            std::cout << "ID = " << id << ", Name = " << name << std::endl;
        }

        // Подтверждаем транзакцию
        txn.commit();
    } catch (const std::exception &e) {
        // Обработка ошибок
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
