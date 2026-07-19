# Casa-nostra---Luis-Novoa
# Sistema de Gestión de Jerarquía Mafia (C++)

Este proyecto implementa un sistema de arboles binarios gestionar la estructura jerárquica de una familia mafiosa utilizando. El programa permite cargar la familia desde un archivo CSV, visualizar la jerarquía, modificar a los miembros y gestionar automáticamente la sucesión del jefe cuando el actual deja su puesto o ya no es apto.

## Estructura del Archivo CSV

Para que el programa funcione correctamente, el archivo `familia_data.csv` debe tener el siguiente formato (con encabezados):

`id,name,last_name,gender,age,id_boss,is_dead,in_jail,was_boss,is_boss`

*   **id**: Entero único.
*   **name/last_name**: Strings.
*   **gender**: 'M' o 'F'.
*   **age**: Entero.
*   **id_boss**: ID del superior (0 si es el jefe supremo).
*   **is_dead/in_jail/was_boss/is_boss**: Booleanos (1 para verdadero, 0 para falso).

## Uso

1.  **Compilación**: Utiliza un compilador de C++ (como `g++`).
    ```bash
    g++ -o mafia_system main.cpp
    ```
2.  **Ejecución**: Asegúrate de tener el archivo `familia_data.csv` en la misma carpeta que el ejecutable.
    ```bash
    ./mafia_system
    ```
3.  **Menú**:
    *   **1**: Ver jerarquía (imprime el árbol visualmente).
    *   **2**: Modificar miembro (cambia datos y evalúa si es necesario activar el protocolo de sucesión).
    *   **0**: Salir.

## Reglas de Sucesión

Cuando un jefe queda fuera de servicio, el sistema busca un reemplazo siguiendo esta jerarquía de prioridades:
1.  Sucesor designado, vivo y fuera de prision.
2.  Cualquier miembro vivo y fuera de prision.
3.  Sucesor designado en prisión.
4.  Cualquier miembro vivo en prisión.
