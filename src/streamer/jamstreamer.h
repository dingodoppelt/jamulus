#ifndef _WIN32
#include <QObject>
#include "../util.h"

namespace streamer {

class CJamStreamer : public QObject {
    Q_OBJECT

public:
    CJamStreamer();
    void Init( const QString strStreamDestination );
    QString getStreamDestination() { return this->strStreamDest; }
    void toggleActive();
    bool getStreamStatus() { return this->bRunning; }
    bool getStreamEnabled() { return this->bEnabled; }
    void startStream();
    bool bInitialized() { return !strStreamDest.isEmpty(); }

public slots:
    void process( int iServerFrameSizeSamples, const CVector<int16_t>& data );
    void OnStarted();
    void OnStopped();

private:
    QString strStreamDest = ""; // stream destination to pass to ffmpeg as output part of arguments
    FILE *pipeout; // pipe for putting out the pcm data to ffmpeg
    bool bRunning = false;
    bool bEnabled = true;
};
}
#endif
