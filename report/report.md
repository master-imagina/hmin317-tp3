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


ECS
---

J'ai intégré la bibliothèque `entityx` (https://github.com/alecthomas/entityx)
pour utiliser le pattern entités-composants-systèmes. Un objet dans le jeu 
n'est plus qu'une coquille vide, pouvant être agrémentée de plusieurs
composants. Pour l'instant, j'ai intégré des composants `Geometry`, `Material`,
`ParticleEffect` et `Transform`.

Au 09 Novembre, l'abstraction OpenGL est suffisemment développée pour me 
permettre de décrire une scène en n'utilisant que des fonctions de haut niveau.


Déroulement d'une itération du gameloop
---------------------------------------

- La caméra est mise à jour selon les actions de l'utilisateur
- Le système des particules met à jour tous les effets de particules de la
scène.
- Le système de rendu déclenche un rendu. Le moteur de rendu détermine alors
les géométries ayant changé et nécessitant une mise à jour des buffers GPU, 
puis génère une liste de draw calls. Enfin, il dessine la scène à l'écran en 
utilisant cette liste.



Gestion des saisons
-------------------

Le but étant de réutiliser le code des TP afin de préparer au mieux le projet, 
j'ai préféré laisser pour l'instant la gestion des saisons à une classe
`SeasonController`, qui wrap un timer et les dates des quatre saisons. 

Le rendu selon une saison est défini par les uniforms passées aux shader
programs du terrain et des particules.

La classe SeasonController émet un signal `seasonChanged()` lorsque la saison courante a changé, ce qui déclenche un envoi des nouvelles uniforms aux shaders 
de la scène.

La partie abstraction me prenant beaucoup de temps, le rendu n'est actuellement 
pas particulièrement flatteur.


Systèmes de particules
----------------------

La classe `ParticleEffect` exploite le concept *Structure of Arrays*.

Chaque particule est définie dans un tableau de positions et dans un tableau de
vies.

La direction, la taille et la vitesse des particules est pour l'instant commune 
à toutes.

Le comportement est définit dans la méthode `ParticleSystem::live(dt)`. Les particules sont générées aléatoirement dans un rayon donné autour de la 
position du ParticleEffect. Lorsqu'une particule "meurt", elle est recyclée.

Un ParticleEffect ne contient que les positions de chaque particule : la 
géométrie d'une particule est générée par un geometry shader.


À venir
-------

- Gérer les lumières
- Gérer les textures

