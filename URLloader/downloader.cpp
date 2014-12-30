#include "downloader.h"
#include <QtWebKit/QWebElement>
#include <QtWebKitWidgets/QWebFrame>
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
    /*view = new QWebView();
    connect(view, SIGNAL(loadFinished(bool)), this, SLOT(replyFinished(bool)));*/

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
    //view->load(QUrl(strURL));
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(strURL)));
    emit setLoadingStatus(nURLIndex);
}

void Downloader::restart()
{
    bPauseFlag = false;
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
        qDebug() << "ERROR!";
        qDebug()<<"Page was not loaded";
        emit setErrorFlag(nURLIndex,"Error occurred");
        qDebug() << reply->errorString();
    }
    else
    {
        QString strHTML = reply->readAll();
        qDebug() << strHTML;

        emit setUploadFlag(nURLIndex);

        bool bFound = strHTML.contains(strTextToSearch,Qt::CaseInsensitive);
        emit setFoundFlag(nURLIndex,bFound);
        QRegExp regexp1("<\\s*a([^>]+)>");
        QRegExp regexp2("href\\s*=\\s*[\\\"\\']?([^\\'\\\"]+)[\\'\\\"]?");
        QRegExp regexp3("(http://[^#]+)");
        regexp2.setCaseSensitivity(Qt::CaseInsensitive);
        int pos = 0;
        while ((pos = regexp1.indexIn(strHTML, pos)) != -1)
        {
            qDebug()<<regexp1.cap(1);
            if(regexp2.indexIn(regexp1.cap(1)) != -1)
            {
                if (regexp3.indexIn(regexp2.cap(1)) != -1)
                {
                    QString strURL = regexp3.cap(1);
                    qDebug()<<strURL;
                    emit newURLFound(strURL);
                }
            }
            pos += regexp1.matchedLength();
        }


    }
}
