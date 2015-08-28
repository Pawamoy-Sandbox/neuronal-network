Neural Network
==============
Auteurs: Timothée Mazzucotelli, Stan Wilhelm


Description du code
-------------------
Nous avions écrit une première version du programme avec UN SEUL individu, pour lequel on modifiait les poids à chaque test passé. Cet algorithme s'est avéré faible face à certains opérateurs à simuler dans le réseau.

Sur vos conseils, nous avons alors réécrit le programme en utilisant l'évolution artificielle et une POPULATION d'individus. Cette 2ème version (exécutable 'subject_optimized') est beaucoup plus efficace car elle trouve une solution pour n'importe quelle table de vérité à simuler dans un réseau.

Un individu est composé des 9 poids du réseau (6 poids IN, 3 poids OUT).
L'algorithme évolutionnaire commence par initialiser une population d'individus, puis les évalue en calculant leur fitness.
Il va ensuite créer autant d'enfants que d'individus par croisement ou clonage, et potentiellement les faire muter (probabilités modifiables via la ligne de commande).
Pour le croisement, on sélectionne aléatoirement des couples d'individus, et pour chaque gène (ou poids) on attribue aléatoirement un gène du parent 1 ou du parent 2 à l'enfant.
L'algorithme peut maintenant réduire la population au nombre initial d'individu en créant des paires aléatoires d'individus parmi les parents et les enfants, qui vont "combattre", l'individu possédant le moins bon fitness étant éliminé.
On réitère autant de tours de boucle que nécessaire pour trouver un individu avec un fitness de 0 (ou très proche), ou bien jusqu'à ce que le nombre limite de générations soit atteint.

Nous avons également développé un 3ème version (exécutable 'generic_network') permettant de spécifier le réseau de neurone à utiliser et une table de vérité à simuler dans ce réseau. La syntaxe des fichiers pour les décrire est donnée plus bas dans ce fichier. On peut, avec cette version, avoir 2 entrées IN ou plus, ainsi que plusieurs sorties OUT. On peut avoir autant de couches intermédiaires (hidden layer) que souhaité, chacune comprenant un nombre différent de neurones.
Les fichiers 'network_model' et 'boolean_table' sont déjà fournis avec un exemple de réseau à 3 entrées.


Questions
---------
 1. Quelle est la signification pratique d'un individu dans le cadre de l'évolution artificielle ?
 2. Est-ce que l'algorithme implémenté réussit toujours à trouver la même solution ?
 3. A propos de la fonction d'évaluation, enregistrez la fitness du meilleur individu et la moyenne de fitness de la population à chaque génération. Analyser l'évolution de ces indicateurs.
 4. Essayez de changer le nombre de génération et le nombre d'individus de la population. Quel est le comportement de l'algorithme ?
 5. Quels sont les rôles des opérateurs génétiques de croisement et mutation? Essayez de changer leur probabilités. Que se passe-t-il si on utilise chaque fois seulement un d'entre eux?
 6. Est-ce que vos observations et réflexions seront valides pour n'importe quelle problématique abordée?


Réponses
--------
 1. Lorsqu'on résout un problème grâce à l'évolution artificielle, l'individu est une structure contenant les différentes variables permettant de trouver une solution à ce problème. Par exemple, dans le cas présent, on cherche quels poids satisfont l'obtention de résultats correspondants à l'opération XOR dans un réseau de neurone. L'individu est donc caractérisé par les poids de ce réseau de neurone.
 2. Dans le cas du sujet, le problème posé n'est pas très compliqué à résoudre, il y a donc beaucoup de solutions différentes si ce n'est une infinité. En revanche, une table de vérité donnée pour un certain réseau peut ne pas avoir de solution ! Ou bien avoir plusieurs solutions toujours très proches les unes des autres. Cela dépend donc de la capacité du réseau à recréer la table de vérité donnée.
 3. Au cours de l'évolution, le meilleur fitness diminue au fil des générations jusqu'à atteindre (ou non) la valeur 0 (ou très proche), alors que la moyenne des fitness continue de varier au fil des générations. Cependant, au départ les moyennes sont plutôt élevées et diminuent largement pour se "stabiliser" par la suite. 
 4. Plus le nombre d'individus est élevé, moins l'algorithme itère de générations pour trouver un meilleur individu, mais cela ne signifie pas qu'il trouve une solution plus rapidement. On voit tout de même dans les résultats de tests plus bas qu'avec un nombre élevé d'individus par génération, la moyenne du nombre de générations pour trouver la solution et l'écart entre les valeurs minimum et maximum est faible.
 Diminuer le nombre limite de générations augmente le risque de ne pas trouver de meilleur individu.
 5. L'utilisation de ces opérations génétiques est essentielle. En effet, sans ces variations, on peut se trouver confronté à des cycles, puisque les individus ne changent pas suffisamment.
 L'utilisation de seulement l'un des deux opérateurs risque fortement de ne pas aboutir à une solution.
 6. Si les mêmes règles d'évolution sont conservées (pas d'élitisme, croisement et réduction par sélection aléatoire de couples, ...), alors ces observations restent valides pour n'importe quelle problématique.


Utilisation
-----------
 * Compilation: make
 * Lancement: ./subject_optimized [ARGS] ou ./generic_netork [ARGS]
 * ARGS = proba_croisement:FLOAT proba_mutation:FLOAT nb_generation:INT nb_individu:INT
 * Tests: ./tests.sh o|g
 
Chaque exécution du programme (ré)écrira automatiquement les fichiers AVERAGE_FITNESS.png et BEST_FITNESS.png


Generic Network
---------------
 * Fichier network_model: on peut donner le nombre N d'entrée avec 'In N', idem pour le nombre de sorties avec 'Out N'. On peut ajouter des couches cachées (hidden layer) avec 'Hl N [N...] 0', avec 0 pour arrêter d'ajouter des couches. On peut maintenant relier les neurones avec 'Connect In|Hl [L] N To Hl|Out [L] N', L étant le numéro de couche cachée, N étant le numéro du neurone. Tous les indices démarrent à 1.
 * Fichier boolean_table: on peut donner une table de vérité en tappant des lignes de la sorte '0,0:1;' ou '0, 1, 0: 1, 0;'. Les seules contraintes sont de respecter les mêmes nombres d'entrées et de sorties sur chaque ligne (un espace est obligatoire après chaque booléen).
 

Quelques résultats de tests (nombre de générations)
---------------------------------------------------

result generic network 0.85 0.1 1000000 100
Moyenne : 12887.2
Ecart   : 54384

result generic network 1 1 1000000 10
Moyenne : 11906.4
Ecart   : 31456

result generic network 1 1 1000000 100
Moyenne : 1158.8
Ecart   : 2055

result generic network 1 1 1000000 1000
Moyenne : 315.7
Ecart   : 595

result generic network 1 1 1000000 10000
Moyenne : 148.5
Ecart   : 83

result subject optimized 0.5 0.5 1000000 10000
Moyenne : 128.9
Ecart   : 168

result subject optimized 0.85 0.1 1000000 100
Moyenne : 25798.5
Ecart   : 83944

result subject optimized 1 1 1000000 10
Moyenne : 37858.1
Ecart   : 73279

result subject optimized 1 1 1000000 100
Moyenne : 2369.7
Ecart   : 6034

result subject optimized 1 1 1000000 1000
Moyenne : 424.3
Ecart   : 454

result subject optimized 1 1 1000000 10000
Moyenne : 90.2
Ecart   : 84

