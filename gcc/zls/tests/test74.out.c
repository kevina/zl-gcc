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
double fabs(double x);
int main();
/* definitions */
double r = 0x1.9e3779beddcbdp+0;
int main()
{
  double q$1 = 0x1.8p+1;
  double r$1 = 0x1p+1;
  {
    __label__ break$1;
    __label__ continue$1;
    ;
  loop$$1:;
    if ((!1))
      goto break$1;
    {
      double a$1 = q$1;
      double b$1 = r$1;
      double a$2 = ((a$1 - (r * b$1)) / (((double)1) + r));
      if ((fabs((a$2 / (a$1 + b$1))) > 0x1.47ae147ae147bp-7))
        {
          (q$1 -= a$2);
          (r$1 += a$2);
        }
      else
        goto break$1;
    }
  continue$1:;
    ;
    goto loop$$1;
  break$1:;
  }
  printf("%f %f\n", q$1, r$1);
}
/* special */
/* done */
