# 🧾 Informe del Proyecto final

**Materia:** Informática II  
**Universidad:** Universidad de Antioquia  
**Estudiante:** Diego Steven Ramírez Molina   
**Profesores:** Aníbal Guerra y Augusto Salazar  
**Semestre:** 2025-1   

---

##  1. Introducción
Este proyecto corresponde al desarrollo de un videojuego como entrega final del curso de Informática II de la Universidad de Antioquia, semestre 2025-1. 

El videojuego fue desarrollado en C++ utilizando Qt Creator, con énfasis en el uso de programación orientada a objetos, gráficos mediante `QGraphicsView` y `QGraphicsScene`, manejo de recursos dinámicos, animaciones y sonido.

El objetivo principal del proyecto era aplicar de forma práctica los conocimientos adquiridos durante el semestre en torno a diseño modular, estructuras dinámicas, contenedores, herencia, e interacción gráfica, culminando en un producto final jugable y evaluable.


##  Inspiración, contexto y análisis del diseño

El videojuego  fue desarrollado tomando como base los capítulos 11 y 12 de la Saga del Emperador Pilaf. En esta parte de la historia, Goku y sus amigos quedan atrapados en el castillo del Rey Pilaf, quien intenta robar las esferas del dragón para cumplir su deseo de dominar el mundo. En estos estos episodios, los protagonistas enfrentan trampas, paredes móviles, gases, cámaras selladas y finalmente un combate directo contra Pilaf en su robot de batalla.

Estos capitulos fueron ideales para la construcción del videojuego con **dos niveles complementarios**: uno enfocado en la **exploración** y el otro en un **combate frontal** que exige decisiones tácticas. Ambos niveles tienen **dinámicas distintas**, con físicas propias, diseños diferentes.

### 🎮 Descripción general del videojuego

El juego consta de dos niveles:

- **Nivel 1**: Goku avanza por una habitación llena de trampas generadas aleatoriamente (ácido, flechas, estacas y pisos falsos). El jugador debe esquivarlas en tiempo real, mientras una barra de progreso indica el avance. Al generarse cierta cantidad de trampas, se desbloquea la segunda habitación, donde el jugador debe sobrevivir a una pared móvil por 40 segundos.

- **Nivel 2**: Combate frontal contra Pilaf. El jugador ya no se mueve, pero puede lanzar ataques (puño, patada, bola de energía) o curarse. Pilaf lanza misiles parabólicos. Se trata de un sistema de combate que evalúa reflejos y estrategia de ataque/defensa.

### 🧩 Análisis del diseño

El diseño se centró en los siguientes objetivos:

- **Abstracción de dificultad**: El juego fue pensado para ser fácil de jugar pero desafiante de superar. El primer nivel tiene trampas aleatorias que requieren reacción rápida. El segundo nivel plantea un combate donde el jugador debe decidir entre atacar o curarse, mientras esquiva los misiles, aumentando la dificultad entre los 2 niveles.

- **Diferenciación entre niveles**: Para cumplir con el criterio de variedad, se desarrollaron dos niveles con mecánicas totalmente distintas: el primero en vista lateral con movimiento libre y obstáculos, el segundo en vista estática con ataques por turnos simulados.

- **Claridad visual**: Cada nivel tiene un fondo distinto, barra de vida visible, animaciones de daño, muerte o victoria y coherencia.

- **físicas**: Se incluyeron tres modelos físicos :  Movimiento parabólico (salto de Goku y misiles ), movimiento oscilatorio (flechas) y movimiento con aceleración (Pared). Todas fueron aplicadas con parámetros controlables.

- **Interfaz gráfica amigable**: Se usó `QGraphicsView`, `QGraphicsPixmapItem`, `QTimer`, `QProgressBar`, y `QStackedWidget` para gestionar los niveles, el menú y las vistas de victoria/derrota.

El desarrollo del videojuego fue orientado por principios de programación orientada a objetos, evitando duplicación de código y reutilizando clases mediante herencia (`Personaje`, `Trampa`, `Nivel`), lo cual permitió un diseño más limpio y modular.

