# Projet : Ma chambre connectée

## Description générale
Ce projet est un système domotique local et autonome conçu autour d'un microcontrôleur ESP32. Son objectif est de transformer une chambre classique en un environnement interactif, contrôlable à distance depuis un smartphone ou un ordinateur. La particularité de ce système est qu'il fonctionne en totale indépendance : il ne nécessite ni connexion à une box internet, ni abonnement à un service cloud externe, garantissant ainsi une réactivité immédiate et une confidentialité totale des données.

## Fonctionnalités principales
* **Réseau Wi-Fi autonome :** L'ESP32 est configuré en mode Point d'Accès (Access Point) et crée son propre réseau sans fil.
* **Tableau de bord web embarqué :** Une interface utilisateur graphique, hébergée directement dans la mémoire flash de l'ESP32, permet d'interagir avec la chambre depuis n'importe quel navigateur web.
* **Contrôle de l'éclairage :** Pilotage indépendant de quatre lampes (ou autres appareils électriques) par l'intermédiaire d'un module relais à quatre canaux, avec un retour d'état visuel sur l'interface.
* **Surveillance environnementale :** Mesure et affichage en temps réel de la température et du taux d'humidité de la pièce grâce à un capteur de précision DHT22.
* **Suivi d'activité :** Un journal d'événements intégré à la page web permet de visualiser l'historique des actions effectuées sur les lampes en direct.

## Technologies et matériel utilisés
### Matériel
* Carte de développement ESP32 (ex: NodeMCU ESP-WROOM-32)
* Capteur de température et d'humidité DHT22
* Module relais 4 voies
* Composants d'éclairage et alimentation externe

### Logiciel
* Programmation en C++ (via l'IDE Arduino)
* Interface web développée en HTML, CSS et JavaScript pur
* Utilisation de PROGMEM pour l'optimisation de la mémoire
* Communication par requêtes asynchrones (Fetch API) pour une interface fluide sans rechargement de page

## Objectif pédagogique
Ce projet démontre l'intégration réussie de plusieurs disciplines : l'électronique de contrôle (interfaçage de capteurs et commande de puissance), la programmation réseau (création d'un serveur web embarqué) et le développement web front-end (création d'une interface responsive et moderne stockée de manière optimisée).
