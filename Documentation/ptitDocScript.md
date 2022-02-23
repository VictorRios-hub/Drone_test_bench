# BeagleBone

## Exectuter un script simple

**Exemple avec le controle d'une led**

## 1
Ecrire le script ou transferer le script préalablement écrit. Pour transferer les fichier il faut utiliser scp

**Depuis Windows** : utiliser WinSCP, se connecter comme avec Putty

**Depuis Linux** : utiliser `scp localFile username@destMachine:destination/Path/for/file`

exemple pour la beaglebone : `scp script.c debian@192.168.7.2:/home/debian/Desktop`

## 2 Préparation
### bash

 Pour pouvoir executer le script, il faut que l'utilisateur ai la permission de l'executer *( rappel : on peut vérifier les permissions avec* `ls -l` *)*

 Pour donner la permission d'execution on utilise la commande suivante :

 `chmod a+x <scriptName>` *(chmod permet de changer les permissions, a+x donne la permission à tout le monde (all) d'executer (x) pour le fichier \<scriptName\>)*

### C

 Avant de pouvoir executer quoi que ce soit, il faut compiler le programme, pour commencer et faire simple, on compile directement sur la beaglebone.

 On utilise la commande : `gcc -o scriptName script.c`

## 3 execution

Il suffit ensuite de taper la commande `./scriptName` (détail : le fichier executable n'est pas dans le PATH, donc on précise le dossier courant avec `./`  )

**Note** : parfois le script a besoin d'être executé en tant que root (`sudo ./scriptName`)
