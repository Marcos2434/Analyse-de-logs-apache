// ANALYZER_CPP

/*************************************************************************
                           Analyzer.cpp -  Fichier d'implémentation
                                -------------------
                        Analyzer est une classe qui permet d'analyser
                        un fichier de log et de generer un fichier
                        GraphViz du graphe analysé.

                             -------------------
    début                : 21/01/2024
    copyright            : (C) 2024 par Marcos Bauch Mira
    e-mail               : marcosbauch2434@gmail.com
*************************************************************************/
//---------- Réalisation de la classe <Analyzer> (fichier Analyzer.cpp) ------------

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <iostream>
#include <map>
#include <fstream>

//------------------------------------------------------ Include personnel
#include "Analyzer.h"

//------------------------------------------------------------- Constantes

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques


Analyzer::Analyzer(std::string filename, bool exclude, bool graphviz, std::string graphvizFilename, bool time, std::string timeHour)
: filename(filename), exclude(exclude), graphviz(graphviz), graphvizFilename(graphvizFilename), time(time), timeHour(timeHour) {
    #ifdef MAP
        cout << "Appel au constructeur de <Analyzer>" << endl;
    #endif
}

Analyzer::~Analyzer() {
    #ifdef MAP
        cout << "Appel au destructeur de <Analyzer>" << endl;
    #endif
}

void Analyzer::Afficher() const {
    for (auto const& x : orderedHits) {
        std::cout << x.second << " (" << x.first << " hits)" << std::endl;
    }
}

int Analyzer::Analyze() {
    std::ifstream f(filename);

    if (!f.is_open()) {
        std::cout << "Erreur lors de l'ouverture du fichier" << std::endl;
        return 1;
    }

    std::string ligne;
    
    // on separe la ligne en 9 parties separees par des espaces
    std::string IP, userLogname, authUser, dateHour, GMTdiff, action, URL, HTTPversion, refererURL, browserID; 
    int status, size;
    
    // order: IP, userLogname, authUser, dateHour, GMTdiff, action, URL, HTTPversion, status, size, refererURL, browserID
    while (f >> IP >> userLogname >> authUser >> dateHour >> GMTdiff >> action 
    >> URL >> HTTPversion >> status >> size >> refererURL) {
        // the rest goes into browserID
        std::getline(f, browserID);

        // strip "[" from dateHour
        dateHour = dateHour.substr(1, dateHour.length() - 1);

        // strip "]" from GMTdiff
        GMTdiff = GMTdiff.substr(0, GMTdiff.length() - 1);

        // strip """ from action
        action = action.substr(1, action.length() - 1);

        // strip """ from HTTPversion
        HTTPversion = HTTPversion.substr(0, HTTPversion.length() - 1);

        // strip """ from refererURL
        refererURL = refererURL.substr(1, refererURL.length() - 2);

        // strip """ from browserID
        browserID = browserID.substr(1, browserID.length() - 2);

        // strip until the last "/" from refererURL
        if (refererURL.length() > 1) {
            // check if refererURL starts with "http://intranet-if.insa-lyon.fr"
            // if so rename it to "intranet_insa/<refererURL.substr(refererURL.find_last_of("/"), refererURL.length() - 1)> "
            // else rename it to the immediate parent directory ex:
            // http://www.google.fr/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&ved=0CDIQFjAB&url=http%3A%2F%2Fintranet-if.insa-lyon.fr%2Ftemps%2F&ei=_wxLUKCwEMaJhQe08oCYCw&usg=AFQjCNEWrHsRdIlhli_31qYl94w-yMCjpw&sig2=eTdZgqmz8aDAi5eiWxEP1g
            // becomes "www.google.fr"

            if (refererURL.substr(0, 31).compare("http://intranet-if.insa-lyon.fr") == 0) {
                size_t lastSlashPos = refererURL.find_last_of("/");
                refererURL = "insa" + refererURL.substr(lastSlashPos);
            } else {
                size_t startPos = refererURL.find("://") + 3;
                size_t endPos = refererURL.find("/", startPos);
                refererURL = refererURL.substr(startPos, endPos - startPos);
            }
        }

        // on ajoute le URL au map en accordance avec les options
        if (exclude) {
            if (URL.find(".jpg") != std::string::npos 
            || URL.find(".png") != std::string::npos 
            || URL.find(".jpeg") != std::string::npos 
            || URL.find(".ics") != std::string::npos 
            || URL.find(".ico") != std::string::npos 
            || URL.find(".gif") != std::string::npos 
            || URL.find(".css") != std::string::npos 
            || URL.find(".js") != std::string::npos) {
                continue;
            }
        }
        if (time) {
            if (dateHour.substr(12, 2) != timeHour
            && dateHour.substr(12, 2) != std::to_string(std::stoi(timeHour)+1)) {
                continue;
            }
        }
        hitsRef[URL][refererURL] += 1;
        hitsRef[URL]["total"] += 1;
    }


    // on affiche le map ordonne par frequence
    // pour le faire on cree un nouveau multimap ordonne par frequence descendante "orderedHits"

    // "Sorting is done according to the comparison function Compare
    // Search, insertion, and removal operations have logarithmic complexity."
    // https://arc.net/l/quote/jsyuirej

    for (auto const& x : hitsRef) {
        orderedHits.insert(std::make_pair(x.second.at("total"), x.first));
    }

    if (graphviz) {
        generate_graphviz(graphvizFilename);
    }
    
    return 0;
}


int Analyzer::generate_graphviz(std::string filename) {
    std::ofstream of(graphvizFilename);
    if (!of.is_open()) {
        std::cout << "Erreur lors de l'ouverture du fichier" << std::endl;
        return 1;
    }
    of << "digraph {" << std::endl;
    for (auto const& x : hitsRef) {
        for (auto const& y : x.second) {
            if (y.first != "total") {
                of << "\"" << y.first << "\" -> \"" << x.first << "\" [label=\"" << y.second << "\"];" << std::endl;
            }
        }
    }
    of << "}" << std::endl;
    of.close();
    return 0;
}

//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées