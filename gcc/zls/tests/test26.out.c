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
struct s1;
typedef struct s1 s1;
struct s2$$1;
typedef struct s2$$1 s2$$2;
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
struct s1 {
  int x;
  int y;
};
struct s2$$1 {
  int x;
  int y;
};
/* function decls */
int printf(char const *, ...);
int snprintf(char const * str, size_t size, char const * format, ...);
int strcmp(char const * s1, char const * s2);
struct Syntax * parse_class(struct Syntax * p, Environ * env);
int foo(int x, int y);
int main();
/* definitions */
int x = 20;
int foo(int x, int y)
{
  {
    int x$1 = 20;
    if (0)
      return (x$1 + y);
    else
      goto x$$1;
  }
  return (x + y);
  int x$1 = 2;
x$$1:;
  return 10;
}
int main()
{
  s1 v1$1;
  s2$$2 v2$1;
  return ((v1$1.x + v2$1.y) + foo(2, 3));
}
/* special */
/* done */
