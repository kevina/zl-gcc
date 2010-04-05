#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <assert.h>

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "c-tree.h"
#include "c-common.h"
#include "real.h"
#include "function.h"

//
//
//

#define CHAR_SPECIAL 0
#define CHAR_ID      1
#define CHAR_NUM     2
#define CHAR_EOL     4
#define CHAR_SPACE   8

static signed char chartype_[256];
#define chartype(x) chartype_[(unsigned char)x]

typedef struct token {
  const char * str; // NOT null terminated
  unsigned len;
  location_t loc;
} token;

static inline tree token_to_tree(token t) {
  return get_identifier_with_length(t.str, t.len);
}

#define NULL_TOKEN (struct token){NULL, 0, 0}

#define token_eq(tok, s) \
  (__builtin_constant_p(s) \
   ? ((tok).len == sizeof(s)-1 && memcmp((tok).str, s, sizeof(s)-1) == 0) \
   : ((tok).len == strlen(s) && memcmp((tok).str, s, (tok).len) == 0))
#define token_prefix(tok, s) \
  (__builtin_constant_p(s) \
   ? ((tok).len >= sizeof(s)-1 && memcmp((tok).str, s, sizeof(s)-1) == 0) \
   : ((tok).len >= strlen(s) && memcmp((tok).str, s, (tok).len) == 0))

extern const char *this_input_filename;
static char * data = NULL;
static unsigned data_size = 0;
static unsigned line_num = 0;
static const char * line_start;
static const char * str;

static inline struct c_expr 
default_function_array_conversion_on_c_expr(struct c_expr t) {
  return default_function_array_conversion(t);
}

static inline tree default_function_array_conversion_on_tree(tree t) {
  struct c_expr e = {t, TREE_CODE(t)};
  return default_function_array_conversion(e).value;
}

#define default_function_array_conversion \
  default_function_array_conversion_on_tree

//
//
//

void c_parse_init (void)
{
  /* The only initialization required is of the reserved word
     identifiers.  */
  /* Used by c-decl.c, thus must be still be init. */
  unsigned int i;
  tree id;
  int mask = 0;

  mask |= D_CXXONLY;
  if (!flag_isoc99)
    mask |= D_C99;
  if (flag_no_asm)
    {
      mask |= D_ASM | D_EXT;
      if (!flag_isoc99)
	mask |= D_EXT89;
    }
  if (!c_dialect_objc ())
    mask |= D_OBJC | D_CXX_OBJC;

  ridpointers = GGC_CNEWVEC (tree, (int) RID_MAX);
  for (i = 0; i < num_c_common_reswords; i++)
    {
      /* If a keyword is disabled, do not enter it into the table
	 and so create a canonical spelling that isn't a keyword.  */
      if (c_common_reswords[i].disable & mask)
	{
	  if (warn_cxx_compat
	      && (c_common_reswords[i].disable & D_CXXWARN))
	    {
	      id = get_identifier (c_common_reswords[i].word);
	      C_SET_RID_CODE (id, RID_CXX_COMPAT_WARN);
	      C_IS_RESERVED_WORD (id) = 1;
	    }
	  continue;
	}

      id = get_identifier (c_common_reswords[i].word);
      C_SET_RID_CODE (id, c_common_reswords[i].rid);
      C_IS_RESERVED_WORD (id) = 1;
      ridpointers [(int) c_common_reswords[i].rid] = id;
    }

  // Also init chartype
  memset(chartype_, 1, 256);
  chartype('(') = CHAR_SPECIAL;
  chartype(')') = CHAR_SPECIAL;
  chartype('[') = CHAR_SPECIAL;
  chartype(')') = CHAR_SPECIAL;
  chartype('{') = CHAR_SPECIAL;
  chartype('}') = CHAR_SPECIAL;
  chartype(':') = CHAR_SPECIAL;
  chartype('"') = CHAR_SPECIAL;
  chartype('#') = CHAR_SPECIAL;
  chartype('\0') = CHAR_EOL;
  chartype('\n') = CHAR_EOL;
  chartype('\r') = CHAR_EOL;
  chartype(' ') = CHAR_SPACE;
  chartype('\t') = CHAR_SPACE;
  chartype('\f') = CHAR_SPACE;
  chartype('\v') = CHAR_SPACE;
  chartype('0') = CHAR_ID | CHAR_NUM;
  chartype('1') = CHAR_ID | CHAR_NUM;
  chartype('2') = CHAR_ID | CHAR_NUM;
  chartype('3') = CHAR_ID | CHAR_NUM;
  chartype('4') = CHAR_ID | CHAR_NUM;
  chartype('5') = CHAR_ID | CHAR_NUM;
  chartype('6') = CHAR_ID | CHAR_NUM;
  chartype('7') = CHAR_ID | CHAR_NUM;
  chartype('8') = CHAR_ID | CHAR_NUM;
  chartype('9') = CHAR_ID | CHAR_NUM;
  // force show_column
  flag_show_column = true;
}

//
//
//

#define BLOCK_SIZE (1024*16)

