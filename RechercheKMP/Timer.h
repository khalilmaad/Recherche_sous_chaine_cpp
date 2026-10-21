
#ifndef TIMER_H
#define TIMER_H

class Timer {
public:
    void demarrer();

    // Retourne le temps ecoule en microsecondes.
    double ecouleMicrosecondes() const;

private:
    // Temps de depart conserve en interne.
    long long debutNanosecondes = 0;
};

#endif
