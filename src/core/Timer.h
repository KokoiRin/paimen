#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QTimer>
#include <memory>
#include "ITimerStrategy.h"

// 核心计时器类 - 重构版
class Timer : public QObject {
    Q_OBJECT
    
public:
    enum State {
        Idle,
        Running,
        Paused
    };
    
    explicit Timer(QObject *parent = nullptr);
    ~Timer() override = default;
    
    // 设置策略（策略模式）
    void setStrategy(std::unique_ptr<ITimerStrategy> strategy);
    
    // 控制方法
    void start();
    void pause();
    void resume();
    void reset();
    
    // 状态查询
    State state() const { return m_state; }
    int remainingSeconds() const { return m_remainingSeconds; }
    int totalSeconds() const;
    QString strategyName() const;
    
signals:
    void timeChanged(int remainingSeconds);
    void stateChanged(State state);
    void completed(const QString& message);
    
private slots:
    void onTimeout();
    
private:
    void setState(State newState);
    
    std::unique_ptr<ITimerStrategy> m_strategy;
    QTimer* m_timer;
    State m_state;
    int m_remainingSeconds;
};

#endif // TIMER_H
