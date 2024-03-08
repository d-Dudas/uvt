import swap
import numpy as np

def gaussSemipivot(a, b):
  n = len(a)
  for i in range(n):
    pivot_line = np.argmax(abs(a[i:, i])) + i
    if(i != pivot_line):
      swap.swapRows(a, i, pivot_line)
      swap.swapRows(b, i, pivot_line)
    
    for j in range(i+1,n):
      if(a[i][i] != 0):
          m = -a[j,i]/a[i,i]
          a[j,i:] = a[j,i:] + a[i, i:] * m
          b[j] = b[j] + b[i] * m

  b[n-1] = b[n-1]/a[n-1,n-1]
  for k in range(n-2,-1,-1):
      b[k] = (b[k] - np.dot(a[k,k+1:n],b[k+1:n]))/a[k,k]

  return b

