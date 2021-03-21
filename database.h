#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>
#include <QCoreApplication>

class Database
{
public:
    Database();
    void connectDB();
    void disConnectDB();
    QSqlQuery Query(QString q);
    QSqlDatabase getDB();
private:
    QSqlDatabase db;
};

#endif // DATABASE_H
