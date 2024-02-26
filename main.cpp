#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;


// Classe représentant un Personnage
class Personnage {
public:
    string nom;
    int sante;
    int force;
    int niveau;
    char arme;
    map<char, int> specs;

//public:
    Personnage(string nom, int sante, int force, int niveau) : nom(nom), sante(sante), force(force), niveau(niveau) {}

    // Méthodes pour accéder aux membres privés
    string getNom() const {
        return nom;
    }

    int getSante() const {
        return sante;
    }

    int getForce() const {
        return force;
    }

    int getNiveau() const {
        return niveau;
    }

    void setForce(int nouvelleForce) {
        force = nouvelleForce;
    }

    void upNiveau() {
        niveau++;
    }

    void setSante(int nouvelleSante) {
        sante = nouvelleSante;
    }


    void init_specs(){
        //ajout specs objets
        specs['z'] = 11;
        specs['x'] = 15;
        specs['c'] = 7;
    }
    
    // Méthode pour attaquer un autre personnage
    virtual void attaquer(Personnage* cible) {
        cout << nom << " attaque " << cible->nom << " !" << endl;
        // Calcul de dégâts simplifié
        int degats = force * (1 + 0.5 * niveau) ;
        cible->sante -= degats;
        cout << cible->nom << " perd " << degats << " points de vie." << endl;
    }

    // Méthode pour vérifier si le personnage est vivant
    bool estVivant() {
        return sante > 0;
    }

    void afficherBarreDeSante() const {
    int tailleBarre = 20; // Taille totale de la barre de santé
    float proportionSante = (float)sante / 100; // Calcul de la proportion de la santé restante, supposons 100 comme santé maximale
    int nbCaracteresRemplis = (int)(tailleBarre * proportionSante); // Nombre de caractères à remplir

    cout << "Santé: [";
    for (int i = 0; i < nbCaracteresRemplis; i++) {
        cout << "="; // Caractère pour la partie remplie de la barre
    }
    for (int i = nbCaracteresRemplis; i < tailleBarre; i++) {
        cout << " "; // Caractère pour la partie vide de la barre
    }
    cout << "] " << sante << "/100" << endl; // Afficher la santé actuelle sur la santé maximale
    }

    // Modification dans afficherInfos pour inclure la barre de santé
    void afficherInfos() {
        cout << "Nom: " << nom << ", Force: " << force << ", Niveau: " << niveau << endl;
        afficherBarreDeSante(); // Affichage de la barre de santé
    }

};



// Classe représentant un Guerrier, dérivée de Personnage
class Guerrier : public Personnage {
public:
    Guerrier(string nom) : Personnage(nom, 100, 5, 1) {
        arme = 'z'; //canon
    }

    // Méthode pour attaquer spécifique à un Guerrier
    void attaquer(Personnage* cible) override {
        arme = 'z';
        // Calcul de dégâts simplifié
        int degats = force * (1.5 + 0.5 * niveau ) + specs[arme];
        cible->sante -= degats;
    }
};

// Classe représentant un Mage, dérivée de Personnage
class Mage : public Personnage {
public:
    Mage(string nom) : Personnage(nom, 100, 4, 1) {
        arme = 'x';
    }

    // Méthode pour attaquer spécifique à un Mage
    void attaquer(Personnage* cible) override {
        arme = 'z'; //sort
        // Calcul de dégâts simplifié
        int degats = force * (1 + 0.5 * niveau) + specs[arme];
        cible->sante -= degats;
    }
};

// Classe représentant un Samurai, dérivée de Personnage
class Samurai : public Personnage {
public:
    Samurai(string nom) : Personnage(nom, 100, 7, 1) {
        arme = 'c'; //katana
    }

    // Méthode pour attaquer spécifique à un Samurai
    void attaquer(Personnage* cible) override {
        arme = 'z';
        // Calcul de dégâts simplifié
        int degats = force * (2 + 0.5 * niveau) + specs[arme];
        cible->sante -= degats;
    }
};


const int hauteur = 7; // Hauteur de la chambre
const int largeur = 20; // Largeur de la chambre

class Graphique {
public:
    int posX; // Position X du personnage
    int posY; // Position Y du personnage
    int salleActuelle; // Numéro de la salle actuelle
    std::map<int, bool> coffreUtilise; // pour assurer l'usage du coffre qu'une seule fois par étage


    Graphique(int x, int y) : posX(x), posY(y), salleActuelle(1) {}

