Moteurs de jeux : compte-rendu de TP3
=====================================

Abstraction autour d'OpenGL
---------------------------

Avec ce TP commence le développement d'une abstraction autour d'OpenGL.

Le but est de diviser la partie rendu en deux niveaux :
- le partie haut-niveau définit des abstractions de haut niveau, indépendentes
de toute API graphique (essentiellement des abstractions de géométrie et de 
paramètres de shaders).
- celle de bas niveau interagit avec OpenGL.

À chaque itération du game loop, la scène est mise à jour. Le moteur de rendu 
détermine ensuite les géométries ayant changé et nécessitant une mise à jour
des buffers GPU, puis génère une liste de draw calls. Enfin, il rend la scène
à l'écran en utilisant cette liste.

La prochaine étape est d'implémenter un système ECS afin de se prémunir des
arbres d'héritage complexes.


Gestion des saisons
-------------------

Le but étant de réutiliser au mieux le code des TP afin de préparer au mieux le 
projet, j'ai préféré laisser pour l'instant la gestion des saisons à une classe
SeasonController.

Chaque viewport est initialisé avec une saison et le temps (simulé par un
timer) s'écoule de la même manière pour tous.

Le rendu selon une saison est défini par les uniforms passées aux shader
programs du terrain et des particules.

La partie abstraction me prenant beaucoup de temps, le rendu n'est actuellement 
pas particulièrement flatteur.


Systèmes de particules
----------------------

La classe ParticleEffect exploite le concept *Structure of Arrays*.

Chaque particule est définie dans un tableau de positions et dans un tableau de
vies.

La direction, la taille et la vitesse des particules est pour l'instant commune 
à toutes.

La prochaine étape est de séparer les données du comportement grâce au pattern 
ECS.

Le comportement est définit dans la méthode `live(dt)`. Les particules sont
générées aléatoirement dans un rayon donné autour de la position du 
ParticleEffect. Lorsqu'une particule "meurt", elle est recyclée.

Un ParticleEffect ne contient que les positions de chaque particule : la 
géométrie d'une particule est générée par un geometry shader.


À venir
-------

- Continuer l'abstraction du moteur de rendu.
- Implémenter un système ECS.
- Améliorer les effets de particules.
