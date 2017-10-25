extern "C" {
#include <Python.h>
#include "structmember.h"
#include "widgets.h"
}

//--------------------------------------------------------------------------------
// structs
typedef struct {
    PyObject_HEAD
    struct Object* obj;
} PyApplication;

typedef struct {
    PyObject_HEAD
    struct Object* obj;
} PyWidget;

typedef struct {
    PyObject_HEAD
    struct Object* obj;
} PyVBoxLayout;

typedef struct {
    PyObject_HEAD
    struct Object* obj;
} PyLabel;

typedef struct {
    PyObject_HEAD
    struct Object* obj;
} PyPushButton;

//--------------------------------------------------------------------------------
// PyApplication
static void PyApplication_dealloc(PyApplication* self)
{
    Object_Delete(self->obj);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static int PyApplication_init(PyApplication *self, PyObject *args, PyObject *kwds)
{
    self->obj = (Object*)Application_New();
    return 0;
}

static PyObject * PyApplication_get_class_name(PyApplication* self)
{
    PyObject * py_result = PyUnicode_FromString( Object_GetClassName(self->obj) );
    return py_result;
}

static PyObject * PyApplication_exec(PyApplication* self)
{
    PyObject * py_result = PyLong_FromLong(Application_Exec((Application*)self->obj));
    return py_result;
}

PyMethodDef PyApplication_methods [] = {
    {"get_class_name", (PyCFunction)PyApplication_get_class_name, METH_NOARGS, "Return the class name" },
    {"exec", (PyCFunction)PyApplication_exec, METH_NOARGS, "Exec" },
    { NULL }
};

static PyTypeObject PyApplicationType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pywidgets.Application",    /* tp_name */
    sizeof(PyApplication),     /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)PyApplication_dealloc, /* tp_dealloc */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "Application objects",     /* tp_doc */
    0, 0, 0, 0, 0, 0,
    PyApplication_methods,             /* tp_methods */
    0, 0, 0, 0, 0, 0, 0,
    (initproc)PyApplication_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,                 /* tp_new */
};

//--------------------------------------------------------------------------------
// PyWidget

static void PyWidget_dealloc(PyWidget* self)
{
    Object_Delete(self->obj);
}

static int PyWidget_init(PyWidget *self, PyObject *args, PyObject *kwds)
{
    self->obj = (Object*)Widget_New(NULL);
    return 0;
}

static PyObject * PyWidget_get_class_name(PyWidget* self)
{
    PyObject* py_result = PyUnicode_FromString( Object_GetClassName(self->obj) );
    return py_result;
}

static PyObject * PyWidget_set_size(PyWidget* self, PyObject *args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);
    PyObject * arg2 = PyTuple_GetItem(args, 1);
    Widget_SetSize((Widget*)self->obj, PyLong_AsLong(arg1), PyLong_AsLong(arg2));
    Py_RETURN_NONE;
}

static PyObject * PyWidget_set_visible(PyWidget* self, PyObject *args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);

    Widget_SetVisible((Widget*)self->obj, PyObject_IsTrue(arg1));
    Py_RETURN_NONE;
}

static PyObject * PyWidget_set_layout(PyWidget* self, PyObject *args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);

    Widget_SetLayout((Widget*)self->obj, (Layout*)((PyVBoxLayout*)(arg1))->obj);
    Py_RETURN_NONE;
}

static PyObject * PyWidget_set_window_title(PyWidget* self, PyObject *args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);

    Widget_SetWindowTitle((Widget*)self->obj, PyUnicode_AsUTF8(arg1));
    Py_RETURN_NONE;
}