    // Afficher le jeu
    void afficher() {
        system("clear"); // Effacer le terminal (Mac)
        std::cout << "Étage : " << salleActuelle << std::endl;
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                if (i == posY && j == posX) {
                    std::cout << "@";
                } else if (i == 0 || i == hauteur - 1 || j == 0  || j == largeur - 1) {
                    std::cout << "#";
                } else if (i == 1 && j == 1) {
                    std::cout << "?"; // Info
                } else if (i == 2*hauteur/3 && j == largeur/2) {
                    std::cout << "E"; // Ennemi
                } else if (i == 1 && j == 2*largeur/3) {
                    std::cout << "C"; // Coffre
                } else if (i == hauteur / 2 && j == largeur - 2 ) {
                    std::cout << ">"; // Porte
                /*} else if (i == hauteur / 2 && j == 1 && salleActuelle != 0) {
                    std::cout << "<"; // Porte retour
                */} else {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        }
    }

    // Déplacer le personnage

    void deplacer(char key) {
        switch (key) {
            case 'w': // Haut
                if (posY > 1) posY--;
                break;
            case 's': // Bas
                if (posY < hauteur - 2) posY++;
                break;
            case 'a': // Gauche
                if (posX > 1) posX--;
                break;
            case 'd': // Droite
                if (posX < largeur - 2) posX++;
                break;
            default:
                break;
        }
    }

    //actions spéciales
    char skip_ennemy_info ='\n';
    bool action(Personnage& joueur, Personnage& ennemi, Graphique& jeu, char key) {
        if (posY == 1 && posX == 1) { //aide
            afficher();
            system("clear");
            cout << "Bienvenue dans 'Les Légendes des Anciens'. Dans ce jeu, vous explorez des terres mystérieuses,\n"
                << "affrontez des ennemis et résolvez des énigmes pour découvrir les secrets d'un ancien empire.\n"
                << "Votre objectif est de triompher des défis, utiliser judicieusement vos ressources et vaincre\n"
                << "le Big Boss Basile pour inscrire votre nom parmi les légendes. Chaque choix et action peut\n"
                << "influencer l'issue de votre quête. Bonne chance dans votre aventure héroïque!\n";
            cout << "Appuyer sur une touche pour continuer" << endl;
            char skip;
            skip = getchar();
            return 0;
        }
        
        if (posY == 1 && posX == 2*largeur/3) { //coffre
            afficher();
            if(!jeu.coffreUtilise[jeu.salleActuelle]){
            system("clear");
            cout << "Le Coffre : Armes, Potions, Boucliers...\n" << endl;
            cout << "Choisissez un (et un seul) objet que vous allez utiliser dans votre prochain combat.\n" << endl;
            cout << "1. Canon : Une arme puissante qui tire des projectiles explosifs." << endl;
            cout << "2. Sort éclair : Un sort magique qui invoque un éclair destructeur." << endl;
            cout << "3. Katana : Lame tranchante et élégante inflige des dégâts rapides." << endl;
            cout << "4. Potion de soin : Restaure votre santé du personnage lorsqu'elle est consommée.\n" << endl;
            cout << "Choisissez soigneusement! Cela pourrait faire la différence entre vie et mort." << endl;
            cout << "Appuyer sur une touche pour continuer" << endl;
            char choix;
            while (true) {
            choix = getchar();
            if (choix != '1' && choix != '2' && choix != '3' && choix != '4') {
                cout << "SVP choisissez parmi les options indiquées." << std::endl;
            } else {
                jeu.coffreUtilise[jeu.salleActuelle] = true;
                break;
            }
            }
            switch (choix){
                case '1': joueur.arme = 'z';
                case '2': joueur.arme = 'x';
                case '3': joueur.arme = 'c';
                case '4': joueur.sante = 100;}
            } else {
                cout << "Vous avez déjà utilisé ce coffre !" << endl;
                usleep(750000);
                }
            return 0;
        }
        if ( abs(posY - 2*hauteur/3) <= 1 && abs(posX - largeur/2) <= 1 && skip_ennemy_info == '\n' /*check si non vide*/ || key == 'e') { //info ennemi
            afficher();
            system("clear");
            if (jeu.salleActuelle==5) {cout << "Et pour finir... tu devras affronter le Big Boss !"<< endl;}
            cout <<"Ton prochain ennemi est "<< ennemi.getNom() << endl;
            ennemi.afficherInfos();
            cout << endl <<"Pour revoir ces infos par la suite, appuyer sur 'e'" << endl;
            cout << "Appuyer sur une touche pour continuer" << endl;
            skip_ennemy_info = getchar();
            skip_ennemy_info = '0';
            return 0;
        }
        if (1){
            if (posY == hauteur / 2 && posX == largeur - 2){
                if (not ennemi.estVivant()){
                    posY = hauteur / 2;
                    posX = 1;
                    salleActuelle++;
                    skip_ennemy_info ='\n';
                    system("clear");
                    int next;
                    if(salleActuelle==2){
                        cout <<"Bien joué, vous avez réussi à franchir le 2ème étage." << endl;
                        cout <<"Mais ne vous enflammez pas, la fête ne fait que commencer..." << endl;
                        next = getchar(); 
                    }
                    if(salleActuelle==3){
                        cout <<"Pas mal, pas mal, vous arrivez au 3ème étage de votre quête." << endl;
                        cout <<"Pourtant, arriverez-vous à tenir tête à votre prochain adversaire?" << endl;
                        cout <<"Le seul, l'unique, le mytique ..." << endl;
                        next = getchar(); 
                    }
                    if(salleActuelle==4){
                        cout <<"Impressionant !!" << endl;
                        cout <<"Mais votre chemin est loin d'être fini." << endl;
                        cout <<"Une mission difficile vous attend au 4ème étage." << endl;
                        next = getchar(); 
                    }
                    if(salleActuelle==5){
                        cout <<"ONE" << endl;
                        usleep(1000000);
                        cout <<"MORE" << endl;
                        usleep(1000000);
                        cout <<"STEP" << endl;
                        usleep(1000000);
                        cout <<"Vous avez éliminé tous ses Hommes" << endl;
                        cout <<"La colère Big Boss vous attend au 5ème et dernier étage..." << endl;
                        next = getchar(); 
                    }
                    
                    return 0;
                } else { 
                    system("clear");
                    cout << "“We don’t quit, we don’t cower, we don’t run.”"<< endl << " “We endure and conquer.” —Kobe Bryant" << endl;
                    cout << "Il faut affronter l'ennemi avant d'avoir accès à la suite..." << endl;
                    char skipl = getchar();
                }}
        }
        /*else if (posY == hauteur / 2 && posX == 1) {  //gonna need changing to add more rooms !!
            posY = hauteur / 2;
            posX = 1;
            salleActuelle--;
            return 0;
        }*/
        if (posY == 2*hauteur/3 && posX == largeur/2) {
            
            return 1;
        }
        return 0;
    }
};

