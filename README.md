# Scripts propios en el proyecto

Aquí se explica cómo crear tus propios scripts y cómo compartir datos entre `main.cc` y el resto de archivos.

---

## 1. Crear un nuevo script

Crea un archivo nuevo con extensión `.cc` (por ejemplo `jhony.cc`). Dentro puedes definir las funciones que quieras y llamarlas luego desde `main.cc`.

---

## 2. Pasar variables desde `main.cc` a un script

Para que un script pueda usar y modificar una variable que tienes en `main.cc`, pásala por referencia con `&`.

Ejemplo en `main.cc`:

```cpp
int test = 10;
jhony(&test);
```

Así el script trabaja con la variable original y cualquier cambio se refleja en `main.cc`.

---

## 3. Usar variables globales de un script en `main.cc`

Si declaras una variable global en tu script (por ejemplo en `jhony.cc`), puedes usarla también desde `main.cc` sin pasarla como parámetro.

Ejemplo en `jhony.cc`:

```cpp
int test2 = 5;  // como variable global
```

Uso en `main.cc`:

```cpp
printf("Test2: %d\n", test2);
```

## 4 Pasar variables usando `extern`

Si quieres usar una variable global en un script y desde `main.cc`, puedes declararla como `extern` en el script.

Ejemplo en `main.cc` tenemos:

```cpp
int test = 10; // definicion de la variable
```

Ejemplo en `jhony.cc` lo podemos llamar así:

```cpp
extern int test; // busqueda de la variable en otro archivo (main.cc)
printf("Test: %d\n", test);
```
