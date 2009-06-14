/* type decls */
typedef struct _IO_FILE FILE;
typedef unsigned int size_t;
typedef int bool;
typedef struct EnvironSnapshot EnvironSnapshot;
typedef struct Mark Mark;
struct UnmarkedSyntax;
struct Syntax;
struct SyntaxList;
struct SyntaxEnum;
struct Match;
typedef struct Context Context;
typedef struct Environ Environ;
enum _s_0_;
typedef enum _s_0_ Position;
struct UserTypeInfo;
struct ModuleInfo;
/* type definitions */
enum _s_0_{
  NoPos = 0,
  OtherPos = 1,
  TopLevel = 2,
  FieldPos = 4,
  StmtDeclPos = 8,
  StmtPos = 16,
  ExpPos = 32,
};
/* macro sep. c. stuff */
unsigned _macro_funs_size = 1;
const char * _macro_funs[1] = {
  "parse_class"
};
unsigned _syntaxes_size = 74;
struct {const char * str; struct UnmarkedSyntax * syn;} _syntaxes[74] = {
  {"raw_syntax (name @ body :(public parent_n))", 0},
  {"syntax name", 0},
  {"syntax body", 0},
  {"syntax parent_n", 0},
  {"syntax {declare_user_type name;}", 0},
  {"syntax _Data", 0},
  {"syntax _Data", 0},
  {"syntax parent", 0},
  {"syntax parent", 0},
  {"syntax _VTable", 0},
  {"syntax _VTable", 0},
  {"syntax _vtable", 0},
  {"syntax _vtable", 0},
  {"syntax _vptr", 0},
  {"syntax _vptr", 0},
  {"syntax _constructor", 0},
  {"syntax _constructor", 0},
  {"syntax _vtable_init", 0},
  {"syntax _vtable_init", 0},
  {"syntax parent_n::_VTable", 0},
  {"syntax {parent_n parent;}", 0},
  {"syntax {make_subtype parent_n class_cast_up;}", 0},
  {"syntax {import parent_n;}", 0},
  {"raw_syntax (declare_user_type (mid name))", 0},
  {"raw_syntax ({...} @body)", 0},
  {"syntax body", 0},
  {"raw_syntax (what n @_)", 0},
  {"syntax what", 0},
  {"syntax n", 0},
  {"syntax {class _VTable : public $1 {$2;};}", 0},
  {"syntax {class _VTable {$1;};}", 0},
  {"syntax {void _vtable_init(_VTable * _vt) {$1;} }", 0},
  {"syntax _vtable_init", 0},
  {"syntax {__static_constructor void init() {parent_n::_vtable_init(&_vtable); _vtable_init(&_vtable);}}", 0},
  {"syntax {__static_constructor void init() {_vtable_init(&_vtable);}}", 0},
  {"syntax {macro _vptr (:this this = this) {*(_VTable * *)&(((parent_n *)this)->_vptr);}}", 0},
  {"raw_syntax (var (mid _vptr) (.ptr (mid _VTable)))", 0},
  {"syntax _vptr", 0},
  {"syntax {void _constructor() {_vptr = &_vtable;}}", 0},
  {"syntax _constructor", 0},
  {"syntax {class _VTable;}", 0},
  {"syntax _VTable", 0},
  {"syntax _VTable`outer", 0},
  {"syntax {struct _Data {$1; $2;};}", 0},
  {"syntax {finalize_user_type struct _Data;}", 0},
  {"syntax {_VTable _vtable;}", 0},
  {"syntax {export $1;}", 0},
  {"syntax {user_type name {$1; $2;}}", 0},
  {"raw_syntax (n @rest)", 0},
  {"syntax static", 0},
  {"raw_syntax(var (w/inner (mid n) internal) (mid rest))", 0},
  {"syntax {macro n () {n`internal;}}", 0},
  {"syntax {macro n (:this this = this) {(*(name *)this)..n;}}", 0},
  {"raw_syntax (n parms ret body)", 0},
  {"raw_syntax (@parms)", 0},
  {"syntax parms", 0},
  {"syntax static", 0},
  {"syntax virtual", 0},
  {"syntax nv", 0},
  {"syntax n`non_virtual", 0},
  {"syntax nv", 0},
  {"syntax n", 0},
  {"raw_syntax ((.ptr (mid name)) (fluid this))", 0},
  {"syntax ((name  *)this)", 0},
  {"syntax parms", 0},
  {"syntax (:this this = this)", 0},
  {"raw_syntax (fun (w/inner (mid n) internal) (.(mid $1)) (mid ret) (mid body))", 0},
  {"syntax {macro nv ($1) {n`internal($2);}}", 0},
  {"syntax {macro n ($1) {(this->_vptr->n)($2);}}", 0},
  {"syntax pfun", 0},
  {"raw_syntax (.ptr (.fun (.(mid $1)) (mid ret)))", 0},
  {"syntax n", 0},
  {"raw_syntax (var (mid n) (mid pfun))", 0},
  {"raw_syntax (assign (-> (id _vt) (id (mid n))) \n                                         (cast (mid pfun) (id (w/inner (mid n) internal))))", 0}
};

