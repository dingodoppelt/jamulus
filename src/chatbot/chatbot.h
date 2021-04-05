#include <QObject>

namespace chatbot {

class CChatBot : public QObject
{
    Q_OBJECT
public:
    CChatBot();
    void Init( const quint16 iPortNumber );

protected:
    const char* chatFile;

public slots:
    void OnIntChatMessReceived( const QString strChatMess );
};
}
