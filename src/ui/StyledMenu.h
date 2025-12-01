#ifndef STYLEDMENU_H
#define STYLEDMENU_H

#include <QMenu>
#include <QWidget>
#include <QPainter>
#include <QStyleOptionMenuItem>
#include <QProxyStyle>

// 自定义菜单样式类
class MenuStyle : public QProxyStyle {
    Q_OBJECT
public:
    explicit MenuStyle(QStyle *style = nullptr);
    
    void drawControl(ControlElement element, const QStyleOption *option,
                    QPainter *painter, const QWidget *widget) const override;
    
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                      QPainter *painter, const QWidget *widget) const override;
    
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                          const QSize &size, const QWidget *widget) const override;
    
    int pixelMetric(PixelMetric metric, const QStyleOption *option,
                   const QWidget *widget) const override;
};

// 自定义样式菜单类
class StyledMenu : public QMenu {
    Q_OBJECT
    
public:
    explicit StyledMenu(QWidget *parent = nullptr);
    explicit StyledMenu(const QString &title, QWidget *parent = nullptr);
    ~StyledMenu() override;
    
    // 添加带图标的动作
    QAction* addStyledAction(const QString &icon, const QString &text);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    
private:
    void setupStyle();
    MenuStyle *m_menuStyle;
};

#endif // STYLEDMENU_H