static void read_soure_file(void) {
  int fd = STDIN_FILENO;
  if (this_input_filename[0] != '\0')
    fd = open(this_input_filename, O_RDONLY);

  char * d = (char *)xmalloc(BLOCK_SIZE);
  unsigned capacity = BLOCK_SIZE;
  data_size = 0;
  ssize_t s;
  while (s = read(fd, d + data_size, BLOCK_SIZE), s) {
    data_size += s;
    if (data_size + BLOCK_SIZE > capacity) {
        capacity *= 2;
        d = (char *)xrealloc(d, capacity);
    }
  }
  d = (char *)xrealloc(d, data_size+1);
  d[data_size] = '\0';
  line_num = 1;
  data = d;
  str = d;
  line_start = d;
  linemap_add(line_table, LC_ENTER, false, this_input_filename, 1);
  input_location = linemap_line_start(line_table, 1, 127);
}

#undef BLOCK_SIZE

static jmp_buf parse_error;

__attribute__ ((noreturn, format (printf, 2, 3)))
static void throw_error (location_t loc, const char * msg, ...) 
{
  diagnostic_info diagnostic;
  va_list ap;

  va_start (ap, msg);
  diagnostic_set_info (&diagnostic, msg, &ap, loc, DK_ERROR);
  report_diagnostic (&diagnostic);
  va_end (ap);
  
  longjmp(parse_error, 1);
}

static location_t make_loc(const char * str) {
  location_t res;
  LINEMAP_POSITION_FOR_COLUMN(res, line_table, str - line_start + 1);
  return res;
}

//
// 
//

static void spacing(void) {
  char chr;
  for (;;) {
    while (chartype(*str) == CHAR_SPACE) ++str;
    chr = *str;
    if (chr == '\0') {
      break;
    } else if (chartype(chr) == CHAR_EOL) {
      ++line_num; 
      input_location = linemap_line_start(line_table, line_num, 127);
      if (chr == '\n') 
        ++str;
      else if (chr == '\r') {
        ++str; 
        if (*str == '\n') ++str;
      }
      line_start = str;
    } else if (chr == '#') {
      ++str; 
      while (chartype(*str) != CHAR_EOL) ++str;
    }
    else break;
  }
}

static void expect_debug(char what, const char * func, const char * file, unsigned line) {
  if (*str != what)
    throw_error (make_loc(str), "Expected '%c'. (in %s at %s:%u)", what, func, file, line);
  ++str;
  spacing();
}

#define expect(what) expect_debug(what, __FUNCTION__, __FILE__, __LINE__)

static token parse_quote(void) {
  token res;
  res.loc = make_loc(str);
  const char * start = str;
  ++str;
  res.str = str;
  while (*str != '"' && chartype(*str) != CHAR_EOL) {
    if (*str == '\\') {
      ++str;
      if (chartype(*str) == CHAR_EOL)
        throw_error(make_loc(str), "Unexpected end of string.");
    }
    ++str;
  }
  res.len = str - res.str;
  if (*str != '"')
    throw_error(make_loc(start), "Unterminated '\"'.");
  ++str;
  spacing();
  return res;
}

static token try_token(void) {
  token res;
  if (*str == '"')
    return parse_quote();
  res.loc = make_loc(str);
  res.str = str;
  while (chartype(*str) & CHAR_ID)
    ++str;
  res.len = str - res.str;
  if (res.len == 0)
    return NULL_TOKEN;
  spacing();
  return res;
}

static token parse_token(void) {
  token res = try_token();
  if (res.len == 0)
    throw_error(make_loc(str), "Expected token.");
  return res;
}

static token parse_id(void) {
  if (chartype(*str) == CHAR_ID) {
    return parse_token();
  } else if (*str == '(') {
    expect('(');
    token what = parse_token();
    if (!token_eq(what, "id"))
      throw_error(what.loc, "Expected \"id\".");
    token res = parse_token();
    expect(')');
    return res;
  } else {
    throw_error(make_loc(str), "Expected identifier.");
  }
}

static token try_id(void) {
  if (chartype(*str) == CHAR_ID) {
    return try_token();
  } else if (*str == '(') {
    const char * orig_pos = str;
    expect('(');
    token what = parse_token();
    if (!token_eq(what, "id"))
      goto fail;
    token res = try_token();
    if (!res.str)
      goto fail;
    expect(')');
    return res;
  fail:
    str = orig_pos;
    return NULL_TOKEN;
  } else {
    return NULL_TOKEN;;
  }
}

static bool more_args(void) {
  return *str != ')' && *str != ':';
}

//
//
//

static tree parse_type(void);
static tree parse_exp(void);

// return 0 if no option found
//        1 if option has no args
//        2 if it has args that need to be parsed

static int get_opt(token * opt) {
  if (*str == ':') {
    ++str;
    if (*str == '(') {
      ++str;
      *opt = parse_token();
      if (*str == ')') {
        ++str;
        spacing();
        return 1;
      } else {
        return 2;
      }
    } else {
      *opt = parse_token();
      return 1;
    }
  } else {
    return 0;
  }
}

//
// Make an attirib (to be processed latter) and add it to the front
// of the list
//

static tree parse_attrib(token * opt, bool w_args, tree tail) {
  tree name = token_to_tree(*opt);
  tree args = NULL;
  if (w_args) {
    if (*str != '(') {
      tree arg = token_to_tree(parse_token());
      args = tree_cons(NULL, arg, args);
    }
    while (*str != ')') {
      tree arg = parse_exp();
      args = tree_cons(NULL, arg, args);
    }
    args = nreverse(args);
    expect(')');
  }
  return tree_cons(name, args, tail);
}

