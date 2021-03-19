#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

class Database
{
public:
    Database();
    void connectDB();
};

#endif // DATABASE_H
