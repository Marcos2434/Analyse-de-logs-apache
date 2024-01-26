#include <iostream>
#include <cstring>
#include "Analyzer.h"


// ./analog exemple-mini-non-exhaustif.txt

int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [options] <nomfichier.log>" << std::endl;
        return 1;
    }

    /* options

    -e Cette option permet d’exclure tous les documents qui ont une extension de type image, css ou javascript. 
    
    -g nomfichier.dot
    Cette option permet de produire un fichier au format GraphViz du graphe analysé. Chaque
    document apparaîtra sous la forme d’un nœud et chaque arc indiquera le nombre de parcours
    associés.

    -t heure
    Cette option permet de ne prendre en compte que les hits qui sont dans le créneau horaire
    correspondant à l’intervalle [heure, heure+1[.
    
    */

    bool exclude = false;
    
    bool graphviz = false;
    std::string graphvizFilename = "";

    bool time = false;
    std::string timeHour = "";

    std::string filename = argv[argc-1];

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) {
            exclude = true;
        }
        else if (strcmp(argv[i], "-g") == 0) {
            graphviz = true;
            if (argv[i+1][0] != '-' && i+2 != argc) graphvizFilename = argv[(i++)+1];
            else {
                std::cerr << "Usage: " << argv[0] << " [options] <nomfichier.log>" << std::endl;
                return 1;
            }
        }
        else if (strcmp(argv[i], "-t") == 0) {
            time = true;
            if (argv[i+1][0] != '-' && i+2 != argc) timeHour = argv[(i++)+1];
            else {
                std::cerr << "Usage: " << argv[0] << " [options] <nomfichier.log>" << std::endl;
                return 1;
            }
        }
    }
    
    // on affiche les options 
    // std::cout << "exclude: " << exclude << std::endl;
    // std::cout << "graphviz: " << graphviz << std::endl;
    // std::cout << "graphvizFilename: " << graphvizFilename << std::endl;
    // std::cout << "time: " << time << std::endl;
    // std::cout << "timeHour: " << timeHour << std::endl;

    
    Analyzer analyzer(filename, exclude, graphviz, graphvizFilename, time, timeHour);

    int status = analyzer.Analyze();
    if (status != 0) return status;

    analyzer.Afficher();

    return 0;
}