//
//
//

struct parms {
  tree type;
  size_t size;   // not including "...";
  struct parm * parms; // not including "...";
};
static struct parms parse_fun_parms(void);

static tree parse_array_type(void) {
  tree type = parse_type();
  tree size = build_binary_op(0, MINUS_EXPR, parse_exp(), integer_one_node, 1);
  return build_array_type(type, build_index_type(size));
}

static tree parse_fun_type(void) {
  struct parms parms = parse_fun_parms();
  free(parms.parms);
  parms.parms = NULL;

  tree ret_type = parse_type();

  return build_function_type(ret_type, parms.type);
}

static tree parse_tagged_type(enum tree_code tag) {
  token id = parse_token();
  //tree type = lookup_tag(tag, get_identifier_with_length(id.str, id.len), false);
  //if (!type)
  //  throw_error(id.loc, "Tagged type \"%.*s\" not found.", id.len, id.str);
  tree type = xref_tag(tag, get_identifier_with_length(id.str, id.len));
  return type;
}

static tree parse_type(void) {
  expect('(');
  token name = parse_token();
  tree type;
  if (token_eq(name, "int"))
    type = integer_type_node;
  else if (token_eq(name, "unsigned"))
    type = unsigned_type_node;
  else if (token_eq(name, "char"))
    type = char_type_node;
  else if (token_eq(name, "void"))
    type = void_type_node;
  else if (token_eq(name, "short"))
    type = short_integer_type_node;
  else if (token_eq(name, "long"))
    type = long_integer_type_node;
  else if (token_eq(name, "long-long"))
    type = long_long_integer_type_node;
  else if (token_eq(name, "signed-char"))
    type = signed_char_type_node;
  else if (token_eq(name, "unsigned-char"))
    type = unsigned_char_type_node;
  else if (token_eq(name, "unsigned-short"))
    type = short_unsigned_type_node;
  else if (token_eq(name, "unsigned-int"))
    type = unsigned_type_node;
  else if (token_eq(name, "unsigned-long"))
    type = long_unsigned_type_node;
  else if (token_eq(name, "unsigned-long-long"))
    type = long_long_unsigned_type_node;
  else if (token_eq(name, "float"))
    type = float_type_node;
  else if (token_eq(name, "double"))
    type = double_type_node;
  else if (token_eq(name, "long-double"))
    type = long_double_type_node;
  else if (token_eq(name, ".ptr")) 
    type = build_pointer_type(parse_type());
  else if (token_eq(name, ".array"))
    type = parse_array_type();
  else if (token_eq(name, ".fun"))
    type = parse_fun_type();
  else if (token_eq(name, "struct"))
    type = parse_tagged_type(RECORD_TYPE);
  else if (token_eq(name, "enum"))
    type = parse_tagged_type(ENUMERAL_TYPE);
  else if (token_eq(name, "union"))
    type = parse_tagged_type(UNION_TYPE);
  else {
    tree decl = lookup_name(token_to_tree(name));
    if (decl && TREE_CODE (decl) == TYPE_DECL)
      type = TREE_TYPE(decl);
    else
      throw_error(name.loc, "Unknown type: %.*s", name.len, name.str);
  }

  int quals = 0;
  while (*str == ':') {
    ++str;
    token q = parse_token();
    if (token_eq(q, "const"))         quals |= TYPE_QUAL_CONST;
    else if (token_eq(q, "volatile")) quals |= TYPE_QUAL_VOLATILE;
    else if (token_eq(q, "restrict")) quals |= TYPE_QUAL_RESTRICT;
    else throw_error(q.loc, "Unknown qualifier: %.*s", q.len, q.str);
  }
  if (quals != 0)
    type = c_build_qualified_type(type, quals);
  expect(')');
  return type;
}

static tree parse_id_exp(location_t loc) {
  token id = parse_token();
  return build_external_ref(get_identifier_with_length(id.str, id.len), 0, loc);
}

static tree parse_literal(bool bare) {
  token val = parse_token();
  char tmp[val.len + 1];
  memcpy(tmp, val.str, val.len);
  tmp[val.len] = '\0';
  char * endptr = NULL;
  long long ival = strtoll(tmp, &endptr, 0);
  if (endptr != tmp + val.len)
    throw_error(val.loc, "Expected integer.");
  tree type = integer_type_node;
  if (!bare && more_args())
    type = parse_type();
  return build_int_cst(type, ival);
}

static tree parse_float(void) {
  token val = parse_token();
  char tmp[val.len + 1];
  memcpy(tmp, val.str, val.len);
  tmp[val.len] = '\0';
  REAL_VALUE_TYPE fval;
  // fixme: real_from_string does not syntax check
  real_from_string(&fval, tmp);
  tree type = parse_type();
  return build_real(type, fval);
}

