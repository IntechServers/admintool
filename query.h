#ifndef INFOQUERY
#define INFOQUERY

#include "serverinfo.h"
#include <QUdpSocket>
#include <QThread>
#include <QTableWidgetItem>

#define QUERY_TIMEOUT 2000

#define A2S_HEADER  0xFFFFFFFF

#define A2S_INFO 0x54
#define A2S_INFO_CHECK 0x49
#define A2S_INFO_GOLDSRC_CHECK 0x6D
#define A2S_INFO_STRING "Source Engine Query"

#define A2S_PLAYER 0x55
#define A2S_PLAYER_CHALLENGE_CHECK 0x41
#define A2S_PLAYER_CHECK 0x44

#define A2S_RULES 0x56
#define A2S_RULES_CHALLENGE_CHECK 0x41
#define A2S_RULES_CHECK 0x45

class MainWindow;
class Worker;

class InfoReply
{
public:
    InfoReply(QByteArray, ServerInfo *);
    ~InfoReply()
    {
        delete [] hostname;
        delete [] map;
        delete [] mod;
        delete [] gamedesc;
    }
    char *hostname;
    char *map;
    char *mod;
    char *gamedesc;
    qint32 appId;
    quint8 players;
    quint8 maxplayers;
    quint8 bots;
    char type;
    char os;
    bool visibility;
    bool vac;
    bool success;
};

class PlayerInfo
{
public:
    //~PlayerInfo(){delete [] name;}
    char *name;
    qint32 score;
    float time;
};

class RulesInfo
{
public:
   //~RulesInfo(){delete [] name; delete[] value;}
   char *name;
   char *value;
};

class InfoQuery : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    InfoQuery(MainWindow *main);
    ~InfoQuery() {
        workerThread.quit();
        workerThread.wait();
    }
signals:
    void query(ServerInfo *, QTableWidgetItem *item);
};

class PlayerQuery : public QObject
{
    Q_OBJECT
public:
    QThread workerThread;
    Worker *worker;
    PlayerQuery(MainWindow *main);
    ~PlayerQuery();
private:
    MainWindow *pMain;

signals:
    void query(ServerInfo *, QTableWidgetItem *);
};

class RulesQuery : public QObject
{
    Q_OBJECT
public:
    QThread workerThread;
    Worker *worker;
    RulesQuery(MainWindow *main);
    ~RulesQuery();
private:
    MainWindow *pMain;

signals:
    void query(ServerInfo *, QTableWidgetItem *);
};

QList<PlayerInfo> *GetPlayerReply(ServerInfo *);
InfoReply *GetInfoReply(ServerInfo *);
QList<RulesInfo> *GetRulesReply(ServerInfo *);
QString SecondsToDisplayTime(float time);
char *GetStringFromStream(QDataStream &stream);
#endif // INFOQUERY
