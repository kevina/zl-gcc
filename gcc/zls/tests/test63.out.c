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
struct _Data$$4;
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
  struct _Data$$2 * _vptr;
  int x_;
};
struct _Data$$2 {
  void (* foo)(struct _Data$$1 * this);
};
struct _Data$$3 {
  struct _Data$$1 parent;
  int y_;
};
struct _Data$$4 {
  struct _Data$$2 parent;
  void (* bar)(struct _Data$$3 * this);
};
/* function decls */
int printf(char const *, ...);
int snprintf(char const * str, size_t size, char const * format, ...);
int strcmp(char const * s1, char const * s2);
struct Syntax * parse_class(struct Syntax * p, Environ * env);
void _constructor$$1(struct _Data$$1 * this$1);
void foo$$1(struct _Data$$1 * this$1);
void _vtable_init$$1(struct _Data$$2 * _vt$1);
void init$$1() __attribute__((constructor));
void _constructor$$2(struct _Data$$3 * this$1);
void bar$$1(struct _Data$$3 * this$1);
void _vtable_init$$2(struct _Data$$4 * _vt$1);
void init$$2() __attribute__((constructor));
int main();
/* definitions */
struct _Data$$2 _vtable$$1;
void _constructor$$1(struct _Data$$1 * this$1)
{
  ((*this$1)._vptr = (&_vtable$$1));
}
void foo$$1(struct _Data$$1 * this$1)
{
  printf("foo\n");
}
void _vtable_init$$1(struct _Data$$2 * _vt$1)
{
  ((*_vt$1).foo = ((void (*)(struct _Data$$1 * this))((void (*)(struct _Data$$1 * this))foo$$1)));
}
void init$$1()
{
  _vtable_init$$1((&_vtable$$1));
}
struct _Data$$4 _vtable$$2;
void _constructor$$2(struct _Data$$3 * this$1)
{
  ((*((struct _Data$$4 * *)(&(*((struct _Data$$1 *)this$1))._vptr))) = (&_vtable$$2));
}
void bar$$1(struct _Data$$3 * this$1)
{
  printf("bar\n");
}
void _vtable_init$$2(struct _Data$$4 * _vt$1)
{
  ((*_vt$1).bar = ((void (*)(struct _Data$$3 * this))((void (*)(struct _Data$$3 * this))bar$$1)));
}
void init$$2()
{
  _vtable_init$$1((&_vtable$$2.parent));
  _vtable_init$$2((&_vtable$$2));
}
int main()
{
  struct _Data$$1 xo$1;
  _constructor$$1((&xo$1));
  struct _Data$$1 * x$1 = (&xo$1);
  (*x$1).x_;
  (*(*x$1)._vptr).foo(((struct _Data$$1 *)(&(*x$1))));
  struct _Data$$3 yo$1;
  _constructor$$2((&yo$1));
  struct _Data$$3 * y$1 = (&yo$1);
  (*((struct _Data$$2 *)(*((struct _Data$$4 * *)(&(*((struct _Data$$1 *)(&(*y$1))))._vptr))))).foo(((struct _Data$$1 *)(&(*y$1))));
  (*(*((struct _Data$$4 * *)(&(*((struct _Data$$1 *)(&(*y$1))))._vptr)))).bar((&(*y$1)));
}
/* special */
/* done */
