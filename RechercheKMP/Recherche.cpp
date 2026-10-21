
#include "Recherche.h"
#include "Timer.h"

#include <algorithm>
#include <set>
#include <vector>
#include <string>

namespace KMP {

    std::vector<int> construireLPS(const std::string& motif) {
        int m = static_cast<int>(motif.size());
        std::vector<int> lps(m, 0);

        int longueur = 0;
        int i = 1;

        while (i < m) {
            if (motif[i] == motif[longueur]) {
                longueur++;
                lps[i] = longueur;
                i++;
            } else {
                if (longueur != 0) {
                    longueur = lps[longueur - 1];
                } else {
                    lps[i] = 0;
                    i++;
                }
            }
        }

        return lps;
    }

    bool contientKMP(
        const std::string& texte,
        const std::string& motif,
        const std::vector<int>& lps
    ) {
        int n = static_cast<int>(texte.size());
        int m = static_cast<int>(motif.size());

        if (m == 0) {
            return true;
        }

        if (n < m) {
            return false;
        }

        int i = 0; // Indice dans le texte
        int j = 0; // Indice dans le motif

        while (i < n) {
            if (texte[i] == motif[j]) {
                i++;
                j++;
            }

            if (j == m) {
                // Le motif a ete trouve.
                return true;
            }

            if (i < n && texte[i] != motif[j]) {
                if (j != 0) {
                    // Recul intelligent dans le motif,
                    // sans reculer dans le texte.
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }

        return false;
    }

}

std::vector<ResultatSousChaine> rechercherSousChaines(
    const std::vector<std::string>& mots,
    const std::string& reference,
    double& tempsRechercheMicrosecondes
) {
    Timer chronometreRecherche;
    chronometreRecherche.demarrer();

    std::vector<ResultatSousChaine> resultats;

    if (reference.empty() || mots.empty()) {
        tempsRechercheMicrosecondes =
            chronometreRecherche.ecouleMicrosecondes();
        return resultats;
    }

    // 1. Generer toutes les sous-chaines distinctes.
    std::set<std::string> sousChaines;

    for (std::size_t i = 0; i < reference.size(); i++) {
        for (std::size_t longueur = 1;
             longueur <= reference.size() - i;
             longueur++) {

            sousChaines.insert(reference.substr(i, longueur));
        }
    }

    // 2. Tester chaque candidate avec KMP.
    for (const std::string& candidate : sousChaines) {
        Timer chronometreCandidate;
        chronometreCandidate.demarrer();

        // Construire LPS une seule fois par candidate.
        std::vector<int> lps = KMP::construireLPS(candidate);

        int nombreMots = 0;

        // Chaque mot est compte au maximum une fois.
        for (const std::string& mot : mots) {
            if (KMP::contientKMP(mot, candidate, lps)) {
                nombreMots++;
            }
        }

        double tempsCandidate =
            chronometreCandidate.ecouleMicrosecondes();

        // Conserver seulement les candidates retrouvees.
        if (nombreMots > 0) {
            ResultatSousChaine resultat;

            resultat.texte = candidate;
            resultat.longueur =
                static_cast<int>(candidate.size());
            resultat.nombreMots = nombreMots;
            resultat.tempsMicrosecondes = tempsCandidate;

            resultats.push_back(resultat);
        }
    }

    // 3. Trier : longueur decroissante, puis ordre alphabetique.
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

    tempsRechercheMicrosecondes =
        chronometreRecherche.ecouleMicrosecondes();

    return resultats;
}