// Fonction pour activer le mode sans buffer de la console
void activerModeSansBuffer() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Fonction pour désactiver le mode sans buffer de la console
void desactiverModeSansBuffer() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void choixtype(Personnage*& perso, char choixType, string nomPersonnage = "Ennemi"){
        if (choixType == '1') {
            perso = new Guerrier(nomPersonnage);
            return;
        } else if (choixType == '2') {
            perso = new Mage(nomPersonnage);
            return;
        } else if (choixType == '3') {
            perso = new Samurai(nomPersonnage);
            return;
        }
}

int combat(Personnage* joueur, Personnage* ennemi, Graphique& jeu) {
    // Combat simplifié entre les personnages
    if (not ennemi->estVivant()){
        system("clear");
        cout << "Félicitations "<< joueur->getNom() <<" ! Vous avez déjà battu l'ennemi." << endl;
        cout << "Dirigez-vous maintenant vers la Porte" << endl;
        char skipfight = getchar();
        return 1;
    }
    
    while (joueur->estVivant() && ennemi->estVivant()) {
    system("clear"); // Effacer le terminal (Mac)
        cout << "Cliquer sur 'a' pour attaquer" << endl;
        joueur->afficherInfos();
        ennemi->afficherInfos();
        char choixAction;
        choixAction = getchar(); // Attend une entrée du joueur
        if (choixAction == 'a') {
            joueur->attaquer(ennemi);
            if (ennemi->estVivant()) {
                ennemi->attaquer(joueur);
            }
        }
    }
    // Affichage du vainqueur
    if (joueur->estVivant()) {
        if(jeu.salleActuelle==5){
            system("clear");
            cout <<"Vous venez de battre le Big Boss Basile" <<endl;
            usleep(3000000);
            cout <<"Vous avez seul pu conquérir son empire millénaire." <<endl;
            usleep(3000000);
            cout <<"Votre nom "<< joueur->getNom() << "restera gravé à tout jamais dans..."<< endl;
            usleep(3000000);
            cout <<"Les Légendes des Anciens" <<endl;
            usleep(3500000);
            return 0;
        }
        joueur->upNiveau();
        joueur->force++;
        joueur->setSante(min(joueur->getSante()+20,100));
        system("clear");
        cout << "Bravo "<< joueur->getNom() <<" ! Vous avez battu "<< ennemi->getNom() <<"." << endl;
        cout << "Vous avez augmenté d'un niveau ! Votre niveau actuel : " << joueur->getNiveau() <<endl;
        cout << "Vous pourrez maintenant accéder à la Porte" << endl;
        char skipcongrats = getchar();
        return 1;
    } else {
        cout << "Vous avez perdu la bataille :( " << endl;
        cout << "Essayez de changer de tactique et réessayez..." << endl;
        char quit = getchar();
;       return 0;
    }
    char skipfight = getchar();
}

