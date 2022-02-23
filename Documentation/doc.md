# Utiliser la BeagleBone

**Note** : ceci n'est qu'un brouillon des choses à ne pas oublier,
il faudra alors compléter la documentation déjà commencée par les groupes précédents.

On se connecte à la beagleBone via le cable USB.
La BeagleBone crée une interface réseaux. Il suffit alors de se connecter
à la carte en ssh avec l'utilisateur `debian` ou `root`.
 L'adresse ip de la carte est alors 192.168.7.2.

*Note* : pour se connecter en ssh depuis Windows on peut utiliser Putty,
depuis Linux, il suffit d'utiliser la commande `ssh debian@192.168.7.2`

Nous n'avons pas réussi à mettre un environnement de développement depuis
l'ordinateur. Nous avons donc tout codé directement sur la beagleBone, via ssh.

Pour coder nous avons donc utilisé `Nano` *(ou `Vim`)* en éditeur de texte, et utilisé
`make` pour compiler nos codes.


