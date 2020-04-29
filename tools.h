#include <fstream>
#include "string.h"
    
void obtenerDatos(istream &file,int nlines,int n,int mode,item* item_list){
    string line;
    file >> line;
    if(nlines==DOUBLELINE) file >> line;

    for(int i=0;i<n;i++){
        switch(mode){
        case INT_FLOAT:
            int e; float r;
            file >> e >> r;
            item_list[i].setIntFloat(e,r);
            break;
        case INT_INT_INT:
            int e1,e2,e3;
            file >> e1 >> e2 >> e3;
            item_list[i].setIntIntInt(e1,e2,e3);
            break;
        }
    }
}


void updateConditionNodes(int n,condition * list,int delta){
    for(int i=0;i<n;i++)
        list[i].setNode1(list[i].getNode1()+delta);
}

void joinConditions(condition *list,int n1,int n2,condition *list1,condition *list2){
    int i;
    for(i=0;i<n1;i++)
        list[i] = list1[i];
    for(int j=0;j<n2;j++){
        list[i] = list2[j];
        i++;
    }
}

void correctConditions(int n,condition *list){
    for(int i=0;i<n-1;i++){
        int pivot = list[i].getNode1();
        for(int j=i;j<n;j++)
            if(list[j].getNode1()>pivot)
                list[j].setNode1(list[j].getNode1()-1);
    }
}

void leerMallayCondiciones(mesh &m){
    char filename[14];
    ifstream file;
    float l,tau,kappa,lambda,ni,psi,alfa,delta,eta;
    int nnodes,neltos,ndirich_y,ndirich_w;

    condition *dirichlet_y_list;
    condition *dirichlet_w_list;

    do{
        cout << "Ingrese el nombre del archivo que contiene los datos de la malla: ";
        cin >> filename;
        file.open(filename);
    }while(!file);
    
    file >> l >> tau >> kappa >> lambda >> ni >> psi >> alfa >> delta >> eta;
    file >> nnodes >> neltos >> ndirich_y >> ndirich_w;

    m.setParameters(l,tau,kappa,lambda,ni,psi,alfa,delta,eta);
    m.setSizes(nnodes,neltos,ndirich_y+ndirich_w);
    m.createData();

    
    dirichlet_y_list = new condition[ndirich_y];
    dirichlet_w_list = new condition[ndirich_w];

    obtenerDatos(file,SINGLELINE,nnodes,INT_FLOAT,m.getNodes());
    obtenerDatos(file,DOUBLELINE,neltos,INT_INT_INT,m.getElements());
    obtenerDatos(file,DOUBLELINE,ndirich_y,INT_FLOAT,dirichlet_y_list);
    obtenerDatos(file,DOUBLELINE,ndirich_w,INT_FLOAT,dirichlet_w_list);

    file.close();

    updateConditionNodes(ndirich_w,dirichlet_w_list,nnodes);
    joinConditions(m.getDirichlet(),ndirich_y,ndirich_w,dirichlet_y_list,dirichlet_w_list);
    correctConditions(ndirich_y+ndirich_w,m.getDirichlet());

}

