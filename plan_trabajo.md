# Plan de Trabajo
## Aplicación de Métodos Numéricos — Unidad 2

**Estudiante:** Carrillo Valencia Ruth
**Grupo:** AM3B — Ingeniería Mecatrónica
**Periodo:** Agosto–Diciembre 2026
**Lenguaje:** C++

Este plan organiza, en fases y tareas concretas, el desarrollo del proyecto a partir de los documentos ya elaborados (`diseño_sistema.md` y `arquitectura_software_carpetas.md`). Las fechas se expresan en semanas relativas al inicio del desarrollo; deben ajustarse a la fecha límite real que indique el docente.

---

## 1. Objetivo de la planeación

Secuenciar el trabajo de modo que:
- cada fase produzca un artefacto verificable (código que compila, módulo probado, sección de informe redactada);
- los seis métodos numéricos y el caso de raíces múltiples queden implementados y probados antes de iniciar la integración final;
- quede tiempo dedicado específicamente a robustez/validación, documentación y video, sin empalmarse con la programación.

## 2. Metodología de trabajo

Se seguirá un desarrollo **incremental por capas**, respetando el orden de dependencias definido en la arquitectura (`arquitectura_software_carpetas.md`, sección 1):

1. Primero el **núcleo de datos** (`core/`), porque todo lo demás depende de él.
2. Después **validación** (`validacion/`), ya que los métodos numéricos la usan antes de ejecutar cualquier cálculo.
3. Luego los **métodos numéricos** (`metodos/`), uno a la vez, de menor a mayor complejidad: Bisección y Falsa Posición (intervalos) → Punto Fijo → Newton-Raphson → Secante → Newton-Raphson Modificado.
4. Enseguida la **graficación** (`grafica/`), que depende solo de `Funcion`.
5. Finalmente la **interfaz de usuario** (`ui/`) y el **controlador** (`control/`), que integran todo lo anterior.

Cada método numérico se considera terminado solo cuando pasa sus pruebas unitarias con al menos dos de las funciones de prueba asignadas (ver `arquitectura_software_carpetas.md`, sección 4).

## 3. Fases y actividades

### Fase 0 — Análisis y diseño (completada)
- Elaboración de `diseño_sistema.md`: arquitectura, clases, casos de uso, validación.
- Elaboración de `arquitectura_software_carpetas.md`: estructura física del proyecto.
- **Entregable:** ambos documentos de diseño.

### Fase 1 — Configuración del proyecto
- Crear la estructura de carpetas definida (`include/`, `src/`, `tests/`, `docs/`, `recursos/`, `evidencias/`).
- Configurar `CMakeLists.txt` y verificar la compilación de un "hola mundo" con la biblioteca gráfica elegida (Qt o SFML).
- Configurar el framework de pruebas unitarias.
- **Entregable:** proyecto vacío que compila y ejecuta una ventana básica.

### Fase 2 — Núcleo de datos (`core/`)
- Implementar `Funcion` (parser/evaluador de expresiones, detección de dominio).
- Implementar `ParametrosMetodo`, `IteracionResultado`, `ResultadoFinal`, `ErrorValidacion`.
- Probar `Funcion` con las cinco familias de funciones del enunciado (polinomial, racional, exponencial, logarítmica, trigonométrica).
- **Entregable:** evaluación correcta de f(x) para todas las funciones de prueba, incluyendo casos fuera de dominio.

### Fase 3 — Validación y manejo de errores (`validacion/`)
- Implementar `Validador` con las reglas por método (cambio de signo, tolerancia positiva, iteraciones máximas válidas, x0 ≠ x1, etc.).
- Implementar `GestorErrores` y el catálogo de mensajes comprensibles.
- Probar cada regla con datos inválidos (tabla de la sección 12 del diseño).
- **Entregable:** módulo de validación cubriendo todos los casos de la tabla de errores.

