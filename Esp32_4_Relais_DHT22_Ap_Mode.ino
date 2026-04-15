#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// =========================================================================
// 1. CONFIGURATION DU RÉSEAU WI-FI (MODE AP - POINT D'ACCÈS)
// =========================================================================
// Définition des identifiants du réseau autonome créé par l'ESP32.
// Le SSID est le nom public qui sera visible lors de la recherche de réseaux sur ton smartphone.
const char* ssidAP = "Mon_ESP32_Dashboard"; 
// Clé de sécurité WPA2 (Doit impérativement contenir au moins 8 caractères pour être valide).
const char* passwordAP = "";

// Instanciation de l'objet WebServer. Le port 80 est le port standard pour le trafic HTTP non sécurisé.
WebServer server(80);

// =========================================================================
// 2. CONFIGURATION DU MATÉRIEL (Assignation des GPIO)
// =========================================================================
// Tableau contenant les numéros des broches physiques (GPIO) de l'ESP32 reliées aux modules relais.
const int brochesRelais[4] = {26, 27, 14, 12}; 

// Configuration de la sonde de température et d'humidité.
#define DHTPIN 4       // Le signal de données du capteur est connecté au GPIO 4.
#define DHTTYPE DHT22  // Spécification du modèle de capteur (DHT22 est plus précis que le DHT11).
DHT dht(DHTPIN, DHTTYPE); // Initialisation de l'instance du capteur avec la broche et le type définis.

