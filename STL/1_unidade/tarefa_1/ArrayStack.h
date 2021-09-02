/*
 * ArrayStack.h
 *
 *  Created on: 2011-11-23
 *      Author: morin
 */

#ifndef ARRAYSTACK_H_
#define ARRAYSTACK_H_
#include "array.h"
#include "utils.h"

namespace ods {

template <class T> class DualArrayDeque;

template <class T> class ArrayStack {
protected:
  friend class DualArrayDeque<T>;
  array<T> a;
  int n;
  virtual void resize();

public:
  ArrayStack();
  virtual ~ArrayStack();
  int size();
  virtual T top();
  virtual void push(T x);
  virtual T pop();
  virtual void clear();
};

template <class T> inline int ArrayStack<T>::size() { return n; }

template <class T> void ArrayStack<T>::clear() {
  n = 0;
  array<T> b(1);
  a = b;
}

template <class T> ArrayStack<T>::ArrayStack() : a(1) { n = 0; }

template <class T> ArrayStack<T>::~ArrayStack() {}

template <class T> void ArrayStack<T>::resize() {
  array<T> b(max(2 * n, 1));
  for (int i = 0; i < n; i++)
    b[i] = a[i];
  a = b;
}

template <class T> T ArrayStack<T>::top() { return a[size() - 1]; }

template <class T> void ArrayStack<T>::push(T x) {
  if (n + 1 > a.length)
    resize();
  a[size()] = x;
  n++;
}

template <class T> T ArrayStack<T>::pop() {
  n--;
  T x = a[size()];
  if (a.length >= 3 * n)
    resize();
  return x;
}

} /* namespace ods */

#endif /* ARRAYSTACK_H_ */
