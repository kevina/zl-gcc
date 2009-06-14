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
struct _Data$$1 {
  int x_;
};
struct _Data$$2 {
  struct _Data$$1 parent;
};
/* function decls */
int printf(char const *, ...);
int snprintf(char const * str, size_t size, char const * format, ...);
int strcmp(char const * s1, char const * s2);
struct Syntax * parse_class(struct Syntax * p, Environ * env);
int foo$$1();
int bar$$1();
int main();
/* definitions */
static int x$$1;
int foo$$1()
{
  return x$$1;
}
static int y$$1;
int bar$$1()
{
  return y$$1;
}
int main()
{
  x$$1;
  x$$1;
  y$$1;
  foo$$1();
  foo$$1();
  bar$$1();
  struct _Data$$1 x$1;
  x$$1;
  foo$$1();
  struct _Data$$2 y$1;
  x$$1;
  y$$1;
  foo$$1();
  bar$$1();
}
/* special */
/* done */
