import numpy as np

def gaussJordan(a, b):
    n = len(a)
    
    # Facem o matrice unitara
    inversa = np.zeros([n, n])
    for i in range(0, n): inversa[i, i] = 1

    # Eliminare Gauss-Jordan
    for i in range(0, n): # Iteram prin fiecare pivot
        for j in range(0, n): # Iteram prin fiecare linie
            if a[i, i] != 0 and i != j: # Sarim peste linia pivotului
                lam = a[j, i]/a[i, i]
                a[j, :] = a[j, :] - a[i, :]*lam

                # Facem modificarile aferente si matricei inverse si vectorului b
                inversa[j, :] = inversa[j, :] - inversa[i, :]*lam
                b[j] = b[j] - b[i]*lam
        
        # Facem calculele necesare ca pivotul sa fie 1
        lam = 1 / a[i, i]
        a[i, :] = a[i, :]*lam
        inversa[i, :] = inversa[i, :]*lam
        b[i] = b[i]*lam

    return a, b, inversa


a = np.array([[1, -1, 3], [2, 1, 2], [-2, -2, 1]], dtype=float)
b = np.array([6, 5, -1])

gaussJordan(a, b)

