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
	            if (nodo->is_successor && !nodo->is_dead) cout << " [SUCESOR]";
	            if (nodo->is_dead) cout << " [DIFUNTO]";
	            if (nodo->in_jail && !nodo->is_dead) cout << " [EN PRISION]";
	            
	            cout << "\n";
	            
            imprimir_Arbol(nodo->left, espaciado + "   ");
            imprimir_Arbol(nodo->right, espaciado + "   ");
        }
    }

    // Busca al primer sucesor que siga vivo (Izquierda primero, luego Derecha)
    Miembro* buscar_Sucesor(Miembro* nodo) {
        if (nodo == nullptr) return nullptr;
        
        // Retorna el nodo si es sucesor
        if (nodo->is_successor && !nodo->is_dead) return nodo;

        // Busca primero en la rama izquierda
        Miembro* izq = buscar_Sucesor(nodo->left);
        if (izq != nullptr) return izq;

        // Si no esta en la izquierda, se busca en la derecha
        return buscar_Sucesor(nodo->right);
    }

public:
    Arbol_Mafia() {
        root = nullptr;
        Jefe_Principal = false;
    }
    
    Miembro* obtener_Raiz() {
        return root;
    }
	
	// Funcion para insertar miembros al arbol
    void insertar_Miembro(int id, string name, string last_name, char gender, int age, 
                      int id_boss, bool is_dead, bool in_jail, bool was_boss, bool is_boss) {
        
		bool actual_in_jail = in_jail;
		bool actual_is_dead = is_dead;
		bool actual_is_boss = is_boss;
		bool actual_is_successor = false;
		
		// LOGICA DE VALIDACION DEL JEFE ACTUAL
		if (actual_is_boss) {
		    if (!Jefe_Principal && !actual_is_dead  && !actual_in_jail) {
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
    
    void modificar_Miembro(int id_buscar) {
        // Reutilizamos la funcion de busqueda DFS que tenemos en vez de hacer una nueva
        Miembro* encontrado = encontrar_Jefe(root, id_buscar);

        if (encontrado == nullptr) {
            cout << "Error: No se encontro ningun miembro con la ID " << id_buscar << ".\n";
            return;
        }

        cout << "\n=== Modificando Miembro (ID: " << encontrado->id << ") ===\n";

        // Variables temporales para la entrada de datos
        int temp_int;
        string temp_str;
        char temp_char;

        cout << "Nombre actual [" << encontrado->name << "]. Nuevo nombre: ";
        cin >> encontrado->name;

        cout << "Apellido actual [" << encontrado->last_name << "]. Nuevo apellido: ";
        cin >> encontrado->last_name;

        cout << "Genero actual [" << encontrado->gender << "]. Nuevo genero (M/F): ";
        cin >> temp_char;
        encontrado->gender = temp_char;

        cout << "Edad actual [" << encontrado->age << "]. Nueva edad: ";
        cin >> temp_int;
        encontrado->age = temp_int;

        cout << "Esta muerto? (1 = Si, 0 = No) [" << encontrado->is_dead << "]: ";
        cin >> temp_int;
        encontrado->is_dead = (temp_int == 1);

        cout << "Esta en prision? (1 = Si, 0 = No) [" << encontrado->in_jail << "]: ";
        cin >> temp_int;
        encontrado->in_jail = (temp_int == 1);

        cout << "Es sucesor? (1 = Si, 0 = No) [" << encontrado->is_successor << "]: ";
        cin >> temp_int;
        encontrado->is_successor = (temp_int == 1);
        
        if (encontrado->is_boss) {
            cout << "\n--- Evaluando estado del Jefe Actual ---\n";
            
            if (encontrado->is_dead) {
                cout << "El jefe ha muerto. Se necesita buscar un nuevo reemplazo.\n";
                encontrado->is_boss = false;
                encontrado->was_boss = true;
                actualizar_Jefe();
            } 
            else if (encontrado->age > 75 || encontrado->in_jail) {
                
                if (encontrado->age > 75) cout << "El jefe se retira debido a su edad avanzada (Edad:" << encontrado->age << ").\n";
                else cout << "El jefe fue arrestado y esta en prision.\n";
                
                encontrado->is_boss = false;
                encontrado->was_boss = true;
                cout << "El puesto de jefe ha quedado vacante, se necesita buscar un nuevo jefe.\n";
                actualizar_Jefe();
            } else{
				cout << "El jefe actual todavia esta en buena condicion para ser jefe.\n";
			}
            
        }

        cout << "\nMiembro modificado exitosamente.\n";
    }
    
    // Funcion para actualizar al jefe y reacomodar el arbol si es necesario
    void actualizar_Jefe() {
        Miembro* jefe_actual = obtener_Raiz();

        if (jefe_actual == nullptr) return;

        Miembro* sucesor = buscar_Sucesor(root);

        if (sucesor == nullptr) {
            cout << "El puesto de jefe esta libre pero no se encontro ningun sucesor vivo disponible.\n";
            return;
        }

        cout << "\n=== ACTUALIZACION DE MANDO ===\n";
        cout << "El sucesor " << sucesor->name << " asume el mando. Reacomodando el arbol...\n";

        // Variables temporales para guardar los datos del sucesor
        int temp_id = sucesor->id;
        string temp_name = sucesor->name;
        string temp_last_name = sucesor->last_name;
        char temp_gender = sucesor->gender;
        int temp_age = sucesor->age;
        bool temp_in_jail = sucesor->in_jail;
        int temp_id_boss = sucesor->id_boss;

        // 1. El nodo inferior (donde estaba el sucesor) recibe los datos del jefe anterior
        sucesor->id = jefe_actual->id;
        sucesor->name = jefe_actual->name;
        sucesor->last_name = jefe_actual->last_name;
        sucesor->gender = jefe_actual->gender;
        sucesor->age = jefe_actual->age;
        sucesor->in_jail = jefe_actual->in_jail;
        sucesor->id_boss = jefe_actual->id_boss;
        
        // El anterior jefe ahora queda marcado como que ya paso por el cargo
        sucesor->is_dead = jefe_actual->is_dead; 
        sucesor->is_boss = false;
        sucesor->was_boss = true; 
        sucesor->is_successor = false;

        // 2. El nodo superior (la cima) recibe los datos del sucesor
        jefe_actual->id = temp_id;
        jefe_actual->name = temp_name;
        jefe_actual->last_name = temp_last_name;
        jefe_actual->gender = temp_gender;
        jefe_actual->age = temp_age;
        jefe_actual->in_jail = temp_in_jail;
        jefe_actual->id_boss = temp_id_boss; // El nuevo jefe mantiene su id_boss original

        jefe_actual->is_dead = false; // El nuevo jefe debe estar vivo
        jefe_actual->is_boss = true; 
        jefe_actual->was_boss = false;
        jefe_actual->is_successor = false;

        cout << "Arbol reacomodado exitosamente.\n\n";
    };
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
    int miembro_modificar; 
    int continuar;
    
    do {
    
    cout << "Ingrese la Id del miembro que quiera modificar" << endl;
    cin >> miembro_modificar;
    familia.modificar_Miembro(miembro_modificar);

    familia.Mostrar_jerarquia();
    
    cout << "Desea modificar otro miembro? (Pon 0 para terminar el programa)" << endl;
    cin >> miembro_modificar;
    
	} while (continuar !=0);

    return 0;
}