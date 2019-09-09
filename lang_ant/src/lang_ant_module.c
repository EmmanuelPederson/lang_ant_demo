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

static PyObject* sim_lang_ant_colony(PyObject *self, PyObject *args){
    const char *rule_def_str;
    unsigned int n;
    unsigned int num_ants;
    StateDirRule *state_dir_rule;
    XYPos *pos_arr, pos;
    Dir *dir_arr;
    PyObject *py_final_pos_list, *py_final_pos;

    if(!PyArg_ParseTuple(args, "sii", &rule_def_str, &n, &num_ants)){
        return NULL;
    }else{
        if(strlen(rule_def_str) > MAX_RULES){
            PyErr_SetString(PyExc_ValueError, "Too many rules provided");
            return NULL;
        }
        state_dir_rule = make_rule_from_str(rule_def_str);
        pos_arr = calloc(sizeof(XYPos), num_ants);
        dir_arr = calloc(sizeof(Dir), num_ants);

        for (int i = 0; i < num_ants; i++){
            pos_arr[i].x = i;
            pos_arr[i].y = i;
            dir_arr[i] = RIGHT;
        }

        langtons_ant_colony(state_dir_rule, n, num_ants, pos_arr, dir_arr);

        /* Instantiate an empty list and then fill it */
        py_final_pos_list = PyList_New(0);
        for (int i = 0; i < num_ants; i++){
            pos = pos_arr[i];
            py_final_pos = Py_BuildValue("(i, i)", pos.x, pos.y);
            PyList_Append(py_final_pos_list, py_final_pos);
        }
        
        free(state_dir_rule);
        free(pos_arr);
        free(dir_arr);

        return py_final_pos_list;
    }
}

static PyMethodDef lang_ant_methods[] = {
    {"sim_lang_ant", sim_lang_ant, METH_VARARGS,
     "Returns the final position of Langton's ant after n iterations."},
    {"sim_lang_ant_colony", sim_lang_ant_colony, METH_VARARGS,
     "Returns the final positions of Langton's ants after n iterations."},
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
