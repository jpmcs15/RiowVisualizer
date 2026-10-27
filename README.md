# Riow Visualizer VST3 — source prototype

Este proyecto es un **prototipo de código fuente**, no un instalador ya compilado. Incluye una interfaz monocromática, visualización de forma de onda/espectro y un teclado MIDI visual.

## Compatibilidad prevista
- Windows 10/11 de 64 bits
- Ableton Live 12 y FL Studio con soporte VST3
- JUCE 8.0.6
- CMake + Visual Studio 2022 (workload **Desktop development with C++**)

## Qué hace esta primera versión
- Pasa el audio de entrada a la salida sin alterarlo.
- Dibuja una forma de onda en estilo Retro Mono.
- Permite elegir Retro Mono, Y2K Grid y Blueprint (los dos últimos usan el espectro).
- Incluye un monitor de notas MIDI entrantes.

## Limitaciones conocidas
- El teclado solo se ilumina si el host realmente envía MIDI al plugin. Algunos DAW no enrutan MIDI a un efecto de audio de la manera esperada; puede requerirse una versión MIDI-effect/instrumento separada para ciertos flujos.
- La tonalidad y el BPM todavía no se detectan automáticamente.
- La exportación MP4 no se hace dentro del plugin: se añadirá en la aplicación complementaria.
- Esta entrega contiene el código fuente; aún debe compilarse y probarse en el equipo destino.

## Compilar en Windows
1. Instala Visual Studio 2022 y marca **Desktop development with C++**.
2. Instala CMake (o usa la versión incluida con Visual Studio).
3. Instala Git.
4. Abre una terminal en esta carpeta.
5. Ejecuta:

   ```powershell
   cmake -S . -B build -A x64
   cmake --build build --config Release
   ```

   CMake descargará JUCE desde GitHub durante la configuración, así que necesitarás conexión a Internet.

6. Busca `RiowVisualizer.vst3` dentro de la carpeta de salida de `build` y copia el bundle a:
   `C:\Program Files\Common Files\VST3\`
7. Reinicia Ableton Live o FL Studio y escanea los plugins.

## Próximos pasos recomendados
- Compilar y probar el plugin en Ableton/FL Studio.
- Añadir selección de tamaño de render y captura de frames.
- Crear la aplicación externa para cargar WAV + MIDI y exportar MP4 vertical 1080×1920.
- Añadir presets visuales y personalización de marca.
