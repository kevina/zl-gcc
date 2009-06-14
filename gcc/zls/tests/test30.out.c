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
struct S;
struct T$$1;
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
struct S {
  int mine;
  int priv;
  int priv$1;
};
struct T$$1 {
  int mine;
  int priv;
};
/* function decls */
int printf(char const *, ...);
int snprintf(char const * str, size_t size, char const * format, ...);
int strcmp(char const * s1, char const * s2);
struct Syntax * parse_class(struct Syntax * p, Environ * env);
void init_s(struct S * s$1);
void use_s(struct S s$1);
int main();
/* definitions */
void init_s(struct S * s$1)
{
  ((*s$1).mine = 0);
  ((*s$1).priv = 0);
  ((*s$1).priv$1 = 20);
}
void use_s(struct S s$1)
{
  ((s$1.mine + s$1.priv) + s$1.priv$1);
}
int main()
{
  struct S s$1;
  init_s((&s$1));
  use_s(s$1);
  (s$1.mine + s$1.priv$1);
  (s$1.mine + s$1.priv);
  struct T$$1 t$1;
  (t$1.mine + t$1.priv);
  (t$1.mine + t$1.priv);
}
/* special */
/* done */
