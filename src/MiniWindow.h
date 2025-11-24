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
#include "PomodoroTimer.h"

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

private slots:
    void onTimeChanged(int remainingSeconds);
    void onStateChanged(PomodoroTimer::State state);
    void onPomodoroCompleted();
    void onStartPauseClicked();
    void onResetClicked();
    void onExpandClicked();
    void onRecordClicked();  // 新增记录按钮槽函数

private:
    void setupUI();
    void applyStyles();
    void saveToExcel(const QString &time, const QString &content);  // 保存到Excel

    PomodoroTimer *m_timer;
    
    QLabel *m_timeLabel;
    QLabel *m_statusIndicator;
    QPushButton *m_startPauseButton;
    QPushButton *m_resetButton;
    QPushButton *m_recordButton;  // 新增记录按钮
    QPushButton *m_expandButton;
    
    QPoint m_dragPosition;
    bool m_dragging;
};

#endif // MINIWINDOW_H
