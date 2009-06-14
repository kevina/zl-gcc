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
struct S$$1;
typedef struct S$$1 S$$2;
struct S$$3;
typedef struct S$$3 S$$4;
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
struct S$$1 {
  int x;
};
struct S$$3 {
  int y;
};
/* function decls */
int printf(char const *, ...);
int snprintf(char const * str, size_t size, char const * format, ...);
int strcmp(char const * s1, char const * s2);
struct Syntax * parse_class(struct Syntax * p, Environ * env);
int main();
/* definitions */
S$$4 x$$1;
struct S$$3 y$$1;
S$$2 x0;
struct S$$1 s0;
S$$4 x1;
struct S$$3 s1;
int main()
{
  x0.x;
  s1.y;
  s0.x;
  s1.y;
  S$$4 x$1;
  struct S$$3 m$1;
  x$1.y;
  m$1.y;
}
/* special */
/* done */
