// ANALYZER_H

/*************************************************************************
                           Analyzer.h  -  Fichier d'en-tête
                                -------------------
                        Analyzer est une classe qui permet d'analyser
                        un fichier de log et de generer un fichier
                        GraphViz du graphe analysé.

                             -------------------
    début                : 20/01/2024
    copyright            : (C) 2024 par Marcos Bauch Mira
    e-mail               : marcosbauch2434@gmail.com
*************************************************************************/


//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//------------------------------------------------------------------------

//------------------------------------------------------------------------
//---------- Interface de la classe <TrajetSimple> (fichier TrajetSimple.h) ----------------
#if !defined(ANALYZER_H)
#define ANALYZER_H


#include <iostream>
#include <map>

class Analyzer {

    //----------------------------------------------------------------- PUBLIC
    public:
        //----------------------------------------------------- Méthodes publiques
        int generate_graphviz(std::string filename);
        void print();

        int Analyze();
        void Afficher() const;

        //-------------------------------------------- Constructeurs - destructeur
        Analyzer(std::string filename, bool exclude=false, bool graphviz=false, std::string graphvizFilename="", bool time=false, std::string timeHour="");
        ~Analyzer();

        //------------------------------------------------- Surcharge d'opérateurs

    //------------------------------------------------------------------ PRIVE
    private:
        std::string filename;
        
        // options
        bool exclude;
        
        bool graphviz;
        std::string graphvizFilename;

        bool time;
        std::string timeHour;

        // une façon de representer un ranking des hits les plus frequentes
        // est d'utiliser un dictionnaire ordonne (map) qui associe un mot a un nombre
        // map<string [URL], int [frequence]>
        std::map<std::string, std::map<std::string, int>> hitsRef;

        // "Sorting is done according to the comparison function Compare
        // Search, insertion, and removal operations have logarithmic complexity."
        // https://arc.net/l/quote/jsyuirej
        std::multimap<int, std::string, std::greater<int>> orderedHits;
    
    protected:
    //----------------------------------------------------- Méthodes protégées

    //----------------------------------------------------- Attributs protégés



    #ifdef MAP
        // rien à déclarer
    #endif
};

//-------------------------------- Autres définitions dépendantes de <Xxx>

#endif // ANALYZER_H