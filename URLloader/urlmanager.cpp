#include "urlmanager.h"
#include "urlloader.h"
#include "downloader.h"
#include <QThread>

QString URL::getFoundStatus()
{
    QString strRet("");
    switch(eFoundFlag)
    {
    case eNotFound:
        strRet = "Not Found";
    case eFound:
        strRet = "Found";
    }
    return strRet;
}

QString URL::getLoadedStatus()
{
    QString strRet("");
    switch(eStatus)
    {
    case eLoadind:
        strRet = "Loading...";
    case eUpload:
        strRet = "Upload";
    case eError:
        strRet = "Error";
    }
    return strRet;
}

URLManager::URLManager(QString strURL, int MaxThread, int nMaxURL, QString strText):
    nMaxThreadCount(MaxThread),nMaxURLCount(nMaxURL),strTextToSeach(strText)
{
    URLlist.append(URL(strURL));
    nThreadCount = 0;
}
void URLManager::addListItem(QString strURL)
{
    if(!LoadedURLset.contains(strURL))
    {
        qDebug()<<"from addListItem = "<<strURL;
        URLlist.append(URL(strURL));
    }
}
void URLManager::setUploadStatus(int nURLIndex)
{
    qDebug()<<"from setUploadStatus = "<<nURLIndex;
    URLlist[nURLIndex].eStatus = eUpload;
}
void URLManager::setFoundStatus(int nURLIndex, bool bFound)
{
    qDebug()<<"from setFoundStatus = "<<nURLIndex <<"bFound = "<<bFound;
    URLlist[nURLIndex].eFoundFlag = bFound?(eFound):(eNotFound);
}
void URLManager::setErrorStatus(int nURLIndex, QString strError)
{
    qDebug()<<"from setErrorStatus = "<<nURLIndex <<"strError = "<<strError;
    URLlist[nURLIndex].eStatus = eError;
}
void URLManager::setLoadingStatus(int nURLIndex)
{
     qDebug()<<"from setLoadingStatus = "<<nURLIndex;
    URLlist[nURLIndex].eStatus = eLoadind;
}
void URLManager::increaseThreadCount()
{
    ++nThreadCount;
}
void URLManager::decreaseThreadCount()
{
    --nThreadCount;
}

void URLManager::doWork()
{
    qDebug()<<"from URLManager::doWork()";
    qDebug()<<URLlist.size();
    for(int i=nCurrentURLIndex;i<URLlist.size()&& nCurrentURLIndex<URLlist.size();i++) {
            if(nThreadCount<nMaxThreadCount)
            {
                Downloader* worker = new Downloader(URLlist[i].strURL,i,strTextToSeach);
                QThread *workerThread = new QThread;
                worker->moveToThread(workerThread);
                connect(workerThread, SIGNAL(started()), worker, SLOT(doDownload()));
                connect(workerThread, SIGNAL(started()), this, SLOT(increaseThreadCount()));

                connect(workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
                connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));
                connect(workerThread, SIGNAL(finished()), this, SLOT(decreaseThreadCount()));

                connect(worker, SIGNAL(newURLFound(QString)), this, SLOT(addListItem(QString)));
                connect(worker, SIGNAL(setFoundFlag(int,bool)), this, SLOT(setFoundStatus(int,bool)));
                connect(worker, SIGNAL(setLoadingStatus(int)),this, SLOT(setLoadingStatus(int)));
                connect(worker, SIGNAL(setErrorFlag(int,QString)),this, SLOT(setErrorStatus(int,QString)));
                connect(worker, SIGNAL(setUploadFlag(int)),this, SLOT(setUploadStatus(int)));

                connect(this, SIGNAL(stop()), worker, SLOT(setStopFlag()));
                connect(this, SIGNAL(pause()), worker, SLOT(setPauseFlag()));
                connect(this, SIGNAL(restart()),worker, SLOT(restart()));
                workerThread->start();
                qDebug()<<workerThread;
                nCurrentURLIndex = i+1;
            }
        }
}
URLManager::~URLManager()
{
}

