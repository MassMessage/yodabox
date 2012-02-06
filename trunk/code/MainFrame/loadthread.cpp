#include "loadthread.h"

#include "mainmodule/boxhandle.h"

extern BoxHandle boxHandel;
LoadThread::LoadThread()
{
}

void LoadThread::run()
{
    boxHandel.loadPlugins();
    //boxHandel.loadContainers();

}
