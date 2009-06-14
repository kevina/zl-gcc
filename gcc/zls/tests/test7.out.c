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
int main();
/* definitions */
int main()
{
  int x$1 = 0;
  int z$1;
  int z$2 = 0;
  (z$2 = 20);
  int y$1 = z$2;
  (y$1 += 1);
  (y$1 = ((y$1 *= 2) + 4));
  {
    __label__ break$1;
    __label__ continue$1;
    ;
  loop$$1:;
    if ((!1))
      goto break$1;
    (x$1 + 1);
  continue$1:;
    ;
    goto loop$$1;
  break$1:;
  }
  if (x$1)
    {
      (x$1 *= 20);
      (y$1 *= 30);
    }
  else
    (y$1 *= 2);
  (z$1 = (5 + 5));
  (z$1 = ((2 + 3) * (2 + 3)));
  ((y$1 * y$1) + (y$1 * y$1));
}
/* special */
/* done */