static char * unescape(const char * s, const char * end, char * res) {
  for (; s != end; ++s)
no_inc:
    if (*s == '\\') {
      ++s;
      assert(s != end);
      switch (*s) {
      case 'a': *res++ = '\a'; break;
      case 'b': *res++ = '\b'; break;
      case 'f': *res++ = '\f'; break;
      case 'n': *res++ = '\n'; break;
      case 'r': *res++ = '\r'; break;
      case 't': *res++ = '\t'; break;
      case 'v': *res++ = '\v'; break;
      case 'x': {
        // hex 
        ++s;
        char * e = CONST_CAST(char *, s);
        unsigned val = strtol(s, &e, 16);
        if (s == e) abort(); // FIXME: Error
        s = e;
        if (val > 255) abort(); // FIXME: Error message, out of range 
        *res++ = (char)val;
        goto no_inc;
      } case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': {
        // oct
        unsigned val = *s - '0'; ++s;
        if (s != end && '0' <= *s && *s <= '7') {val *= 8; val += *s - '0'; ++s;}
        if (s != end && '0' <= *s && *s <= '7') {val *= 8; val += *s - '0'; ++s;}
        if (val > 255) abort(); // FIXME: Error message, out of range
        *res++ = (char)val;
        goto no_inc;
      } default:
        *res++ = *s;
      }
    } else if (*s != '"') {
      *res++ = *s;
    }
  return res;
}

static tree parse_char(void) {
  token val = parse_token();
  char tmp[val.len + 1];
  char * end = unescape(val.str, val.str + val.len, tmp);
  size_t len = end - tmp;
  if (len > 1)
    throw_error(val.loc, "Expected token of length one.");
  return build_int_cst(char_type_node, *tmp);
}

static tree parse_string(void) {
  token val = try_token();
  tree res;
  if (val.len > 0) {
    char tmp[val.len + 1];
    char * end = unescape(val.str, val.str + val.len, tmp);
    *end = '\0';
    size_t len = end - tmp;
    res = build_string(len+1, tmp);
  } else {
    res = build_string(1, "");
  }
  TREE_TYPE(res) = char_array_type_node;
  res = fix_string_type(res);
  return res;
}

static tree parse_exp(void);
static struct c_expr parse_exp_init(tree type, bool nested);

static tree parse_exp_conv(void) {
  tree exp = parse_exp();
  return default_function_array_conversion (exp);
}

static tree parse_binop(location_t loc, enum tree_code code) {
  tree lhs = parse_exp_conv();
  tree rhs = parse_exp_conv();
  return build_binary_op(loc, code, lhs, rhs, 1);
}

static tree parse_assign(location_t loc, enum tree_code code) {
  tree lhs = parse_exp();
  tree rhs = parse_exp_conv();
  rhs = default_function_array_conversion(rhs);
  return build_modify_expr(loc, lhs, code, rhs);
}

static tree parse_addrof(location_t loc) {
  tree exp = parse_exp();
  return build_unary_op(loc, ADDR_EXPR, exp, 1);
}

static tree parse_deref(location_t loc) {
  tree exp = parse_exp_conv();
  return build_indirect_ref(loc, exp, "deref");
}

static tree parse_cast(location_t loc) {
  tree type = parse_type();
  tree exp = parse_exp_conv();
  tree cast = build_c_cast(type, exp);
  protected_set_expr_location(cast, loc);
  return cast;
}

static tree parse_unop(location_t loc, enum tree_code code) {
  tree exp = parse_exp_conv();
  return build_unary_op(loc, code, exp, 0);
}

static tree parse_list(void) {
  tree list = NULL;
  while (*str != ')') {
    tree exp = parse_exp_conv();
    list = tree_cons(NULL, exp, list);
  }
  return nreverse(list);
}

static tree parse_call(location_t loc) {
  tree fun;
  token id = try_id();
  if (token_eq(id, "__builtin_va_arg")) {
    expect('(');
    parse_token(); // expect "."
    tree v = parse_exp_conv();
    tree t = parse_type();
    expect(')');
    return build_va_arg(v,t);
  } else if (id.len > 0) {
    fun = build_external_ref(get_identifier_with_length(id.str, id.len), 0, loc);
  } else {
    fun = parse_exp();
  }

  expect('(');
  parse_token(); // expect "."
  tree parms = parse_list();
  expect(')');

  tree call = build_function_call(fun, parms);
  protected_set_expr_location(call, loc);
  //if (call == error_mark_node) longjmp(parse_error, 1);
  return call;
}

static tree parse_member(location_t loc) {
  tree datum = parse_exp_conv();
  token name = parse_id();
  tree id = get_identifier_with_length(name.str, name.len);
  tree exp = build_component_ref(datum, id);
  protected_set_expr_location(exp, loc);
  return exp;
}

static tree parse_eif(location_t loc) {
  tree cond = parse_exp_conv();
  cond = c_objc_common_truthvalue_conversion(EXPR_LOCATION(cond), cond);
  tree if_true = parse_exp_conv();
  tree if_false = parse_exp_conv();
  tree exp = build_conditional_expr (cond, if_true, if_false);
  protected_set_expr_location(exp, loc);
  return exp;
}

static tree parse_seq(location_t loc) {
  tree exp = parse_exp();
  if (*str == ')') return exp;
  tree res = parse_seq(loc);
  return build_compound_expr(exp, res);
}

static void parse_stmts(void);

static tree parse_eblock(location_t loc) {
  tree stmt = c_begin_stmt_expr ();
  parse_stmts();
  tree exp = c_finish_stmt_expr (stmt);
  SET_EXPR_LOCATION(exp, loc);
  return exp;
}
 
static tree try_exp_bare(location_t loc) {
  if (chartype(*str) == CHAR_ID) return parse_id_exp(loc);
  if (chartype(*str) & CHAR_NUM) return parse_literal(true);
  return NULL;
}

