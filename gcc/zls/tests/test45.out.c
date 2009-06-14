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
  int x;
};
/* function decls */
int printf(char const *, ...);
int snprintf(char const * str, size_t size, char const * format, ...);
int strcmp(char const * s1, char const * s2);
struct Syntax * parse_class(struct Syntax * p, Environ * env);
int f$$1(struct _Data$$1 * this$1);
int g$$1(struct _Data$$1 * this$1);
int h0$$1(struct _Data$$1 * this$1, int j);
int h1$$1(struct _Data$$1 * this$1, int j);
int y$$1(struct _Data$$1 * this$1);
int z$$1(struct _Data$$1 * this$1);
int main();
/* definitions */
int f$$1(struct _Data$$1 * this$1)
{
  return 20;
}
int g$$1(struct _Data$$1 * this$1)
{
  return f$$1(this$1);
}
int h0$$1(struct _Data$$1 * this$1, int j)
{
  return 20;
}
int h1$$1(struct _Data$$1 * this$1, int j)
{
  return ((*this$1).x + j);
}
int y$$1(struct _Data$$1 * this$1)
{
  return (*this$1).x;
}
int z$$1(struct _Data$$1 * this$1)
{
  return g$$1(this$1);
}
int main()
{
  struct _Data$$1 v$1;
  int x$1;
  (x$1 += v$1.x);
  (x$1 += f$$1((&v$1)));
  (x$1 += g$$1((&v$1)));
  (x$1 += h0$$1((&v$1), 20));
  (x$1 += h1$$1((&v$1), 20));
  (x$1 += y$$1((&v$1)));
  (x$1 += z$$1((&v$1)));
  return x$1;
}
/* special */
/* done */
