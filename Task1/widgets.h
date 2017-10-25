#pragma once
#ifndef WIDGETS_H
#define WIDGETS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Object;
struct Widget /* extends Object */;
struct PushButton /* extends Widget */;
struct Label /* extends Widget */;
struct Application /* extends Object */;
struct Layout /* extends Object */;
struct VBoxLayout /* extends Layout */;

typedef void (NoArgumentsCallback)(struct Object *sender);

const char* Object_GetClassName(struct Object *object);
void Object_Delete(struct Object *object);
void Object_SetAllowDeleteChildren(bool v);

struct Application* Application_New();

struct Widget* Widget_New(struct Widget *parent);
int Application_Exec(struct Application *app);

struct VBoxLayout* VBoxLayout_New(struct Widget *parent);
void Layout_AddWidget(struct Layout *layout, struct Widget *widget);

struct Widget* Widget_New(struct Widget *parent);

void Widget_SetVisible(struct Widget *widget, bool v);
void Widget_SetWindowTitle(struct Widget *widget, const char *title);
void Widget_SetLayout(struct Widget *widget, struct Layout *layout);
void Widget_SetSize(struct Widget *widget, int w, int h);

struct Label* Label_New(struct Widget *parent);
void Label_SetText(struct Label *label, const char *text);

struct PushButton* PushButton_New(struct Widget *parent);
void PushButton_SetText(struct PushButton *button, const char *text);
void PushButton_SetOnClicked(struct PushButton *button, NoArgumentsCallback *callback);

#ifdef __cplusplus
}
#endif

#define QT_H
#endif

/*#include <stdbool.h>
struct QApplication;
struct QWidget;
struct QLayout;
struct QLabel;

struct Object {
    int type;
};

struct Application {
    struct Object base_obj;
    struct QApplication* qobj;
};

struct Widget {
    struct Object base_obj;
    struct QWidget* qobj;
};

struct Layout {
    struct Object base_obj;
    struct QLayout* qobj;
    struct Widget* window;
};

struct Label {
    struct Object base_obj;
    struct QLabel* qobj;
    struct Widget* window;
};

struct PushButton {
    struct Object base_obj;
    struct QPushButton* qobj;
};

struct Application* Application_New(int argc, char** argv);
const char* Object_GetClassName(struct Object* app);
struct Widget* Widget_New(struct Widget* parent);
struct Layout* VBoxLayout_New(struct Widget* window);
void Widget_SetLayout(struct Widget* widget, struct Layout* layout);
struct Label* Label_New(struct Widget* window);
void Widget_SetWindowTitle(struct Widget* window, const char* text);
void Widget_SetSize(struct Widget* window, int, int);
void Widget_SetVisible(struct Widget* window, bool isVisible);
int Application_Exec(struct Application* app);
*/
