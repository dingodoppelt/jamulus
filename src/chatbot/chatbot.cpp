#include <QThread>
#include "chatbot.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <QByteArray> 

using namespace chatbot;

CChatBot::CChatBot()
{
}

void CChatBot::Init( const quint16 iPortNumber )
{
    QByteArray str = QString("/tmp/JamChat-%1").arg(iPortNumber).toUtf8();
    chatFile = strdup(str.constData());
    mkfifo(chatFile, 0777);
}

void CChatBot::OnIntChatMessReceived( const QString strChatMess )
{
    int fd = open(chatFile, O_WRONLY | O_NONBLOCK);
    write(fd, strChatMess.toUtf8().data(), strlen(strChatMess.toUtf8().data()));
    //close(fd);
}
