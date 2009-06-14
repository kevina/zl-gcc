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
struct _Data$$3;
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
  int y;
};
struct _Data$$2 {
  struct _Data$$1 parent;
  int z;
};
struct _Data$$3 {
  struct _Data$$2 parent;
  int z2;
};
/* function decls */
int printf(char const *, ...);
int snprintf(char const * str, size_t size, char const * format, ...);
int strcmp(char const * s1, char const * s2);
struct Syntax * parse_class(struct Syntax * p, Environ * env);
void foo_p(struct _Data$$1 * p);
void foo_c(struct _Data$$2 * p);
void foo_s(struct _Data$$3 * p);
int main();
/* definitions */
void foo_p(struct _Data$$1 * p)
{
  ;
}
void foo_c(struct _Data$$2 * p)
{
  ;
}
void foo_s(struct _Data$$3 * p)
{
  ;
}
int main()
{
  struct _Data$$1 p$1;
  foo_p((&p$1));
  struct _Data$$2 c$1;
  foo_c((&c$1));
  foo_p((&c$1.parent));
  struct _Data$$3 s$1;
  foo_s((&s$1));
  foo_c((&s$1.parent));
  foo_p((&s$1.parent.parent));
  p$1.x;
  p$1.y;
  c$1.z;
  (*((struct _Data$$1 *)(&c$1))).x;
  (*((struct _Data$$1 *)(&c$1))).y;
  s$1.z2;
  (*((struct _Data$$2 *)(&s$1))).z;
  (*((struct _Data$$1 *)(&s$1))).x;
  (*((struct _Data$$1 *)(&s$1))).y;
  return 0;
}
/* special */
/* done */
