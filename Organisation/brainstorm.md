# Brainstorming

**Acronymes**  
BBB : BeagleBone Black  
CMH : couple Moteur + Hélice

## Mon Projet en quelques lignes

Le projet consiste en la réalisation d'un **banc d'essai de moteurs de drone**. 
Il faut que l'utilisateur puisse récupérer la poussée générée par un *couple moteur + hélice* (CMH) 
en fonction de la commande du moteur. L'utilisateur pourra monter le moteur sur le banc facilement, 
puis utiliser l'interface pour lancer la caractérisation du moteur, puis pourra 
récupérer les données *Poussée = f(Commande)* (et *VitesseRotation = f(Commande)*).

Pour simplifier le projet, on suppose les moteurs testés comme brushless controllés en PWM

## Diviser pour mieux regner

### Découverte de Linux Embarqué
- commandes de base
- lien ssh beagleBone
- environnement de développement C pour la BBB
- contrôler une led embarqué
- input/output GPIO

### Contrôle du moteur
- Pilotage du moteur Brushless en PWM (entrées PWM)
- Pour simplifier le projet, on considère qu’on ne testera que des moteurs de ce type

### Mesure de la poussée
- Récupération des données du HX711 (UART)
- petit calcul *(Delta & Bras de Levier)* pour avoir la valeur de poussée du CMH (a mettre dans une config qui sera accessible par l'utilisateur au cas où)

### Mesure de la vitesse réelle de rotation
- capteur de vitesse de rotation
- codeur incrémental
    - précis
    - difficulté d'installation rapide pour différents CMH??
- capteur optique
    - pas besoin de le réinstaller entre différents CMH (soyons prudent et ajoutons : *a priori*)
    - surement moins précis


- Ne remplace pas la mesure de la commande (ici PWM), mais apporte une donnée complémentaire

### Banc de mesure "complet"
- Intégration des différents composants (contrôle du moteur / mesure de la poussée / mesure de la vitesse)
- rédaction d’un protocole pour que l’utilisateur puisse caractériser un CMH rapidement (on simplifiera au maximum)
- réaliser le banc en “dur”
- récupération des valeurs (clé USB, Wifi, …)

### Documentation
- **À rédiger au fur et à mesure des parties abordées dans le projet**
- GPIO
- PWM
- UART

### Affichage
- Interface Beagle WEB
    - Interface Web pour une UX digne des meilleurs, des contrôles clairs, intuitifs
    - Une configuration complete mais accessible
    - Un téléchargement des données
        - raw csv pour ceux qui pourraient en avoir besoin
        - un super rapport pdf, (techno possibles : python, C + pandoc, et surement plein d'autres)
- Ecran tactile
    - Récupération des données par usb ?
    - Écran tactile pour un affichage parfaitement magnifique
    - Écran tactile pour des commandes intuitives
    - Écran tactile pour des remarques admiratives de tous ceux qui verront le projet, et qui auront par conséquent vu ce qu’il y a de plus beau, complet et formidable sur la planète toute entière. Ils pourront alors partir vers l’autre monde serein, reposé et en paix

### Améliorations / Bonus
*Dans le cas où on finit en avance (mdr)*
- Utilisation des PRU
- controle supplémentaires de stabilité du moteur ( isEnSurchauffe || isThereTooMuchWiggleWiggle for example)
