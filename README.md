# TP-Info
Projet d'informatique - Promotion ROB3 2023/2024

De DELFLY Agathe et MAUVOISIN Paul

## Objectif principal
Coder un robot en Arduino sur CoppeliaSim avec HoRoSim.

Ce robot se trouve dans un labyrinthe quadrillé et a pour objectif de trouver la case noire, puis de retourner sur la case rouge de départ.

## Exemples en vidéo
- Vidéo 1 : [maze_normal1](https://www.youtube.com/watch?v=HaleZ_vFznU&ab_channel=PaulMauvoisin)
- Vidéo 2 : [maze_big](https://www.youtube.com/watch?v=fDX4a9C5hy8&ab_channel=PaulMauvoisin)
- Vidéo 3 : [case_noire_extremite_mur2](https://www.youtube.com/watch?v=wzbpw3iPApY&ab_channel=PaulMauvoisin)

Attention, dans cette troisième vidéo le robot effectue directement la recherche aléatoire, car on l'a déjà informé que la case noire n'était pas adjacente au mur d'enceinte.

Il est aussi possible de ne pas l'en informer, dans ce cas le robot aurait d'abord fait un tour en longeant le mur puis se mettre à chercher aléatoirement la case noire.
