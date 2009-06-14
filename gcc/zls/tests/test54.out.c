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
struct D$$1;
struct _Data$$1;
struct _Data$$2;
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
struct D$$1 {
  int x;
};
struct _Data$$1 {
  int x;
};
struct _Data$$2 {
  struct D$$1 d;
  struct _Data$$1 c;
};
/* function decls */
int printf(char const *, ...);
int snprintf(char const * str, size_t size, char const * format, ...);
int strcmp(char const * s1, char const * s2);
struct Syntax * parse_class(struct Syntax * p, Environ * env);
struct D$$1 get_d$$1(struct _Data$$2 * this$1);
struct _Data$$1 get_c$$1(struct _Data$$2 * this$1);
struct _Data$$2 make_x$$1(struct _Data$$2 * this$1);
/* definitions */
struct D$$1 get_d$$1(struct _Data$$2 * this$1)
{
  return (*this$1).d;
}
struct _Data$$1 get_c$$1(struct _Data$$2 * this$1)
{
  return (*this$1).c;
}
struct _Data$$2 make_x$$1(struct _Data$$2 * this$1)
{
  struct _Data$$2 x$1;
  return x$1;
}
struct D$$1 d;
struct _Data$$1 c;
struct _Data$$2 x;
/* special */
/* done */
