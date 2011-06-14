#include <stdio.h>
#include "algbase.h"

datatype RBTree<T> = Node(T,RBType,RBTree<T>,RBTree<T>) | Empty
and      RBType    = Red | Black;

void sub(RBTree<int> tr)
{
	RBTree<int> tr2;
	tr2 = tr;
}

void inorder(RBTree<int> tr)
{
	match(tr) {
	case Node(a,b,c,d):
		inorder(c);
		printf("%d ",a);
		inorder(d);
	case Empty:
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

datatype List<T> = Cons(T,List<T>) | Nil;

template<class T> List<T> haar(List<T> a,List<T> b=Nil<T>,List<T> c=Nil<T>)
{
	match(a,b,c) {
	case Cons(s,Nil),Nil,d:
		return Cons(s,d);
	case Nil,s,d:
		return haar(s,Nil<int>,d);
	case Cons(h1,Cons(h2,t)),s,d:
		return haar(t,Cons(h1+h2,s),Cons(h1-h2,d));
	}
}

template<class T> void printlist(List<T> l)
{
	match(l) {
	case Cons(h,t):
		printf("%d ",h);
		printlist(t);
	case Nil:
	}
}

int main(int argc,char* argv[])
{
	RBTree<int> tr = Node(1,Red,Node(2,Black,Empty<int>,Empty<int>),Node(3,Red,Empty<int>,Empty<int>));
	sub(Node(9,Black,tr,tr));

/*	match(tr) {
	case Node(a,b,Node(e,f,g,h),Node(i,j,k,l)):
		printf("%d/%d/%d\n",a,e,i);
	} */
	inorder(tr);
	printf("\n");

	List<int> l = haar( Cons(1,Cons(2,Cons(3,Cons(4,Cons(-4,Cons(-3,Cons(-2,Cons(-1,Nil<int>)))))))));
	printlist(l);
	printf("\n");

	return 0;
}
