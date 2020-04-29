#include <iostream>
#include "math_tools.h"
#include "classes.h"
#include "display_tools.h"
#include "tools.h"
#include "sel.h"


int main()
{
    vector<Matrix> localKs;
    vector<Vector> localbs;
    Matrix K;
    Vector b;
    Vector T;

    
    cout << "IMPLEMENTACION DEL METODO DE LOS ELEMENTOS FINITOS\n"
         << "\t- 1 DIMENSION\n"
         << "\t- FUNCIONES DE FORMA LINEALES\n" << "\t- PESOS DE GALERKIN\n"
         << "*********************************************************************************\n\n";

    mesh m;
    leerMallayCondiciones(m);

    crearSistemasLocales(m,localKs,localbs);
    
    zeroes(K,m.getSize(NODES)*2);
    zeroes(b,m.getSize(NODES)*2);
    ensamblaje(m,localKs,localbs,K,b);
    
    //Solo se aplica Dirichlet, no habra condiciones de Neumann
    applyDirichlet(m,K,b);
    
    showMatrix(K);

    return 0;
}
