Moteurs de jeux : Projet
========================


Pré-requis
----------

- Un compilateur supportant C++14
- CMake >= 3.5
- Qt >= 5.5


Infos
-----

- Les dossiers `3rdparty`, `core`, `extras`, `input`, `physics`, `render` et 
`script` contiennent le code source du moteur.

- Le dossier `lab/marble` contient le code source du jeu Marble-like.

- Le dossier `editor` contient le code source de l'éditeur de niveaux.


Compilation
-----------

Depuis QtCreator ou CLion : ouvrir simplement le fichier `CMakeLists.txt` situé
à la racine du dossier source.

Depuis la ligne de commande : créer un dossier de compilation à côté du dossier
des sources, puis ouvrir un terminal et

```
mkdir <buildDir> && cd <buildDir>
cmake ../<sourceDir>
cmake --build . --target install
make install
```

Si vous rencontrez une erreur CMake expliquant que Qt5 n'a pas été trouvé,
remplacez la deuxième ligne par :

`cmake ../<sourceDir> -DCMAKE_PREFIX_PATH=<chemin_install_Qt5>/gcc_64`


Exécution
---------

- L'exécutable `marble` lance le jeu Marble-like.

- `editor` lance l'éditeur de niveaux.

Pour lancer un exécutable depuis l'explorateur de fichiers, ouvrez un terminal :

```
cd <buildDir>/install
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:lib
./<exe>
```

