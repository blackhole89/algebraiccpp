#include <stdio.h>
#include "algbase.h"

template<class T> class RBTree;
class RBType;
typedef enum { Red=1, Black, _RBType_discr_max } _RBType_discr;
class RBType : public _alg_tracked<RBType> {
public:
_RBType_discr _discr;
union {
struct {
} sRed;
struct {
} sBlack;
};
~RBType() {
switch(_discr) {
case Red:
break;
case Black:
break;
}
}
};
RBType *ctor_Red() {
RBType*r=new RBType;
r->_discr=Red;
return r;
}
RBType *ctor_Black() {
RBType*r=new RBType;
r->_discr=Black;
return r;
}
typedef enum { Node=1, Empty, _RBTree_discr_max } _RBTree_discr;
template<class T> class RBTree : public _alg_tracked<RBTree<T> > {
public:
_RBTree_discr _discr;
union {
struct {
T p0;
_minimal_ptr<RBType > p1;
_minimal_ptr<RBTree<T> > p2;
_minimal_ptr<RBTree<T> > p3;
} sNode;
struct {
} sEmpty;
};
~RBTree() {
switch(_discr) {
case Node:
sNode.p1._alg_unref();
sNode.p2._alg_unref();
sNode.p3._alg_unref();
break;
case Empty:
break;
}
}
};
template<class T> RBTree<T>  *ctor_Node(T pp0,_alg_ptr<RBType > const &pp1,_alg_ptr<RBTree<T> > const &pp2,_alg_ptr<RBTree<T> > const &pp3) {
RBTree<T> *r=new RBTree<T> ;
r->_discr=Node;
r->sNode.p0=pp0;
r->sNode.p1=pp1;
r->sNode.p1._alg_ref();
r->sNode.p2=pp2;
r->sNode.p2._alg_ref();
r->sNode.p3=pp3;
r->sNode.p3._alg_ref();
return r;
}
template<class T> RBTree<T>  *ctor_Empty() {
RBTree<T> *r=new RBTree<T> ;
r->_discr=Empty;
return r;
}
void sub(_alg_ptr<RBTree<int> >tr)
{
	_alg_ptr<RBTree<int> >tr2;
	tr2 = tr;
}

void inorder(_alg_ptr<RBTree<int> >tr)
{
	auto _fv0=tr;
if(true&& ((_fv0.ptr->_discr)==(Node))) {
auto a=_fv0.ptr->sNode.p0;
auto b=_fv0.ptr->sNode.p1.evolve();
auto c=_fv0.ptr->sNode.p2.evolve();
auto d=_fv0.ptr->sNode.p3.evolve();
inorder(c);
		printf("%d ",a);
		inorder(d);
	}
 else if(true&& ((_fv0.ptr->_discr)==(Empty))) {
printf("e ");
	}
}

/*  - fun haar l = let
      fun aux [s] [] d = s :: d
        | aux [] s d = aux s [] d
        | aux (h1::h2::t) s d = aux t (h1+h2 :: s) (h1-h2 :: d)
        | aux _ _ _ = raise Empty
      in  
        aux l [] []
      end;*/

template<class T> class List;
typedef enum { Cons=1, Nil, _List_discr_max } _List_discr;
template<class T> class List : public _alg_tracked<List<T> > {
public:
_List_discr _discr;
union {
struct {
T p0;
_minimal_ptr<List<T> > p1;
} sCons;
struct {
} sNil;
};
~List() {
switch(_discr) {
case Cons:
printf("destroy Cons(%d)\n",sCons.p0); fflush(stdout);
sCons.p1._alg_unref();
break;
case Nil:
printf("destroy Nil\n"); fflush(stdout);
break;
}
}
};
template<class T> List<T>  *ctor_Cons(T pp0,_alg_ptr<List<T> > const &pp1) {
printf("create Cons(%d)\n",pp0);
List<T> *r=new List<T> ;
r->_discr=Cons;
r->sCons.p0=pp0;
r->sCons.p1=pp1;
r->sCons.p1._alg_ref();
return r;
}
template<class T> List<T>  *ctor_Nil() {
printf("create Nil\n");
List<T> *r=new List<T> ;
r->_discr=Nil;
return r;
}
template<class T> _alg_ptr<List<T> >haar(_alg_ptr<List<T> >a,_alg_ptr<List<T> >b,_alg_ptr<List<T> >c)
{
	auto _fv1=a;
auto _fv2=b;
auto _fv3=c;
if(true&& ((_fv1.ptr->_discr)==(Cons))&& ((_fv1.ptr->sCons.p1.ptr->_discr)==(Nil))&& ((_fv2.ptr->_discr)==(Nil))) {
auto d=_fv3;
auto s=_fv1.ptr->sCons.p0;
return _algebraize(ctor_Cons(s,d));
	}
 else if(true&& ((_fv1.ptr->_discr)==(Nil))) {
auto d=_fv3;
auto s=_fv2;
return haar(s,_algebraize(ctor_Nil<int>()),d);
	}
 else if(true&& ((_fv1.ptr->_discr)==(Cons))&& ((_fv1.ptr->sCons.p1.ptr->_discr)==(Cons))) {
auto d=_fv3;
auto h1=_fv1.ptr->sCons.p0;
auto h2=_fv1.ptr->sCons.p1.ptr->sCons.p0;
auto s=_fv2;
auto t=_fv1.ptr->sCons.p1.ptr->sCons.p1.evolve();
return haar(t,_algebraize(ctor_Cons(h1+h2,s)),_algebraize(ctor_Cons(h1-h2,d)));
	}
}

template<class T> void printlist(_alg_ptr<List<T> >l)
{
	auto _fv4=l;
if(true&& ((_fv4.ptr->_discr)==(Cons))) {
auto h=_fv4.ptr->sCons.p0;
auto t=_fv4.ptr->sCons.p1.evolve();
printf("%d ",h);
		printlist(t);
	}
 else if(true&& ((_fv4.ptr->_discr)==(Nil))) {
;
	}
}

int main(int argc,char* argv[])
{
	_alg_ptr<RBTree<int> >tr = _algebraize(ctor_Node(1,_algebraize(ctor_Red()),_algebraize(ctor_Node(2,_algebraize(ctor_Black()),_algebraize(ctor_Empty<int>()),_algebraize(ctor_Empty<int>()))),_algebraize(ctor_Node(3,_algebraize(ctor_Red()),_algebraize(ctor_Empty<int>()),_algebraize(ctor_Empty<int>())))));
	sub(_algebraize(ctor_Node(9,_algebraize(ctor_Black()),tr,tr)));

/*	match(tr) {
	case Node(a,b,Node(e,f,g,h),Node(i,j,k,l)):
		printf("%d/%d/%d\n",a,e,i);
	} */
	inorder(tr);
	printf("\n");

	_alg_ptr<List<int> >l = haar( _algebraize(ctor_Cons(1,_algebraize(ctor_Cons(2,_algebraize(ctor_Cons(3,_algebraize(ctor_Cons(4,_algebraize(ctor_Cons(-4,_algebraize(ctor_Cons(-3,_algebraize(ctor_Cons(-2,_algebraize(ctor_Cons(-1,_algebraize(ctor_Nil<int>()))))))))))))))))), _algebraize(ctor_Nil<int>()), _algebraize(ctor_Nil<int>()));
	printlist(l);
	printf("\n");

	return 0;
}
