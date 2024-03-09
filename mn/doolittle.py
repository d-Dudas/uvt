import numpy as np

# Functia returneaza o matrice in care
# sunt retinute matricele L si U
# si face modificarile necesare si in b
def DoolittleLUdecomp(a, b):
    n = len(a)
    for k in range(0, n-1):
        for i in range(k+1, n):
            if a[i,k] != 0.0:
                lam = a[i,k]/a[k,k]
                a[i,k+1:n] = a[i, k+1:n] - lam*a[k,k+1:n]
                a[i,k] = lam

                # Modificare in plus pentru a rezolva
                # si vectorul b in acelasi timp
                b[i] = b[i] - lam*b[k]
    return a

# Functia calculeaza solutia sistemului
# folosind matricele formatate cu
# cu factorizarea LU
def DoolitleLUSolve(a,b):
    n = len(a)

    # Urmatoarele 2 linii de cod sunt redundante
    # dupa optimizare functiei DoolitleLUdecomp
    #for k in range(1, n):
    #   b[k] = b[k] - np.dot(a[k, 0:k], b[0:k])
    b[n-1] = b[n-1]/a[n-1, n-1]
    for k in range(n-2, -1, -1):
        b[k] = (b[k] - np.dot(a[k, k+1:n], b[k+1:n]))/a[k, k]

    return b

