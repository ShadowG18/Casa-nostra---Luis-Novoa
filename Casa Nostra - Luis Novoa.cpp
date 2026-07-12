#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// Estructura del nodo con los datos del CSV
struct Miembro {
    int id;
    string name;
    string last_name;
    char gender;
    int age;
    int id_boss;
    bool is_dead;
    bool in_jail;
    bool was_boss;
    bool is_boss;
    bool is_successor; // En caso de que varios tengan is_boss en 1 si no hay jefe actual se converitirian en sucesores

    Miembro* left;
    Miembro* right;
	
	// Inicializacion de los datos
    Miembro(int _id, string _name, string _last_name, char _gender, int _age, 
              int _id_boss, bool _is_dead, bool _in_jail, bool _was_boss, bool _is_boss) {
        id = _id;
        name = _name;
        last_name = _last_name;
        gender = _gender;
        age = _age;
        id_boss = _id_boss;
        is_dead = _is_dead;
        in_jail = _in_jail;
        was_boss = _was_boss;
        is_boss = _is_boss;
        is_successor = false; // Por defecto es falso
        
        left = nullptr;
        right = nullptr;
    }
};

class Arbol_Mafia {
private:
    Miembro* root;
    bool Jefe_Principal; // Para determinar quien es el jefe actual

    // Busqueda en Profundidad (DFS) usando recursividad pura
    Miembro* encontrar_Jefe(Miembro* actual, int boss_id) {
        if (actual == nullptr) return nullptr;
        if (actual->id == boss_id) return actual;

        Miembro* Busqueda_left = encontrar_Jefe(actual->left, boss_id);
        if (Busqueda_left != nullptr) return Busqueda_left;

        return encontrar_Jefe(actual->right, boss_id);
    }
	
	// Imprime los datos del arbol
    void imprimir_Arbol(Miembro* nodo, string espaciado) {
        if (nodo != nullptr) {
            cout << espaciado << "|_ " << nodo->name << " " << nodo->last_name 
                 << " (ID: " << nodo->id << ")";
                 
                if (nodo->is_boss) cout << " [JEFE ACTUAL]";
	            if (nodo->is_successor) cout << " [SUCESOR]";
	            if (nodo->is_dead) cout << " [DIFUNTO]";
	            if (nodo->in_jail) cout << " [EN PRISION]";
	            
	            cout << "\n";
	            
            imprimir_Arbol(nodo->left, espaciado + "   ");
            imprimir_Arbol(nodo->right, espaciado + "   ");
        }
    }

public:
    Arbol_Mafia() {
        root = nullptr;
        Jefe_Principal = false;
    }
	
	// Funcion para insertar miembros al arbol
    void insertar_Miembro(int id, string name, string last_name, char gender, int age, 
                      int id_boss, bool is_dead, bool in_jail, bool was_boss, bool is_boss) {
        


		bool actual_is_boss = is_boss;
		bool actual_is_successor = false;
		
		// LOGICA DE VALIDACION DEL JEFE ACTUAL
		if (actual_is_boss) {
		    if (!Jefe_Principal) {
		        // Al leer el csv mientras no haya un jefe principal ya, entonces el primer miembro que tenga is_boss en 1 sera el jefe actual
		        Jefe_Principal = true;
		    } else {
		    	// Si ya hay un jefe, los miembros con is_boss en 1 seran sucesores
		        actual_is_boss = false;
		        actual_is_successor = true;
		        }
		}

        // Creamos el nodo con los valores ya filtrados
        Miembro* Nuevo_nodo = new Miembro(id, name, last_name, gender, age, 
                                           id_boss, is_dead, in_jail, was_boss, actual_is_boss);
        Nuevo_nodo->is_successor = actual_is_successor;
        
        // Si el arbol esta vacio, el primero es la raiz
        if (root == nullptr) {
            root = Nuevo_nodo;
            return;
        }

        Miembro* Nodo_jefe = encontrar_Jefe(root, id_boss);

        if (Nodo_jefe != nullptr) {
            if (Nodo_jefe->left == nullptr) {
                Nodo_jefe->left = Nuevo_nodo;
            } else if (Nodo_jefe->right == nullptr) {
                Nodo_jefe->right = Nuevo_nodo;
            } else {
                cout << "Advertencia: El jefe " << Nodo_jefe->name 
                     << " ya tiene 2 subordinados. No se pudo anadir a " << name << ".\n";
            }
        } else {
            cout << "Error: No se encontro al jefe con ID " << id_boss 
                 << " en el arbol para asignar a " << name << ".\n";
        }
    }

    void Mostrar_jerarquia() {
        cout << "\n--- Jerarquia de la Familia Mafiosa ---\n";
        imprimir_Arbol(root, "");
        cout << "---------------------------------------\n";
    }
};

// Funcion independiente para leer el CSV y llenar el arbol
void cargarDesdeCSV(Arbol_Mafia& familia, const string& filename) {
    ifstream file(filename);
    
    // Verificar si el archivo se abrio correctamente
    if (!file.is_open()) {
        cout << "Error: No se pudo abrir el archivo '" << filename << "'.\n";
        cout << "Asegurate de que exista en la misma carpeta que el ejecutable.\n";
        return;
    }

    string line;
    // 1. Leer y descartar la primera linea (los encabezados del CSV)
    getline(file, line);

    // 2. Leer el archivo linea por linea hasta el final
    while (getline(file, line)) {
        // Se usa stringstream para separar los datos por comas
        stringstream ss(line);
        string temp; // Variable temporal para guardar el texto entre comas

        int id, age, id_boss;
        string name, last_name;
        char gender;
        bool is_dead, in_jail, was_boss, is_boss;

        // Se extraen y convierten cada campo en el orden exacto del CSV
        getline(ss, temp, ',');
        id = stoi(temp); 

        getline(ss, name, ',');
        
        getline(ss, last_name, ',');
        
        getline(ss, temp, ',');
        gender = temp[0]; // Tomamos el primer caracter del string
        
        getline(ss, temp, ',');
        age = stoi(temp);
        
        getline(ss, temp, ',');
        id_boss = stoi(temp);
        
        getline(ss, temp, ',');
        is_dead = (stoi(temp) == 1); // Si es "1", sera true. Si es "0", sera false.
        
        getline(ss, temp, ',');
        in_jail = (stoi(temp) == 1);
        
        getline(ss, temp, ',');
        was_boss = (stoi(temp) == 1);
        
        getline(ss, temp, ',');
        is_boss = (stoi(temp) == 1);

        // 3. Insertar al miembro directamente en el arbol
        familia.insertar_Miembro(id, name, last_name, gender, age, id_boss, is_dead, in_jail, was_boss, is_boss);
    }

    file.close();
    cout << "Datos cargados exitosamente desde " << filename << "\n";
}

int main() {
    Arbol_Mafia familia;

    // Llamamos a la funcion 
    cargarDesdeCSV(familia, "familia_data.csv");

    // Mostramos la jerarquia final
    familia.Mostrar_jerarquia();

    return 0;
}