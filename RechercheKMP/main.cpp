
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

#include "Timer.h"
#include "Recherche.h"

using namespace std;

// Affiche une duree dans une unite adaptee.
// La valeur recue est toujours exprimee en microsecondes.
void afficherTemps(double microsecondes) {
    cout << fixed << setprecision(3);

    if (microsecondes < 1000.0) {
        cout << microsecondes << " us";
    }
    else if (microsecondes < 1000000.0) {
        cout << microsecondes / 1000.0 << " ms";
    }
    else if (microsecondes < 60000000.0) {
        cout << microsecondes / 1000000.0 << " s";
    }
    else {
        cout << microsecondes / 60000000.0 << " min";
    }
}

int main() {
    // 1. Lecture du fichier.
    Timer chronometreLecture;
    chronometreLecture.demarrer();

    ifstream fichier("../fichierTxt/mot4.txt");

    if (!fichier.is_open()) {
        cerr << "Erreur : impossible d'ouvrir mots.txt\n";
        cerr << "Verifie le dossier de travail du programme.\n";
        return 1;
    }

    vector<string> mots;
    string ligne;

    while (getline(fichier, ligne)) {
        if (!ligne.empty()) {
            mots.push_back(ligne);
        }
    }

    fichier.close();

    double tempsLecture =
        chronometreLecture.ecouleMicrosecondes();

    cout << "========================================\n";
    cout << "  RECHERCHE DE SOUS-CHAINES COMMUNES\n";
    cout << "       Algorithme KMP\n";
    cout << "========================================\n\n";

    cout << "Nombre de mots lus : " << mots.size() << '\n';

    cout << "Temps de lecture : ";
    afficherTemps(tempsLecture);
    cout << "\n\n";

    if (mots.empty()) {
        cout << "Aucun mot valide dans le fichier.\n";
        return 0;
    }

    // 2. Choisir le mot de reference.
    string reference;

    cout << "Entrez le mot de reference : ";
    cin >> reference;

    // 3. Rechercher les sous-chaines.
    double tempsRecherche = 0.0;

    vector<ResultatSousChaine> resultats =
        rechercherSousChaines(
            mots,
            reference,
            tempsRecherche
        );

    // 4. Afficher les resultats.
    cout << "\n========================================\n";
    cout << "             RESULTATS KMP\n";
    cout << "========================================\n";

    cout << "Mot de reference : " << reference << '\n';
    cout << "Nombre de mots : " << mots.size() << '\n';

    cout << "Sous-chaines retrouvees : "
         << resultats.size() << "\n\n";

    if (resultats.empty()) {
        cout << "Aucune sous-chaine retrouvee.\n";
    }
    else {
        const ResultatSousChaine& meilleure =
            resultats.front();

        cout << "----- PLUS GRANDE SOUS-CHAINE -----\n";

        cout << "Sous-chaine : "
             << meilleure.texte << '\n';

        cout << "Taille : "
             << meilleure.longueur << '\n';

        cout << "Nombre de mots contenant cette sous-chaine : "
             << meilleure.nombreMots
             << " / " << mots.size() << '\n';

        double pourcentage =
            100.0 * meilleure.nombreMots / mots.size();

        cout << fixed << setprecision(2);
        cout << "Pourcentage des mots : "
             << pourcentage << " %\n";

        cout << "Temps de recherche de cette candidate : ";
        afficherTemps(meilleure.tempsMicrosecondes);
        cout << "\n\n";

        cout << "----- AUTRES SOUS-CHAINES RETROUVEES -----\n";

        cout << left
             << setw(22) << "Sous-chaine"
             << setw(12) << "Taille"
             << setw(18) << "Nb de mots"
             << "Temps de recherche\n";

        cout << string(75, '-') << '\n';

        for (size_t i = 1; i < resultats.size(); i++) {
            const ResultatSousChaine& r = resultats[i];

            cout << left
                 << setw(22) << r.texte
                 << setw(12) << r.longueur;

            string compteur =
                to_string(r.nombreMots) + " / " +
                to_string(mots.size());

            cout << setw(18) << compteur;

            afficherTemps(r.tempsMicrosecondes);
            cout << '\n';
        }
    }

    // 5. Bilan des temps.
    // Le temps total ne comprend ni la saisie ni les affichages.
    double tempsTotal = tempsLecture + tempsRecherche;

    cout << "\n========================================\n";
    cout << "             BILAN DES TEMPS\n";
    cout << "========================================\n";

    cout << "Lecture du fichier : ";
    afficherTemps(tempsLecture);
    cout << '\n';

    cout << "Recherche complete : ";
    afficherTemps(tempsRecherche);
    cout << '\n';

    cout << "Temps total (lecture + recherche) : ";
    afficherTemps(tempsTotal);
    cout << '\n';

    cout << "========================================\n";

    return 0;
}