// =========================================================================
// 3. LA PAGE WEB (STOCKÉE DANS LA MÉMOIRE FLASH - PROGMEM)
// =========================================================================
// L'utilisation de PROGMEM permet de stocker cette longue chaîne de caractères directement 
// dans la mémoire Flash de l'ESP32 au lieu d'encombrer la mémoire RAM (SRAM) beaucoup plus limitée.
const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Mon Dashboard ESP32</title>
    <style>
        body {
            background: linear-gradient(to bottom right, #e2f8e9, #b3d4ff);
            font-family: Arial, sans-serif;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
            margin: 0;
            padding: 20px;
            box-sizing: border-box;
        }
        .dashboard {
            display: flex;
            flex-direction: column;
            gap: 20px;
            max-width: 800px;
        }
        .top-section {
            display: flex;
            flex-direction: row;
            gap: 20px;
            flex-wrap: nowrap;
            justify-content: center;
        }
        .panel {
            background-color: #3bb3ff;
            border-radius: 20px;
            padding: 20px;
            box-shadow: 0px 8px 15px rgba(0, 0, 0, 0.1);
        }
        .button-row {
            display: flex;
            gap: 15px;
            margin-bottom: 10px;
        }
        .button-row:last-child {
            margin-bottom: 0;
        }
        .btn {
            border: none;
            color: white;
            padding: 10px 20px;
            font-size: 16px;
            border-radius: 10px;
            cursor: pointer;
            width: 120px;
            font-weight: bold;
            display: flex;
            flex-direction: column;
            align-items: center;
            transition: 0.2s;
        }
        .btn:active {
            transform: scale(0.95);
        }
        .btn-on {
            background-color: #81d742;
        }
        .btn-off {
            background-color: #ff0000;
        }
        .sensor-panel {
            display: flex;
            align-items: center;
            justify-content: space-around;
            gap: 20px;
            min-width: 250px;
        }
        .sensor-display {
            display: flex;
            flex-direction: column;
            gap: 15px;
        }
        .sensor-value {
            background-color: #2c2c2c;
            color: white;
            padding: 10px 20px;
            font-size: 22px;
            border-radius: 5px;
            font-weight: bold;
        }
        .activity-title {
            color: white;
            text-align: center;
            margin: 0 0 10px 0;
            font-size: 18px;
        }
        .activity-log {
            background-color: white;
            color: black;
            height: 120px;
            overflow-y: auto;
            padding: 10px;
            border-radius: 5px;
            font-family: monospace;
            font-size: 14px;
            line-height: 1.5;
        }
        @media (max-width: 600px) {
            .top-section {
                flex-direction: column;
                gap: 10px;
            }
            .dashboard {
                padding: 10px;
            }
            .btn {
                width: 100%;
                padding: 15px;
                font-size: 18px;
            }
            .sensor-panel {
                flex-direction: column;
                gap: 10px;
            }
            .activity-log {
                height: 100px;
            }
            /* Cible la zone de signature qui sera située tout en bas de la page.
           Comme le body utilise "display: flex" et "flex-direction: column", 
           cet élément viendra s'empiler naturellement sous votre tableau de bord.
        */
        .signature {
            /* Centre le texte horizontalement pour un rendu esthétique et symétrique */
            text-align: center; 
            /* Utilise un gris très foncé (#2c2c2c) pour assurer un bon contraste sur le fond bleu clair, tout en étant moins agressif qu'un noir pur (#000000) */
            color: #2c2c2c; 
            /* Définit une taille de police de 14px, légèrement plus petite que le reste pour indiquer qu'il s'agit d'une information secondaire (crédits) */
            font-size: 14px; 
            /* Force un espace vide de 30 pixels au-dessus de la signature pour la séparer nettement des panneaux de contrôle de l'ESP32 */
            margin-top: 30px; 
            /* Ajoute un espace interne de 10 pixels pour éviter que le texte ne touche les bords de l'écran sur les très petits téléphones */
            padding: 10px; 
            /* Incline légèrement le texte (italique) pour appuyer visuellement le fait que c'est une signature ou une mention légale */
            font-style: italic; 
        }

        /*
           Cible spécifiquement un lien hypertexte (balise <a>) s'il y en a un à l'intérieur de la div ".signature" (ex: lien vers votre GitHub).
        */
        .signature a {
            /* Applique une couleur bleue (#0056b3) typique des liens cliquables pour indiquer l'interactivité à l'utilisateur */
            color: #0056b3; 
            /* Supprime la ligne de soulignement par défaut imposée par les navigateurs pour obtenir un design plus épuré et moderne */
            text-decoration: none; 
            /* Rend le texte du lien plus épais (gras) pour le faire ressortir subtilement par rapport au reste de la phrase */
            font-weight: bold; 
            /* Ajoute une transition fluide de 0.3 seconde pour que le changement d'état au survol de la souris ne soit pas brutal */
            transition: color 0.3s ease;
        }

        /*
           Définit le comportement visuel du lien exclusivement lorsque l'utilisateur passe le curseur de sa souris dessus (état pseudo-classe :hover).
        */
        .signature a:hover {
            /* Réactive le soulignement uniquement au survol pour confirmer visuellement à l'utilisateur que l'élément est bien cliquable */
            text-decoration: underline; 
            /* Assombrit légèrement le bleu au survol pour accentuer l'effet interactif */
            color: #003d82;
        }

        }
        

    </style>
</head>
<body>
    <div class="dashboard">
        <div class="top-section">
            <div class="panel">
                <div class="button-row">
                    <button class="btn btn-on" onclick="actionLampe(1, 'ON')"><span>Lampe1</span><span>ON</span></button>
                    <button class="btn btn-off" onclick="actionLampe(1, 'OFF')"><span>Lampe1</span><span>OFF</span></button>
                </div>
                <div class="button-row">
                    <button class="btn btn-on" onclick="actionLampe(2, 'ON')"><span>Lampe2</span><span>ON</span></button>
                    <button class="btn btn-off" onclick="actionLampe(2, 'OFF')"><span>Lampe2</span><span>OFF</span></button>
                </div>
                <div class="button-row">
                    <button class="btn btn-on" onclick="actionLampe(3, 'ON')"><span>Lampe3</span><span>ON</span></button>
                    <button class="btn btn-off" onclick="actionLampe(3, 'OFF')"><span>Lampe3</span><span>OFF</span></button>
                </div>
                <div class="button-row">
                    <button class="btn btn-on" onclick="actionLampe(4, 'ON')"><span>Lampe4</span><span>ON</span></button>
                    <button class="btn btn-off" onclick="actionLampe(4, 'OFF')"><span>Lampe4</span><span>OFF</span></button>
                </div>
            </div>
            <div class="panel sensor-panel">
                <svg width="80" height="120" viewBox="0 0 100 150" fill="none" xmlns="http://www.w3.org/2000/svg">
                    <path d="M50 10C42 10 35 17 35 25V90C25 96 20 107 22 119C25 133 37 142 50 142C63 142 75 133 78 119C80 107 75 96 65 90V25C65 17 58 10 50 10Z" stroke="#333" stroke-width="6" fill="#e6f2ff"/>
                    <circle cx="50" cy="115" r="15" fill="#38b6ff" stroke="#333" stroke-width="4"/>
                    <rect x="44" y="40" width="12" height="65" fill="#38b6ff"/>
                    <line x1="25" y1="30" x2="35" y2="30" stroke="#333" stroke-width="4" stroke-linecap="round"/>
                    <line x1="20" y1="50" x2="35" y2="50" stroke="#333" stroke-width="4" stroke-linecap="round"/>
                    <line x1="25" y1="70" x2="35" y2="70" stroke="#333" stroke-width="4" stroke-linecap="round"/>
                </svg>
                <div class="sensor-display">
                    <div class="sensor-value" id="valeur-temp">TEM: --°C</div>
                    <div class="sensor-value" id="valeur-hum">HUM: --%</div>
                </div>
            </div>
        </div>
        <div class="panel">
            <h3 class="activity-title">Activité</h3>
            <div class="activity-log" id="log-box"></div>
        </div>
    </div>
    <div class="signature">
        Dashboard ESP32 &copy; 2026 - Conçu par 
        <a href="https://github.com/Tinite-22 target="_blank">Low_Cost_Ingenieur</a>
    </div>

    <script>
        function actionLampe(numeroLampe, etat) {
            const texteAction = "-Lampe" + numeroLampe + ": " + etat;
            const boiteActivite = document.getElementById("log-box");
            
            boiteActivite.innerHTML += texteAction + "<br>";
            boiteActivite.scrollTop = boiteActivite.scrollHeight;

            fetch('/api/lampe?id=' + numeroLampe + '&etat=' + etat)
                .then(response => {
                    if(!response.ok) {
                        boiteActivite.innerHTML += "<i>Erreur : L'ESP32 n'a pas répondu correctement.</i><br>";
                    }
                })
                .catch(error => {
                    boiteActivite.innerHTML += "<i>Erreur réseau : Connexion perdue.</i><br>";
                });
        }

        function recupererCapteurs() {
            fetch('/api/capteurs')
                .then(response => response.json())
                .then(data => {
                    if(data.erreur) {
                        console.error("Problème de lecture du capteur DHT.");
                        return;
                    }
                    document.getElementById("valeur-temp").innerText = "TEM: " + data.temperature + "°C";
                    document.getElementById("valeur-hum").innerText = "HUM: " + data.humidite + "%";
                })
                .catch(err => console.error("Erreur de récupération des données :", err));
        }

        setInterval(recupererCapteurs, 5000);
        recupererCapteurs();
    </script>
</body>
</html>
)=====";

