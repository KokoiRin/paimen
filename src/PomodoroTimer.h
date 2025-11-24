#ifndef POMODOROTIMER_H
#define POMODOROTIMER_H

#include <QObject>
#include <QTimer>
#include <QString>

class PomodoroTimer : public QObject {
    Q_OBJECT

public:
    enum State {
        Idle,
        Running,
        Paused
    };

    explicit PomodoroTimer(QObject *parent = nullptr);
    
    void start();
    void pause();
    void reset();
    
    inline int remainingSeconds() const noexcept { return m_remainingSeconds; }
    inline int totalSeconds() const noexcept { return m_totalSeconds; }
    inline State state() const noexcept { return m_state; }
    
    QString formatTime() const;

signals:
    void timeChanged(int remainingSeconds);
    void stateChanged(State state);
    void pomodoroCompleted();

private slots:
    void onTimeout();

private:
    QTimer *m_timer;
    int m_totalSeconds;      // 25分钟 = 1500秒
    int m_remainingSeconds;
    State m_state;
};

#endif // POMODOROTIMER_H
