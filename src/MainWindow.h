#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "PomodoroTimer.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

signals:
    void switchToMiniMode();

private slots:
    void onTimeChanged(int remainingSeconds);
    void onStateChanged(PomodoroTimer::State state);
    void onPomodoroCompleted();
    void onStartClicked();
    void onPauseClicked();
    void onResetClicked();
    void onMiniModeClicked();

private:
    void setupUI();
    void applyStyles();

    PomodoroTimer *m_timer;
    
    QLabel *m_timeLabel;
    QLabel *m_statusLabel;
    QPushButton *m_startButton;
    QPushButton *m_pauseButton;
    QPushButton *m_resetButton;
    QPushButton *m_miniModeButton;
};

#endif // MAINWINDOW_H
