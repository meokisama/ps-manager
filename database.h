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
    bool connectDB();
    bool Login(QString, QString);
    void disConnectDB();
    QSqlQuery Query(QString);
    bool checkInfo(QString);
    bool Signup(QString, QString, QString, QString, QString);
    void initialQuery();
    bool Update(QString, QString, QString, QString, QString);
    void addProject(QString, QString, int, int);
    void delProject(QString);
    int findMaxID();
private:
    static QSqlDatabase db;
};

#endif // DATABASE_H
