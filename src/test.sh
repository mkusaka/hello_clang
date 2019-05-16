#!/bin/bash

try() {
  expected="$1"
  input="$2"

  ./tim "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$expected expected, but got $actual"
  fi
}

try 0 0
try 42 42
try 21 '5+20-4'

echo OK