PyMethodDef PyWidget_methods [] = {
    {"get_class_name", (PyCFunction)PyWidget_get_class_name, METH_NOARGS, "Return the class name" },
    {"set_layout", (PyCFunction)PyWidget_set_layout, METH_VARARGS, "Set layout" },
    {"set_window_title", (PyCFunction)PyWidget_set_window_title, METH_VARARGS, "Set window title" },
    {"set_size", (PyCFunction)PyWidget_set_size, METH_VARARGS, "Set widget size" },
    {"set_visible", (PyCFunction)PyWidget_set_visible, METH_VARARGS, "Set visible" },
    { NULL }
};
static PyTypeObject PyWidgetType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pywidgets.Widget",    /* tp_name */
    sizeof(PyWidget),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)PyWidget_dealloc, /* tp_dealloc */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "Widget object",           /* tp_doc */
    0, 0, 0, 0, 0, 0,
    PyWidget_methods,             /* tp_methods */
    0, 0, 0, 0, 0, 0, 0,
    (initproc)PyWidget_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,                 /* tp_new */
};


//--------------------------------------------------------------------------------
// PyVBoxLayout
static void PyVBoxLayout_dealloc(PyVBoxLayout* self)
{
    Object_Delete(self->obj);
}

static int PyVBoxLayout_init(PyVBoxLayout *self, PyObject *args, PyObject *kwds)
{
    PyObject * arg1=NULL, *tmp;
    static char *kwlist[] = { NULL};
    if (!PyArg_ParseTuple(args, "O", &arg1))
            return -1;

    if (arg1) {
        Py_INCREF(arg1);
        self->obj = (Object*)VBoxLayout_New((Widget*)((PyWidget*)(arg1))->obj);
    }
    return 0;
}

static PyObject * PyVBoxLayout_get_class_name(PyVBoxLayout* self)
{
    PyObject* py_result =PyUnicode_FromString( Object_GetClassName(self->obj) );
    return py_result;
}

static PyObject * PyVBoxLayout_add_widget(PyVBoxLayout* self, PyObject *args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);

    Layout_AddWidget((struct Layout*)self->obj, (Widget*)((PyWidget*)(arg1))->obj);
    Py_RETURN_NONE;
}

PyMethodDef PyVBoxLayout_methods [] = {
    {"get_class_name", (PyCFunction)PyVBoxLayout_get_class_name, METH_NOARGS, "Return the class name" },
    {"add_widget", (PyCFunction)PyVBoxLayout_add_widget, METH_VARARGS, "Add widget" },
    { NULL }
};

static PyTypeObject PyVBoxLayoutType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pywidgets.VBoxLayout",    /* tp_name */
    sizeof(PyVBoxLayout),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)PyVBoxLayout_dealloc, /* tp_dealloc */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "VBoxLayout object",           /* tp_doc */
    0, 0, 0, 0, 0, 0,                         /* tp_iternext */
    PyVBoxLayout_methods,             /* tp_methods */
    0, 0, 0, 0, 0, 0, 0,
    (initproc)PyVBoxLayout_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,                 /* tp_new */
};

//--------------------------------------------------------------------------------
// PyLabel

static void PyLabel_dealloc(PyLabel* self)
{
    Object_Delete(self->obj);
}

static int PyLabel_init(PyLabel *self, PyObject *args, PyObject *kwds)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);

    self->obj = (Object*)Label_New((Widget*)((PyWidget*)(arg1))->obj);
    return 0;
}

static PyObject * PyLabel_get_class_name(PyLabel* self)
{
    PyObject* py_result =PyUnicode_FromString( Object_GetClassName(self->obj) );
    return py_result;
}

static PyObject * PyLabel_set_text(PyLabel* self, PyObject *args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);

    Label_SetText((Label*)self->obj, PyUnicode_AsUTF8(arg1));
    Py_RETURN_NONE;
}


PyMethodDef PyLabel_methods [] = {
    {"get_class_name", (PyCFunction)PyLabel_get_class_name, METH_NOARGS, "Return the class name" },
    {"set_text", (PyCFunction)PyLabel_set_text, METH_VARARGS, "Set text" },
    { NULL }
};
static PyTypeObject PyLabelType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pywidgets.Label",    /* tp_name */
    sizeof(PyLabel),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)PyLabel_dealloc, /* tp_dealloc */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "Widget object",           /* tp_doc */
    0, 0, 0, 0, 0, 0,
    PyLabel_methods,             /* tp_methods */
    0, 0, 0, 0, 0, 0, 0,
    (initproc)PyLabel_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,                 /* tp_new */
};
//--------------------------------------------------------------------------------
// PyPushButton