// =========================================================================
// 4. LES ROUTES DU SERVEUR WEB (Traitement des requêtes entrantes)
// =========================================================================

// Fonction appelée quand l'utilisateur tape l'adresse IP de l'ESP32 (Route racine "/").
void handleRoot() {
  // Le serveur répond avec un code de succès (200), spécifie qu'il envoie du format HTML, 
  // et transmet l'intégralité de la chaîne 'index_html' stockée dans la mémoire Flash.
  server.send(200, "text/html", index_html);
}

// Fonction appelée quand le JavaScript exécute : fetch('/api/lampe?id=X&etat=Y')
void handleLampe() {
  // Validation de sécurité : Vérifie que l'URL contient bien les deux paramètres obligatoires "id" et "etat".
  if (server.hasArg("id") && server.hasArg("etat")) {
    
    // Extraction et conversion du numéro de lampe transmis dans l'URL (String vers Entier).
    int idLampe = server.arg("id").toInt();
    // Extraction de la commande d'état ("ON" ou "OFF").
    String etat = server.arg("etat");
    
    // Ajustement de l'index : Le HTML demande les lampes 1 à 4, mais le tableau C++ 'brochesRelais' est indexé de 0 à 3.
    int indexTableau = idLampe - 1;

    // Protection contre les débordements de mémoire : Vérifie que l'index demandé correspond bien à une lampe existante.
    if (indexTableau >= 0 && indexTableau < 4) {
      
      // Logique d'activation des relais.
      // Attention : De nombreux modules relais s'activent lorsqu'ils reçoivent un état BAS (LOW) et se coupent sur HAUT (HIGH).
      if (etat == "ON") {
        digitalWrite(brochesRelais[indexTableau], LOW); // Allume la lampe.
      } else if (etat == "OFF") {
        digitalWrite(brochesRelais[indexTableau], HIGH); // Éteint la lampe.
      }
      
      // Confirme au JavaScript que l'ordre matériel a bien été exécuté.
      server.send(200, "text/plain", "Commande executee");
    } else {
      // Renvoie une erreur HTTP 400 (Bad Request) si le numéro de lampe est en dehors de la plage 1-4.
      server.send(400, "text/plain", "Erreur : Numéro de lampe invalide");
    }
  } else {
    // Renvoie une erreur HTTP 400 si l'URL est mal formatée.
    server.send(400, "text/plain", "Erreur : Paramètres manquants");
  }
}

