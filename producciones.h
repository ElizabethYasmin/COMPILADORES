

class produccion {
private:
    std::string izquierda;
    std::string derecha;
public:
    produccion(std::string produccion){
        int breakk= 0;
        //ubica la posicion de separacion (::=)     
        for(int j=0 ; j<produccion.length() ; j++){
            if (produccion[j]==32) 
            {breakk=j; break;}
        }
        //extrae el lado izquierdo
            for(int j=0 ; j<breakk ; j++){
                izquierda += produccion[j];
        }
        //extrae el lado derecho
        for (int j=breakk+5; j<produccion.length();j++){
                derecha += produccion[j];
        }
    }
    std::string getIzquierda(){ 
        return izquierda;
    }
    std::string getDerecha(){ 
        return derecha;
    }
    //devuelva la cantidad de elementos a la derecha de la produccion
    int getlength(){
        int cont = 0;
        for(int i = 0 ; i<derecha.size() ; i++){
            if(derecha[i]==32)
                cont ++;
        }
        return cont+1;
    }
    friend ostream& operator<<(ostream& os, const produccion& dt);
    friend bool operator!=(produccion const& x, produccion const& y);
};

