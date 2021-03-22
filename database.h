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
    bool Login(QString u, QString p);
    void disConnectDB();
    QSqlQuery Query(QString q);
    int checkInfo(QString u, QString e);
    bool Signup(QString fn, QString ln, QString u, QString p, QString e);
    void initialQuery();
    bool Update(QString fn, QString ln, QString u, QString p, QString e);
private:
    static QSqlDatabase db;
};

#endif // DATABASE_H
