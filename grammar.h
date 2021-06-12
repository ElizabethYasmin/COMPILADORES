class Grammar{
private:
    std::vector<produccion>producciones_terminales;
    std::vector<produccion>producciones_no_terminales;
public:
    Grammar(std::string G[],int n);
    void print();
    
};

void Grammar::print(){
    std::cout<<"No terminales:\n";
    for (auto it = producciones_no_terminales.begin(); it != producciones_no_terminales.end(); it++){
        std::cout<<*it<<std::endl;
    }
    std::cout<<"Terminales:\n";
    for (auto it = producciones_terminales.begin(); it != producciones_terminales.end(); it++){
        std::cout<<*it<<std::endl;
    }

}


Grammar::Grammar(std::string G[],int n){
    std::vector<produccion> lista_de_producciones;
    for(int i=0;i<n;i++)
        lista_de_producciones.push_back(produccion(G[i]));
    
    for (int i = 0; i<lista_de_producciones.size() ; i++){
        int cont=0;
        produccion temp1 = lista_de_producciones.at(i);
        //es produccion no terminal?
        //si tiene mas de un caracter en la derecha no es terminal
        if(temp1.getlength()>1){
            cont++;
            producciones_no_terminales.push_back(temp1);
        }
        //si tiene un caracter en la derecha se verifica que este caracter no tenga produccion
        else{
            std::string temp2 = temp1.getDerecha();
            //buscando en las produccion que no haya una produccion del caracter derecho
            for (int j = 0; j<lista_de_producciones.size() ; j++){
                produccion temp3 = lista_de_producciones.at(j);
                if(temp2 == temp3.getIzquierda()){
                    cont++;
                    producciones_no_terminales.push_back(temp1);
                    break;
                }
            }
        }
        if(cont==0)
            producciones_terminales.push_back(temp1);
    }
} 

