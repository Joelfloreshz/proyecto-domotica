Sistema de Domótica IoT — ESP32 + MQTT
Universidad Don Bosco
Proyecto Fase 2 — Sistemas Embebidos / IoT

👥 Integrantes del equipo
NombreResponsabilidadMarco Rodrigo Funes BonillaSensores + lectura de datosHazel Alexia García VásquezActuadores + controlLuis Alexander Arteaga SigüenzaComunicación MQTTJoel Alexander Flores HernándezDashboard de monitoreo y controlGabriel Alejandro Diaz ValenciaIntegración final + evidencias/Git

📋 Descripción del proyecto
Sistema de domótica simulado en ESP32 que integra 5 sensores y 5 actuadores con comunicación MQTT hacia un dashboard web de monitoreo y control en tiempo real.

🔧 Hardware simulado
Sensores
SensorPin GPIOFunciónDHT22GPIO 16Temperatura y humedadHC-SR04GPIO 12 (TRIG) / 15 (ECHO)Distancia — control de acceso vehicularPIRGPIO 27Detección de presencia/intrusosLDR (fotoresistor)GPIO 34Nivel de iluminaciónPotenciómetroGPIO 35Simulación sensor de gas
Actuadores
ActuadorPin GPIOFunciónServo MotorGPIO 4Portón de acceso vehicularRelayGPIO 25Control de ventiladorLED AzulGPIO 14Sistema de iluminaciónLED RojoGPIO 18Alerta visual de fuga de gasBuzzerGPIO 13Alarma sonora de intrusos

📡 Tópicos MQTT
Broker público: broker.hivemq.com
Publicación (ESP32 → Dashboard)
TópicoDescripciónudb/domotica/sensor/temperaturaTemperatura en °Cudb/domotica/sensor/humedadHumedad relativa %udb/domotica/sensor/luzNivel de luz (0–4095)udb/domotica/sensor/distanciaDistancia en cmudb/domotica/sensor/presenciaMovimiento detectado (0/1)udb/domotica/sensor/gasNivel de gas (0–4095)
Suscripción (Dashboard → ESP32)
TópicoValoresEfectoudb/domotica/control/ventiladorON / OFFEnciende/apaga relayudb/domotica/control/lucesON / OFFEnciende/apaga LED azuludb/domotica/control/alertaON / OFFEnciende/apaga LED rojoudb/domotica/control/buzzerONActiva pulso de alarmaudb/domotica/control/servoABRIR / CERRARMueve el portón

🚀 Cómo correr el proyecto
Requisitos previos

VS Code
Extensión Wokwi for VS Code
Licencia gratuita de Wokwi activada (se activa en F1 → Wokwi: Request a Free License)
Navegador web moderno (Chrome, Firefox, Edge)


Paso 1 — Clonar el repositorio
bashgit clone https://github.com/TU_USUARIO/TU_REPOSITORIO.git
cd TU_REPOSITORIO

Paso 2 — Abrir en VS Code
bashcode .

Paso 3 — Iniciar la simulación en Wokwi

Abre el archivo diagram.json en VS Code
Presiona F1 y escribe:

   Wokwi: Start Simulator

Se abrirá el simulador del ESP32 dentro de VS Code
Espera a que aparezca en el Serial Monitor:

   ¡WiFi Conectado!
   ¡Conectado al Broker!
Esto confirma que el ESP32 está enviando datos por MQTT ✅

Paso 4 — Abrir el Dashboard

Abre el archivo dashboard.html en tu navegador
(doble clic sobre el archivo, o clic derecho → Abrir con → navegador)
En el campo Broker debe decir broker.hivemq.com y puerto 8884
Haz clic en Conectar
En pocos segundos verás los datos del simulador aparecer en tiempo real 🎉


⚠️ Importante: La simulación de Wokwi debe estar corriendo antes de conectar el dashboard, para que haya datos fluyendo por MQTT.


Paso 5 — Interactuar con la simulación
Acción en WokwiEfecto en el DashboardGirar el potenciómetroSube el nivel de gas — pasa de 2500 activa alertaActivar el sensor PIR (clic)Detecta movimiento, activa buzzerTapar el LDR (clic)Oscurece el ambiente, enciende LED azulAcercar objeto al HC-SR04Abre el portón automáticamenteSubir temperatura en DHT22Al pasar 30°C activa el ventilador

📁 Estructura del repositorio
📦 proyecto-domotica/
├── 📄 sketch.ino       # Código fuente del ESP32
├── 📄 diagram.json     # Circuito Wokwi (componentes y conexiones)
├── 📄 wokwi.toml       # Configuración del simulador
├── 📄 dashboard.html   # Dashboard web MQTT
└── 📄 README.md        # Este archivo

⚙️ Automatizaciones del sistema
CondiciónAcción automáticaTemperatura > 30°CEnciende ventilador (relay)Oscuridad (LDR > 2000)Enciende luces (LED azul)Gas > 2500Activa alerta (LED rojo)Movimiento detectado (PIR)Activa buzzer 200msObjeto a < 20 cm (HC-SR04)Abre portón (servo 90°)

📊 Porcentaje de avance
MóduloEstado%Integración de hardware y programación✅ Completo100%Comunicación MQTT✅ Completo100%Dashboard de monitoreo y control✅ Completo100%Simulación funcionando✅ Completo100%Evidencias y repositorio Git✅ Completo100%Presentación final🔲 Pendiente—

🛠️ Tecnologías utilizadas

ESP32 — microcontrolador principal
Wokwi — simulador de circuitos IoT
MQTT — protocolo de mensajería IoT
HiveMQ — broker MQTT público
HTML / CSS / JavaScript — dashboard web
Arduino C++ — programación del ESP32


Universidad Don Bosco — Proyecto IoT Fase 2
