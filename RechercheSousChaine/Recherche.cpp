
#include "Recherche.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <set>
#include <sstream>

// Charge les mots du fichier : un mot par ligne.
bool chargerMots(
    const std::string& nomFichier,
    std::vector<std::string>& mots
) {
    std::ifstream fichier(nomFichier);

    if (!fichier.is_open()) {
        return false;
    }

    mots.clear();
    std::string ligne;

    while (std::getline(fichier, ligne)) {
        auto debut = std::find_if_not(
            ligne.begin(), ligne.end(),
            [](unsigned char c) {
                return std::isspace(c);
            }
        );

        auto fin = std::find_if_not(
            ligne.rbegin(), ligne.rend(),
            [](unsigned char c) {
                return std::isspace(c);
            }
        ).base();

        if (debut < fin) {
            mots.emplace_back(debut, fin);
        }
    }

    return !fichier.bad();
}

// Recherche toutes les sous-chaines distinctes de la reference.
// Pour chaque sous-chaine, compte le nombre de mots qui la contiennent.
std::vector<ResultatSousChaine> rechercherSousChaines(
    const std::vector<std::string>& mots,
    const std::string& reference
) {
    std::vector<ResultatSousChaine> resultats;

    if (reference.empty() || mots.empty()) {
        return resultats;
    }

    // Eviter de traiter plusieurs fois la meme sous-chaine.
    std::set<std::string> sousChaines;

    for (std::size_t longueur = 1;
         longueur <= reference.size();
         ++longueur) {

        for (std::size_t debut = 0;
             debut + longueur <= reference.size();
             ++debut) {

            sousChaines.insert(
                reference.substr(debut, longueur)
            );
        }
    }

    // Compter les mots correspondants pour chaque sous-chaine.
    for (const std::string& candidate : sousChaines) {
        const auto debutRecherche =
            std::chrono::steady_clock::now();

        std::size_t compteur = 0;

        for (const std::string& mot : mots) {
            // Un mot est compte une seule fois,
            // meme si la sous-chaine apparait plusieurs fois.
            if (mot.find(candidate) != std::string::npos) {
                ++compteur;
            }
        }

        const auto finRecherche =
            std::chrono::steady_clock::now();

        const double duree =
            std::chrono::duration<double, std::micro>(
                finRecherche - debutRecherche
            ).count();

        // Ne conserver que les sous-chaines trouvees
        // dans au moins un mot du fichier.
        if (compteur > 0) {
            resultats.push_back({
                candidate,
                candidate.size(),
                compteur,
                duree
            });
        }
    }

    // Trier : longueur decroissante, puis ordre alphabetique.
    std::sort(
        resultats.begin(),
        resultats.end(),
        [](const ResultatSousChaine& a,
           const ResultatSousChaine& b) {

            if (a.longueur != b.longueur) {
                return a.longueur > b.longueur;
            }

            return a.texte < b.texte;
        }
    );

    return resultats;
}

// Formater une duree en us, ms, s ou min.
std::string formaterDuree(double microsecondes) {
    std::ostringstream resultat;
    resultat << std::fixed << std::setprecision(3);

    if (microsecondes < 1000.0) {
        resultat << microsecondes << " us";
    }
    else if (microsecondes < 1000000.0) {
        resultat << microsecondes / 1000.0 << " ms";
    }
    else if (microsecondes < 60000000.0) {
        resultat << microsecondes / 1000000.0 << " s";
    }
    else {
        resultat << microsecondes / 60000000.0 << " min";
    }

    return resultat.str();
}
