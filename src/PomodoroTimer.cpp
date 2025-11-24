#include "PomodoroTimer.h"

PomodoroTimer::PomodoroTimer(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_totalSeconds(25 * 60)  // 25分钟
    , m_remainingSeconds(m_totalSeconds)
    , m_state(Idle)
{
    connect(m_timer, &QTimer::timeout, this, &PomodoroTimer::onTimeout);
    m_timer->setInterval(1000);  // 每秒触发一次
}

void PomodoroTimer::start() {
    if (m_state == Running) return;
    
    m_state = Running;
    m_timer->start();
    emit stateChanged(m_state);
}

void PomodoroTimer::pause() {
    if (m_state != Running) return;
    
    m_state = Paused;
    m_timer->stop();
    emit stateChanged(m_state);
}

void PomodoroTimer::reset() {
    m_timer->stop();
    m_remainingSeconds = m_totalSeconds;
    m_state = Idle;
    
    emit timeChanged(m_remainingSeconds);
    emit stateChanged(m_state);
}

QString PomodoroTimer::formatTime() const {
    int minutes = m_remainingSeconds / 60;
    int seconds = m_remainingSeconds % 60;
    return QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

void PomodoroTimer::onTimeout() {
    if (m_remainingSeconds > 0) {
        --m_remainingSeconds;
        emit timeChanged(m_remainingSeconds);
        
        if (m_remainingSeconds == 0) {
            m_timer->stop();
            m_state = Idle;
            emit pomodoroCompleted();
            emit stateChanged(m_state);
        }
    }
}
