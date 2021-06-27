#include "database.h"

#include <QSqlRecord>
#include <QCryptographicHash>

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
        QString pw = "1082001";
        firstUser.prepare("INSERT INTO USER VALUES ('meokisama', 'Hoàng Đình', 'Sáng', 'hi@meoki.net', :pw)");
        firstUser.bindValue(":pw", QString(QCryptographicHash::hash(pw.toLocal8Bit(), QCryptographicHash::Md5)));
        firstUser.exec();
    }

    QSqlQuery createPROJECTS(db);
    createPROJECTS.prepare("CREATE TABLE projects (id INTEGER NOT NULL, name TEXT NOT NULL,	startdate INTEGER NOT NULL,	findate	INTEGER NOT NULL)");
    createPROJECTS.exec();

    QSqlQuery createTASKS(db);
    createTASKS.prepare("CREATE TABLE tasks (id INTEGER NOT NULL, name TEXT NOT NULL, state TEXT NOT NULL, recip TEXT NOT NULL)");
    createTASKS.exec();
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

    QSqlQuery deleteT(db);
    deleteT.prepare("DELETE FROM tasks WHERE id=(:id)");
    deleteT.bindValue(":id", id);
    deleteT.exec();
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

QString Database::fetchName(QString id)
{
    QSqlQuery fetch(db);
    fetch.prepare("SELECT name FROM projects WHERE id=(:id)");
    fetch.bindValue(":id", id);
    fetch.exec();
    QString nm;
    int iN = fetch.record().indexOf("name");
    while (fetch.next()) {
        nm = fetch.value(iN).toString();
    }
    return nm;
}

void Database::addTasks(QString id, QString nm, QString st, QString rc)
{
    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO tasks VALUES(:id, :nm, :st, :rc)");
    insertQuery.bindValue(":id", id);
    insertQuery.bindValue(":nm", nm);
    insertQuery.bindValue(":st", st);
    insertQuery.bindValue(":rc", rc);

    insertQuery.exec();
}

void Database::updateTasks(QString id, QString nm, QString st)
{
    QSqlQuery updateQuery(db);
    updateQuery.prepare("UPDATE tasks SET state=(:st) WHERE id=(:id) AND name=(:nm)");
    updateQuery.bindValue(":st", st);
    updateQuery.bindValue(":nm", nm);
    updateQuery.bindValue(":id", id);

    updateQuery.exec();

}

void Database::deleteTasks(QString id, QString nm)
{
    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM tasks WHERE id=(:id) AND name=(:nm)");
    deleteQuery.bindValue(":id", id);
    deleteQuery.bindValue(":nm", nm);

    deleteQuery.exec();
}
