#include "database.h"

Database::Database() {}

void Database::connectDB()
{
    QString dbPath(QCoreApplication::applicationDirPath()+"/../../ProjectManager/database.db");

    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(dbPath);
    db.open();
}

void Database::disConnectDB()
{
    this->db.close();
}

QSqlDatabase Database::getDB()
{
    return db;
}

QSqlQuery Query(QString q)
{
    QSqlQuery query(q);
    return query;
}
