
#include "Timer.h"
#include "Recherche.h"

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

int main() {
    const std::string nomFichier = "../fichierTxt/mot4.txt";
    std::vector<std::string> mots;

    // 1. Chargement du fichier.
    Timer timerLecture;
    timerLecture.start();

    const bool lectureReussie =
        chargerMots(nomFichier, mots);

    timerLecture.stop();

    if (!lectureReussie) {
        std::cerr << "Erreur de lecture du fichier : "
                  << nomFichier << '\n';
        return 1;
    }

    std::cout << "========== CHARGEMENT ==========\n";
    std::cout << "Fichier : " << nomFichier << '\n';
    std::cout << "Nombre de mots recuperes : "
              << mots.size() << '\n';
    std::cout << "Temps de lecture : "
              << formaterDuree(
                     timerLecture.elapsedMicroseconds()
                 )
              << "\n\n";

    if (mots.empty()) {
        std::cout << "Le fichier ne contient aucun mot.\n";
        return 0;
    }

    // 2. Saisie du mot de reference.
    std::string reference;

    std::cout << "Saisissez le mot de reference : ";
    std::cin >> reference;

    // 3. Recherche et mesure du temps total.
    Timer timerRecherche;
    timerRecherche.start();

    const std::vector<ResultatSousChaine> resultats =
        rechercherSousChaines(mots, reference);

    timerRecherche.stop();

    const double tempsRecherche =
        timerRecherche.elapsedMicroseconds();

    // 4. Afficher les resultats.
    std::cout << "\n========== RESULTATS ==========\n";
    std::cout << "Mot de reference : " << reference << '\n';
    std::cout << "Nombre de mots analyses : "
              << mots.size() << '\n';

    if (resultats.empty()) {
        std::cout
            << "Aucune sous-chaine non vide trouvee dans le fichier.\n";
    }
    else {
        const ResultatSousChaine& meilleure = resultats.front();

        std::cout << "\n--- PLUS GRANDE SOUS-CHAINE TROUVEE ---\n";
        std::cout << "Sous-chaine : " << meilleure.texte << '\n';
        std::cout << "Taille : " << meilleure.longueur << '\n';
        std::cout << "Nombre de mots correspondants : "
                  << meilleure.nombreMots << " / "
                  << mots.size() << '\n';
        std::cout << "Temps pour cette sous-chaine : "
                  << formaterDuree(meilleure.tempsMicrosecondes)
                  << '\n';

        std::cout << "\n--- TOUTES LES SOUS-CHAINES TROUVEES ---\n";

        std::cout << std::left
                  << std::setw(24) << "Sous-chaine"
                  << std::setw(12) << "Taille"
                  << std::setw(20) << "Nb de mots"
                  << "Temps de recherche\n";

        std::cout << std::string(75, '-') << '\n';

        for (const ResultatSousChaine& r : resultats) {
            std::cout << std::left
                      << std::setw(24) << r.texte
                      << std::setw(12) << r.longueur;

            std::cout << std::setw(20)
                      << (std::to_string(r.nombreMots)
                          + " / " + std::to_string(mots.size()));

            std::cout << formaterDuree(r.tempsMicrosecondes)
                      << '\n';
        }
    }

    // 5. Bilan des performances.
    std::cout << "\n========== PERFORMANCES ==========\n";
    std::cout << "Temps de chargement : "
              << formaterDuree(
                     timerLecture.elapsedMicroseconds()
                 )
              << '\n';

    std::cout << "Temps total de recherche : "
              << formaterDuree(tempsRecherche) << '\n';

    std::cout << "Nombre de sous-chaines trouvees : "
              << resultats.size() << '\n';

    std::cout << "Temps total (lecture + recherche) : "
              << formaterDuree(
                     timerLecture.elapsedMicroseconds()
                     + tempsRecherche
                 )
              << '\n';

    return 0;
}
