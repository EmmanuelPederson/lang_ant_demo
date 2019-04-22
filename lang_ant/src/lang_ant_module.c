/*
 * Returns the distance from the origin after simulation
 * of langton's ant after n steps.
 */
#include <Python.h>
#include <string.h>
#include <stdio.h>
#include "lang_ant_types.h"
#include "lang_rule.h"

static PyObject* sim_lang_ant(PyObject *self, PyObject *args){
    const char *rule_def_str;
    unsigned int n;
    StateDirRule *state_dir_rule;
    XYPos final_pos;

    if(!PyArg_ParseTuple(args, "si", &rule_def_str, &n)){
        return NULL;
    }else{
        if(strlen(rule_def_str) > MAX_RULES){
            PyErr_SetString(PyExc_ValueError, "Too many rules provided");
            return NULL;
        }
        state_dir_rule = make_rule_from_str(rule_def_str);
        final_pos = langtons_ant(state_dir_rule, n);
        free(state_dir_rule);
        return Py_BuildValue("(i, i)", final_pos.x, final_pos.y);
    }
}

static PyMethodDef lang_ant_methods[] = {
    {"sim_lang_ant", sim_lang_ant, METH_VARARGS,
     "Returns the dist of Langton's ant from the origin after n iterations."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef lang_ant_module = {
    PyModuleDef_HEAD_INIT,
    "lang_ant",
    NULL,
    -1,
    lang_ant_methods
};

PyMODINIT_FUNC PyInit_lang_ant(void){
    return PyModule_Create(&lang_ant_module);
};
