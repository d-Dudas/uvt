import numpy as np

# Functia returneaza o matrice in care
# sunt retinure matricele L si U Crout
def CroutLUdecomp(a):
    n = len(a)
    for i in range(n):

        # Se calculeaza coloana l[j, i]
        for j in range(i, n):
            sum_l = sum(a[k, i] * a[j, k] for k in range(i))
            a[j, i] = a[j, i] - sum_l

        # Se calculeaza linia u[i, j]
        for j in range(i+1, n):
            sum_u = sum(a[i, k] * a[k, j] for k in range(i))
            a[i, j] = (a[i, j] - sum_u) / a[i][i]

    for i in range(n):
        for j in range(i+1):
            a[i, j], a[j, i] = a[j, i], a[i, j]

    return a

# Functia calculeaza solutia sistemului
# folosind LU Crout
def CroutLUSolve(a,b):
    n = len(a)
    for k in range(1, n):
       b[k] = b[k] - np.dot(a[k, 0:k], b[0:k])
    b[n-1] = b[n-1]/a[n-1, n-1]
    for k in range(n-2, -1, -1):
        b[k] = (b[k] - np.dot(a[k, k+1:n], b[k+1:n]))/a[k, k]

    return b

