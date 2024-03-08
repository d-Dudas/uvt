# Laborator 2
# Exercițiul 3
# Subpunctul b
# Să se determine polinomul de gradul 4 care trece
# prin punctele (0, -1), (1, 1), (3, 3), (5, 2) și (6, -2).

import numpy as np
from gaussSemipivot import gaussSemipivot

# Inițializăm un array cu punctele din cerință.
puncte = np.array([(0, -1), (1, 1), (3, 3), (5, 2), (6, -2)])

# Creăm un matrice folosind polinomul P(x)=x**4 + x**3 + x**2 + x + 1
a = np.array([[x**4, x**3, x**2, x, 1] for x, _ in puncte], dtype=float)

# Rezultatele pentru fiecare linie, P(x)=y
b = np.array([y for _, y in puncte], dtype=float)

# Aflăm coeficienții folosind metoda gaussiană cu semi-pivot
coeficienti = gaussSemipivot(a,b)

# Afișăm coeficienții
for i in range(len(coeficienti)):
    print("x{}: {}".format(i+1, coeficienti[i]))

print("Polinomul de gradul 4 care trece prin punctele date este:")
print("({}*x**4) + ({}*x**3) + ({}*x**2) + ({}*x) + ({})".format(coeficienti[0],coeficienti[1],coeficienti[2],coeficienti[3],coeficienti[4]))

