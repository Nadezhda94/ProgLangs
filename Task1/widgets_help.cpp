
#include <QApplication>
#include <QWidget>
#include <QLayout>


#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

const char* ids[] = {"Object", "Application", "Widget", "Layout", "Label", "PushButton", "VBoxLayout"};

int argsCount = 1;


struct Widget : public Object
{
public:
    Widget():Widget(new QWidget, 2) {}
    QWidget* getQObject() { return static_cast<QWidget*>(qobj);}
protected:
    Widget(QObject* qobj, int type) : Object(qobj, type) {}
};

struct PushButton : public Widget
{
    PushButton() { type = 5; }
};

struct Label : public Widget
{
    Label(){ type = 4;}
};

struct QName {
public:
    QName() { name = new char*[1];

                name[0] = new char[2];
                name[0][0] = 'd';
                name[0][1] = '\0';}
    char** getName() { return name;}
    ~QName() { delete[] name;}
private:
    char** name;
};
/*
struct Application : public Object
{
public:

private:
};

struct Application : public Object
{
public:
    Application(): Application(new QApplication(argsCount, qApplicationName.getName()), 1) { }
    int exec() { qApp->exec();}
protected:
    Application(QObject* qobj, int type) : Object(qobj, type) {}
    QApplication qApp;
    static QName qApplicationName;
};

QName Application::qApplicationName;*/
/*
struct Layout : public Object
{
public:
    Layout() { type = 3; }
    void Layout_AddWidget(struct Widget *widget) {
        layout->addWidget(widget->getQObject());
    }
private:
};
struct VBoxLayout : public Layout
{
public:
    VBoxLayout() { type = 6; }
private:
};

typedef void (NoArgumentsCallback)(struct Object *sender);

const char* Object_GetClassName(struct Object* object)
{
    return ids[object->getType()];
}*/

//void Object_Delete(struct Object *object);
//void Object_SetAllowDeleteChildren(bool v);





struct VBoxLayout* VBoxLayout_New(struct Widget *parent) {
    return new VBoxLayout;
}

void Layout_AddWidget(struct Layout *layout, struct Widget *widget) {
}



void Widget_SetVisible(struct Widget *widget, bool v)
{
    widget->qobj->setVisible();
}

void Widget_SetWindowTitle(struct Widget *widget, const char *title)
{
    widget->qobj->setWindowTitle();
}

void Widget_SetLayout(struct Widget *widget, struct Layout *layout)
{
    widget->qobj->setLayout(layout);
}
void Widget_SetSize(struct Widget *widget, int w, int h);

struct Label* Label_New(struct Widget *parent);
void Label_SetText(struct Label *label, const char *text);

struct PushButton* PushButton_New(struct Widget *parent);
void PushButton_SetText(struct PushButton *button, const char *text);
void PushButton_SetOnClicked(struct PushButton *button, NoArgumentsCallback *callback);

#ifdef __cplusplus
}
#endif

/*
extern "C" {
#include "widgets.h"
const char* ids[] = {"Application", "Widget", "Layout", "Label"};

struct Application* Application_New(int argc, char** argv){
    struct Application* app = new Application();
    app->qobj = new QApplication(argc, argv);
    app->base_obj.type = 0;
    return app;
}
const char* Object_GetClassName(struct Object* app) {
    return ids[app->type];
}

}
struct Widget* Widget_New(struct Widget* parent) {
    struct Widget* widget = new Widget();
    QWidget* qparent = NULL;
    if (parent != NULL) {
        qparent = parent->qobj;
    }
    widget->qobj = new QWidget();
    widget->base_obj.type = 1;
    return widget;
}


void Widget_SetLayout(struct Widget* widget, struct Layout* layout) {
    widget->qobj->setLayout(layout->qobj);
}


struct Label* Label_New(struct Widget* window) {
    Label* label = new Label;
    label->qobj = new QLabel(window->qobj);
    label->window = window;
    label->base_obj.type = 3;
    return label;
}

void Label_SetText(struct Label*, const char*);

void Layout_AddWidget(struct Layout*, struct Widget*);

struct PushButton* PushButton_New(struct Widget* window);


void PushButton_SetText(struct PushButton*, const char*);


void Widget_SetWindowTitle(struct Widget* window, const char* text) {
    window->qobj->setWindowTitle(text);
}

void Widget_SetSize(struct Widget* window, int, int) {

}
void Widget_SetVisible(struct Widget* window, bool isVisible) {
    window->qobj->setVisible(isVisible);
}

int Application_Exec(struct Application* app) {
    return app->qobj-> exec();
}


// Also deletes all children of main_window
void Object_Delete(struct Object*);

*/
