//
// Created by fabio on 15/11/2023.
//

#include "Zona.h"

using namespace std;

void Zona::adicionaProps(){
    propriedades.push_back(Propriedade("Temperatura",0,-273,INT_MAX));
    propriedades.push_back(Propriedade("Luz",0,0,INT_MAX));
    propriedades.push_back(Propriedade("Radiacao",0,0,INT_MAX));
    propriedades.push_back(Propriedade("Vibracao",0,0,INT_MAX));
    propriedades.push_back(Propriedade("Humidade",0,0,100));
    propriedades.push_back(Propriedade("Fumo",0,0,100));
    propriedades.push_back(Propriedade("Som",0,0,INT_MAX));
}

Zona::Zona(const int id, const int linha, const int coluna,int indice_janela) {
    this->id=id;
    this->linha=linha;
    this->coluna=coluna;
    this->indice_janela=indice_janela;

    adicionaProps();
}

int Zona::getId() const {
    return id;
}

int Zona::getLinha() const {
    return linha;
}

int Zona::getColuna() const {
    return coluna;
}

string Zona::descricao() {
    return "ID: " + std::to_string(id) +
    " Aparelhos: " +std::to_string(getNumAparelhos())+
    "\nSensores: " +std::to_string(getNumSensores())+
    " Processadores: " + std::to_string(getNumProcessadores());
}

int Zona::getIndiceJanela() const {
    return indice_janela;
}

int Zona::getNumProps() const {
    return propriedades.size();
}

int Zona::getNumProcessadores() const {
    return processadores.size();
}

int Zona::getNumSensores() const {
    return sensores.size();
}

int Zona::getNumAparelhos() const {
    return aparelhos.size();
}

Propriedade Zona::getProp(int indiceVetorProps){
    return propriedades[indiceVetorProps];
}

Aparelho* Zona::getAparelho(int indiceVetorAparelhos) {
    return aparelhos[indiceVetorAparelhos];
}

Sensor* Zona::getSensor(int indiceVetorSensores) {
    return sensores[indiceVetorSensores];
}

Processador* Zona::getProcessador(int indiceVetorPropcessadores) {
    return processadores[indiceVetorPropcessadores];
}

int Zona::setPropsByName(std::string nome,int valor){

    for(int i=0;i<propriedades.size();i++){
        if(propriedades[i].getChave()==nome){
            if(valor>=propriedades[i].getValor_min() && valor<=propriedades[i].getValor_max()){
                propriedades[i].setValor(valor);
                return 1;
            }
        }
    }
    return 0;
}

int Zona::adicionaComponente(string tipo,string tipo_in,int id_ap,int id_s,int id_p,int instante_inicial) {
    if (tipo == "a") // APARELHOS
    {
        if (tipo_in == "a") {
            Aparelho *novoAparelho = new Aquecedor(false,id_ap,instante_inicial);
            aparelhos.push_back(novoAparelho);
            return 1;
        } else if (tipo_in == "s") {
            Aparelho *novoAparelho = new Aspersor(false,id_ap,instante_inicial);
            aparelhos.push_back(novoAparelho);
            return 1;
        } else if (tipo_in == "r") {
            Aparelho *novoAparelho = new Refrigerador(false,id_ap,instante_inicial);
            aparelhos.push_back(novoAparelho);
            return 1;
        } else if (tipo_in == "l") {
            Aparelho *novoAparelho = new Lampada(false,id_ap,instante_inicial);
            aparelhos.push_back(novoAparelho);
            return 1;
        }
    } else if (tipo == "s")//SENSORES
    {
        if (tipo_in == "t") {
            Sensor *novoSensor = new sTemperatura(id_s);
            sensores.push_back(novoSensor);
            return 1;
        } else if (tipo_in == "v") {
            Sensor *novoSensor = new sMovimento(id_s);
            sensores.push_back(novoSensor);
            return 1;
        } else if (tipo_in == "m") {
            Sensor *novoSensor = new sLuz(id_s);
            sensores.push_back(novoSensor);
            return 1;
        } else if (tipo_in == "d") {
            Sensor *novoSensor = new sRadiacao(id_s);
            sensores.push_back(novoSensor);
            return 1;
        } else if (tipo_in == "h") {
            Sensor *novoSensor = new sHumidade(id_s);
            sensores.push_back(novoSensor);
            return 1;
        } else if (tipo_in == "o") {
            Sensor *novoSensor = new sSom(id_s);
            sensores.push_back(novoSensor);
            return 1;
        } else if (tipo_in == "f") {
            Sensor *novoSensor = new sFumo(id_s);
            sensores.push_back(novoSensor);
            return 1;
        }
    }else if(tipo=="p"){
            Processador *novoProcessador = new Processador(tipo_in,id_p, getId());
            processadores.push_back(novoProcessador);
            return 1;
    }
    return 0;
}

