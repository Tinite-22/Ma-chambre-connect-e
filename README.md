# Projet : Ma chambre connectée

## Description générale
Ce projet est un système domotique local et autonome conçu autour d'un microcontrôleur ESP32. Son objectif est de transformer une chambre classique en un environnement interactif, contrôlable à distance depuis un smartphone ou un ordinateur. La particularité de ce système est qu'il fonctionne en totale indépendance : il ne nécessite ni connexion à une box internet, ni abonnement à un service cloud externe, garantissant ainsi une réactivité immédiate et une confidentialité totale des données.

## Architecture et Fonctionnement du Système

L'infrastructure de ce projet repose sur la polyvalence du microcontrôleur ESP32, qui opère simultanément sur quatre niveaux logiques et matériels distincts :

**1. Mode Point d'Accès (Access Point - AP)**
Le système est conçu pour fonctionner en totale autarcie. L'ESP32 est configuré en mode AP, ce qui signifie qu'il agit comme un routeur. Il génère un réseau Wi-Fi local auquel le client (smartphone ou ordinateur) se connecte directement. Cette topologie garantit que l'installation reste pilotable de manière ininterrompue, indépendamment de toute box internet ou d'infrastructures réseau externes.

**2. Serveur Web Embarqué et Optimisation Mémoire**
Une fois la connexion physique établie, l'ESP32 fait office de serveur web (port 80). Lorsqu'un client se connecte à l'adresse IP locale (généralement 192.168.4.1), le microcontrôleur distribue l'interface utilisateur. Le code source de l'interface (HTML, CSS, Vanilla JS) est stocké de manière contiguë dans la mémoire Flash du microcontrôleur grâce à la directive `PROGMEM`. Cette architecture préserve la mémoire vive (SRAM), qui reste disponible pour l'exécution dynamique des tâches.

**3. Routage API et Communication Asynchrone**
L'interaction entre l'interface utilisateur et le matériel est gérée par des requêtes asynchrones. Le serveur web embarqué expose des endpoints d'API (ex: `/api/lampe` et `/api/capteurs`). L'interface client utilise l'API Fetch (JavaScript) pour interroger ou commander ces terminaux en arrière-plan. Cela permet une mise à jour des données environnementales et un contrôle des actionneurs en temps réel, sans aucun rechargement de la page web.

**4. Interfaçage Matériel (Contrôle et Acquisition)**
Le code effectue la traduction entre les requêtes logicielles et les états électriques :
* **Commande de puissance :** La réception d'une requête d'allumage déclenche la mise à l'état BAS (`LOW`) de la broche GPIO correspondante. Cela active le canal du module relais, fermant le circuit de puissance de la lampe de manière électriquement isolée.
* **Acquisition de données :** Pour la surveillance environnementale, l'ESP32 interroge le capteur DHT22 sur une broche numérique dédiée, convertit les variations du signal analogique en valeurs flottantes de température et d'humidité, puis les sérialise au format JSON pour transmission au client web.

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
