#ifndef DOWNLOADER_H
#define DOWNLOADER_H
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QtWebKitWidgets/QWebView>
#include <QWaitCondition>
#include <QMutex>

class Downloader:public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QString str, int nIndex,QString strText, QObject *parent = 0);
    ~Downloader()
    {
        delete(manager);
    }
signals:
    void newURLFound(QString strURL);
    void setFoundFlag(int nURLIndex,bool bFound);
    void setUploadFlag(int nURLIndex);
    void setErrorFlag(int nURLIndex,QString nErrorMsg);
    void setLoadingStatus(int nURLIndex);
    void workFinished();
public slots:
    void doDownload();
    void replyFinished (QNetworkReply *reply);
    void restart();
    void setStopFlag();
    void setPauseFlag();
private:
     QNetworkAccessManager* manager;
     QString strURL;
     int nURLIndex;
     bool bStopFlag;
     bool bPauseFlag;
     QWaitCondition pause;
     QMutex mutex;
     QString strTextToSearch;
};

#endif // DOWNLOADER_H
