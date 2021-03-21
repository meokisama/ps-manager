#include "database.h"

Database::Database() {}

bool Database::connectDB()
{
    QString dbPath(QCoreApplication::applicationDirPath()+"/../../ProjectManager/database.db");

    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(dbPath);

    if(db.open()) return true;
    else return false;
}

void Database::disConnectDB()
{
    this->db.close();
}

QSqlQuery Query(QString q)
{
    QSqlQuery query(q);
    return query;
}

bool Database::Login(QString u, QString p)
{
    bool exist = false;

    QSqlQuery check(db);

    // 1. Check username or email with Qt function
    /*
    if (u.contains("@",Qt::CaseInsensitive))
    {
        check.prepare("SELECT USERNAME FROM USER WHERE EMAIL = (:un) AND PASSWORD = (:pw)");
    } else {
        check.prepare("SELECT USERNAME FROM USER WHERE USERNAME = (:un) AND PASSWORD = (:pw)");
    }
    */

    // 2. Or check with SQL query
    check.prepare("SELECT USERNAME FROM USER WHERE (USERNAME = (:un) OR EMAIL = (:un)) AND PASSWORD = (:pw)");

    check.bindValue(":un", u);
    check.bindValue(":pw", p);

    if (check.exec()) if (check.next()) exist = true;

    return exist;
}

int Database::checkInfo(QString u, QString e)
{
    int checker = 0;

    // Check if username is used
    QSqlQuery uQuery(db);
    uQuery.prepare("SELECT USERNAME FROM USER WHERE USERNAME = (:un)");
    uQuery.bindValue(":un", u);
    if(uQuery.exec()) if(uQuery.next()) checker = 1;

    // Check if email is used
    QSqlQuery eQuery(db);
    eQuery.prepare("SELECT EMAIL FROM USER WHERE EMAIL = (:em)");
    eQuery.bindValue(":em", e);
    if(eQuery.exec()) if(uQuery.next()) checker = 2;

    return checker;
}

bool Database::Signup(QString fn, QString ln, QString u, QString p, QString e)
{
    QSqlQuery signupQuery(db);
    signupQuery.prepare("INSERT INTO USER VALUES(:un, :fn, :ln, :em, :pw)");
    signupQuery.bindValue(":un", u);
    signupQuery.bindValue(":fn", fn);
    signupQuery.bindValue(":ln", ln);
    signupQuery.bindValue(":em", e);
    signupQuery.bindValue(":pw", p);

    if(signupQuery.exec()) return true;
    else return false;
}
