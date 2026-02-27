# ascpp

L'objectif de ce projet est de travailler l'[analyse statique](#analyse_statique)
d'un programme en utilisant l'[interprétation_abstraite](#interpretation_abstraite)
sur un [mini-langage](#mini_langage).
La première étape fixée est de donner le signe d'une expression donnée en entrée.
Par exemple, l'expression ci-dessous doit renvoyer `ZER`, pour zéro,
car c'est une addition de deux zéros.

```cpp
0 + 0
```

La seconde étape est d'améliorer à la fois le mini-langage
pour qu'il puisse prendre en compte davantage de structures,
notamment des structures de flot de contrôle.
Par exemple, l'expression ci-dessous doit renvoyer `x = POS` et `y = TOP`,
avec `POS` pour strictement positif et `TOP` pour entier,
aussi bien positif que négatif, que nul.

```cpp
x = 2
y = -4
while (x < 5) {
  y = y + 1;
  x = x + 1;
}
```

## Analyse statique
:warning: TODO

## Interprétation abstraite
:warning: TODO

## Mini-langage
Pour le moment, le mini-langage n'est constitué que d'entiers et d'additions,
avec la grammaire suivante.

```
entier  n := int
valeur  v := <name> e
expr    e := n | v | add e e | neg e
```

## Sources
- [Keean Schupke. C++ Parser Combinator Library](http://lambda-the-ultimate.org/node/4944)
- [Jean Goubault-Larrecq. Analyse statique de programmes](https://lsv.ens-paris-saclay.fr/~goubault/CoursProgrammation/slides%20analyse%20statique-step-by-step_compressed.pdf)
- [CPP Script. Mastering The C++ Parser: Quick Guide to Efficient Parsing](https://cppscripts.com/cpp-parser)