static void PyPushButton_dealloc(PyPushButton* self)
{
    Object_Delete(self->obj);
}

static int PyPushButton_init(PyPushButton *self, PyObject *args, PyObject *kwds)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);

    self->obj = (Object*)PushButton_New((Widget*)((PyWidget*)(arg1))->obj);
    return 0;
}

static PyObject * PyPushButton_get_class_name(PyPushButton* self)
{
    PyObject* py_result =PyUnicode_FromString( Object_GetClassName(self->obj) );
    return py_result;
}

static PyObject * PyPushButton_set_text(PyPushButton* self, PyObject *args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);

    PushButton_SetText((PushButton*)self->obj, PyUnicode_AsUTF8(arg1));
    Py_RETURN_NONE;
}

static PyObject *callback_arg = NULL;
static PyObject *callback = NULL;

void f(Object *sender)
{
     PyObject *result =  PyEval_CallObject(callback, callback_arg);
     Py_DECREF(result);
}

static PyObject * PyPushButton_set_on_clicked(PyPushButton* self, PyObject *args)
{

    callback = PyTuple_GetItem(args, 0);
    callback_arg = Py_BuildValue("(O)", self);

    PushButton_SetOnClicked((PushButton*)self->obj, f);
    Py_RETURN_NONE;
}

PyMethodDef PyPushButton_methods [] = {
    {"get_class_name", (PyCFunction)PyPushButton_get_class_name, METH_NOARGS, "Return the class name" },
    {"set_text", (PyCFunction)PyPushButton_set_text, METH_VARARGS, "Set text" },
    {"set_on_clicked", (PyCFunction)PyPushButton_set_on_clicked, METH_VARARGS, "Set on clicked" },
    { NULL }
};

static PyTypeObject PyPushButtonType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pywidgets.PushButton",    /* tp_name */
    sizeof(PyPushButton),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)PyLabel_dealloc, /* tp_dealloc */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "Widget object",           /* tp_doc */
    0, 0, 0, 0, 0, 0,
    PyPushButton_methods,             /* tp_methods */
    0, 0, 0, 0, 0, 0, 0,
    (initproc)PyPushButton_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyType_GenericNew,                 /* tp_new */
};
//--------------------------------------------------------------------------------

static PyModuleDef pywidgetsmodule = {
    PyModuleDef_HEAD_INIT,
    "pywidgets",
    "Example module that creates an extension type.",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_pywidgets(void)
{
    PyObject* m;

    if ((PyType_Ready(&PyApplicationType) < 0) ||
        (PyType_Ready(&PyWidgetType) < 0) ||
        (PyType_Ready(&PyVBoxLayoutType) < 0) ||
        (PyType_Ready(&PyLabelType) < 0) ||
        (PyType_Ready(&PyPushButtonType) < 0))
        return NULL;

    m = PyModule_Create(&pywidgetsmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyApplicationType);
    Py_INCREF(&PyWidgetType);
    Py_INCREF(&PyVBoxLayoutType);
    Py_INCREF(&PyLabelType);
    Py_INCREF(&PyPushButtonType);

    PyModule_AddObject(m, "Application", (PyObject *)&PyApplicationType);
    PyModule_AddObject(m, "Widget", (PyObject *)&PyWidgetType);
    PyModule_AddObject(m, "VBoxLayout", (PyObject *)&PyVBoxLayoutType);
    PyModule_AddObject(m, "Label", (PyObject *)&PyLabelType);
    PyModule_AddObject(m, "PushButton", (PyObject *)&PyPushButtonType);
    return m;
}