### Fase 4 — Métodos de intervalo
- Implementar `MetodoNumerico` (interfaz) y `FabricaDeMetodos`.
- Implementar `Biseccion` y `FalsaPosicion`.
- Pruebas con funciones polinomial y logarítmica (respetando dominio).
- **Entregable:** ambos métodos generan tabla de iteraciones y raíz correcta, verificados contra valores de referencia.

### Fase 5 — Métodos abiertos
- Implementar `PuntoFijo` (incluyendo detección de divergencia).
- Implementar `NewtonRaphson`.
- Implementar `Secante`.
- Pruebas con funciones exponencial y trigonométrica.
- **Entregable:** tres métodos funcionando, con manejo de derivada nula y de x0 = x1.

### Fase 6 — Raíces múltiples
- Implementar `NewtonRaphsonModificado`.
- Prueba con función combinada/trascendente que presente raíz múltiple (o construida para el caso).
- **Entregable:** método modificado recuperando convergencia cuadrática frente al Newton-Raphson estándar.

### Fase 7 — Graficación integrada (`grafica/`)
- Implementar `MotorGrafico` y `DatosGrafica`: muestreo de f(x), trazo de ejes/curva, marcador de la raíz.
- Integrar el método gráfico (`MetodoGrafico`) como modo de exploración de intervalos.
- **Entregable:** gráfica embebida en la aplicación, sin ventanas ni programas externos.

### Fase 8 — Interfaz de usuario y controlador (`ui/`, `control/`)
- Implementar `ControladorApp`: flujo completo entrada → validación → cálculo → resultados → gráfica.
- Implementar `VentanaPrincipal`, `PanelParametros` (dinámico por método), `PanelResultados`, `PanelGrafica`.
- Implementar el flujo de "nuevo cálculo" sin cerrar la aplicación.
- **Entregable:** aplicación integrada de punta a punta, usable con cualquier método desde la interfaz.

### Fase 9 — Pruebas integrales y robustez
- Ejecutar la aplicación completa con todas las funciones de prueba del enunciado (sección 16 del diseño) y registrar evidencias en `evidencias/`.
- Probar deliberadamente entradas inválidas: expresiones mal escritas, intervalos sin cambio de signo, tolerancias negativas, iteraciones máximas inválidas, valores fuera de dominio, divisiones entre cero.
- Corregir defectos encontrados.
- **Entregable:** evidencias completas por familia de función y por tipo de error, aplicación estable.

### Fase 10 — Informe técnico
- Redactar el informe siguiendo el índice de la sección 6 del enunciado original, usando como base el contenido de `diseño_sistema.md` y las evidencias de la Fase 9.
- Incluir capturas de la tabla de iteraciones, la gráfica integrada y los mensajes de error.
- **Entregable:** informe técnico/documental completo.

### Fase 11 — Video de presentación
- Guion del video siguiendo la lista de la sección 7 del enunciado (problema, métodos, herramientas, demostración en vivo, validaciones, dificultades, conclusiones).
- Grabación mostrando la aplicación funcionando (no solo diapositivas).
- Edición y carga del video; inclusión de la liga en el informe.
- **Entregable:** video final con liga incluida en el informe.

### Fase 12 — Revisión final y entrega
- Verificar los criterios de calidad del producto (tabla de la sección 8 del enunciado): funcionalidad, validación, interfaz, gráfica, pruebas, informe, video, profesionalismo.
- Empaquetar entregables: aplicación, informe, video, evidencias.
- **Entregable:** proyecto completo entregado.

## 4. Cronograma propuesto (semanas relativas)

