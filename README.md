# projet_cpp_groupe2

Mon but a été de créer un jeu RPG textuel respectant toutes les exigences du projet, et ajouter aussi une interface graphique pour que l'évolution du jeu soit plus parlante.
J'ai donc crée deux classes indépendante : Personnage et Graphique.
La première gère comme son nom l'indique les différents types personnages que ce soit du niveau du joueur ou de ses ennemis, et gère aussi les objets (armes, potions..) et les combats (exemple perso1->attaque(perso2)) en incorporant des méthodes propres à chaque type de personnage (qui vont du coup avoir leur propre classe dérivée).
La classe Graphique gère l'affichage en 2D du pesonnage dans un plan de jeu pour illustrer le parcours du caractère. Elle permet de monter vers l'étage suivant, d'espionner et avoir des informations sur le prochain ennemi, et d'accéder au coffre fort pour obtenir des ressources supplémentaires.
Un défi important à gérer était de savoir faire marcher les deux classes ensembles, nottament que la majorité des objets manipulés étaient des pointeurs des classes dérivés de Personnage. Les manipuler à l'aide de fonctions dans la classe Graphique était un peu tricky.
Une autre difficulté rencontrée était avec le module <termios> pour pouvoir utiliser la fonction getchar() en mode sans buffer, que ce soit pour les deplacement dans la map, ou encore pour skip des messages d'information en un seul clic (et du coup sans passer par std::cin>>;)
Je regrette avoir perdu beaucoup de temps sur une piste de jeu que j'ai dû abandonner car elle ne me satisfaisait pas beaucoup, je pense qu'avec plus de temps sur ce jeu là je pourrait l'améliorer considérablement (ou même faire un truc style "Minecraft")
Bon jeu!
