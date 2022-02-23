# Utilisation de la cellule HX711 avec cellule de poids

Ce document a pour objectif de simplifier l'utilisation du HX711. Il présente dans les grandes lignes comment récupérer une mesure d'une cellule de poids et l'interpréter. Il n'est le fruit que de l'utilisation simple que nous avons fait du HX711. Il peut donc être sujet à amélioration en développant certains points. Pour avoir des informations plus précises et techniques, il peut être préférable de se référer à la documentation officiel du HX711.


**Acronymes**

MSB = Most Significant Bit (EN) = Bit de poids fort (FR)

LSB = Less Significant Bit (EN) = Bit de poids faible (FR)

ADC = Analog to Digital Converter (EN) = Convertisseur Analogique Numérique (FR)

## Presentation du HX711

**A COMPLETER**

Le HX711 est un ADC 24 bits spécialisé dans les mesures avec cellule de poids.
	
	
![Aperçu du module](./Img/HX711.jpg)
	

Les pins de gauche sont celles connectées à la cellule, et celles de droite au reste du système.

Pour connecter la cellule, il suffit de suivre le code couleur des fils de la cellule avec les inscriptions sur la carte.

De l'autre coté, le cablage est plus réfléchi :
- VDD doit porter la tension qui servira de référence comme niveau logique (2.7 - 5V)
- VCC est la tension d'alimentation du circuit (2.7 - 5V)
- DAT est le fil par lequel les données vont être transmises
- CLK est une horloge permettant de communiquer avec la carte
- GND est la masse du circuit


## Fonctionnement

La récupération d'une donnée passe par l'utilisation de deux fils : DAT et CLK.

Le fil CLK correspond à une horloge qui va cadencer la communication. La pin correspondante est donc une **sortie** au niveau de la carte à programmer (et une entrée pour la cellule). Quand l'on ne veut pas recevoir de donnée, elle reste à l'état bas. Si CLK reste à l'état haut plus de 60us, le HX711 s'éteindra (mode "power-down") jusqu'à ce qu'elle retourne à l'état bas.

Le fil DAT est le fil par lequel la cellule HX711 va envoyer les données. La pin correspondante est donc une **entrée** au niveau de la carte à programmer (et une sortie pour la cellule). Elle est par défaut à l'état haut.


### Récupération d'une mesure

En passant la pin CLK à l'état bas, on demande une mesure à la cellule. Celle-ci va alors forcer la pin DAT au niveau bas pour signaler qu'elle est prête à envoyer des données.

En démarrant "l'horloge" sur CLK, la cellule va envoyer un bit pour chaque front montant. Au total, il y a entre 24 et 27 bit de données (le nombre dépend du "GAIN" **(PAS COMPRIS CETTE PARTIE LA)** ), le premier étant le MSB.
A la fin de la séquence, DAT retourne à l'état haut.


![Exemple de séquence](./Img/Sequence_data_clk_HX711.PNG)


### Interprétation de la donnée

La donnée reconstitué est sous la forme d'un entier signé codé en complément à deux.
Cependant, il n'existe pas de type de variable de 24 / 27 bits. En C par exemple, il est alors nécessaire d'utiliser une variable de 32 bits (ex : entier non signé), de faire la conversion, puis de la transférer dans un entier signé.

Pour se faire, on peut en language C, pour une data de 24 bits :
- Décaler la data de 8 bits vers la gauche
- Récupérer l'adresse de la variable qui contient les données
- Affecter la valeur à adresse à la variable entière signé
- Diviser la variable non signé par 256 (et non pas la décaler de 8, car la variable est signé et on doit préserver le bit de signe !)


La valeur obtenue peut maintenant être exploitée.


### Exploitation de la mesure

La valeur calculée précédemment ne correspond pas directement à une mesure.
En effet un "offset" fixe est présent et doit être soustrait à la valeur. Celui-ci dépend de l'effort appliqué sur la cellule de poids par la structure sur laquelle elle est située. On le mesure donc en appliquant aucune charge extérieure sur la cellule.

Ensuite, on doit diviser le résultat obtenu par une constante (scale). Elle peut être évaluer en appliquant deux charges de masse connues sur la cellule, et caractérisant le rapport entre les deux valeurs mesurées moins l'offset par rapport au rapport des masses.


Ainsi, *masse(en gramme) = (Valeur interprétée - Offset) / scale*