// Fonction appelée quand le JavaScript exécute : fetch('/api/capteurs')
void handleCapteurs() {
  // Interroge physiquement le composant DHT22 pour obtenir les mesures.
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // "isnan" (Is Not A Number) vérifie si la lecture matérielle a échoué (ex: mauvais câblage, interférence).
  if (isnan(h) || isnan(t)) {
    // Formate une réponse d'erreur au format JSON pour que le JavaScript la comprenne et ne plante pas.
    server.send(500, "application/json", "{\"erreur\": true}");
    return; // Stoppe l'exécution de la fonction ici.
  }

  // Construction manuelle de la chaîne JSON pour empaqueter les deux valeurs de manière structurée.
  // Le résultat final ressemblera exactement à ceci : {"temperature": 25.50, "humidite": 60.20}
  String json = "{";
  json += "\"temperature\": " + String(t) + ",";
  json += "\"humidite\": " + String(h);
  json += "}";

  // Expédie le paquet de données au navigateur avec le type MIME correct pour du JSON.
  server.send(200, "application/json", json);
}

// =========================================================================
// 5. INITIALISATION (Exécutée une seule fois au démarrage de la carte)
// =========================================================================
void setup() {
  // Ouvre le port série de l'ordinateur pour lire les messages de débogage (vitesse de 115200 bauds).
  Serial.begin(115200);
  delay(100);

  // Configuration initiale systématique des broches reliées aux relais.
  for (int i = 0; i < 4; i++) {
    // Définit la broche comme une SORTIE de courant.
    pinMode(brochesRelais[i], OUTPUT);
    // Force un état HIGH immédiat pour s'assurer que toutes les lampes sont ÉTEINTES au démarrage (logique inversée).
    digitalWrite(brochesRelais[i], HIGH); 
  }

  // Démarrage de la bibliothèque gérant le composant physique DHT22.
  dht.begin();
  Serial.println("Capteur environnemental initialisé.");

  // Configuration du réseau Wi-Fi de l'ESP32.
  // WIFI_AP signifie "Access Point" (L'ESP32 crée son propre routeur interne, sans avoir besoin de ta box internet locale).
  WiFi.mode(WIFI_AP); 
  
  // Démarre la diffusion du réseau sans-fil en utilisant le nom et le mot de passe définis en haut du code.
  WiFi.softAP(ssidAP, passwordAP);

  // Affichage des informations de connexion dans le Moniteur Série pour l'utilisateur.
  Serial.println("");
  Serial.println("Réseau Wi-Fi local créé avec succès !");
  Serial.print("Recherchez le SSID : ");
  Serial.println(ssidAP);
  
  // Affiche l'adresse IP fixe attribuée par défaut à l'ESP32 en mode AP (généralement 192.168.4.1).
  Serial.print("Ouvrez le navigateur sur cette adresse locale : http://");
  Serial.println(WiFi.softAPIP());

  // Cartographie des requêtes web : "Quand le navigateur demande l'adresse X, exécute la fonction C++ Y".
  server.on("/", handleRoot);              // Route pour l'affichage de la page web.
  server.on("/api/lampe", handleLampe);    // Route d'API pour la commande des relais.
  server.on("/api/capteurs", handleCapteurs); // Route d'API pour l'envoi des données DHT22.

  // Démarre formellement le composant serveur web pour le mettre à l'écoute des requêtes HTTP.
  server.begin();
  Serial.println("Le Serveur Web est actif et en attente d'instructions !");
}

// =========================================================================
// 6. BOUCLE PRINCIPALE (Tourne à l'infini et à très haute fréquence)
// =========================================================================
void loop() {
  // Demande continuellement au processeur réseau de vérifier si un nouvel appareil (comme ton smartphone)
  // s'est connecté ou a envoyé une nouvelle requête HTTP. C'est le moteur central du serveur.
  // Address IP: http://192.168.4.1/
  server.handleClient();
}