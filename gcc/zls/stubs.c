
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "c-common.h"
#include "c-pragma.h"

// used by c-opts.c

void init_c_lex (void) {
}

void preprocess_file (cpp_reader * ARG_UNUSED(r)) {
}

void init_pp_output(FILE * f) {
}

void pp_file_change (const struct line_map * ARG_UNUSED(d)) {
}

void fe_file_change (const struct line_map * ARG_UNUSED(d)) {
}

void pp_dir_change (cpp_reader * ARG_UNUSED(r), const char * ARG_UNUSED(s)) {
}

void init_pragma (void) {
}

void pch_init (void) {
}

void c_common_print_pch_checksum (FILE * ARG_UNUSED(f)) {
}

// used by c-decl.c

void c_common_write_pch (void) {
}

void maybe_apply_pragma_weak (tree ARG_UNUSED(t)) {
}

void maybe_apply_pending_pragma_weaks (void) {
}

tree maybe_apply_renaming_pragma (tree ARG_UNUSED(t), tree asmname) {
  return asmname;
}

// used by c-lex.c but GC also thinks its used to to GTY tag in c-common.h

int pending_lang_change;
