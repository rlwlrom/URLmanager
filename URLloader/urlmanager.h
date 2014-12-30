#ifndef URLMANAGER_H
#define URLMANAGER_H
#include <QObject>
#include <QStringListModel>
#include <QSet>
enum FoundStatus{eNone,eFound,eNotFound};
enum LoadedStatus{eNoneStatus,eLoadind,eUpload,eError};

struct URL
{
    QString strURL;
    FoundStatus eFoundFlag;
    LoadedStatus eStatus;
    URL(QString str)
    {
        strURL = str;
        eFoundFlag = eNone;
        eStatus = eNoneStatus;
    }
    QString getFoundStatus();
    QString getLoadedStatus();
};
class URLManager:public QObject
{
    Q_OBJECT
public:
    explicit URLManager(QString strURL, int MaxThread, int nMaxURL, QString strText);
    ~URLManager();
    void doWork();
    void setStop(){bStop = true;}
    void setPause(){bPause = true;}
    void setStart(){bStop = bPause = false;nCurrentURLIndex=0;}
    QList<URL>* getURLlist(){return &URLlist;}
signals:
    void stop();
    void pause();
    void restart();
public slots:
    void addListItem(QString strURL);
    void setUploadStatus(int nURLIndex);
    void setFoundStatus(int nURLIndex, bool bFound);
    void setErrorStatus(int nURLIndex, QString strError);
    void setLoadingStatus(int nURLIndex);
    void increaseThreadCount();
    void decreaseThreadCount();
private:
    int nThreadCount;
    int nMaxThreadCount;
    int nMaxURLCount;
    QString strTextToSeach;
    QList<URL> URLlist;
    QSet<QString> LoadedURLset;
    bool bStop;
    bool bPause;
    int nCurrentURLIndex;
};

#endif // URLMANAGER_H
