# **INSTITUTO TECNOLÓGICO DE COLIMA MÉTODOS NUMÉRICOS** 

## **TRABAJO FINAL – UNIDAD 2** 

_Desarrollo y aplicación de métodos para la solución de ecuaciones no lineales_ 

GRUPO: AM3B Ingeniería Mecatrónica Número de estudiantes: 23 Periodo: AGO–DIC 2026 

### **1. Propósito del proyecto** 

El proyecto final de la Unidad 2 consiste en completar y profesionalizar la aplicación de Métodos Numéricos que el estudiante ya ha desarrollado, integrando los métodos estudiados para la solución de ecuaciones no lineales. El producto principal será una aplicación funcional, robusta, validada y fácil de utilizar, acompañada de un informe técnico y un video en el que se explique el desarrollo y funcionamiento. 

### **2. Métodos de la Unidad 2** 

La aplicación deberá contemplar los siguientes seis métodos: 

**1. Método gráfico.** Localización aproximada de las raíces mediante la representación de f(x) y la identificación de sus intersecciones con el eje x. 

**2. Método de Bisección.** Método de intervalos basado en dividir sucesivamente un intervalo [a,b] que contiene un cambio de signo. 

**3. Método de Falsa Posición.** Método de intervalos que utiliza una recta secante entre los extremos del intervalo para aproximar la raíz. 

**4. Método de Iteración de Punto Fijo.** Transformación de f(x)=0 a una forma x=g(x) y generación iterativa de aproximaciones. 

**5. Método de Newton-Raphson.** Método abierto que emplea la derivada de la función para generar aproximaciones sucesivas. 

**6. Método de la Secante.** Método abierto que aproxima la derivada mediante dos aproximaciones anteriores, sin requerir la derivada explícita. 

Además, el sistema deberá contemplar el tratamiento de raíces múltiples mediante el procedimiento de Newton-Raphson modificado, como parte del apartado de métodos para raíces múltiples. 

### **3. Requisitos funcionales de la aplicación** 

- Permitir que el usuario ingrese o seleccione una función f(x) dentro de la sintaxis admitida por el sistema. 

- Permitir seleccionar el método numérico que se desea ejecutar. 

- Solicitar los parámetros requeridos por cada método: intervalos, valores iniciales, tolerancia, número máximo de iteraciones u otros que correspondan. 

- Validar los datos antes de iniciar el cálculo. 

- Detectar intervalos inválidos, divisiones entre cero, argumentos fuera del dominio, derivadas no utilizables, valores no numéricos y demás situaciones que impidan el cálculo. 

- Mostrar el resultado de forma clara: raíz aproximada, número de iteraciones, error y criterio de paro. 

- Mostrar una tabla de iteraciones con los valores relevantes de cada método. 

- Graficar la función dentro de la propia aplicación. No se permitirá depender de GeoGebra, Desmos, Excel u otro programa externo para visualizar la gráfica. 

- Permitir repetir los cálculos con diferentes funciones sin cerrar ni abandonar la aplicación. 

- Presentar mensajes de error comprensibles para que el usuario pueda corregir los datos. 

### **4. Funciones de prueba** 

La aplicación deberá probarse con diferentes familias de funciones. No se aceptará demostrar el funcionamiento con una sola función. Se deberán incluir, como mínimo, ejemplos de: 

- Funciones polinomiales y algebraicas. 

- Funciones racionales. 

- Funciones exponenciales. 

- Funciones logarítmicas, respetando su dominio. 

- Funciones trigonométricas. 

- Funciones combinadas o trascendentes. 

- Funciones con raíces simples y, cuando corresponda, funciones con raíces múltiples. 

Ejemplos de prueba sugeridos: 

- f(x)=x³−4x−9 

- f(x)=2eˣ−5 

- f(x)=ln(x)−x+2 

- f(x)=sen(x)−0.5x 

- f(x)=x²e^(−x)−1 

### **5. Robustez y validación** 

La calidad de la aplicación dependerá en gran medida de su capacidad para evitar errores de ejecución y orientar al usuario. Se deberá comprobar qué sucede cuando se introducen expresiones inválidas, datos incompletos, intervalos que no contienen cambio de signo cuando el método lo requiere, tolerancias negativas, número de iteraciones inválido, valores fuera del dominio y situaciones que produzcan división entre cero. 

### **6. Informe técnico/documental** 

El informe deberá explicar el desarrollo del proyecto y contener, como mínimo: 

- Portada con datos del estudiante, grupo y proyecto. 

- Introducción y propósito. 

- Descripción del problema que se pretende resolver. 

- Fundamento matemático de los seis métodos. 

- Lenguaje de programación utilizado y justificación. 

- Herramientas, bibliotecas y recursos empleados. 

- Descripción del diseño y funcionamiento de la aplicación. 

- Explicación de la implementación de los métodos. 

- Estrategias de validación y manejo de errores. 

- Descripción de la gráfica integrada. 

- Pruebas realizadas con diferentes tipos de funciones. 