int Zona::removeComponente(std::string tipo, int id_aparelho, int id_sensor,int id_processador) {

    if (tipo == "a" && !aparelhos.empty()) {
        for (int i = 0; i < aparelhos.size(); i++) {
            if (aparelhos[i]->getId() == id_aparelho) {
                delete aparelhos[i];
                aparelhos.erase(aparelhos.begin() + i);
                return 1;
            }
        }
    } else if (tipo == "s" && !sensores.empty()) {
        for (int i = 0; i < sensores.size(); i++) {
            if (sensores[i]->getId() == id_sensor) {
                delete sensores[i];
                sensores.erase(sensores.begin() + i);
                return 1;
            }
        }
    } else if (tipo == "p" && !processadores.empty()) {
        for (int i = 0; i < processadores.size(); i++) {
            if (processadores[i]->getId_Proc() == id_processador) {
                delete processadores[i];
                processadores.erase(processadores.begin() + i);
                return 1;
            }
        }
    }

    return 0;
}
std::string Zona::descricaoComponentes() {
    std::string info;

    for (int i = 0; i < aparelhos.size(); ++i) {
        Aparelho* aparelho = aparelhos[i];
        info += "Aparelho: " + aparelho->getLetra() + ", Ligado: " + (aparelho->isLigado() ? "sim" : "nao") +",ID: " + std::to_string(aparelho->getId()) +"\n";
    }

    for (int i = 0; i < sensores.size(); ++i) {
        Sensor* sensor = sensores[i];
        info += "Sensor: " + sensor->getPropObservada() + ",ID: " + std::to_string(sensor->getId())  +", Medicao: "+std::to_string(sensor->getValorMedido())+"\n" ;
    }

    for (int i = 0; i < processadores.size(); ++i) {
        Processador* processador = processadores[i];
        info += "Processador: " + std::to_string(processador->getId_Proc()) + ",Comando: " + processador->getComando() +"\n";
    }

    return info;
}

void Zona::medir_props_para_sensores() {

    for (int i = 0; i < sensores.size(); i++) {
        if (  sensores[i]) {

            sensores[i]->get_propriedade(propriedades);

        }
    }
}

std::string Zona::getConteudoZona() const {
    std::string conteudo;

    conteudo += "ID Zona - [" + std::to_string(id) + "]\n";

    for (int i = 0; i < aparelhos.size(); ++i) {
        conteudo += "-" + aparelhos[i]->getLetra() + std::to_string(aparelhos[i]->getId()) + "-\n";
    }

    for (int i = 0; i < sensores.size(); ++i) {
        conteudo += "-" + sensores[i]->getLetraVisualizacao() + std::to_string(sensores[i]->getId()) + "-\n";
    }

    for (int i = 0; i < processadores.size(); ++i) {
        conteudo += "-p" + std::to_string(processadores[i]->getId_Proc()) + "-\n";
    }

    return conteudo;
}

