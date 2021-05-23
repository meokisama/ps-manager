#include "database.h"

#include <QSqlRecord>

Database::Database() {}

QSqlDatabase Database::db;

bool Database::connectDB()
{
    QString dbPath(QCoreApplication::applicationDirPath()+"/../database.db");

    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(dbPath);

    if(db.open())
    {
        initialQuery();
        return true;
    }
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

void Database::initialQuery()
{
    QSqlQuery createUSER(db);
    createUSER.prepare("CREATE TABLE IF NOT EXISTS USER (USERNAME TEXT UNIQUE, HO TEXT, TEN TEXT, EMAIL TEXT, PASSWORD TXT)");
    if(createUSER.exec())
    {
        QSqlQuery firstUser(db);
        firstUser.prepare("INSERT INTO USER VALUES ('meokisama', 'Hoàng Đình', 'Sáng', 'hi@meoki.net', '1082001')");
        firstUser.exec();
    }

    QSqlQuery createPROJECTS(db);
    createPROJECTS.prepare("CREATE TABLE projects (id INTEGER NOT NULL, name TEXT NOT NULL,	startdate INTEGER NOT NULL,	findate	INTEGER NOT NULL)");
    createPROJECTS.exec();
}

bool Database::Login(QString u, QString p)
{
    bool exist = false;

    QSqlQuery check(db);

    // 1. Check username or email with Qt function
    #if 0
        if (u.contains("@",Qt::CaseInsensitive))
            check.prepare("SELECT USERNAME FROM USER WHERE EMAIL = (:un) AND PASSWORD = (:pw)");
        else
            check.prepare("SELECT USERNAME FROM USER WHERE USERNAME = (:un) AND PASSWORD = (:pw)");
    #endif

    // 2. Or check with SQL query
    check.prepare("SELECT USERNAME FROM USER WHERE (USERNAME = (:un) OR EMAIL = (:un)) AND PASSWORD = (:pw)");

    check.bindValue(":un", u);
    check.bindValue(":pw", p);

    if (check.exec()) if (check.next()) exist = true;

    return exist;
}

bool Database::checkInfo(QString u)
{
    QSqlQuery uQuery(db);
    uQuery.prepare("SELECT USERNAME FROM USER WHERE USERNAME = (:un)");
    uQuery.bindValue(":un", u);
    if(uQuery.exec()) if(uQuery.next()) return true;

    return false;
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

bool Database::Update(QString fn, QString ln, QString u, QString p, QString e)
{
    QSqlQuery updateQuery(db);
    updateQuery.prepare("UPDATE USER SET USERNAME=(:un), HO=(:fn), TEN=(:ln), EMAIL=(:em), PASSWORD=(:pw) WHERE USERNAME=(:un)");
    updateQuery.bindValue(":un", u);
    updateQuery.bindValue(":fn", fn);
    updateQuery.bindValue(":ln", ln);
    updateQuery.bindValue(":em", e);
    updateQuery.bindValue(":pw", p);

    if(updateQuery.exec()) return true;
    else return false;
}

void Database::addProject(QString id, QString pn, int ps, int pf)
{
    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO projects VALUES(:id, :pn, :ps, :pf)");
    insertQuery.bindValue(":id", id);
    insertQuery.bindValue(":pn", pn);
    insertQuery.bindValue(":ps", ps);
    insertQuery.bindValue(":pf", pf);

    insertQuery.exec();
}

void Database::delProject(QString id)
{
    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM projects WHERE id=(:id)");
    deleteQuery.bindValue(":id", id);

    deleteQuery.exec();
}

int Database::findMaxID()
{
    QSqlQuery maxQuery(db);
    maxQuery.prepare("SELECT MAX(id) AS maxid FROM projects");
    maxQuery.exec();
    int maxID;
    int ID = maxQuery.record().indexOf("maxid");
    maxID = maxQuery.value(ID).toInt();
    return maxID;
}
