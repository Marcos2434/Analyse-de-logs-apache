# Analyse-de-logs-apache
Analyse de logs apache


Usage: ./analog [options] <nomfichier.log>
Ex. 
```
./analog -e -g diagram.dot anonyme.log
```

-e Cette option permet d’exclure tous les documents qui ont une extension de type image, css ou javascript. 

-g nomfichier.dot
Cette option permet de produire un fichier au format GraphViz du graphe analysé. Chaque
document apparaîtra sous la forme d’un nœud et chaque arc indiquera le nombre de parcours
associés.

-t heure
Cette option permet de ne prendre en compte que les hits qui sont dans le créneau horaire
correspondant à l’intervalle [heure, heure+1[.