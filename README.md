# Banc d’essais pour les systèmes de désherbage alternatifs

## Description : 

Projet d'application en ligne de commande développée en C++ utilisant OpenCV qui permet de caractériser un système de désherbage propre par traitement d'images acquises via un banc d'essais. L'objectif principal est de détecter les adventices dans les cultures, estimer leur position, et évaluer la précision des systèmes de désherbage alternatif, tels que le désherbage thermique.

## Fonctionnalités :

- Extraction des lignes lasers à partir d'images acquises.
- Estimation des droites correspondant aux faisceaux lasers à l’aide de la transformée de Hough.
- Segmentation des plantes et adventices à partir d'algorithmes de traitement d'image.
- Évaluation de la précision du jet de désherbage par comparaison avec les positions des adventices.
- Génération de fichiers de sortie contenant les positions calculées (CSV) et des images annotées.
- Structure du Projet

## Structure :

<img width="1024" alt="image" src="https://github.com/user-attachments/assets/7472c44c-c17c-480e-a162-fc17dc6fad93">


## Utilisation

Le programme prend en entrée un dossier contenant des images acquises par le banc d'essai. Il traite ces images et génère :

- Des fichiers CSV contenant les informations sur les centres de gravité des adventices et plantes.
- Des images annotées montrant les droites des lasers et les positions des adventices.


# OPEN CV

https://docs.opencv.org/4.x/d3/d63/classcv_1_1Mat.html
