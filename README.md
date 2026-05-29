# 🏠 Sistema de Domótica IoT — ESP32 + MQTT

**Universidad Don Bosco**  
Proyecto Fase 2 — Sistemas Embebidos / IoT  

---

## 👥 Integrantes del equipo

| Nombre | Responsabilidad |
|--------|----------------|
| Marco Rodrigo Funes Bonilla | Sensores + lectura de datos |
| Hazel Alexia García Vásquez | Actuadores + control |
| Luis Alexander Arteaga Sigüenza | Comunicación MQTT |
| Joel Alexander Flores Hernández | Dashboard de monitoreo y control |
| Gabriel Alejandro Diaz Valencia | Integración final + evidencias/Git |

---

## 📋 Descripción del proyecto

Sistema de domótica simulado en ESP32 que integra **5 sensores** y **5 actuadores** con comunicación **MQTT** hacia un dashboard web de monitoreo y control en tiempo real.

---

## 🔧 Hardware simulado

### Sensores
| Sensor | Pin GPIO | Función |
|--------|----------|---------|
| DHT22 | GPIO 16 | Temperatura y humedad |
| HC-SR04 | GPIO 12 (TRIG) / 15 (ECHO) | Distancia — control de acceso vehicular |
| PIR | GPIO 27 | Detección de presencia/intrusos |
| LDR (fotoresistor) | GPIO 34 | Nivel de iluminación |
| Potenciómetro | GPIO 35 | Simulación sensor de gas |

### Actuadores
| Actuador | Pin GPIO | Función |
|----------|----------|---------|
| Servo Motor | GPIO 4 | Portón de acceso vehicular |
| Relay | GPIO 25 | Control de ventilador |
| LED Azul | GPIO 14 | Sistema de iluminación |
| LED Rojo | GPIO 18 | Alerta visual de fuga de gas |
| Buzzer | GPIO 13 | Alarma sonora de intrusos |

---

## 📡 Tópicos MQTT

**Broker público:** `broker.hivemq.com`

### Publicación (ESP32 → Dashboard)
| Tópico | Descripción |
|--------|-------------|
| `udb/domotica/sensor/temperatura` | Temperatura en °C |
| `udb/domotica/sensor/humedad` | Humedad relativa % |
| `udb/domotica/sensor/luz` | Nivel de luz (0–4095) |
| `udb/domotica/sensor/distancia` | Distancia en cm |
| `udb/domotica/sensor/presencia` | Movimiento detectado (0/1) |
| `udb/domotica/sensor/gas` | Nivel de gas (0–4095) |

### Suscripción (Dashboard → ESP32)
| Tópico | Valores | Efecto |
|--------|---------|--------|
| `udb/domotica/control/ventilador` | `ON` / `OFF` | Enciende/apaga relay |
| `udb/domotica/control/luces` | `ON` / `OFF` | Enciende/apaga LED azul |
| `udb/domotica/control/alerta` | `ON` / `OFF` | Enciende/apaga LED rojo |
| `udb/domotica/control/buzzer` | `ON` | Activa pulso de alarma |
| `udb/domotica/control/servo` | `ABRIR` / `CERRAR` | Mueve el portón |

---

## 🚀 Cómo correr el proyecto

### Requisitos previos
- [VS Code](https://code.visualstudio.com/)
- Extensión [Wokwi for VS Code](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode)
- Licencia gratuita de Wokwi activada (se activa en `F1 → Wokwi: Request a Free License`)
- Navegador web moderno (Chrome, Firefox, Edge)

---

### Paso 1 — Clonar el repositorio

```bash
git clone https://github.com/TU_USUARIO/TU_REPOSITORIO.git
cd TU_REPOSITORIO
```

---

### Paso 2 — Abrir en VS Code

```bash
code .
```

---

### Paso 3 — Iniciar la simulación en Wokwi

1. Abre el archivo `diagram.json` en VS Code
2. Presiona `F1` y escribe:
   ```
   Wokwi: Start Simulator
   ```
3. Se abrirá el simulador del ESP32 dentro de VS Code
4. Espera a que aparezca en el Serial Monitor:
   ```
   ¡WiFi Conectado!
   ¡Conectado al Broker!
   ```
   Esto confirma que el ESP32 está enviando datos por MQTT ✅

---

### Paso 4 — Abrir el Dashboard

1. Abre el archivo `dashboard.html` en tu navegador  
   *(doble clic sobre el archivo, o clic derecho → Abrir con → navegador)*
2. En el campo **Broker** debe decir `broker.hivemq.com` y puerto `8884`
3. Haz clic en **Conectar**
4. En pocos segundos verás los datos del simulador aparecer en tiempo real 🎉

> ⚠️ **Importante:** La simulación de Wokwi debe estar corriendo **antes** de conectar el dashboard, para que haya datos fluyendo por MQTT.

---

### Paso 5 — Interactuar con la simulación

| Acción en Wokwi | Efecto en el Dashboard |
|----------------|----------------------|
| Girar el **potenciómetro** | Sube el nivel de gas — pasa de 2500 activa alerta |
| Activar el **sensor PIR** (clic) | Detecta movimiento, activa buzzer |
| Tapar el **LDR** (clic) | Oscurece el ambiente, enciende LED azul |
| Acercar objeto al **HC-SR04** | Abre el portón automáticamente |
| Subir temperatura en **DHT22** | Al pasar 30°C activa el ventilador |

---

## 📁 Estructura del repositorio

```
📦 proyecto-domotica/
├── 📄 sketch.ino       # Código fuente del ESP32
├── 📄 diagram.json     # Circuito Wokwi (componentes y conexiones)
├── 📄 wokwi.toml       # Configuración del simulador
├── 📄 dashboard.html   # Dashboard web MQTT
└── 📄 README.md        # Este archivo
```

---

## ⚙️ Automatizaciones del sistema

| Condición | Acción automática |
|-----------|------------------|
| Temperatura > 30°C | Enciende ventilador (relay) |
| Oscuridad (LDR > 2000) | Enciende luces (LED azul) |
| Gas > 2500 | Activa alerta (LED rojo) |
| Movimiento detectado (PIR) | Activa buzzer 200ms |
| Objeto a < 20 cm (HC-SR04) | Abre portón (servo 90°) |

---

## 📊 Porcentaje de avance

| Módulo | Estado | % |
|--------|--------|---|
| Integración de hardware y programación | ✅ Completo | 100% |
| Comunicación MQTT | ✅ Completo | 100% |
| Dashboard de monitoreo y control | ✅ Completo | 100% |
| Simulación funcionando | ✅ Completo | 100% |
| Evidencias y repositorio Git | ✅ Completo | 100% |
| Presentación final | 🔲 Pendiente | — |

---

## 🛠️ Tecnologías utilizadas

- **ESP32** — microcontrolador principal
- **Wokwi** — simulador de circuitos IoT
- **MQTT** — protocolo de mensajería IoT
- **HiveMQ** — broker MQTT público
- **HTML / CSS / JavaScript** — dashboard web
- **Arduino C++** — programación del ESP32

---

*Universidad Don Bosco — Proyecto IoT Fase 2*
