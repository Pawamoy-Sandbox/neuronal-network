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

C'est vraiment aléatoire ce truc j'ai l'impression:

24 itérations
Poids d'arrivée: 
wih_11 : 0.13712
wih_12 : 0.560082
wih_13 : 0.387517
wih_21 : 0.464084
wih_22 : 0.629517
wih_23 : 0.361115
woh_11 : -0.0132696
woh_21 : 0.90425
woh_31 : -0.808398

Marge d'erreur : 0
0,0: 0
0,1: 1
1,0: 1
1,1: 0

Alors que parfois en plus de 10 millions d'itérations il trouve pas O_o !
Et encore j'ai été gentil, dans GetError je retourne 0 si m_error est compris entre -0.1 et 0.1 ...
