extern "C" {
#include <Python.h>
#include "widgets.h"
}

extern "C" PyObject* py_Application_New(PyObject* module, PyObject* args)
{
    PyObject * py_result = PyLong_FromLong((long)Application_New());
    return py_result;
}

extern "C" PyObject* py_Object_GetClassName(PyObject* module, PyObject* args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);

    PyObject * py_result = PyUnicode_FromString(Object_GetClassName((struct Object*)PyLong_AsLong(arg1)));
    return py_result;
}

extern "C" PyObject* py_Widget_New(PyObject* module, PyObject* args)
{
    PyObject * py_result =  PyLong_FromLong((long)Widget_New(NULL));
    return py_result;
}

extern "C" PyObject* py_VBoxLayout_New(PyObject* module, PyObject* args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);

    PyObject * py_result = PyLong_FromLong((long)VBoxLayout_New((Widget*)PyLong_AsLong(arg1)));
    return py_result;
}

extern "C" PyObject* py_Widget_SetLayout(PyObject* module, PyObject* args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);
    PyObject * arg2 = PyTuple_GetItem(args, 1);

    Widget_SetLayout((Widget*)PyLong_AsLong(arg1), (Layout*)PyLong_AsLong(arg2));
    Py_RETURN_NONE;
}

extern "C" PyObject* py_Label_New(PyObject* module, PyObject* args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);

    PyObject * py_result = PyLong_FromLong((long)Label_New((Widget*)PyLong_AsLong(arg1)));
    return py_result;
}

extern "C" PyObject* py_Label_SetText(PyObject* module, PyObject* args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);
    PyObject * arg2 = PyTuple_GetItem(args, 1);

    Label_SetText((Label*)PyLong_AsLong(arg1), PyUnicode_AsUTF8(arg2));
    Py_RETURN_NONE;
}

extern "C" PyObject* py_Layout_AddWidget(PyObject* module, PyObject* args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);
    PyObject * arg2 = PyTuple_GetItem(args, 1);

    Layout_AddWidget((struct Layout*)PyLong_AsLong(arg1), (Widget*)PyLong_AsLong(arg2));
    Py_RETURN_NONE;
}

extern "C" PyObject* py_PushButton_New(PyObject* module, PyObject* args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);

    PyObject * py_result = PyLong_FromLong((long)PushButton_New((Widget*)PyLong_AsLong(arg1)));
    return py_result;
}

extern "C" PyObject* py_PushButton_SetText(PyObject* module, PyObject* args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);
    PyObject * arg2 = PyTuple_GetItem(args, 1);

    PushButton_SetText((PushButton*)PyLong_AsLong(arg1), PyUnicode_AsUTF8(arg2));
    Py_RETURN_NONE;
}

extern "C" PyObject* py_Widget_SetWindowTitle(PyObject* module, PyObject* args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);
    PyObject * arg2 = PyTuple_GetItem(args, 1);

    Widget_SetWindowTitle((Widget*)PyLong_AsLong(arg1), PyUnicode_AsUTF8(arg2));
    Py_RETURN_NONE;
}

extern "C" PyObject* py_Widget_SetSize(PyObject* module, PyObject* args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);
    PyObject * arg2 = PyTuple_GetItem(args, 1);
    PyObject * arg3 = PyTuple_GetItem(args, 2);

    Widget_SetSize((Widget*)PyLong_AsLong(arg1), PyLong_AsLong(arg2), PyLong_AsLong(arg3));
    Py_RETURN_NONE;
}

extern "C" PyObject* py_Widget_SetVisible(PyObject* module, PyObject* args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);
    PyObject * arg2 = PyTuple_GetItem(args, 1);

    Widget_SetVisible((Widget*)PyLong_AsLong(arg1), PyObject_IsTrue(arg2));
    Py_RETURN_NONE;
}

extern "C" PyObject* py_Application_Exec(PyObject* module, PyObject* args)
{
    PyObject * arg1 = PyTuple_GetItem(args, 0);

    PyObject * py_result = PyLong_FromLong(Application_Exec((Application*)PyLong_AsLong(arg1)));
    return py_result;
}


PyMODINIT_FUNC PyInit__pywidgets() {

    static PyMethodDef methods[] = {
      {"Application_New", py_Application_New, METH_VARARGS, ""},
      {"Object_GetClassName", py_Object_GetClassName, METH_VARARGS, ""},
      {"Widget_New", py_Widget_New, METH_VARARGS, ""},
      {"VBoxLayout_New", py_VBoxLayout_New, METH_VARARGS, ""},
      {"Widget_SetLayout", py_Widget_SetLayout, METH_VARARGS, ""},
      {"Label_New", py_Label_New, METH_VARARGS, ""},
      {"Label_SetText", py_Label_SetText, METH_VARARGS, ""},
      {"Layout_AddWidget", py_Layout_AddWidget, METH_VARARGS, ""},
      {"PushButton_New", py_PushButton_New, METH_VARARGS, ""},
      {"PushButton_SetText", py_PushButton_SetText, METH_VARARGS, ""},
      {"Widget_SetWindowTitle", py_Widget_SetWindowTitle, METH_VARARGS, ""},
      {"Widget_SetSize", py_Widget_SetSize, METH_VARARGS, ""},
      {"Widget_SetVisible", py_Widget_SetVisible, METH_VARARGS, ""},
      {"Application_Exec", py_Application_Exec, METH_VARARGS, ""},
      {NULL, NULL, 0, NULL}
    };

    static PyModuleDef modDef = {
        PyModuleDef_HEAD_INIT,
        "_pywidgets",
        "_pywidgets module",
        -1, methods
    };

    PyObject* mod = PyModule_Create(&modDef);
    return mod;
}
