#ifndef MINIWINDOW_H
#define MINIWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
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

private:
    void setupUI();
    void applyStyles();

    PomodoroTimer *m_timer;
    
    QLabel *m_timeLabel;
    QLabel *m_statusIndicator;
    QPushButton *m_startPauseButton;
    QPushButton *m_resetButton;
    QPushButton *m_expandButton;
    
    QPoint m_dragPosition;
    bool m_dragging;
};

#endif // MINIWINDOW_H