static enum tree_code to_binop_code(const token * what) {
  if (token_eq(*what, "plus"))   return PLUS_EXPR;
  if (token_eq(*what, "minus"))  return MINUS_EXPR;
  if (token_eq(*what, "times"))  return MULT_EXPR;
  if (token_eq(*what, "div"))    return TRUNC_DIV_EXPR;
  if (token_eq(*what, "lshift")) return LSHIFT_EXPR;
  if (token_eq(*what, "rshift")) return RSHIFT_EXPR;
  if (token_eq(*what, "mod"))    return TRUNC_MOD_EXPR;
  if (token_eq(*what, "bor"))    return BIT_IOR_EXPR;
  if (token_eq(*what, "xor"))    return BIT_XOR_EXPR;
  if (token_eq(*what, "band"))   return BIT_AND_EXPR;
  return NOP_EXPR;
}

static tree try_exp(location_t loc, const token * what) {

  if (token_eq(*what, "id")) return parse_id_exp(loc);
  if (token_eq(*what, "n"))  return parse_literal(false);
  if (token_eq(*what, "f"))  return parse_float();
  if (token_eq(*what, "c"))  return parse_char();
  if (token_eq(*what, "s"))  return parse_string();

  enum tree_code code = to_binop_code(what);
  if (code != NOP_EXPR) return parse_binop(loc, code);

  if (token_eq(*what, "assign"))
    return parse_assign(loc, NOP_EXPR);
  if (token_eq(*what, "c-assign")) {
    token op = parse_token();
    code = to_binop_code(&op);
    if (code == NOP_EXPR)
      throw_error(op.loc, "Expected binary operator after c-assign.");
    return parse_assign(loc, code);
  }
  
  if (token_eq(*what, "eq")) return parse_binop(loc, EQ_EXPR);
  if (token_eq(*what, "ne")) return parse_binop(loc, NE_EXPR);
  if (token_eq(*what, "lt")) return parse_binop(loc, LT_EXPR);
  if (token_eq(*what, "gt")) return parse_binop(loc, GT_EXPR);
  if (token_eq(*what, "le")) return parse_binop(loc, LE_EXPR);
  if (token_eq(*what, "ge")) return parse_binop(loc, GE_EXPR);
  if (token_eq(*what, "postinc"))  return parse_unop(loc, POSTINCREMENT_EXPR);
  if (token_eq(*what, "postdec"))  return parse_unop(loc, POSTDECREMENT_EXPR);
  if (token_eq(*what, "neg"))      return parse_unop(loc, NEGATE_EXPR);
  if (token_eq(*what, "not"))      return parse_unop(loc, TRUTH_NOT_EXPR);
  if (token_eq(*what, "bnot"))     return parse_unop(loc, BIT_NOT_EXPR);
  if (token_eq(*what, "addrof")) return parse_addrof(loc);
  if (token_eq(*what, "deref"))  return parse_deref(loc);
  if (token_eq(*what, "cast")) return parse_cast(loc);
  if (token_eq(*what, "call")) return parse_call(loc);
  if (token_eq(*what, "member")) return parse_member(loc);
  if (token_eq(*what, "eif")) return parse_eif(loc);
  if (token_eq(*what, "seq")) return parse_seq(loc);
  if (token_eq(*what, "eblock")) return parse_eblock(loc);
  return NULL;
}

static tree parse_exp(void) {
  location_t loc = make_loc(str);
  tree res = try_exp_bare(loc);
  if (res) 
    return res;
  expect('(');
  token what = parse_token();
  res = try_exp(loc, &what);
  if (!res)
    throw_error(loc, "Expected expression.");
  expect(')');
  return res;
}

static struct c_expr parse_init(location_t loc, tree type, bool nested) {
  if (nested)
    push_init_level(0);
  else
    really_start_incremental_init (type);
  while (*str != ')') {
    struct c_expr init = parse_exp_init(NULL, true);
    process_init_element (init, false);
  }
  return pop_init_level (0);
}

static struct c_expr parse_exp_init(tree type, bool nested) {
  location_t loc = make_loc(str);
  struct c_expr init;
  init.value = try_exp_bare(loc);
  if (init.value) {
    init.original_code = TREE_CODE(init.value);
    return init;
  }
  expect('(');
  token what = parse_token();
  if (token_eq(what, ".")) {
    init = parse_init(loc, type, nested);
  } else {
    init.value = try_exp(loc, &what);
    if (init.value)
      init.original_code = TREE_CODE(init.value);
    if (init.value
        && TREE_CODE (init.value) != STRING_CST 
        && TREE_CODE (init.value) != COMPOUND_LITERAL_EXPR)
      init = default_function_array_conversion_on_c_expr(init);
  }
  if (!init.value)
    throw_error(loc, "Expected initializer expression.");
  expect(')');
  return init;
}

// parse the fun parameters

struct parm {
  location_t loc;
  tree type;
  token name;
};

