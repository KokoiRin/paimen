#ifndef MINIWINDOW_H
#define MINIWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QInputDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QDialog>
#include <QDir>
#include <QTimer>
#include "Timer.h"
#include "StyledMenu.h"

class MiniWindow : public QWidget {
    Q_OBJECT

public:
    explicit MiniWindow(QWidget *parent = nullptr);
    ~MiniWindow() = default;

signals:
    void switchToNormalMode();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onTimeChanged(int remainingSeconds);
    void onStateChanged(Timer::State state);
    void onStartPauseClicked();
    void onResetClicked();
    void onExpandClicked();
    void onMoreButtonClicked();  // 更多按钮点击
    void onRecordClicked();      // 记录时间
    void onSwitchToIntervalMode();  // 切换到记录间隔模式
    void onSwitchToPomodoroMode();  // 切换回番茄钟模式
    void onEditTaskTitle();         // 编辑任务标题
    void updateIntervalTime();      // 更新间隔时间显示

private:
    void setupUI();
    void applyStyles();
    void switchMode(bool isIntervalMode);

    Timer *m_timer;
    
    QLabel *m_timeLabel;
    QLabel *m_statusIndicator;
    QLabel *m_taskTitleLabel;     // 任务标题标签
    QPushButton *m_startPauseButton;
    QPushButton *m_resetButton;
    QPushButton *m_moreButton;    // 更多菜单按钮
    QPushButton *m_expandButton;
    StyledMenu *m_moreMenu;       // 自定义下拉菜单
    
    QPoint m_dragPosition;
    bool m_dragging;
    
    // 记录间隔模式相关
    bool m_isIntervalMode;        // 是否处于间隔模式
    QDateTime m_lastRecordTime;   // 上次记录时间
    QTimer *m_intervalTimer;      // 间隔计时器
    QString m_taskTitle;          // 当前任务标题
};

#endif // MINIWINDOW_H
