ProjetIA
========

Sujet choisi
------------
Entraînement d'un réseau de neurones artificiels

Compilation
-----------
make
 
Exécution
---------
./main


Commentaires
------------
	Avec #define CLOSE_TO_ZERO 0.01 dans Individu.cpp:

	Poids de départ: 
	wih_11 : 0.851185
	wih_12 : 0.996461
	wih_13 : 0.610156
	wih_21 : 0.818521
	wih_22 : 0.658704
	wih_23 : 0.382559
	woh_11 : 0.815148
	woh_21 : 0.727674
	woh_31 : 0.12617

	1825216 itérations
	Poids d'arrivée: 
	wih_11 : 0.644732
	wih_12 : 0.681225
	wih_13 : 0.403704
	wih_21 : 0.696703
	wih_22 : 0.536887
	wih_23 : 0.260742
	woh_11 : 0.697791
	woh_21 : 0.293522
	woh_31 : -0.981853

	Marge d'erreur : 0
	0,0: 0
	0,1: 1
	1,0: 1
	1,1: 0

En affichant la marge d'erreur (valeur absolue) tous les 1M d'itérations,
on s'aperçoit que parfois on tombe dans un cycle: le 1er test ne modifie pas les poids,
le 2ème, 3ème et 4ème tests apportent des modifications qui retombent sur la configuration du 1er -> cycle...
Parfois on a des cycles finis mais tout de même très grand: à chaque itération (les 4 -ou 3- tests)
les poids modifiés tournent en rond sans jamais retomber sur la même valeur mais sans jamais changer suffisament
pour apporter un déclic nécessaire à un meilleur apprentissage.

Pour corriger ça, je pense qu'il faudrait introduire une toute petite mutation entre chaque itération !