static struct parms parse_fun_parms(void) {
  struct parms r = {NULL, 0, NULL};
  expect('(');
  parse_token(); // should be ".t"
  size_t alloc_size = 8;
  r.parms = xmalloc(sizeof(struct parm) * alloc_size);
  unsigned i = 0;
  bool w_dots = false;
  for (;;) {
    if (i >= alloc_size) {
      alloc_size = alloc_size * 3 / 2;
      r.parms = xrealloc(r.parms, sizeof(struct parm) * alloc_size);
    }
    if (*str == '(') {
      r.parms[i].loc = make_loc(str);
      expect('(');
      tree type = parse_type();
      r.type = tree_cons (NULL, type, r.type);
      r.parms[i].type = type;
      if (*str != ')') {
        r.parms[i].name = parse_token();
      } else {
        r.parms[i].name = NULL_TOKEN;
      }
      expect(')');
    } else if (*str != ')') {
      parse_token(); // expect "...";
      w_dots = true;
      break;
    } else {
      break;
    }
    ++i;
  }
  r.size = i;
  if (!w_dots) 
    r.type = tree_cons (NULL, void_type_node, r.type);
  r.type = nreverse(r.type);
  expect(')');
  return r;
}

static tree parse_var_init(tree decl, bool top_level) {
  start_init (decl, NULL, top_level);
  tree init = parse_exp_init(NULL, false).value;
  if (init) init = default_function_array_conversion(init);
  finish_init();
  return init;
}

static void parse_top_level_var(location_t loc) {
  token name = parse_token();
  tree id = get_identifier_with_length(name.str, name.len);
  tree type = parse_type();
  tree decl = build_decl(VAR_DECL, id, type);
  DECL_SOURCE_LOCATION(decl) = loc;

  int r;
  token opt;
  tree attrs = NULL;
  bool have_static = false, have_extern = false;
  while ((r = get_opt(&opt))) {
    if (token_eq(opt, "static")) {
      have_static = true;
    } else if (token_eq(opt, "extern")) {
      have_extern = true;
    } else if (token_prefix(opt, "__")) {
      attrs = parse_attrib(&opt, r > 1, attrs);
    } else {
      throw_error(opt.loc, "Invalid flag for top-level \"var\" declaration: %.*s", opt.len, opt.str);
    }
  }
  bool need_init = more_args();
  bool extern_ref = !need_init && have_extern;
  
  TREE_PUBLIC(decl) = !have_static;
  TREE_STATIC(decl) = !extern_ref;
  DECL_EXTERNAL(decl) = have_extern;

  decl_attributes(&decl, attrs, 0);

  tree init = NULL;

  decl = prep_decl(decl, need_init);

  if (need_init)
    init = parse_var_init(decl, true);

  finish_decl(decl, init, NULL);
}

static void parse_struct_union(location_t loc, enum tree_code what) {
  token name = parse_token();
  tree id = get_identifier_with_length(name.str, name.len);
  if (*str == ')') { // just a forward-reference
    xref_tag(what, id);
    return;
  }
  tree decl = start_struct(what, id);
  tree fields = NULL;
  while (*str == '(') {
    expect('(');
    tree ftype = parse_type();
    token fname = try_token();
    tree fid = fname.str ? get_identifier_with_length(fname.str, fname.len) : NULL;
    tree bitsize = more_args() ? parse_exp() : NULL;
    // FIXME: handle bitfields, see grokfield and grokdeclarator (width arg)
    tree fdecl = build_decl(FIELD_DECL, fid, ftype);
    TREE_CHAIN(fdecl) = fields;
    fields = fdecl;
    expect(')');
  }
  fields = nreverse(fields);
  finish_struct(decl, fields, NULL);
}

static void parse_enum(location_t loc) {
  token name = parse_token();
  tree id = get_identifier_with_length(name.str, name.len);
  if (*str == ')') { // just a forward-reference
    xref_tag(ENUMERAL_TYPE, id);
    return;
  }
  tree values = NULL;
  struct c_enum_contents the_enum;
  tree type = start_enum (&the_enum, id);

  while (*str == '(') {
    location_t value_loc = make_loc(str);
    token n;
    tree enum_id;
    tree enum_value = NULL;
    tree enum_decl;

    expect('(');
    n = parse_token();
    enum_id = get_identifier_with_length(n.str, n.len);
    if (more_args())
      enum_value = parse_exp();
    expect(')');

    enum_decl = build_enumerator (&the_enum, enum_id, enum_value, 
                                  value_loc);
    TREE_CHAIN (enum_decl) = values;
    values = enum_decl;
  }

  values = nreverse (values);
  finish_enum (type, values, NULL);
}

static void parse_var(location_t loc) {
  token name = parse_token();
  tree id = get_identifier_with_length(name.str, name.len);
  tree type = parse_type();
  tree decl = build_decl(VAR_DECL, id, type);
  DECL_SOURCE_LOCATION(decl) = loc;

  int r;
  token opt;
  tree attrs = NULL;
  while ((r = get_opt(&opt))) {
    if (token_eq(opt, "register")) {
      DECL_REGISTER(decl) = true;
    } else if (token_prefix(opt, "__")) {
      attrs = parse_attrib(&opt, r > 1, attrs);
    } else {
      throw_error(opt.loc, "Invalid flag for \"var\" declaration: %.*s", opt.len, opt.str);
    }
  }
  decl_attributes(&decl, attrs, 0);

  bool need_init = more_args();
  tree init = NULL;

  decl = prep_decl(decl, need_init);

  if (need_init)
    init = parse_var_init(decl, false);

  finish_decl(decl, init, NULL);
}

