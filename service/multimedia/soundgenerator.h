#ifndef SOUNDGENERATOR_H
#define SOUNDGENERATOR_H

#include <QObject>
#include <QIODevice>
#include <QAudioFormat>

class SoundGenerator : public QIODevice
{
    Q_OBJECT

public:
    SoundGenerator(const QAudioFormat &format, qint64 durationUs, int sampleRate);

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;
    qint64 bytesAvailable() const override;

    void reset(const QAudioFormat &format, qint64 durationUs, int sampleRate);

private:
    void generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate);

private:
    qint64 m_pos = 0;
    QByteArray m_buffer;
};



#endif // SOUNDGENERATOR_H
