#include <QThread>
#include "chatbot.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

using namespace chatbot;

CChatBot::CChatBot()
{
    mkfifo("/tmp/JamChat", 0777);
}

void CChatBot::OnIntChatMessReceived( const QString strChatMess )
{
    int fd = open("/tmp/JamChat", O_WRONLY | O_NONBLOCK);
    write(fd, strChatMess.toLatin1().data(), strlen(strChatMess.toLatin1().data()));
    //close(fd);
}
