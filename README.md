# ascpp

L'objectif de ce projet est de réaliser un analyseur statique
pour le [mini-langage WHILE](#mini_langage) en utilisant
l'[interprétation abstraite](#interpretation_abstraite).

L'analyseur calcule, pour chaque variable d'un programme,
un intervalle abstrait sur-approximant l'ensemble des valeurs
qu'elle peut prendre à la sortie du programme.

Par exemple, le programme ci-dessous :

```while
n = n * n + 4;
i = 1;
while 10 > i do
  if i > 5 then
    n = n + 1
  else
    n = n + 2;
  i = i + 1;
x = 10 / n
```

avec `n` initialement inconnu (`TOP = (-∞, +∞)`), produit :
```
n -> [4; +inf)
i -> [1; +inf)
```

Puisque `n ∈ [4, +∞)` au point de division, l'analyseur prouve
formellement que la division par `n` est toujours sûre,
pour toutes les valeurs possibles de `n`.

## Analyse statique
:warning: TODO

## Interprétation abstraite
:warning: TODO

## Mini-langage
Le mini-langage s'inspire du langage While.
Ci-dessous la grammaire du langage utilisé ici.

```
expression arithmétique
a ::= n
    | x
    | a + a
    | -a
expression booléenne
b ::= true
    | false
    | a > a
    | a = a
statement
s ::= skip
    | x = a
    | s ; s
    | if b then s else s
    | while b do s
```

## Sources
- [Keean Schupke. C++ Parser Combinator Library](http://lambda-the-ultimate.org/node/4944)
- [Jean Goubault-Larrecq. Analyse statique de programmes](https://lsv.ens-paris-saclay.fr/~goubault/CoursProgrammation/slides%20analyse%20statique-step-by-step_compressed.pdf)
- [CPP Script. Mastering The C++ Parser: Quick Guide to Efficient Parsing](https://cppscripts.com/cpp-parser)
