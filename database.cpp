#include "database.h"

Database::Database() {}

void Database::connectDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    QSqlQuery query("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT)");
}
