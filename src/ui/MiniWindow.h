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

private slots:
    void onTimeChanged(int remainingSeconds);
    void onStateChanged(Timer::State state);
    void onStartPauseClicked();
    void onResetClicked();
    void onExpandClicked();
    void onMoreButtonClicked();  // 更多按钮点击
    void onRecordClicked();      // 记录时间  // 新增记录按钮槽函数

private:
    void setupUI();
    void applyStyles();

    Timer *m_timer;
    
    QLabel *m_timeLabel;
    QLabel *m_statusIndicator;
    QPushButton *m_startPauseButton;
    QPushButton *m_resetButton;
    QPushButton *m_moreButton;    // 更多菜单按钮
    QPushButton *m_expandButton;
    StyledMenu *m_moreMenu;       // 自定义下拉菜单
    
    QPoint m_dragPosition;
    bool m_dragging;
};

#endif // MINIWINDOW_H
