#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./10cc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 "0;"
assert 42 "42;"
assert 2 "1+1;"
assert 6 "3*2;"
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5 ;"
assert 47 "5+6*7 ;"
assert 15 "5*(9-6) ;"
assert 4 "(3+5)/2;"
assert 60 "(-3*+4)*-5;"
assert 0 "0==1;"
assert 1 "1==1;"
assert 1 "1!=0;"
assert 0 "1!=1;"
assert 1 "1>0;"
assert 0 "1>1;"
assert 1 "1>=1;"
assert 0 "1>=2;"
assert 1 "1<2;"
assert 0 "1<1;"
assert 1 "1<=1;"
assert 0 "1<=0;"
assert 2 "+(4==2*2)+(-2*-2>7/2);"
assert 3 'a=3; a;'
assert 8 'a=3; z=5; a+z;'
assert 6 'a=b=3; a+b;'


echo OK