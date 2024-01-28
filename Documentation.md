# Documentation

## Sommaire
1. **Logique fondamentale**
2. **Fonctionnement du suivi de mur**
3. **Fonctionnement de la recherche aléatoire**
4. **Problèmes de fonctionnement par rapport au PC utilisé**

## Logique fondamentale
[A voir sous forme de logigramme en cliquant ici](https://github.com/Poblit0/TP-Info/blob/main/Logigrammes/TP%20Info%20-%20Logique%20fondamentale.pdf)

Le robot démarre sur la case rouge et se met à suivre le mur de gauche en appliquant la fonction `wall_following` à la ligne 211, il va donc réaliser un tour complet du labyrinthe.

Durant ce 1er tour de labyrinthe, le robot est à la recherche de la case noire en appliquant la fonction `black_found` à la ligne 315.

Avec cette logique, 2 possibilités s'envisagent :
- Le robot est passé sur la case noire avant de revenir sur la case rouge :
  - Dans ce cas, lorsque le robot est passé sur la case noire, il va arrêter de suivre la fonction `black_found` et bascule sur la fonction `red_found` à la ligne 331, qui elle va rechercher la case rouge, puis clôturer le programme lorsque le robot se trouvera sur la case rouge.
- Le robot est revenu sur la case rouge sans être passé par la case noire :
  - Dans ce cas, lorsque le robot se retrouve sur la case rouge sans avoir vu la case noire, il va arrêter de suivre la fonction `black_found` et bascule sur la fonction `noBlackWall` à la ligne 346, qui elle va déclencher la recherche aléatoire qui est expliqué dans le point 3 de cette documentation.

## Fonctionnement du suivi de mur
A voir sous forme de logigramme en cliquant ici

La stratégie du robot est de suivre le mur de gauche, pour ce faire il applique les conditions de la fonction `wall_following` à la ligne 211 :


- Si le robot ne détecte pas de mur devant lui (Capteurs : *Front left* et *Front right*), **alors il avance tout droit**
- Si le robot détecte un mur devant lui (Capteurs : *Front left* et *Front right*), **alors il tourne à sa droite pour l'éviter**
- Si le robot ne détecte pas de mur sur sa gauche (Capteurs : *Rear left*, *Left* et *Diag left*), **alors il tourne à gauche pour s'en rapprocher**

## Fonctionnement de la recherche aléatoire
A voir sous forme de logigramme en cliquant ici

La stratégie du robot est d'éviter les murs, pour ce faire il applique les conditions de la fonction `goCrazy` à la ligne 229 :

- Si le robot détecte un mur sur sa droite (Capteurs : *Rear right* et *Right*), **alors il tourne à gauche pour s'en éloigner**
- Si le robot détecte un mur sur sa gauche (Capteurs : *Rear left* et *Left*), **alors il tourne à droite pour s'en éloigner**
- Si le robot détecte un mur devant lui (Capteurs : *Front left* et *Front right*), **alors il tourne à gauche pour l'éviter**
- Si le robot détecte un mur devant lui à droite (Capteurs : *Front right*), **alors il tourne à gauche pour l'éviter**
- Si le robot détecte un mur devant lui à gauche (Capteurs : *Front left*), **alors il tourne à droite pour l'éviter**
- Sinon le robot avance tout droit

Une fois la case noire trouvée, le robot alterne entre les 2 états :
- Recherche aléatoire pendant **10s**
- Suivi de mur pendant **30s**

L'alternance entre ces 2 états est permis grâce à la fonction `waitThirtySeconds` à la ligne 250 qui maintient ces 2 états pendant 10s ou 30s selon l'état.

Cette méthode a pour objectif de se mettre à retrouver le mur d'enceinte et le suivre à nouveau pour atteindre la case rouge.

Par conséquent, le robot va se déplacer aléatoirement pendant 10s pour trouver un mur au hasard.

Et va le suivre pendant une longue durée pour être certain d'avoir parcouru tous les recoins de celui-ci.

Ce temps peut être ajustable en fonction de la puissance de votre PC, mais ce n'est vital que pour améliorer la performance et non le fait que le robot soit simplement fonctionnel.

## Problèmes de fonctionnement par rapport au PC utilisé

Tout d'abord, il est nécessaire que le robot distingue la différence entre une ligne noire et la case noire.

Pour cela, il suffit de regarder le temps que reste le robot sur du noir.

Si ce temps est assez long, alors c'est qu'il s'agit de la case noire.

Evidemment, il est possible que le robot suive une ligne noire par malchance, alors il faut que le temps soit le plus long possible sans que celui néglige que le robot ne passera surement pas au centre de celle-ci.

Alors, il faut trouver une valeur convenable empiriquement, indiqué à la ligne 75 du code :

`int time_CELL = -200;`

Or, d'un ordinateur à l'autre, le déplacement du robot est plus ou moins rapide, à cause de la puissance de calcul.

Exemple : sur mon PC cette valeur est -200, tandis que sur mon ordinateur portable cette valeur est de -700

Pour trouver votre bonne valeur, il est nécessaire de la trouver vous-même, sachez que la valeur -200 est celle attribuée au robot sur les vidéos disponibles dans le README.md
