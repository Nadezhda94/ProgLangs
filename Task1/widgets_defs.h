#ifndef WIDGETS_DEFS_H
#define WIDGETS_DEFS_H

#include <QApplication>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>

#ifdef __cplusplus
extern "C" {
#endif

struct Object
{
public:
    Object(): Object(new QObject) { }
    virtual ~Object() { delete qobj;}
    QObject* qobj;
protected:
    Object(QObject* qobj) : qobj(qobj) {}
};

struct QArgs {
public:
    QArgs();
    ~QArgs();
    char** name;
    int argc;
};

struct Application : public Object
{
public:
    Application(): Object(new QApplication(Application::qApplicationArgs.argc, Application::qApplicationArgs.name)) { }
    int exec() { return ((QApplication*)qobj)->exec();}
    ~Application()=default;
protected:
    Application(QObject* qobj) : Object(qobj) {}
    static QArgs qApplicationArgs;
};

struct Widget;
struct Layout : public Object
{
public:
    void addWidget(struct Widget *widget);
    ~Layout()=default;
protected:
    Layout(QObject* qobj) : Object(qobj) {}
};

struct Widget : public Object
{
public:
    Widget(Widget* parent):Object(new QWidget(
                                      (parent == NULL)?NULL:(QWidget*)parent->qobj
                                      )) { }
    void setVisible(bool v) {((QWidget*)qobj)->setVisible(v);}
    void setWindowTitle(const char *title) {((QWidget*)qobj)->setWindowTitle(title);}
    void setLayout(struct Layout *layout) {((QWidget*)qobj)->setLayout((QLayout*)(layout->qobj));}
    void setSize(int w, int h) {((QWidget*)qobj)->setFixedSize(w,h);}
    ~Widget()=default;
protected:
    Widget(QObject* qobj) : Object(qobj) {}
};


struct VBoxLayout : public Layout
{
public:
    VBoxLayout(struct Widget *parent):Layout(new QVBoxLayout((QWidget*)parent->qobj)) { }
    ~VBoxLayout()=default;
protected:
    VBoxLayout(QObject* qobj) : Layout(qobj) {}
};

struct Label : public Widget
{
public:
    Label( Widget *parent): Widget(new QLabel((QWidget*)(parent->qobj))){  }
    void setText(const char *text) {((QLabel*)qobj)->setText(text);}
protected:
    Label(QObject* qobj) : Widget(qobj) {}
};

typedef void (NoArgumentsCallback)(struct Object *sender);

struct PushButton : public Widget
{
public:
    PushButton( Widget *parent):Widget(new QPushButton((QWidget*)parent->qobj)) {  }
    void setText(const char *text) {((QPushButton*)qobj)->setText(text);}
    void setOnClicked(NoArgumentsCallback * callback) {
        QObject::connect((QPushButton*)qobj, &QPushButton::clicked, [=] { callback(this); });
    }
protected:
    PushButton(QObject* qobj) : Widget(qobj) {}
};

#ifdef __cplusplus
}
#endif

#endif // WIDGETS_DEFS_H