/* function decls */
int printf(char const *, ...);
int snprintf(char const * str, size_t size, char const * format, ...);
int strcmp(char const * s1, char const * s2);
Mark * new_mark_f(EnvironSnapshot *);
struct Syntax * syntax_flag(struct Syntax *, struct UnmarkedSyntax *);
struct SyntaxList * new_syntax_list();
int syntax_list_empty(struct SyntaxList const *);
void syntax_list_append(struct SyntaxList *, struct Syntax *);
struct Syntax * syntax_enum_next(struct SyntaxEnum *);
struct Match * match(struct Match *, struct UnmarkedSyntax * pattern, struct Syntax * with);
struct Match * match_args(struct Match *, struct UnmarkedSyntax * pattern, struct Syntax * with);
struct Match * match_local(struct Match *, ...);
struct Syntax * match_var(struct Match *, struct UnmarkedSyntax *);
struct SyntaxEnum * match_varl(struct Match *, struct UnmarkedSyntax *);
struct Syntax * replace(struct UnmarkedSyntax *, struct Match *, Mark *);
Context * get_context(struct Syntax *);
struct Syntax * replace_context(struct UnmarkedSyntax *, Context *);
struct Syntax * partly_expand(struct Syntax *, Position pos, Environ *);
struct SyntaxEnum * partly_expand_list(struct SyntaxEnum *, Position pos, Environ *);
struct Syntax * pre_parse(struct Syntax *, Environ *);
struct Syntax * reparse(struct Syntax *, char const *, Environ *);
struct UnmarkedSyntax * string_to_syntax(char const * str);
char const * syntax_to_string(struct UnmarkedSyntax *);
void dump_syntax(struct UnmarkedSyntax *);
struct UserTypeInfo * user_type_info(struct Syntax *, Environ *);
struct ModuleInfo * user_type_module(struct UserTypeInfo *);
struct ModuleInfo * module_info(struct Syntax *, Environ *);
struct SyntaxEnum * module_symbols(struct ModuleInfo *);
bool module_have_symbol(struct ModuleInfo *, struct Syntax *);
Environ * temp_environ(Environ *);
size_t ct_value(struct Syntax *, Environ *);
struct Syntax * error(struct Syntax *, char const *, ...);
struct Syntax * get_symbol_prop(struct Syntax * sym, struct Syntax * prop, Environ * env);
int symbol_exists(struct UnmarkedSyntax * sym, struct Syntax * where, Mark *, Environ *);
struct Syntax * parse_class(struct Syntax * p, Environ * env);
void parse_class_var(struct Syntax * p, struct Match * m, Mark * mark, struct SyntaxList * struct_b, struct SyntaxList * module_b);
void parse_class_fun(struct Syntax * p, struct Match * m, Mark * mark, struct SyntaxList * struct_b, struct SyntaxList * module_b, struct UserTypeInfo * parent_vtable, struct SyntaxList * vtable_b, struct SyntaxList * vtable_i, Environ * env);
/* definitions */
struct EnvironSnapshot * parse_class$env_ss;
struct Syntax * parse_class(struct Syntax * p, Environ * env)
{
  Mark * mark$1 = new_mark_f(parse_class$env_ss);
  struct Match * m$1 = match_args(0, _syntaxes[0].syn, p);
  struct Syntax * name$1 = match_var(m$1, _syntaxes[1].syn);
  struct Syntax * body$1 = match_var(m$1, _syntaxes[2].syn);
  struct Syntax * parent_s$1 = match_var(m$1, _syntaxes[3].syn);
  if ((!body$1))
    {
      return replace(_syntaxes[4].syn, m$1, mark$1);
    }
  struct UserTypeInfo * parent$1 = (((int)parent_s$1) ? user_type_info(parent_s$1, env) : 0);
  Context * context$1 = get_context(name$1);
  struct ModuleInfo * parent_m$1 = 0;
  struct UserTypeInfo * parent_vtable$1 = 0;
  struct Syntax * parent_vtable_n$1 = 0;
  struct Syntax * vtable$1 = 0;
  struct SyntaxList * struct_p$1 = new_syntax_list();
  struct SyntaxList * struct_b$1 = new_syntax_list();
  struct SyntaxList * module_p$1 = new_syntax_list();
  struct SyntaxList * module_b$1 = new_syntax_list();
  struct SyntaxList * vtable_b$1 = new_syntax_list();
  struct SyntaxList * vtable_i$1 = new_syntax_list();
  struct SyntaxList * exports$1 = new_syntax_list();
  (m$1 = match(m$1, _syntaxes[5].syn, replace_context(_syntaxes[6].syn, context$1)));
  (m$1 = match(m$1, _syntaxes[7].syn, replace_context(_syntaxes[8].syn, context$1)));
  (m$1 = match(m$1, _syntaxes[9].syn, replace_context(_syntaxes[10].syn, context$1)));
  (m$1 = match(m$1, _syntaxes[11].syn, replace_context(_syntaxes[12].syn, context$1)));
  (m$1 = match(m$1, _syntaxes[13].syn, replace_context(_syntaxes[14].syn, context$1)));
  (m$1 = match(m$1, _syntaxes[15].syn, replace_context(_syntaxes[16].syn, context$1)));
  (m$1 = match(m$1, _syntaxes[17].syn, replace_context(_syntaxes[18].syn, context$1)));
  if (((int)parent$1))
    {
      (parent_m$1 = user_type_module(parent$1));
      (parent_vtable_n$1 = replace(_syntaxes[19].syn, m$1, mark$1));
      (parent_vtable$1 = user_type_info(parent_vtable_n$1, env));
      syntax_list_append(struct_p$1, replace(_syntaxes[20].syn, m$1, mark$1));
      syntax_list_append(module_p$1, replace(_syntaxes[21].syn, m$1, mark$1));
      syntax_list_append(module_p$1, replace(_syntaxes[22].syn, m$1, mark$1));
      struct SyntaxEnum * itr$1 = module_symbols(parent_m$1);
      struct Syntax * syn$1;
      {
        __label__ break$1;
        __label__ continue$1;
      continue$1:;
        if ((!(syn$1 = syntax_enum_next(itr$1))))
          goto break$1;
        {
          syntax_list_append(exports$1, syn$1);
        }
        goto continue$1;
      break$1:;
      }
    }
  Environ * lenv$1 = temp_environ(env);
  pre_parse(replace(_syntaxes[23].syn, m$1, mark$1), lenv$1);
  struct SyntaxEnum * itr$1 = partly_expand_list(match_varl(match(0, _syntaxes[24].syn, body$1), _syntaxes[25].syn), FieldPos, lenv$1);
  struct Syntax * member$1;
  {
    __label__ break$1;
    __label__ continue$1;
  continue$1:;
    if ((!(member$1 = syntax_enum_next(itr$1))))
      goto break$1;
    {
      struct Match * m0$1 = match(m$1, _syntaxes[26].syn, member$1);
      char const * what$1 = syntax_to_string(((struct UnmarkedSyntax *)match_var(m0$1, _syntaxes[27].syn)));
      if ((strcmp(what$1, "label") == 0))
        goto continue$1;
      if ((strcmp(what$1, "var") == 0))
        parse_class_var(member$1, m$1, mark$1, struct_b$1, module_b$1);
      else
        if ((strcmp(what$1, "fun") == 0))
          parse_class_fun(member$1, m$1, mark$1, struct_b$1, module_b$1, parent_vtable$1, vtable_b$1, vtable_i$1, lenv$1);
        else
          syntax_list_append(module_p$1, pre_parse(member$1, lenv$1));
      syntax_list_append(exports$1, match_var(m0$1, _syntaxes[28].syn));
    }
    goto continue$1;
  break$1:;
  }
  bool const need_vtable$1 = (!syntax_list_empty(vtable_i$1));
  if (need_vtable$1)
    {
      if (((int)parent_vtable$1))
        {
          (vtable$1 = replace(_syntaxes[29].syn, match_local(m$1, parent_vtable_n$1, vtable_b$1, 0), mark$1));
        }
      else
        {
          (vtable$1 = replace(_syntaxes[30].syn, match_local(m$1, vtable_b$1, 0), mark$1));
        }
      syntax_list_append(module_b$1, replace(_syntaxes[31].syn, match_local(m$1, vtable_i$1, 0), mark$1));
      syntax_list_append(exports$1, replace(_syntaxes[32].syn, m$1, mark$1));
      if (((int)parent_vtable$1))
        {
          syntax_list_append(module_b$1, replace(_syntaxes[33].syn, m$1, mark$1));
        }
      else
        {
          syntax_list_append(module_b$1, replace(_syntaxes[34].syn, m$1, mark$1));
        }
      if (((int)parent_vtable$1))
        {
          syntax_list_append(module_b$1, replace(_syntaxes[35].syn, m$1, mark$1));
        }
      else
        {
          struct Syntax * v$1 = replace(_syntaxes[36].syn, m$1, mark$1);
          parse_class_var(v$1, m$1, mark$1, struct_p$1, module_b$1);
        }
      syntax_list_append(exports$1, replace(_syntaxes[37].syn, m$1, mark$1));
      struct Syntax * f$1 = partly_expand(replace(_syntaxes[38].syn, m$1, mark$1), FieldPos, env);
      parse_class_fun(f$1, m$1, mark$1, struct_p$1, module_p$1, 0, vtable_b$1, vtable_i$1, env);
      syntax_list_append(exports$1, replace(_syntaxes[39].syn, m$1, mark$1));
    }
  if (need_vtable$1)
    {
      syntax_list_append(module_p$1, replace(_syntaxes[40].syn, m$1, mark$1));
      syntax_list_append(exports$1, replace(_syntaxes[41].syn, m$1, mark$1));
      syntax_list_append(exports$1, replace(_syntaxes[42].syn, m$1, mark$1));
    }
  syntax_list_append(module_p$1, replace(_syntaxes[43].syn, match_local(m$1, struct_p$1, struct_b$1, 0), mark$1));
  syntax_list_append(module_p$1, replace(_syntaxes[44].syn, m$1, mark$1));
  if (need_vtable$1)
    {
      syntax_list_append(module_p$1, vtable$1);
      syntax_list_append(module_p$1, replace(_syntaxes[45].syn, m$1, mark$1));
    }
  syntax_list_append(module_b$1, replace(_syntaxes[46].syn, match_local(m$1, exports$1, 0), mark$1));
  struct Syntax * module_$1 = replace(_syntaxes[47].syn, match_local(m$1, module_p$1, module_b$1, 0), mark$1);
  return module_$1;
}
void parse_class_var(struct Syntax * p, struct Match * m, Mark * mark, struct SyntaxList * struct_b, struct SyntaxList * module_b)
{
  (m = match_args(m, _syntaxes[48].syn, p));
  bool is_static$1 = ((bool)syntax_flag(p, _syntaxes[49].syn));
  if (is_static$1)
    {
      syntax_list_append(module_b, replace(_syntaxes[50].syn, m, mark));
      syntax_list_append(module_b, replace(_syntaxes[51].syn, m, mark));
    }
  else
    {
      syntax_list_append(struct_b, p);
      syntax_list_append(module_b, replace(_syntaxes[52].syn, m, mark));
    }
}
void parse_class_fun(struct Syntax * p, struct Match * m, Mark * mark, struct SyntaxList * struct_b, struct SyntaxList * module_b, struct UserTypeInfo * parent_vtable, struct SyntaxList * vtable_b, struct SyntaxList * vtable_i, Environ * env)
{
  (m = match_args(m, _syntaxes[53].syn, p));
  (m = match_args(m, _syntaxes[54].syn, match_var(m, _syntaxes[55].syn)));
  bool is_static$1 = ((bool)syntax_flag(p, _syntaxes[56].syn));
  bool is_virtual$1 = ((bool)syntax_flag(p, _syntaxes[57].syn));
  if (is_virtual$1)
    (m = match(m, _syntaxes[58].syn, replace(_syntaxes[59].syn, m, mark)));
  else
    (m = match(m, _syntaxes[60].syn, replace(_syntaxes[61].syn, m, mark)));
  struct SyntaxList * new_parms$1 = new_syntax_list();
  if ((!is_static$1))
    syntax_list_append(new_parms$1, replace(_syntaxes[62].syn, m, mark));
  struct SyntaxList * macro_parms$1 = new_syntax_list();
  struct SyntaxList * call_parms$1 = new_syntax_list();
  if ((!is_static$1))
    syntax_list_append(call_parms$1, replace(_syntaxes[63].syn, m, mark));
  struct SyntaxEnum * parms_enum$1 = match_varl(m, _syntaxes[64].syn);
  unsigned int i$1 = 0u;
  struct Syntax * parm$1;
  {
    __label__ break$1;
    __label__ continue$1;
  continue$1:;
    if ((!(parm$1 = syntax_enum_next(parms_enum$1))))
      goto break$1;
    {
      syntax_list_append(new_parms$1, parm$1);
      char sbuf$1[8];
      snprintf(sbuf$1, ((size_t)8), "arg%d", i$1);
      struct Syntax * arg$1 = replace(string_to_syntax(sbuf$1), 0, mark);
      syntax_list_append(macro_parms$1, arg$1);
      syntax_list_append(call_parms$1, arg$1);
      (i$1 += ((unsigned int)1));
    }
    goto continue$1;
  break$1:;
  }
  if ((!is_static$1))
    syntax_list_append(macro_parms$1, replace(_syntaxes[65].syn, m, mark));
  syntax_list_append(module_b, replace(_syntaxes[66].syn, match_local(m, new_parms$1, 0), mark));
  syntax_list_append(module_b, replace(_syntaxes[67].syn, match_local(m, macro_parms$1, call_parms$1, 0), mark));
  if (is_virtual$1)
    {
      syntax_list_append(module_b, replace(_syntaxes[68].syn, match_local(m, macro_parms$1, call_parms$1, 0), mark));
      (m = match(m, _syntaxes[69].syn, replace(_syntaxes[70].syn, match_local(m, new_parms$1, 0), mark)));
      if ((!(((int)parent_vtable) ? module_have_symbol(user_type_module(parent_vtable), match_var(m, _syntaxes[71].syn)) : 0)))
        {
          syntax_list_append(vtable_b, replace(_syntaxes[72].syn, m, mark));
        }
      syntax_list_append(vtable_i, replace(_syntaxes[73].syn, m, mark));
    }
}
/* special */
/* done */
