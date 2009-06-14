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
/* function decls */
int printf(char const *, ...);
int snprintf(char const * str, size_t size, char const * format, ...);
int strcmp(char const * s1, char const * s2);
struct Syntax * parse_class(struct Syntax * p, Environ * env);
void f();
void g();
int main();
/* definitions */
void f()
{
  float x$1 = ((float)({
    double t$1 = 0x0p+0;
    (((int)t$1) ? t$1 : 0x1.b333333333333p+2);
  }));
  printf("%f\n", x$1);
}
void g()
{
  float x$1 = 0x1.8p+2f;
  printf("%f\n", x$1);
}
int main()
{
  f();
  g();
}
/* special */
/* done */
