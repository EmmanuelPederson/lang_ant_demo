/*
 * Returns the distance from the origin after simulation
 * of langton's ant after n steps.
 */
#include <Python.h>
#include "lang_ant_types.h"

static PyObject* sim_lang_ant(PyObject *self, PyObject *args){
    const char *rule_def;
    int state_cnt;
    unsigned int n;
    StateDirRule state_dir_rule;

    if(!PyArg_ParseTuple(args, "y#i", &rule_def, &state_cnt, &n)){
        return NULL;
    }else{
        if(state_cnt > MAX_RULES){
            PyErr_SetString(PyExc_ValueError, "Too many rules provided");
            return NULL;
        }
        for(int i = 0; i < state_cnt; i++){
            state_dir_rule.state_dir_arr[i] = rule_def[i] ? rule_def[i] : -1;
        }
        state_dir_rule.state_dir_cnt = state_cnt;
        return PyFloat_FromDouble(langtons_ant(state_dir_rule, n));
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
