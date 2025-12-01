#ifndef ITIMERSTRATEGY_H
#define ITIMERSTRATEGY_H

#include <QString>

// 策略模式 - 计时器策略接口
class ITimerStrategy {
public:
    virtual ~ITimerStrategy() = default;
    
    // 获取时长（秒）
    virtual int duration() const = 0;
    
    // 获取策略名称
    virtual QString name() const = 0;
    
    // 获取完成提示消息
    virtual QString completionMessage() const = 0;
};

// 番茄钟策略
class PomodoroStrategy : public ITimerStrategy {
public:
    explicit PomodoroStrategy(int minutes = 25);
    
    int duration() const override;
    QString name() const override;
    QString completionMessage() const override;
    
private:
    int m_minutes;
};

// 短休息策略
class ShortBreakStrategy : public ITimerStrategy {
public:
    explicit ShortBreakStrategy(int minutes = 5);
    
    int duration() const override;
    QString name() const override;
    QString completionMessage() const override;
    
private:
    int m_minutes;
};

// 长休息策略
class LongBreakStrategy : public ITimerStrategy {
public:
    explicit LongBreakStrategy(int minutes = 15);
    
    int duration() const override;
    QString name() const override;
    QString completionMessage() const override;
    
private:
    int m_minutes;
};

#endif // ITIMERSTRATEGY_H