| Fase | Semana 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|---|---|---|---|---|---|---|---|---|---|---|
| 0. Análisis y diseño | ✔ | | | | | | | | | |
| 1. Configuración del proyecto | ✔ | | | | | | | | | |
| 2. Núcleo de datos | | ✔ | | | | | | | | |
| 3. Validación y errores | | ✔ | ✔ | | | | | | | |
| 4. Métodos de intervalo | | | ✔ | | | | | | | |
| 5. Métodos abiertos | | | | ✔ | ✔ | | | | | |
| 6. Raíces múltiples | | | | | ✔ | | | | | |
| 7. Graficación integrada | | | | | | ✔ | | | | |
| 8. UI y controlador | | | | | | ✔ | ✔ | | | |
| 9. Pruebas integrales y robustez | | | | | | | ✔ | ✔ | | |
| 10. Informe técnico | | | | | | | | ✔ | ✔ | |
| 11. Video de presentación | | | | | | | | | ✔ | ✔ |
| 12. Revisión final y entrega | | | | | | | | | | ✔ |

> Las semanas son relativas al inicio real del desarrollo; deben alinearse con la fecha límite de entrega definida por el docente en el periodo Agosto–Diciembre 2026.

## 5. Hitos (entregables parciales verificables)

| Hito | Semana | Criterio de cumplimiento |
|---|---|---|
| H1 — Esqueleto compilando | 1 | Ventana vacía ejecutándose desde el binario generado por CMake |
| H2 — Núcleo validado | 2 | `Funcion` evalúa correctamente las 5 familias de funciones de prueba |
| H3 — Validación completa | 3 | Todos los errores de la tabla de validación son detectados |
| H4 — Métodos de intervalo listos | 3 | Bisección y Falsa Posición entregan raíz correcta con tabla de iteraciones |
| H5 — Métodos abiertos listos | 5 | Punto Fijo, Newton-Raphson y Secante funcionando |
| H6 — Raíces múltiples resuelto | 5 | Newton-Raphson Modificado mejora la convergencia frente al estándar |
| H7 — Gráfica integrada | 6 | f(x) y la raíz se visualizan dentro de la misma ventana |
| H8 — Aplicación integrada | 7 | Flujo completo operable desde la interfaz, incluyendo "nuevo cálculo" |
| H9 — Evidencias completas | 8 | Capturas de todas las familias de funciones y de los casos de error |
| H10 — Informe terminado | 9 | Documento completo según el índice del enunciado |
| H11 — Video terminado | 10 | Video publicado y liga verificada en el informe |
| H12 — Entrega final | 10 | Todos los entregables empaquetados y revisados contra la tabla de criterios de calidad |

## 6. Gestión de riesgos

| Riesgo | Impacto | Mitigación |
|---|---|---|
| Dificultad para implementar el parser de expresiones matemáticas | Alto, bloquea todo lo demás | Resolverlo en la Fase 2 antes de tocar cualquier método; si se complica, acotar la sintaxis admitida y documentarlo |
| Divergencia no controlada en Punto Fijo | Medio | Incluir un límite de iteraciones y detección de crecimiento del error desde el diseño de la clase |
| Integración tardía de la gráfica | Medio | Desarrollar `MotorGrafico` como módulo independiente desde la Fase 7, probable antes de la UI completa |
| Subestimar el tiempo del informe y el video | Alto, son entregables obligatorios | Reservar las Fases 10 y 11 exclusivamente para documentación/video, sin tareas de programación en paralelo |
| Pruebas insuficientes con funciones de raíz múltiple | Medio | Diseñar deliberadamente un caso de prueba con raíz múltiple conocida (ej. f(x) = (x−2)²(x+1)) desde la Fase 6 |

## 7. Relación con los documentos previos

- Las fases 2 a 8 implementan, en orden, los componentes definidos en `diseño_sistema.md` (secciones 8 y 9) y ubicados físicamente según `arquitectura_software_carpetas.md` (secciones 2 y 3).
- La Fase 9 ejecuta el plan de pruebas de la sección 16 de `diseño_sistema.md`.
- La Fase 12 verifica el cumplimiento frente a la tabla de criterios de calidad del enunciado original (`trabajo final 2.md`, sección 8).

---

**Fin del plan de trabajo.**
