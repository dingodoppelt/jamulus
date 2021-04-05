#include <QThread>
#include "chatbot.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

using namespace chatbot;

CChatBot::CChatBot()
{
}

void CChatBot::Init( const quint16 iPortNumber )
{
    this->chatFile = QString("/tmp/JamChat-%1").arg(iPortNumber).toUtf8().constData();
    mkfifo(this->chatFile, 0777);
}

void CChatBot::OnIntChatMessReceived( const QString strChatMess )
{
    int fd = open(this->chatFile, O_WRONLY | O_NONBLOCK);
    write(fd, strChatMess.toLatin1().data(), strlen(strChatMess.toLatin1().data()));
    //close(fd);
}
