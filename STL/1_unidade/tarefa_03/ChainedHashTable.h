
/*
 * ChainedHashTable.h
 *
 *  Created on: 2011-11-30
 *      Author: morin
 */

#ifndef CHAINEDHASHTABLE_H_
#define CHAINEDHASHTABLE_H_
#include "ArrayStack.h"
#include "array.h"
#include "utils.h"
#include <climits>
#include <cmath>

#define ll long long int

namespace ods {
template <class T> class ChainedHashTable {
protected:
  typedef ArrayStack<T> List;
  T null;
  array<List> t;
  int n;
  int d;
  int z;
  int m;
  static const int w = 32; // sizeof(int)*8;
  void resize();
  int hash(T x);

public:
  ChainedHashTable(int m);
  virtual ~ChainedHashTable();
  void setHashTable(int x) { d = x; };
  int getHash(T x) { return hash(x); }
  bool add(T x);
  T remove(T x);
  T find(T x);
  ArrayStack<T> check(int x) { return t[x]; };
  int size() { return n; }
  void clear();
};

/**
 * FIXME:  A copy-constructor for arrays would be useful here
 */

template <class T> int ChainedHashTable<T>::hash(T x) {
  ll sum = 0, p = 1000000007;

  for (int i = 0; i < (int)x.size(); i++)
    sum += (int)x[i] * pow(263, i);

  sum %= p;
  sum %= d;
  return sum;
}

template <class T> ChainedHashTable<T>::ChainedHashTable(int m) : t(m) {
  n = 0;
  d = m;
  null = "";
}

template <class T> ChainedHashTable<T>::~ChainedHashTable() {}

template <class T> bool ChainedHashTable<T>::add(T x) {
  if (find(x) != null)
    return false;
  t[hash(x)].add(x);
  n++;
  return true;
}

template <class T> T ChainedHashTable<T>::remove(T x) {
  int j = hash(x);
  for (int i = 0; i < t[j].size(); i++) {
    T y = t[j].get(i);
    if (x == y) {
      t[j].remove(i);
      n--;
      return y;
    }
  }
  return null;
}

template <class T> T ChainedHashTable<T>::find(T x) {
  int j = hash(x);
  for (int i = 0; i < t[j].size(); i++)
    if (x == t[j].get(i))
      return t[j].get(i);
  return null;
}

template <class T> void ChainedHashTable<T>::clear() {
  n = 0;
  d = 1;
  array<List> b(2);
  t = b;
}

} /* namespace ods */
#endif /* CHAINEDHASHTABLE_H_ */
