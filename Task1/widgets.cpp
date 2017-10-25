#include "widgets_defs.h"

#ifdef __cplusplus
extern "C" {
#endif
//--------------------------------------------------------------------------------
// Common methods

const char* Object_GetClassName(struct Object* object)
{
    QString s(object->qobj->metaObject()->className());
    return s.toUtf8().constData() + 1;
}

void Object_Delete(struct Object *object) {
    delete object;
}

//--------------------------------------------------------------------------------
// Application

QArgs Application::qApplicationArgs;

struct Application* Application_New() {
    return new Application;
}

int Application_Exec(struct Application *app) {
    return app->exec();
}

//--------------------------------------------------------------------------------
// Layout

void Layout::addWidget(struct Widget *widget) { ((QLayout*)qobj)->addWidget((QWidget*)(widget->qobj));}

void Layout_AddWidget(struct Layout *layout, struct Widget *widget) {
    layout->addWidget(widget);
}

//--------------------------------------------------------------------------------
// Widget

struct Widget* Widget_New(struct Widget *parent) {
    return new Widget(parent);
}

void Widget_SetVisible(struct Widget *widget, bool v)
{
    widget->setVisible(v);
}

void Widget_SetWindowTitle(struct Widget *widget, const char *title)
{
    widget->setWindowTitle(title);
}

void Widget_SetLayout(struct Widget *widget, struct Layout *layout)
{
    widget->setLayout(layout);
}

void Widget_SetSize(struct Widget *widget, int w, int h)
{
    widget->setSize(w, h);
}

//--------------------------------------------------------------------------------
// VBoxLayout
struct VBoxLayout* VBoxLayout_New(struct Widget *parent) {
    return new VBoxLayout(parent);
}

//--------------------------------------------------------------------------------
// Label

struct Label* Label_New(struct Widget *parent) {
    return new Label(parent);
}

void Label_SetText(struct Label *label, const char *text) {
    label->setText(text);
}

//--------------------------------------------------------------------------------
// PushButton

struct PushButton* PushButton_New(struct Widget *parent) {
    return new PushButton(parent);
}

void PushButton_SetText(struct PushButton *button, const char *text)
{
    button->setText(text);
}

void PushButton_SetOnClicked(struct PushButton *button, NoArgumentsCallback *callback)
{
    button->setOnClicked(callback);
}

//--------------------------------------------------------------------------------
QArgs::QArgs():argc(1)
{
    name = new char*[1];
    name[0] = new char[2];
    name[0][0] = 't';
    name[0][1] = '\0';
}

QArgs::~QArgs()
{
    delete[] name[0];
    delete[] name;
}
#ifdef __cplusplus
}
#endif
