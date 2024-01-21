#include <iostream>
#include <fstream>
#include <map>

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
    std::string graphvizFilename;

    bool time = false;
    std::string timeHour;

    std::string filename = argv[argc-1];

    for (size_t i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-e") == 0) {
            exclude = true;
        }
        else if (std::strcmp(argv[i], "-g") == 0) {
            graphviz = true;
            if (argv[i+1][0] != '-' && i+2 != argc) graphvizFilename = argv[(i++)+1];
            else {
                std::cerr << "Usage: " << argv[0] << " [options] <nomfichier.log>" << std::endl;
                return 1;
            }
        }
        else if (std::strcmp(argv[i], "-t") == 0) {
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

    std::ifstream f(filename);

    if (!f.is_open()) {
        std::cout << "Erreur lors de l'ouverture du fichier" << std::endl;
        return 1;
    }

    // la meilleire façon de representer un ranking des hits les plus frequentes
    // est d'utiliser un dictionnaire ordonne (map) qui associe un mot a un nombre
    // map<string [URL], int [frequence]>

    std::map<std::string, std::map<std::string, int>> hitsRef;

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

        // std::cout << IP << std::endl;
        // std::cout << userLogname << std::endl;
        // std::cout << authUser << std::endl;
        // std::cout << dateHour << std::endl;
        // std::cout << GMTdiff << std::endl;
        // std::cout << action << std::endl;
        // std::cout << URL << std::endl;
        // std::cout << HTTPversion << std::endl;
        // std::cout << status << std::endl;
        // std::cout << size << std::endl;
        // std::cout << refererURL << std::endl;
        // std::cout << browserID << std::endl;

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
    // for (auto const& x : hits) {
    //     std::cout << x.first << ": " << x.second << std::endl;
    // }

    // on cree un nouveau multimap ordonne par frequence descendante

    // "Sorting is done according to the comparison function Compare
    // Search, insertion, and removal operations have logarithmic complexity."
    // https://arc.net/l/quote/jsyuirej
    std::multimap<int, std::string, std::greater<int>> orderedHits; 
    for (auto const& x : hitsRef) {
        orderedHits.insert(std::make_pair(x.second.at("total"), x.first));
    }

    for (auto const& x : orderedHits) {
        std::cout << x.second << " (" << x.first << " hits)" << std::endl;
    }

    if (graphviz) {
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
    }

    return 0;
}