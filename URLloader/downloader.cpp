#include "downloader.h"
#include <QUrl>
#include <QDebug>

Downloader::Downloader(QString str,int nIndex, QString strText, QObject *parent) :
    QObject(parent),strURL(str),nURLIndex(nIndex),strTextToSearch(strText)

{
    bStopFlag = false;
    bPauseFlag = false;
}

void Downloader::doDownload()
{
	qDebug()<<"from DownLoad";
    if (bPauseFlag)
    {
        pause.wait(&mutex);
    }

    if (bStopFlag)
    {
       return;
    }
    qDebug()<<strURL;
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(strURL)));
    emit setLoadingStatus(nURLIndex);
}

void Downloader::restart()
{
    bStopFlag = bPauseFlag = false;
    pause.wakeAll();
}
void Downloader::setStopFlag()
{
    bStopFlag = true;
}
void Downloader::setPauseFlag()
{
    bPauseFlag = true;
}

void Downloader::replyFinished (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR! " << reply->errorString();
        emit setErrorFlag(nURLIndex,reply->errorString());
    }
    else
    {
        emit setUploadFlag(nURLIndex);

        QString strHTML = reply->readAll();
        bool bFound = strHTML.contains(strTextToSearch,Qt::CaseInsensitive);
        emit setFoundFlag(nURLIndex,bFound);

        QRegExp regexp1("<\\s*a([^>]+)>");
        QRegExp regexp2("href\\s*=\\s*[\\\"\\']?([^\\'\\\"]+)[\\'\\\"]?");
        QRegExp regexp3("(http://[^#]+)");
        regexp2.setCaseSensitivity(Qt::CaseInsensitive);
        int pos = 0;
        while ((pos = regexp1.indexIn(strHTML, pos)) != -1)
        {
            if((regexp2.indexIn(regexp1.cap(1)) != -1)
                    &&(regexp3.indexIn(regexp2.cap(1)) != -1))
            {
               QString strURL = regexp3.cap(1);
               emit newURLFound(strURL);
            }
            pos += regexp1.matchedLength();
        }
    }
    emit workFinished();
}