void Zona::atualizaAparelho() {
    for(int i=0;i<aparelhos.size();i++){
        for(int j=0;j<processadores.size();j++){
            for(int y=0;y<processadores[j]->getNumAparelhos();y++){
                if(aparelhos[i]->getId()==processadores[j]->getAparelho(y).getId()){
                    aparelhos[i]->setLigado(processadores[j]->getAparelho(y).isLigado());
                    aparelhos[i]->setInstante_ligou(processadores[j]->getAparelho(y).getInstante_ligou());
                    aparelhos[i]->setInstante_desligou(processadores[j]->getAparelho(y).getInstante_desligou());
                    aparelhos[i]->setUltimo_Incremento(processadores[j]->getAparelho(y).getUltimo_Incremento());
                }
            }

        }
    }
}

void Zona::setPropsbyAparelhos(int instante_atual,int instante_momentaneo){

    std::string letraAparelho;

    for(int i=0;i<aparelhos.size();i++){
        letraAparelho = (char)std::tolower(aparelhos[i]->getLetra()[0]);
        if(letraAparelho=="a" && aparelhos[i]->isLigado()){ //AQUECEDOR LIGADO
            int instantes_ligado=instante_atual-aparelhos[i]->getInstante_ligou();
            int instantes_passados_momento=instante_momentaneo-aparelhos[i]->getInstante();
            for(int j=0;j<propriedades.size();j++){

                if(propriedades[j].getChave()=="Som" && instantes_passados_momento==aparelhos[i]->getInstante_ligou()) {
                    propriedades[j].setValor(propriedades[j].getValor() + 5); //adiciona 5db ruido uma unica vez ao ligar
                }

                if(propriedades[j].getChave()=="Temperatura" && instantes_ligado>0 && propriedades[j].getValor()<50) {
                    int incremento = instantes_ligado / 3;
                    int incremento_atual=incremento-aparelhos[i]->getUltimo_Incremento();

                    propriedades[j].setValor(propriedades[j].getValor()+incremento_atual); //adiciona 1º temperatura a cada 3 instantes
                    aparelhos[i]->setUltimo_Incremento(incremento);
                }
            }
        }else if(letraAparelho=="a" && !aparelhos[i]->isLigado()) { //AQUECEDOR DESLIGADO
            int instantes_passados_momento = instante_momentaneo - aparelhos[i]->getInstante();

            for (int j = 0; j < propriedades.size(); j++) {
                if (propriedades[j].getChave() == "Som" && instantes_passados_momento == aparelhos[i]->getInstante_desligou()) {
                    propriedades[j].setValor(propriedades[j].getValor() - 5); //remove 5db ruido uma unica vez ao desligar
                }
            }
        }else if(letraAparelho=="s" && aparelhos[i]->isLigado()){ //ASPERSOR LIGADO
            int instantes_passados_momento=instante_momentaneo-aparelhos[i]->getInstante();
            for(int j=0;j<propriedades.size();j++){

                if(propriedades[j].getChave()=="Humidade" && instantes_passados_momento==aparelhos[i]->getInstante_ligou() && propriedades[j].getValor()<75) {
                    if(propriedades[j].getValor()+50>75){
                        propriedades[j].setValor(75);
                    }else{
                        propriedades[j].setValor(propriedades[j].getValor() + 50);
                    } //adiciona 50% de humidade até um maximo de 75% uma unica vez ao ligar
                }

                if(propriedades[j].getChave()=="Vibracao" && instantes_passados_momento==aparelhos[i]->getInstante_ligou()) {
                    propriedades[j].setValor(propriedades[j].getValor() + 100); //adiciona 100 Hz de vibracao uma unica vez ao ligar
                }

                if(propriedades[j].getChave()=="Fumo" && instantes_passados_momento==aparelhos[i]->getInstante_ligou()+1) {
                    propriedades[j].setValor(0); //Coloca o fumo a 0 no segundo instante
                }

            }
        }else if(letraAparelho=="s" && !aparelhos[i]->isLigado()) { //ASPERSOR DESLIGADO
            int instantes_passados_momento = instante_momentaneo - aparelhos[i]->getInstante();

            for (int j = 0; j < propriedades.size(); j++) {
                if (propriedades[j].getChave() == "Vibracao" && instantes_passados_momento == aparelhos[i]->getInstante_desligou()) {
                    propriedades[j].setValor(propriedades[j].getValor() - 100); //remove 100Hz de vibracao uma unica vez ao desligar
                }
            }
        }else if(letraAparelho=="r" && aparelhos[i]->isLigado()){ //REFRIGERADOR LIGADO
            int instantes_ligado=instante_atual-aparelhos[i]->getInstante_ligou();
            int instantes_passados_momento=instante_momentaneo-aparelhos[i]->getInstante();
            for(int j=0;j<propriedades.size();j++){

                if(propriedades[j].getChave()=="Som" && instantes_passados_momento==aparelhos[i]->getInstante_ligou()) {
                    propriedades[j].setValor(propriedades[j].getValor() + 20) ; //adiciona 20db ruido uma unica vez ao ligar
                }

                if(propriedades[j].getChave()=="Temperatura" && instantes_ligado>0 && propriedades[j].getValor()<50) {
                    int incremento = instantes_ligado / 3;
                    int incremento_atual=incremento-aparelhos[i]->getUltimo_Incremento();

                    if(propriedades[j].getValor()>propriedades[j].getValor_min()){
                        propriedades[j].setValor(propriedades[j].getValor()-incremento_atual); //remove 1º temperatura a cada 3 instantes
                        aparelhos[i]->setUltimo_Incremento(incremento);
                    }
                }
            }
        }else if(letraAparelho=="r" && !aparelhos[i]->isLigado()) { //REFRIGERADOR DESLIGADO
            int instantes_passados_momento = instante_momentaneo - aparelhos[i]->getInstante();

            for (int j = 0; j < propriedades.size(); j++) {
                if (propriedades[j].getChave() == "Som" && instantes_passados_momento == aparelhos[i]->getInstante_desligou()) {
                    propriedades[j].setValor(propriedades[j].getValor() - 20); //remove 20db ruido uma unica vez ao desligar
                }
            }
        }else if(letraAparelho=="l" && aparelhos[i]->isLigado()){ //LAMPADA LIGADA
            int instantes_passados_momento=instante_momentaneo-aparelhos[i]->getInstante();
            for(int j=0;j<propriedades.size();j++){

                if(propriedades[j].getChave()=="Luz" && instantes_passados_momento==aparelhos[i]->getInstante_ligou()) {
                    propriedades[j].setValor(propriedades[j].getValor() + 900); //adiciona 900 lumens uma unica vez ao ligar
                }

            }
        }else if(letraAparelho=="l" && !aparelhos[i]->isLigado()) { //LAMPADA DESLIGADA
            int instantes_passados_momento = instante_momentaneo - aparelhos[i]->getInstante();

            for (int j = 0; j < propriedades.size(); j++) {
                if (propriedades[j].getChave() == "Luz" && instantes_passados_momento == aparelhos[i]->getInstante_desligou()) {
                    propriedades[j].setValor(propriedades[j].getValor() - 900); //remove 900 lumens uma unica vez ao desligar
                }
            }
        }
    }

}

void Zona::adicionaProcessador(Processador novo) {
    Processador *novoProcessador = new Processador(novo.getId_Proc(),novo.getComando(),novo.getIdZonaAssociada(),novo.getVetorRegras(),novo.getVetorAparelhos());
    processadores.push_back(novoProcessador);
}

void Zona::substituiProc(Processador substituto,Zona* zonaAtual) {
    vector<Aparelho> substituir_a;
    vector<Regra> substituir_r;

    for(int i=0;i<processadores.size();i++){
        if(processadores[i]->getId_Proc()==substituto.getId_Proc()){
            processadores[i]->setComando(substituto.getComando());
            processadores[i]->setVetorAparelhos(substituto.getVetorAparelhos());
            processadores[i]->setVetorRegras(substituto.getVetorRegras());
        }
    }
}

Zona::~Zona(){
    for(int i=0;i<getNumProcessadores();i++){
        delete processadores[i];
        processadores[i]= nullptr;
    }
    for(int i=0;i<getNumSensores();i++){
        delete sensores[i];
        sensores[i]= nullptr;
    }
    for(int i=0;i<getNumAparelhos();i++){
        delete aparelhos[i];
        aparelhos[i]= nullptr;
    }
}