// Fonction principale
int main() {
    system("clear");

    while (1){
        // Menu
        while(1){
            system("clear");
            cout << "=======================================\n";
            cout << "  Bienvenue dans Légendes des Anciens\n";
            cout << "=======================================\n";
            cout << "Dans ce jeu, vous explorez des terres mystérieuses, affrontez \n"
                 << "des ennemis pour découvrir les secrets d'un ancien empire.\n"
                 << "Votre objectif est de triompher des défis, utiliser judicieusement \n"
                 << "vos ressources et vaincre le Big Boss Basile pour inscrire votre nom \n"
                 << "parmi les légendes. Chaque choix et action peut influencer l'issue \n"
                 << "de votre quête. Bonne chance dans votre aventure héroïque!\n";
            cout << "================================================================\n";
            cout << "1. Commencer le jeu\n";
            cout << "2. Voir les crédits\n";
            cout << "3. Quitter\n";
            cout << "Choisissez une option: ";
            char option;
            cin >> option;
            cin.ignore();
            if(option=='1'){break;}
            else if(option=='2'){
                    system("clear");
                    cout << "\nJeu développé par 23Oulhazzan\n";
                    cout << "Tous droits réservés © 2024\n\n";
                    cout << "Cliquer sur une touche pour revenir au menu.";
                    activerModeSansBuffer();
                    char retour;
                    retour = getchar();
                    desactiverModeSansBuffer();}
            else if(option=='3'){
                return 0;}
        }

        system("clear");
        // Demande à l'utilisateur de saisir le nom du personnage
        cout << "Entrez le nom de votre personnage: ";
        string nomPersonnage;
        cin >> nomPersonnage;
        
        system("clear");
        // Demande à l'utilisateur de choisir le type de personnage
        cout << "Choisissez le type de personnage (1: Guerrier, 2: Mage, 3: Samurai): ";
        char choixType;
        while (true) {
            cin >> choixType;
            if (choixType != '1' && choixType != '2' && choixType != '3') {
                cout << "SVP choisissez parmi les options indiquées." << std::endl;
            } else {
                break;
            }
        }

        // Création du personnage en fonction du choix de l'utilisateur
        Personnage* joueur;
        choixtype(joueur, choixType, nomPersonnage);

        // Création d'un ennemi
        Personnage* ennemi;
        int type_ennemi = rand() % 3 + 1;
        char type = '0' + type_ennemi;
        choixtype(ennemi, type);

        Personnage* e1 = new Personnage("Valérie", 100, 5, 1);
        Personnage* e2 = new Personnage("Béa", 100, 8, 2);
        Personnage* e3 = new Personnage("MatMaz", 100, 10, 3);
        Personnage* e4 = new Personnage("Fontane", 100, 13, 4);
        Personnage* e5 = new Personnage("Basile", 120, 16, 5);
        vector<Personnage*> list_ennemis;
        list_ennemis.push_back(e1);
        list_ennemis.push_back(e2);
        list_ennemis.push_back(e3);
        list_ennemis.push_back(e4);
        list_ennemis.push_back(e5);


        joueur->init_specs();

        activerModeSansBuffer(); // Activer le mode sans buffer de la console
        
        Graphique jeu(1, hauteur / 2); // Position initiale du personnage
        
        char input;
        while (true) {
            ennemi = list_ennemis[jeu.salleActuelle-1];
            jeu.afficher();
            std::cout << "Utilisez les touches w, a, s, d pour vous déplacer, q pour quitter " << std::endl;
            input = getchar(); // Attend une entrée du joueur

            if (input == 'q') {
                break; // Quitter le jeu si la touche q est pressée
            } else {
                jeu.deplacer(input);
                bool fight = jeu.action(*joueur, *ennemi, jeu, input);
                if (fight) {
                    if ( not combat(joueur, ennemi, jeu) ) {
                        break;
                    }
                
                }
            }
        }
        desactiverModeSansBuffer(); // Désactiver le mode sans buffer de la console

        // Libération de la mémoire allouée dynamiquement
        delete joueur;
        delete ennemi;
        system("clear");}
}