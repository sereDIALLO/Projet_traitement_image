# WeedingProject

## Description : 

WeedingProject est un projet d'application en ligne de commande développée en C++ utilisant OpenCV. Ce projet permet de caractériser un système de désherbage propre par traitement d'images acquises via un banc d'essais. L'objectif principal est de détecter les adventices dans les cultures, estimer leur position, et évaluer la précision des systèmes de désherbage alternatif, tels que le désherbage thermique.

## Fonctionnalités :

- Extraction des lignes lasers à partir d'images acquises.
- Estimation des droites correspondant aux faisceaux lasers à l’aide de la transformée de Hough.
- Segmentation des plantes et adventices à partir d'algorithmes de traitement d'image.
- Évaluation de la précision du jet de désherbage par comparaison avec les positions des adventices.
- Génération de fichiers de sortie contenant les positions calculées (CSV) et des images annotées.
- Structure du Projet

## Structure :

WeedingProject/
│
├── include/
│   ├── DisplayManager.hpp  # Gestion de l'affichage des résultats
│   └── ImageProcessor.hpp  # Traitement des images
│
├── src/
│   ├── DisplayManager.cpp  # Implémentation de l'affichage
│   └── ImageProcessor.cpp  # Implémentation des algorithmes de traitement
│
├── tests/
│   └── main.cpp            # Tests unitaires et validation des fonctionnalités
│
├── data/
│   ├── input/              # Images d'entrée pour le traitement
│   └── output/             # Résultats générés (images annotées, fichiers CSV)
│
├── CMakeLists.txt          # Configuration CMake pour la compilation
└── README.md               # Ce fichier

## Utilisation

Le programme prend en entrée un dossier contenant des images acquises par le banc d'essai. Il traite ces images et génère :

- Des fichiers CSV contenant les informations sur les centres de gravité des adventices et plantes.
- Des images annotées montrant les droites des lasers et les positions des adventices.


# Projet_traitement_image

https://docs.opencv.org/4.x/d3/d63/classcv_1_1Mat.html
## pour accèder à la présentation : https://docs.google.com/presentation/d/1kZOGRLe4i5pcgo1ZQtgiZxh9wSG2bqX0/edit?usp=sharing&ouid=115741324209830049209&rtpof=true&sd=true
https://www.canva.com/design/DAGTd-FxQrY/ISrGS1dS8kwGhwkP4ez-KQ/edit?utm_content=DAGTd-FxQrY&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton

https://docs.google.com/presentation/d/1fx0DZfTKDgp3wzem9cbK7shnKt4G0_rTb5rSs14Tf7Y/edit#slide=id.gd1bf8d60a4_0_0