static void parse_local_label(location_t loc) {
  token name = parse_token();
  tree id = token_to_tree(name);
  tree label = declare_label(id);
  C_DECLARED_LABEL_FLAG (label) = 1;
  add_stmt (build_stmt (DECL_EXPR, label));
}

static void parse_block(location_t loc);
//static void parse_slist(location_t loc);
static void parse_stmt(void);

// parse a statement which needs a block around it, for if, switch, etc.
static tree parse_block_stmt(void) {
  tree stmt = c_begin_compound_stmt (false);
  parse_stmt();
  return c_end_compound_stmt (stmt, false);
}

static void parse_if(location_t loc) {
  tree block = c_begin_compound_stmt (false);
  tree cond = parse_exp_conv();
  cond = c_objc_common_truthvalue_conversion(EXPR_LOCATION(cond), cond);
  tree if_true = parse_block_stmt();
  tree if_false = NULL;
  if (more_args()) 
    if_false = parse_block_stmt();
  c_finish_if_stmt (loc, cond, if_true, if_false, false);
  tree if_ = add_stmt(c_end_compound_stmt (block, false));
  SET_EXPR_LOCATION(if_, loc);
}

static void parse_switch(location_t loc) {
  tree block = c_begin_compound_stmt (false);
  tree exp = parse_exp_conv();
  c_start_case(exp);
  tree body = parse_block_stmt();
  c_finish_case(body);
  tree switch_ = add_stmt(c_end_compound_stmt (block, false));
  SET_EXPR_LOCATION(switch_, loc);
}

static void parse_goto(location_t loc) {
  token id = parse_id();
  tree goto_ = c_finish_goto_label(get_identifier_with_length(id.str, id.len));
  SET_EXPR_LOCATION(goto_, loc);
}

static void parse_return(location_t loc) {
  tree return_;
  if (*str != ')') {
    tree exp = parse_exp_conv();
    return_ = c_finish_return(exp);
  } else {
    return_ = c_finish_return(NULL_TREE);
  }
  SET_EXPR_LOCATION(return_, loc);
}

static void parse_label(location_t loc) {
  token id = parse_token();
  tree tlab = define_label(id.loc, get_identifier_with_length(id.str, id.len));
  tree label = add_stmt(build_stmt (LABEL_EXPR, tlab));
  SET_EXPR_LOCATION(label, loc);
}

static void parse_case(location_t loc) {
  tree exp = NULL;
  if (more_args())
    exp = parse_exp();
  tree label = do_case (exp, NULL);
  SET_EXPR_LOCATION(label, loc);
}

static bool try_decl(location_t loc, token * what) {
  if (token_eq(*what, "var"))
    parse_var(loc);
  else if (token_eq(*what, "local_label"))
    parse_local_label(loc);
  else
    return false;
  return true;
}

static void add_exp_stmt(location_t loc, tree exp) {
  exp = default_function_array_conversion(exp);
  exp = c_process_expr_stmt (exp);
  exp = add_stmt(exp);
  SET_EXPR_LOCATION(exp, loc);
}

static bool try_stmt(location_t loc, token * what) {
  tree exp;
  if (token_eq(*what, "if"))
    parse_if(loc);
  else if (token_eq(*what, ".switch"))
    parse_switch(loc);
  else if (token_eq(*what, "goto"))
    parse_goto(loc);
  else if (token_eq(*what, "return"))
    parse_return(loc);
  else if (token_eq(*what, "block"))
    parse_block(loc);
  //else if (token_eq(*what, "slist"))
  //  parse_slist(loc);
  else if (token_eq(*what, "label"))
    parse_label(loc);
  else if (token_eq(*what, "case"))
    parse_case(loc);
  else if (token_eq(*what, "noop"))
    add_stmt(build_empty_stmt());
  else if ((exp = try_exp(loc, what)))
    add_exp_stmt(loc, exp);
  else 
    return false;
  return true;
}

static void parse_stmt(void) {
  location_t loc = make_loc(str);
  tree exp = try_exp_bare(loc);
  if (exp) {
    add_exp_stmt(loc, exp);
    return;
  }
  expect('(');
  token what = parse_token();
  bool res = try_stmt(loc, &what);
  if (!res)
    throw_error(loc, "Expected statement.");
  expect(')');
}

static void parse_stmts(void) {
  while (*str != ')') {
    location_t loc = make_loc(str);
    tree exp = try_exp_bare(loc);
    if (exp) {
      add_exp_stmt(loc, exp);
      continue;
    }
    expect('(');
    token what = parse_token();
    if (token_eq(what, "cleanup")) {
      // first parse cleanup part
      tree cleanup = parse_block_stmt();
      expect(')');
      // now parse rest of block, note that we are _not_ nested inside
      // another stmt, hence we need to break out when we are done
      tree rest = c_begin_compound_stmt (false);
      parse_stmts();
      rest = c_end_compound_stmt (rest, false);
      tree stmt = build_stmt(TRY_FINALLY_EXPR, rest, cleanup);
      SET_EXPR_LOCATION(stmt, loc);
      add_stmt(stmt);
      return;
    }
    bool res;
    res = try_decl(loc, &what);
    if (!res)
      res = try_stmt(loc, &what);
    if (!res)
      throw_error(loc, "Expected declaration or statement.");
    expect(')');
  }
}

