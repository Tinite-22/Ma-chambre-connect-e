# Ma chambre connectée : Tableau de Bord IoT Autonome

![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-blue?style=for-the-badge&logo=espressif)
![Language: C++](https://img.shields.io/badge/Language-C++-green?style=for-the-badge&logo=c%2B%2B)
![Protocol: HTTP](https://img.shields.io/badge/Protocol-HTTP-orange?style=for-the-badge)

Une solution domotique légère, robuste et "low-cost" pour le contrôle d'équipements électriques et le suivi environnemental. Ce projet transforme un microcontrôleur ESP32 en un **Point d'Accès Wi-Fi (AP)** autonome qui héberge un tableau de bord web moderne et réactif. Il permet de piloter 4 relais et de lire un capteur DHT22 en temps réel, sans nécessiter de connexion internet ou de routeur externe.

---

## Fonctionnalités Clés

- **Réseau Local Dédié :** Configure l'ESP32 en mode Point d'Accès (Access Point) avec sécurité WPA2. Idéal pour les zones isolées ou les réseaux locaux sécurisés.
- **Mesures en Temps Réel :** Collecte et affichage dynamique de la température et de l'humidité via un capteur DHT22 (rappel toutes les 5 secondes).
- **Contrôle Quad-Relais :** Interface de commutation pour 4 charges électriques (lampes, moteurs, etc.) avec retour visuel immédiat.
- **Optimisation de la RAM :** Intégration complète de l'interface UI (HTML/CSS/JS) dans la mémoire Flash du processeur via la directive `PROGMEM`.
- **Journal d'Activité Intégré :** Un historique interactif côté client affiche en continu le statut des commandes et les erreurs de communication réseau.

---

## Configuration Matérielle (Branchements)

| Composant | Broche ESP32 (GPIO) | Type | Description |
| :--- | :--- | :--- | :--- |
| **Capteur DHT22** | GPIO 4 | Entrée | Capteur de Température & Humidité |
| **Relais 1 (Lampe 1)** | GPIO 26 | Sortie | Commande du premier relais    |
| **Relais 2 (Lampe 2)** | GPIO 27 | Sortie | Commande du deuxième relais   |
| **Relais 3 (Lampe 3)** | GPIO 14 | Sortie | Commande du troisième relais  |
| **Relais 4 (Lampe 4)** | GPIO 12 | Sortie | Commande du quatrième relais  |

---

## Installation et Déploiement

### 1. Prérequis Logiciels
- **IDE Arduino** ou **PlatformIO**
- Instalation de la carte**ESP32** dans l'IDE Arduino
- Bibliothèques requises (disponibles dans le gestionnaire de bibliothèques) :
  - `DHT sensor library` par Adafruit
  - `Adafruit Unified Sensor`

### 2. Personnalisation du Code
Avant de téléverser le code, définissez le mot de passe de votre réseau Wi-Fi autonome à la ligne 12 :
```cpp
const char* motDePasseAP = "MON_MOT_DE_PASSE_SECURISE"; // Minimum 8 caractères

```

### 3. Téléversement

1. Connectez votre carte ESP32 à l'ordinateur.
2. Sélectionnez le modèle de carte approprié (ex: `DOIT ESP32 DEVKIT V1/ Esp32 Dev Module`).
3. Compilez et téléversez le croquis.
4. Ouvrez le Moniteur Série (vitesse `115200 bauds`) pour confirmer le lancement du serveur.

---

##  Guide d'Utilisation

1. **Connexion :** Activez le Wi-Fi sur votre smartphone ou PC, puis connectez-vous au réseau nommé `Mon_Tableau_de_Bord_ESP32`.
2. **Accès au Dashboard :** Lancez votre navigateur web favori et entrez l'adresse IP par défaut : `http://192.168.4.1`
3. **Contrôle :** Utilisez les boutons **ON** et **OFF** pour commuter instantanément les relais matériels.

---

## Architecture du Code & Fonctionnement (Deep Dive)

Le projet utilise une architecture **API REST asynchrone** permettant une interaction fluide sans rechargement de page :

* **Le Frontend (Single Page Application) :** Conçu en HTML5, CSS3 (avec support Responsive Design pour mobiles) et JavaScript moderne. L'application utilise l'API native `fetch()` pour émettre des requêtes en arrière-plan vers l'ESP32.
* **Les Points d'Accès API (Endpoints C++) :**
* `GET /` : Appelle la fonction `gererRacine()` pour envoyer le fichier d'interface stocké dans la mémoire Flash.
* `GET /api/lampe?id=X&etat=Y` : Extrait les paramètres de requêtes et applique l'état électrique sur le tableau de broches. Notez que la logique est configurée en **Active Low** (l'état `LOW` active le relais), ce qui correspond à la majorité des modules de relais du marché.
* `GET /api/capteurs` : Effectue une vérification de la validité de la lecture matérielle à l'aide de la fonction `isnan()`. Si la lecture est valide, elle assemble et renvoie une chaîne au format structuré **JSON** : `{"temperature": XX.XX, "humidite": YY.YY}`.



---

## 👤 Auteur

**Bignon Codjia**
