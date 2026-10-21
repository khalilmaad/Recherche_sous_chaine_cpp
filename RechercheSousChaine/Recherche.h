
#ifndef RECHERCHE_H_INCLUDED
#define RECHERCHE_H_INCLUDED

#include <string>
#include <vector>
#include <cstddef>

struct ResultatSousChaine {
    std::string texte;
    std::size_t longueur;
    std::size_t nombreMots;
    double tempsMicrosecondes;
};

bool chargerMots(
    const std::string& nomFichier,
    std::vector<std::string>& mots
);

std::vector<ResultatSousChaine> rechercherSousChaines(
    const std::vector<std::string>& mots,
    const std::string& reference
);

std::string formaterDuree(double microsecondes);

#endif // RECHERCHE_H_INCLUDED
