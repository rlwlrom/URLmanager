#ifndef URLLOADER_H
#define URLLOADER_H
#include <QObject>
#include <QWaitCondition>
#include <QUrl>
#include <QMutex>


class URLloader:public QObject
{
    Q_OBJECT
public:
    explicit URLloader(QString str,int nIndex);
    ~URLloader();
public slots:
    void doWork();
    void restart();
    void setStopFlag();
    void setPauseFlag();
signals:
    void addNewURL(QString strURL);
    void setFoundFlagToURL(int nURLIndex,bool bFound);
    void setUploadFlagToURL(int nURLIndex,bool bUpload);
    void setErrorFlagToURL(int nURLIndex,QString nErrorMsg);
private:
    QString strURL;
    int nURLIndex;
    bool bStopFlag;
    bool bPauseFlag;
    QWaitCondition pause;
    QMutex mutex;
};

#endif // URLLOADER_H
