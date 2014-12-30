#include "urlloader.h"

URLloader::URLloader(QString str, int nIndex)
{
   strURL = str;
   nURLIndex = nIndex;
   bStopFlag = false;
   bPauseFlag = false;
}

URLloader::~URLloader()
{

}

void URLloader::doWork()
{
    forever{
    if (bPauseFlag)
    {
        pause.wait(&mutex);
    }

    if (bStopFlag)
    {
       return;
    }
    }

}
void URLloader::restart()
{
    bPauseFlag = false;
    pause.wakeAll();
}
void URLloader::setStopFlag()
{
    bStopFlag = true;
}
void URLloader::setPauseFlag()
{
    bPauseFlag = true;
}
