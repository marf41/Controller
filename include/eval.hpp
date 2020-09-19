#include <Arduino.h>
#include <WiFi.h>

#include "oled.hpp"

extern "C" {
    #include "tcl.h"
}

#define tcl_func_register(cmd, name) tcl_register(&tcl, #cmd, tcl_cmd_##name, 1, NULL)
#define tcl_func_one(name, cmd) tcl_func(tcl_cmd_##name, cmd)
#define tcl_func(name, cmd) static int (name)(struct tcl *tcl, tcl_value_t *args, void *arg) { \
    (void)arg; cmd(); return tcl_result(tcl, FNORMAL, tcl_alloc("", 0)); }

String tclEval(String input);