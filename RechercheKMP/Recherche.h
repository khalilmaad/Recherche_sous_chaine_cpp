
#ifndef RECHERCHE_H
#define RECHERCHE_H

#include <string>
#include <vector>

struct ResultatSousChaine {
    std::string texte;
    int longueur;
    int nombreMots;
    double tempsMicrosecondes;
};

namespace KMP {

    // Construit le tableau des plus longs prefixes/suffixes.
    std::vector<int> construireLPS(const std::string& motif);

    // Recherche le motif en reutilisant un tableau LPS deja construit.
    bool contientKMP(
        const std::string& texte,
        const std::string& motif,
        const std::vector<int>& lps
    );

}

// Genere les sous-chaines candidates, puis les recherche
// dans les mots fournis.
std::vector<ResultatSousChaine> rechercherSousChaines(
    const std::vector<std::string>& mots,
    const std::string& reference,
    double& tempsRechercheMicrosecondes
);

#endif
