import Adafruit_BBIO.PWM as PWM
import ctypes
from os import system
from time import sleep

# On charge notre librairie C pour pouvoir l'utiliser ici, en Python
iobb = ctypes.CDLL("./myiobb/myiobb.so", ctypes.RTLD_GLOBAL)
HX711 = ctypes.CDLL("./hx711.so")
# on precise les types de nos fonctions :
HX711.HX711_read_mean.restype = ctypes.c_float

# Pins
pin_moteur = "P9_14"

# Commande Moteur  en pourcentage
firstCommande = 4
lastCommande = 99
#Pas de 1 entre les commandes

#Tab est le tableau dans lequel on stocke toute les donnees
#Il possede trois lignes : La commande du moteur en pourcentage
# la vitesse mesuree par l'encodeur
# la poussee mesuree avec la cellule de poids
Tab = [[],[],[]]

# A partir du point ou on allume une PWM, on se met
# en try catch pour pouvoir l'eteindre si l'utilisateur
# appuie sur ^C
try:
    # On demarre le moteur, a 0
    PWM.start(pin_moteur, 75, 500, 0)
    # On attend un peu pour que l'ESC finisse l'initialisation
    sleep(1)
    
    # On prepare les pins pour la cellule de poids
    HX711.init_pins()

    # On prend l'offset de la celulle de poid, pendant que le
    # moteur est a l'arret
    HX711.HX711_set_offset(ctypes.c_int(100))
    
    unPourcent = float(100-75)/100

    for i,commande in enumerate(range(firstCommande, lastCommande, 1)):

        print(commande)

        # On calcule la valeur du DutyCycle a mettre en fonction du pourcentage
        # Cette ligne depend de l'ESC (ici ESC Basic BlueRobotics)
        # dutyCyle = DCzero + i * (1% du range de commande)
        dutyCycle = 75 + commande * unPourcent
        
        Tab[0].append(dutyCycle)

        # On demarre le moteur a la bonne vitesse
        PWM.set_duty_cycle(pin_moteur, dutyCycle)
        
        # On attend un peu, pour que la vitesse soit stabilise
        sleep(0.4)

        # On mesure la vitesse reelle avec l'encodeur
        Tab[1].append(0)

        # On mesure la poussee du moteur
        Tab[2].append(HX711.HX711_get_value_g(ctypes.c_int(10)))
        # float HX711_get_value_g(int n)


    PWM.stop(pin_moteur)
    PWM.cleanup()
except KeyboardInterrupt:
    PWM.stop(pin_moteur)
    PWM.cleanup()
    print("")



    
# On enregistre les donnees dans un fichier csv
separateur = ";"
chaine = ""
for i in range(len(Tab[1])):
    chaine += str(Tab[0][i]) + separateur
    chaine += str(Tab[1][i]) + separateur
    chaine += str(Tab[2][i]) + "\n"
f = open("file.csv","w")
f.write(chaine)
f.close()

# On genere un graphe png a partir du csv
system("gnuplot plotcsv.gnuplot")
