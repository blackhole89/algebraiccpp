#ifndef _ALG_BASE_
#define _ALG_BASE_

template<class T> class _alg_ptr;
template<class T> class _minimal_ptr;

template<class T> class _alg_tracked {
	friend class _alg_ptr<T>;
	friend class _minimal_ptr<T>;
private:
	int _refc;
public:
	_alg_tracked() {
		_refc=0;
	}
};

template<class T> class _alg_ptr;

template<class T> class _minimal_ptr {
public:
	T *ptr;
	void _alg_ref() {
		ptr->_refc++;
	}
	void _alg_unref() {
		ptr->_refc--;
		if(!ptr->_refc) delete ptr;
	}
	_alg_ptr<T> evolve() {
		return (_alg_ptr<T>)*this;
	}
};

template<class T> _alg_ptr<T> _algebraize(T *p) {
	_alg_ptr<T> r(p);
	return r;
}

template<class T> class _alg_ptr : public _minimal_ptr<T> {
public:
	_alg_ptr() {
		this->ptr=NULL;
	}
	_alg_ptr(T *it) {
		this->ptr=it;
		this->_alg_ref();
	}
	~_alg_ptr() {
		this->_alg_unref();
	}
	_alg_ptr(_alg_ptr<T> const &other) {
		this->ptr=other.ptr;
		this->_alg_ref();
	}
	_alg_ptr(_minimal_ptr<T> const &other) {
		this->ptr=other.ptr;
		this->_alg_ref();
	}
	_alg_ptr &operator=(_alg_ptr<T> const &other) {
		if(this->ptr) {
			this->_alg_unref();
		}
		this->ptr=other.ptr;
		this->_alg_ref();
		return *this;
	}
	_alg_ptr &operator=(_minimal_ptr<T> const &other) {
		if(this->ptr) {
			this->_alg_unref();
		}
		this->ptr=other.ptr;
		this->_alg_ref();
		return *this;	
	}
	_alg_ptr &operator=(T* other) {
		this->ptr=other;
		this->_alg_ref();
	}
};

#endif
