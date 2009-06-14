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
  int * data;
  size_t sz;
};
/* function decls */
int printf(char const *, ...);
int snprintf(char const * str, size_t size, char const * format, ...);
int strcmp(char const * s1, char const * s2);
struct Syntax * parse_class(struct Syntax * p, Environ * env);
int * begin$$1(struct _Data$$1 * this$1);
int * end$$1(struct _Data$$1 * this$1);
int main();
/* definitions */
int * begin$$1(struct _Data$$1 * this$1)
{
  return (*this$1).data;
}
int * end$$1(struct _Data$$1 * this$1)
{
  return ((*this$1).data + (*this$1).sz);
}
int main()
{
  struct _Data$$1 x$1;
  int d$1[4] = {
    1,
    5,
    3,
    2,
  };
  (x$1.data = d$1);
  (x$1.sz = ((size_t)4));
  struct _Data$$1 * what$1 = (&x$1);
  int * i$1 = begin$$1((&(*what$1)));
  int * e$1 = end$$1((&(*what$1)));
  {
    __label__ break$1;
    __label__ continue$1;
    ;
  loop$$1:;
    if ((!(i$1 != e$1)))
      goto break$1;
    {
      int * el$1 = (&(*i$1));
      {
        printf("%d\n", (*el$1));
      }
    }
  continue$1:;
    ({
      (i$1 += 1);
    });
    goto loop$$1;
  break$1:;
  }
}
/* special */
/* done */
