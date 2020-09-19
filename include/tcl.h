#pragma once

#include <stdlib.h>

#include <stdio.h>
#include <string.h>

#if 0
#define DBG printf
#else
#define DBG(...)
#endif

#define MAX_VAR_LENGTH 256

struct tcl;
int tcl_eval(struct tcl *tcl, const char *s, size_t len);

/* Token type and control flow constants */
enum { TCMD, TWORD, TPART, TERROR };
enum { FERROR, FNORMAL, FRETURN, FBREAK, FAGAIN };

int tcl_is_special(char c, int q);
int tcl_is_space(char c);
int tcl_is_end(char c);

int tcl_next(const char *s, size_t n, const char **from, const char **to,
             int *q);

/* A helper parser struct and macro (requires C99) */
struct tcl_parser {
  const char *from;
  const char *to;
  const char *start;
  const char *end;
  int q;
  int token;
};

#define tcl_each(s, len, skiperr)                                              \
  for (struct tcl_parser p = {NULL, NULL, (s), (s) + (len), 0, TERROR};        \
       p.start < p.end &&                                                      \
       (((p.token = tcl_next(p.start, p.end - p.start, &p.from, &p.to,         \
                             &p.q)) != TERROR) ||                              \
        (skiperr));                                                            \
       p.start = p.to)

/* ------------------------------------------------------- */
/* ------------------------------------------------------- */
/* ------------------------------------------------------- */
/* ------------------------------------------------------- */
/* ------------------------------------------------------- */
typedef char tcl_value_t;

const char *tcl_string(tcl_value_t *v);
int tcl_int(tcl_value_t *v);
int tcl_length(tcl_value_t *v);

void tcl_free(tcl_value_t *v);

tcl_value_t *tcl_append_string(tcl_value_t *v, const char *s, size_t len);
tcl_value_t *tcl_append(tcl_value_t *v, tcl_value_t *tail);
tcl_value_t *tcl_alloc(const char *s, size_t len);
tcl_value_t *tcl_dup(tcl_value_t *v);
tcl_value_t *tcl_list_alloc();

int tcl_list_length(tcl_value_t *v);
void tcl_list_free(tcl_value_t *v);

tcl_value_t *tcl_list_at(tcl_value_t *v, int index);
tcl_value_t *tcl_list_append(tcl_value_t *v, tcl_value_t *tail);

/* ----------------------------- */
/* ----------------------------- */
/* ----------------------------- */
/* ----------------------------- */

typedef int (*tcl_cmd_fn_t)(struct tcl *, tcl_value_t *, void *);

struct tcl_cmd {
  tcl_value_t *name;
  int arity;
  tcl_cmd_fn_t fn;
  void *arg;
  struct tcl_cmd *next;
};

struct tcl_var {
  tcl_value_t *name;
  tcl_value_t *value;
  struct tcl_var *next;
};

struct tcl_env {
  struct tcl_var *vars;
  struct tcl_env *parent;
};

struct tcl_env *tcl_env_alloc(struct tcl_env *parent);
struct tcl_var *tcl_env_var(struct tcl_env *env, tcl_value_t *name);
struct tcl_env *tcl_env_free(struct tcl_env *env);

struct tcl {
  struct tcl_env *env;
  struct tcl_cmd *cmds;
  tcl_value_t *result;
};

tcl_value_t *tcl_var(struct tcl *tcl, tcl_value_t *name, tcl_value_t *v);

int tcl_result(struct tcl *tcl, int flow, tcl_value_t *result);
int tcl_subst(struct tcl *tcl, const char *s, size_t len);
int tcl_eval(struct tcl *tcl, const char *s, size_t len);

/* --------------------------------- */
/* --------------------------------- */
/* --------------------------------- */
/* --------------------------------- */
/* --------------------------------- */
void tcl_register(struct tcl *tcl, const char *name, tcl_cmd_fn_t fn, int arity,
                  void *arg);

int tcl_cmd_set(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_subst(struct tcl *tcl, tcl_value_t *args, void *arg);
#ifndef TCL_DISABLE_PUTS
int tcl_cmd_puts(struct tcl *tcl, tcl_value_t *args, void *arg);
#endif

int tcl_user_proc(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_proc(struct tcl *tcl, tcl_value_t *args, void *arg);

int tcl_cmd_if(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_flow(struct tcl *tcl, tcl_value_t *args, void *arg);
int tcl_cmd_while(struct tcl *tcl, tcl_value_t *args, void *arg);

#ifndef TCL_DISABLE_MATH
int tcl_cmd_math(struct tcl *tcl, tcl_value_t *args, void *arg);
#endif

void tcl_init(struct tcl *tcl);
void tcl_destroy(struct tcl *tcl);