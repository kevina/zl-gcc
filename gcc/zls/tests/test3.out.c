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
void foo();
void foo2();
int main();
/* definitions */
int main()
{
  int x$1 = 0;
  {
    __label__ break$1;
    __label__ continue$1;
  continue$1:;
    if ((!((x$1 != 20) ? ((x$1 != 30) ? 1 : (x$1 != 40)) : 0)))
      goto break$1;
    (x$1 += 1);
    goto continue$1;
  break$1:;
  }
  {
    __label__ break$1;
    __label__ continue$1;
    int i$1 = 9;
    int j$1 = 11;
  loop$$1:;
    if ((!1))
      goto break$1;
    {
      (x$1 = (i$1 + x$1));
    }
  continue$1:;
    ;
    goto loop$$1;
  break$1:;
  }
  (x$1 |= 3);
}
/* special */
/* done */
