#include <QObject>

namespace chatbot {

class CChatBot : public QObject
{
    Q_OBJECT
public:
    CChatBot();
    void Init();

public slots:
    void OnIntChatMessReceived( const QString strChatMess );
};
}
