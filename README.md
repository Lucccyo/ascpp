# ascpp

L'objectif de ce projet est de travailler l'[analyse statique](#analyse_statique)
d'un programme en utilisant l'[interprétation_abstraite](#interpretation_abstraite)
sur un [mini-langage](#mini_langage).
L'objectif est de donner le signe abstrait minimal de chaque variable,
à partir d'un programme donné en entrée.
Le mini-langage est grandement inspiré du langage while introduit
par Bernhard Reus dans [Limits of Computation](https://link.springer.com/book/10.1007/978-3-319-27889-6).

Par exemple, le programme ci-dessous doit renvoyer `x = POS` et `y = TOP`,
avec `POS` pour strictement positif et `TOP` pour entier,
aussi bien positif que négatif, que nul.

```cpp
x = 2;
y = -4;
while 5 > x do
  y = y + 1;
  x = x + 1
```

Le programme précédent a pour AST:
```
Seq = (
  Seq = (
    Assign = ( name = x, N = 2);
    Assign = ( name = y, Neg = (N = 4))
  );
  While = (
    Gt = (N = 5; Var = x);
    Seq = (
      Assign = ( name = y, Add = (Var = y; N = 1));
      Assign = ( name = x, Add = (Var = x; N = 1))
    )
  )
)
```

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