- Resultados obtenidos. 

- Dificultades encontradas durante el desarrollo y cómo fueron solucionadas. 

- Conclusiones. 

- Referencias consultadas. 

- Liga al video de presentación. 

### **7. Video de presentación** 

El estudiante deberá realizar un video en el que explique el desarrollo completo del proyecto y demuestre el funcionamiento de la aplicación. El video deberá mostrar la aplicación funcionando, no solamente diapositivas. 

- Presentación del problema y objetivo. 

- Explicación general de los métodos implementados. 

- Lenguaje y herramientas utilizadas. 

- Explicación del ingreso de funciones y parámetros. 

- Demostración de los cálculos y tabla de iteraciones. 

- Demostración de la gráfica dentro del sistema. 

- Pruebas con diferentes tipos de funciones. 

- Demostración de validaciones y manejo de errores. 

- Dificultades encontradas y soluciones. 

- Conclusiones y aprendizajes. 

### **8. Criterios de calidad del producto** 

|Aspecto|Condición esperada|
|---|---|
|Funcionalidad|Los métodos deben ejecutar correctamente y<br>entregar resultados verificables.|
|Validación|La aplicación debe controlar entradas inválidas y<br>situaciones numéricas problemáticas.|
|Interfaz|Debe ser clara, ordenada, consistente y fácil de<br>utilizar.|
|Gráfica|Debe estar integrada en la propia aplicación.|
|Pruebas|Debe demostrar funcionamiento con diferentes<br>tipos de funciones.|
|Informe|Debe documentar el desarrollo, dificultades,<br>resultados y conclusiones.|
|Video|Debe demostrar y explicar el funcionamiento real<br>de la aplicación.|
|Profesionalismo|El producto final debe mostrar orden, claridad y<br>trabajo técnico propio de una aplicación de<br>ingeniería.|



### **9. Asignación del lenguaje de programación** 

La asignación se realiza mediante un ciclo de tres posiciones según el número de lista del estudiante: 1 = Python, 2 = HTML/JavaScript y 3 = C++. El ciclo se repite: 4 = Python, 5 = HTML/JavaScript, 6 = C++, etc. 

|No. lista|Estudiante|Lenguaje|
|---|---|---|
|1|AGUAYO MEJIA JUAN PABLO|Python|
|2|AGUIRRE GUIZAR ANGEL<br>ISAAC|HTML/JavaScript|
|3|ALVAREZ CONTRERAS<br>MARVIN|C++|
|4|ANAYA MARTINEZ ANGEL<br>GUSTAVO|Python|
|5|AVALOS ALVIZAR MELANI<br>HANNALY|HTML/JavaScript|
|6|CARRILLO VALENCIA RUTH|C++|



|7|CHOCOTECO AGUILAR<br>CRISTOPHERJOSE|Python|
|---|---|---|
|8<br>|COBIAN ZAMORA KAROL<br>GABRIEL<br>|HTML/JavaScript<br>|
|9|ELIZONDO DAVILA CESAR<br>ALEJANDRO|C++|
|10|GARCIA RODRIGUEZ JOHET<br>EMMANUEL|Python|
|11|GOMEZ GUTIERREZ ALAN<br>RAZZIEL|HTML/JavaScript|
|12|GUZMAN GOMEZ GERMAN<br>ANTONIO|C++|
|13|HERNANDEZ LOPEZ PAVEL|Python|
|14|HUEZO CERDA ALFREDO|HTML/JavaScript|
|15|MANZO OCHOA JUAN<br>CARLOS|C++|
|16|MARTINEZ GUTIERREZ LIA<br>ROMINA|Python|
|17|PADILLA CASTELLANOS<br>OMAR EDUARDO|HTML/JavaScript|
|18|PUENTE JUAREZ RAFAEL ISAI|C++|
|19|RAMIREZ FIGUEROA<br>MARIANNA ITANDEHUI|Python|
|20|RAMOS ESPINOSA KALEL<br>ISAHID|HTML/JavaScript|
|21|ROLON MORIN ANGEL GAEL|C++|
|22|SALAS JIMENEZ IKER<br>ORLANDO|Python|
|23|TORRES ROBLADA OCTAVIO<br>CESAR|HTML/JavaScript|



### **10. Entregables** 

- Aplicación funcional con los métodos de la Unidad 2 integrados. 

- Informe técnico/documental en formato indicado por el docente. 

- Video de explicación y demostración del proyecto. 

- Evidencias de pruebas con diferentes funciones. 

- La aplicación deberá poder ejecutarse y demostrarse sin depender de otro programa para realizar los cálculos o generar la gráfica. 

### **11. Consideración final** 

El objetivo no es únicamente programar fórmulas. El estudiante deberá integrar los conocimientos matemáticos, numéricos y de programación en una herramienta funcional. La evaluación considerará tanto la correcta implementación de los métodos como la validación, presentación de resultados, gráfica integrada, documentación y capacidad de explicar el desarrollo realizado. 

