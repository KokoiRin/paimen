#include "Timer.h"

Timer::Timer(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_state(Idle)
    , m_remainingSeconds(0)
{
    m_timer->setInterval(1000);  // 1秒间隔
    connect(m_timer, &QTimer::timeout, this, &Timer::onTimeout);
    
    // 默认策略：25分钟番茄钟
    setStrategy(std::make_unique<PomodoroStrategy>(25));
}

void Timer::setStrategy(std::unique_ptr<ITimerStrategy> strategy) {
    m_strategy = std::move(strategy);
    reset();
}

void Timer::start() {
    if (m_state == Idle) {
        m_remainingSeconds = m_strategy->duration();
        emit timeChanged(m_remainingSeconds);
    }
    
    m_timer->start();
    setState(Running);
}

void Timer::pause() {
    if (m_state == Running) {
        m_timer->stop();
        setState(Paused);
    }
}

void Timer::resume() {
    if (m_state == Paused) {
        m_timer->start();
        setState(Running);
    }
}

void Timer::reset() {
    m_timer->stop();
    m_remainingSeconds = m_strategy ? m_strategy->duration() : 0;
    setState(Idle);
    emit timeChanged(m_remainingSeconds);
}

int Timer::totalSeconds() const {
    return m_strategy ? m_strategy->duration() : 0;
}

QString Timer::strategyName() const {
    return m_strategy ? m_strategy->name() : QString();
}

void Timer::onTimeout() {
    if (m_remainingSeconds > 0) {
        m_remainingSeconds--;
        emit timeChanged(m_remainingSeconds);
        
        if (m_remainingSeconds == 0) {
            m_timer->stop();
            setState(Idle);
            emit completed(m_strategy->completionMessage());
        }
    }
}

void Timer::setState(State newState) {
    if (m_state != newState) {
        m_state = newState;
        emit stateChanged(m_state);
    }
}
