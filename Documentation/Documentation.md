# Documentation

## Sommaire
1. **Fonctionnement du suivi de mur**
2. **Fonctionnement de la recherche aléatoire**
3. **Problèmes de fonctionnement par rapport au PC utilisé**

## Fonctionnement du suivi de mur
Sous forme de logigramme

- Si le robot ne détecte pas de mur devant lui (Capteures : Front left et Front right), **alors il avance tout droit**

- Si le robot détecte un mur devant lui (Capteures : Front left et Front right), **alors il tourne à sa droite pour l'éviter**

- Si le robot ne détecte pas de mur sur sa gauche (Capteurs : Rear left, Left et Diag left), **alors il tourne à gauche pour s'en rapprocher**

## Fonctionnement de la recherche aléatoire
Sous forme de logigramme








## Problèmes de fonctionnement par rapport au PC utilisé

Tout d'abord, il est nécessaire que le robot distingue la différence entre une ligne noire et la case noire.

Pour cela, il suffit de regarder le temps que reste le robot sur du noir.

Si ce temps est assez long, alors c'est qu'il s'agit de la case noire.

Evidemment, il est possible que le robot suive une ligne noire par malchance, alors il faut que le temps soit le plus long possible sans que celui néglige que le robot ne passera surement pas au centre de celle-ci.

Alors, il faut trouver une valeur convenable empiriquement, indiqué à la ligne 76 du code

`76   int time_CELL = -200;`

Or, d'un ordinateur à l'autre, le déplacement du robot est plus ou moins rapide, à cause de la puissance de calcul.

Exemple : sur mon PC cette valeur est -200

tandis que sur mon ordinateur portable cette valeur est de -700

Pour trouver votre bonne valeur, il est nécessaire de la trouver vous même, sachez que la valeur -200 est celle attribuée au robot sur les vidéos disponibles dans le README.md
