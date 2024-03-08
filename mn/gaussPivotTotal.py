import numpy as np
import swap

# Funcția mută pivotul pe linia potrivită
def moveRowPivot(a, b, i, pivot_row):
    swap.swapRows(a, i, pivot_row)
    swap.swapRows(b, i, pivot_row)

# Funcția mută pivotul pe coloana potrivită
def moveColumnPivot(a, i, pivot_column):
    swap.swapCols(a, i, pivot_column)

# Funcția returnează poziția pivotului total
def getTotalPivotPosition(a):
    line, row = np.unravel_index(np.argmax(abs(a)), a.shape)

    return line, row

# Funcția face zero-uri sub pivot
def makeZeroesUnderPivot(a, b, i, n):
    for j in range(i+1,n):
        if(a[i][i] != 0):
            m = -a[j,i]/a[i,i]
            a[j,i:] = a[j,i:] + a[i, i:] * m
            b[j] = b[j] + b[i] * m

# Funcția face subsituția și se calculează soluția ecuației
def backSubstitution(a, b, n):
    b[n-1] = b[n-1]/a[n-1,n-1]
    for k in range(n-2, -1, -1):
        b[k] = (b[k] - np.dot(a[k, k+1:n], b[k+1:n]))/a[k,k]

def gaussPivotTotal(a, b):
    n = len(a)
    for i in range(n):
        # Se găsește poziția pivotului total in submatricea
        # de sub și de dincolo de poziția i
        line, row = getTotalPivotPosition(a[i:, i:])

        # Deoarece funția getTotalPivotPosition returnează
        # poziția relativă submatricii, incrementăm pozițiile
        # returnate cu i să obținem pozițiile relative matricii
        # originale
        line += i
        row += i

        # Dacă e necesar, se mută pivotul pe linia potrivită
        if(line != i):
            moveRowPivot(a, b, i, line)

        # Dacă a necesar, se mută pivotul pe coloana potrivită
        if(row != i):
            moveColumnPivot(a, i, row)

        # Se fac zero-uri sub pivot
        makeZeroesUnderPivot(a, b, i, n)
    
    # Se calculează soluția cu substituția inversă
    backSubstitution(a, b, n)
        
    return b

