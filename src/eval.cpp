#include "eval.hpp"


struct tcl tcl;
bool tclInit;

tcl_func_one(oled_clear, clear)
tcl_func_one(oled_off, oledOff)

// FIXME empty return value
int tcl_cmd_get_ip(struct tcl *tcl, tcl_value_t *args, void *arg) {
    (void)arg;
    String ip = WiFi.localIP().toString();
    return tcl_result(tcl, FNORMAL, tcl_alloc(ip.c_str(), ip.length()));
}

static int tcl_cmd_oled_string(struct tcl *tcl, tcl_value_t *args, void *arg) {
  (void)arg;
  tcl_value_t *strval = tcl_list_at(args, 1);
  tcl_value_t *xval = tcl_list_at(args, 2);
  tcl_value_t *yval = tcl_list_at(args, 3);
  const char *str = tcl_string(strval);
  int x = tcl_int(xval);
  int y = tcl_int(yval);
  oledSetup(); drawString(str, x, y); draw();
  tcl_free(strval);
  tcl_free(xval);
  tcl_free(yval);
  return tcl_result(tcl, FNORMAL, tcl_alloc("", 0));
}


String tclEval(String input) {
    // if (!tclInit) { tcl_init(&tcl); tclInit = true; }
    tcl_init(&tcl);
    tcl_register(&tcl, "display", tcl_cmd_oled_string, 4, NULL);
    tcl_register(&tcl, "ip", tcl_cmd_oled_clear, 1, NULL);
    tcl_func_register(clear, oled_clear);
    tcl_func_register(blank, oled_off);
    if (tcl_eval(&tcl, input.c_str(), input.length()) != FERROR) {
        return String(tcl_string(tcl.result));
    }
    tcl_destroy(&tcl);
    return String("ERROR");
}