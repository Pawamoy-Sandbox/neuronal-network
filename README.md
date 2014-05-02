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

 * ./main
 * ./main pc pm nb_generations

	pc -> probabilité de croisement
	pm -> probabilité de mutation
	nb_generations -> nombre max de générations
	valeurs par défaut si les arguments ne sont pas fournis :
	pc = 0.85
	pm = 0.1
	nb_generations = 1000


Commentaires
------------

 * Avec #define CLOSE_TO_ZERO 0.01 dans Individu.cpp:

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