static void parse_block(location_t loc) {
  tree stmt = c_begin_compound_stmt (true);
  parse_stmts();
  add_stmt(build_empty_stmt()); 
  // ^^FIXME: Hack, apparently gcc doesn't like empty blocks, but the
  //     C front end seams to produce them just fine, need to figure out
  //     what I am doing differently
  tree block = add_stmt(c_end_compound_stmt (stmt, true));
  SET_EXPR_LOCATION(block, loc);
}

//static void parse_slist(location_t loc) {
//  tree stmt = c_begin_compound_stmt (false);
//  parse_stmts();
//  tree slist = add_stmt(c_end_compound_stmt (stmt, false));
//  SET_EXPR_LOCATION(slist, loc);
//}

static void parse_fun(location_t loc) {

  token name = parse_token();
  tree id = get_identifier_with_length(name.str, name.len);
  struct parms parms = parse_fun_parms();
  tree ret_type = parse_type();

  tree fun_type = build_function_type(ret_type, parms.type);
  
  tree fn_decl =  build_decl (FUNCTION_DECL, id, fun_type);
  TREE_PUBLIC (fn_decl) = true;
  //DECL_EXTERNAL (fn_decl) = true;
  //DECL_ARTIFICIAL (fn_decl) = false;
  //DECL_CONTEXT (fn_decl) = NULL;
  DECL_SOURCE_LOCATION (fn_decl) = loc;
  //TREE_STATIC (fn_decl) = true;

  // now handle flags, if any

  int r;
  token opt;
  tree attrs = NULL;
  while ((r = get_opt(&opt))) {
    if (token_eq(opt, "static")) {
      TREE_PUBLIC(fn_decl) = 0;
    } else if (token_eq(opt, "extern")) {
      DECL_EXTERNAL(fn_decl) = 1;
    } else if (token_prefix(opt, "__")) {
      attrs = parse_attrib(&opt, r > 1, attrs);
    } else {
      throw_error(opt.loc, "Invalid flag for \"fun\" declaration: %.*s", opt.len, opt.str);
    }
  }
  decl_attributes(&fn_decl, attrs, 0);

  if (more_args()) {
    prep_function(fn_decl);

    struct c_arg_info arg_info = {NULL, NULL, parms.type, NULL, NULL, 0};
    
    /* Build the decl. list */
    unsigned i;
    tree parm_list = NULL;
    for (i = 0; i != parms.size; ++i) {
      struct parm * p = &parms.parms[i];
      tree type = p->type; 
      tree id = get_identifier_with_length(p->name.str, p->name.len);
      if (TREE_CODE(type) == ARRAY_TYPE)
        type = build_pointer_type (TREE_TYPE(type));
      else if (TREE_CODE (type) == FUNCTION_TYPE)
        type = build_pointer_type (TREE_TYPE(type));
      tree parm_decl = build_decl(PARM_DECL, id, type);
      // FIXME: Is this always correct
      DECL_ARG_TYPE (parm_decl) = TREE_TYPE (parm_decl);
      DECL_SOURCE_LOCATION (parm_decl) = p->loc;
      TREE_CHAIN(parm_decl) = parm_list;
      parm_list = parm_decl;
    }
    parm_list = nreverse (parm_list);
    arg_info.parms = parm_list;
    
    store_parm_decls_from(&arg_info);
    
    location_t loc = make_loc(str);
    expect('(');
    parse_token(); // expect "block"
    parse_block(loc);
    expect(')');

    finish_function();

  } else {
    
    fn_decl = prep_decl(fn_decl, false);
    finish_decl(fn_decl, NULL, NULL);

  }

  free(parms.parms);
  parms.parms = NULL;

}

static void parse_talias(location_t loc) {
  token name = parse_token();
  tree id = get_identifier_with_length(name.str, name.len);
  tree type = parse_type();
  tree decl  = build_decl (TYPE_DECL, id, type);
  DECL_SOURCE_LOCATION(decl) = loc;
  decl = prep_decl(decl, false);
  finish_decl(decl, NULL, NULL);
}

static void parse_top_level(void) {
  location_t loc = make_loc(str);
  expect('(');
  token what = parse_token();
  if (token_eq(what, "var"))
    parse_top_level_var(loc);
  else if (token_eq(what, "fun"))
    parse_fun(loc);
  else if (token_eq(what, "talias"))
    parse_talias(loc);
  else if (token_eq(what, ".struct"))
    parse_struct_union(loc, RECORD_TYPE);
  else if (token_eq(what, ".union"))
    parse_struct_union(loc, UNION_TYPE);
  else if (token_eq(what, ".enum"))
    parse_enum(loc);
  else
    throw_error(loc, "Expected top-level form.");
  expect(')');
}

static void parse_translation_unit(void) {
  spacing();
  while (*str) {
    parse_top_level();
  }
}

void c_parse_file (void) {
  read_soure_file();
  if (!setjmp(parse_error)) {
    parse_translation_unit();
  } else /* error */ {
    // nothing to do
  }
}

#if 0

static tree try_stmt(const token * what) {
  
}

/*

  COMPOUND_EXPR "," expr
  COMPOUND_LITERAL_EXPR
  
  build_empty_stmt


  ?CLEANUP_POINT_EXPR -- full expressions
 */

#endif

