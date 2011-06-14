#include <stdio.h>
#include "algbase.h"

datatype List<T> = Cons(T,List<T>) | Nil;

datatype Tree<T> = Node(T,Tree<T>,Tree<T>) | Leaf;

Tree<float> funtree(int,float)
	    funtree(0,*) {
	return Leaf<float>;
}         | funtree(n,f) {
	return Node(f,funtree(n-1,f/2.0),funtree(n-1,f+2.0));
}

template<class T>
List<T> append(List<T>,List<T>)
        append(Nil,l) {
	return l;
}     | append(Cons(h1,t1),l) {
	return Cons(h1,append(t1,l));
}

template<class T>
List<T> inorder(Tree<T>)
	inorder(Leaf) {
	return Nil<T>;
}     | inorder(Node(a,l,r)) {
	return append(inorder(l),Cons(a,inorder(r)));
}

template<class T>
void printlist(List<T>,char*)
     printlist(Cons(h,t),fmt) {
	printf(fmt,h);
	printlist(t,fmt);
}  | printlist(Nil,*) {
	printf("\n");
}

int main(int argc,char* argv[])
{
	printlist(inorder(funtree(4,1.0f)),"%.3f ");
	return 0;
}
