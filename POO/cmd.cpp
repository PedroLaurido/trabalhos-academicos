//
// Created by 35191 on 31/10/2023.
//

#include "cmd.h"
#include "Terminal.h"
#include "Zona.h"
#include "Habitacao.h"
#include "Simulacao.h"
#include <vector>

using namespace term;

std::vector<Window> windows;  // Vetor de windows dinamico

void clearWindows() {
    for (auto& window : windows) {
        window.clear();
    }
    windows.clear();
}

void processarComando(const string& comando, const string& arg1, const string& arg2, const string& arg3, const string& arg4,const string &arg5,const string &arg6, Simulacao& simulacao, Window &w,int &instante,int &id_ap,int &id_s,int &id_p,int &id_r) {
    Terminal& t = Terminal::instance();

    int aux_avanca,aux_mandar_comando=0;;
    if(simulacao.getHabitacao()){ //medir props para sensores
        for (int i = 0; i < simulacao.getHabitacao()->getLinhas(); ++i) {
            for (int j = 0; j < simulacao.getHabitacao()->getColunas(); ++j) {
                Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);
                if (zonaAtual) {
                    zonaAtual->medir_props_para_sensores();
                }
            }
        }
    }



    if(comando=="prox"){

        if(!simulacao.getHabitacao()){
            w << set_color(4) << "[Nao existe habitacao]" <<" \n";
            return;
        }

        w<<set_color(10)<<"[Avancei 1 instante]\n";

        instante++;

    }else if(comando=="avanca"){
        if(arg1.empty()) {
            w << set_color(4) << "[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int valor=stoi(arg1);

            if(!simulacao.getHabitacao()){
                w << set_color(4) << "[Nao existe habitacao]" << "\n";
                return;
            }
            aux_avanca=valor;
            instante=instante+valor;

            w<<set_color(10)<<"[Avancei "<< valor<<" instantes]\n";
        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO, argumento(s) invalido(s)]\n";
            return;
        }
    }
    //ATUALIZAÇÕES PROPRIEDADES ATRAVEZ DE APARELHOS
    if(comando=="prox" || comando=="avanca"){
        int instante_aux;
        if(comando=="prox"){
            instante_aux=instante-1;
        }else if(comando=="avanca"){
            instante_aux=instante-aux_avanca;
        }

        if(simulacao.getHabitacao()) {
            for (int i = 0; i < simulacao.getHabitacao()->getLinhas(); ++i) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); ++j) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);
                    if (zonaAtual) {
                        zonaAtual->setPropsbyAparelhos(instante,instante_aux);
                    }
                }
            }
        }

    }

    if(simulacao.getHabitacao()){ //avaliar regras processador & atualiza valor medido na regra
        for (int i = 0; i < simulacao.getHabitacao()->getLinhas(); ++i) {
            for (int j = 0; j < simulacao.getHabitacao()->getColunas(); ++j) {
                Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);
                if (zonaAtual) {
                    if(zonaAtual->getNumProcessadores()>0){
                        for(int k=0;k<zonaAtual->getNumProcessadores();k++){
                            zonaAtual->getProcessador(k)->atualizaRegra();
                            for(int y=0;y<zonaAtual->getProcessador(k)->getNumAparelhos();y++){

                                if(zonaAtual->getProcessador(k)->getComando()=="liga" && zonaAtual->getProcessador(k)->getAparelho(y).isLigado()){
                                    //se aparelho ligado e comando ligar nao mandar comando
                                    aux_mandar_comando=0;
                                }else if(zonaAtual->getProcessador(k)->getComando()=="desliga" && !zonaAtual->getProcessador(k)->getAparelho(y).isLigado()){
                                    //se aparelho desligado e comando desligar nao mandar comando
                                    aux_mandar_comando=0;
                                }else aux_mandar_comando=1;

                                if (aux_mandar_comando==1) {
                                    if (zonaAtual->getProcessador(k)->getComando() == "desliga") {
                                        zonaAtual->getAparelho(y)->setInstante_desligou(instante);
                                    } else if (zonaAtual->getProcessador(k)->getComando() == "liga") {
                                        zonaAtual->getAparelho(y)->setInstante_ligou(instante);
                                    }
                                }

                                if(zonaAtual->getProcessador(k)->mandar_comando(instante,aux_mandar_comando)){
                                    zonaAtual->atualizaAparelho();
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //COMANDOS PARA GERIR HABITAÇÃO E ZONAS
    if(comando=="hnova"){
        if(arg1.empty() || arg2.empty()){
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente] \n";
            return;
        }

        try{
            int n_linhas=stoi(arg1);
            int n_colunas=stoi(arg2);

            if (n_linhas>4 || n_linhas<2 || n_colunas>4 || n_colunas<2){
                w<< set_color(4) << "[Capacidade minima [2x2], maxima[4x4]]\n";
                return;
            }

            if(simulacao.getHabitacao() != nullptr){
                w << set_color(4) << "[Precisa de remover primeiro a habitacao existente]\n";
                return;
            }

            simulacao.setHabitacao(new Habitacao(n_linhas, n_colunas));

            // apagas as windows existentes
            clearWindows();

            // cria as windows e mete-as no vetor
            for (int j = 0; j < n_linhas; ++j) {
                for (int i = 0; i < n_colunas; ++i) {
                    windows.emplace_back(8 + i * 27, 3 + j * 9, 22, 8);
                }
            }

            w<<set_color(10)<<"Nova habitacao criada ["<<n_linhas<<"x"<<n_colunas<<"]\n";
        }
        catch(const std::invalid_argument &e){
            w<< set_color(4) << "[ERRO,  argumento(s) invalido(s)]\n";
            return;
        }

    }
    else if(comando=="hrem"){
        w<<set_color(10)<<"[Habitacao removida]\n";
        delete simulacao.getHabitacao();
        simulacao.setHabitacao(nullptr);
        clearWindows();
        instante=0;
    }
    else if(comando=="znova"){
        if(arg1.empty() || arg2.empty()){
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int linha=stoi(arg1);
            int coluna=stoi(arg2);

            if(!simulacao.getHabitacao()){
                w<< set_color(4) << "[Habitacao nao foi criada]\n";
                return;
            }

            if(coluna>simulacao.getHabitacao()->getColunas() || linha>simulacao.getHabitacao()->getLinhas() ||coluna<=0 || linha<=0){
                w<< set_color(4) << "Capacidade maxima da habitacao["<<simulacao.getHabitacao()->getLinhas()<<"x"<<simulacao.getHabitacao()->getColunas()<<"]\nPrimeira posicao [1x1]"<<"\n";
                return;
            }

            for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i,j);

                    if (zonaAtual && zonaAtual->getLinha() == linha && zonaAtual->getColuna() == coluna) {
                        w << set_color(4) << "[Ja existe uma zona criada nas coordenadas que inseriu]\n";
                        return;
                    }
                }
            }

            int indiceJanela = (linha - 1) * simulacao.getHabitacao()->getColunas() + (coluna - 1);

            // cria nova zona aqui
            Zona zonaNova(simulacao.getIdZona(), linha, coluna,indiceJanela);
            string infoZona = zonaNova.descricao();

            simulacao.getHabitacao()->adicionaZona(zonaNova, linha-1, coluna-1);
            windows[simulacao.getHabitacao()->getZona(linha-1,coluna-1)->getIndiceJanela()]<<"ID Zona - ["<<zonaNova.getId()<<"]";
            simulacao.incrementaIdZona();

            w<<set_color(10)<<"Nova zona criada coordenada ["<<linha<<"x"<<coluna<<"]\n";
            w<<set_color(0)<<"Info da nova zona criada:\n"<<infoZona<< "\n";
        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO,  argumento(s) invalido(s)]\n";
            return;
        }

    }
    else if(comando=="zrem"){
        if(arg1.empty()){
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona=stoi(arg1);

            if(!simulacao.getHabitacao()){
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }
            for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                    if(simulacao.getHabitacao()->getZona(i, j) != nullptr && id_zona==simulacao.getHabitacao()->getZona(i,j)->getId()){
                        windows[simulacao.getHabitacao()->getZona(i,j)->getIndiceJanela()].clear();
                        int idZonaRemovida = simulacao.getHabitacao()->getZona(i, j)->getId();
                        simulacao.getHabitacao()->removeZona(i, j);
                        w <<set_color(10)<< "A Zona com id [" << idZonaRemovida << "] foi apagada!\n";
                        return;
                    }
                }
            }
            w << set_color(4)<< "A Zona com o id ["<<id_zona<<"] nao existe\n";
        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO,  argumento(s) invalido(s)]\n";
            return;
        }

    }
    else if(comando=="zlista"){
        if(simulacao.getHabitacao()) {
            w<<set_color(10)<<"Lista de zonas:\n";
            for (int i = 0; i < simulacao.getHabitacao()->getLinhas(); ++i) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); ++j) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);
                    if (zonaAtual) {
                        w << set_color(0) << zonaAtual->descricao() << "\n";
                        w << set_color(10) << "--------------" << "\n";
                    }
                }
            }
        }else{
            w << set_color(4) << "[Nao existe habitacao]\n";
        }
    }
        //COMANDOS PARA GERIR ZONAS E SEU CONTEÚDO
    else if(comando=="zcomp"){
        if(arg1.empty()){
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona=stoi(arg1);
            bool zonaEncontrada = false;

            if(!simulacao.getHabitacao()){
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }

            for (int i = 0; i < simulacao.getHabitacao()->getLinhas(); ++i) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); ++j) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);

                    if (zonaAtual && zonaAtual->getId()==id_zona) {
                        zonaEncontrada=true;
                        w<<set_color(10)<< "Componentes da zona com ID["<< id_zona <<"]\n";
                        w<< set_color(0)<<zonaAtual->descricaoComponentes();
                        return;
                    }
                }
            }
            if(!zonaEncontrada){
                w<< set_color(4)<<"Nao existe zona com ID inserido\n";
            }

        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO,  argumento(s) invalido(s)]\n";
            return;
        }
    }
    else if(comando=="zprops"){
        if(arg1.empty()){
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona = stoi(arg1);
            bool zonaEncontrada = false;

            if(!simulacao.getHabitacao()) {
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }

            for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i,j);

                    if (zonaAtual && zonaAtual->getId() == id_zona) {
                        zonaEncontrada = true;
                        w<<set_color(10) <<"Propriedades da zona ID ["<<id_zona<<"]\n";
                        for(int p=0;p<simulacao.getHabitacao()->getZona(i,j)->getNumProps();p++){
                            w <<set_color(0)<<simulacao.getHabitacao()->getZona(i,j)->getProp(p).getChave()<<": "<<simulacao.getHabitacao()->getZona(i,j)->getProp(p).getValor()<<"\n";

                        }
                    }
                }
            }
            if(!zonaEncontrada){
                w << set_color(4) << "[ERRO], ID nao existe\n";
            }
        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO,  argumento(s) invalido(s)]\n";
            return;
        }
    }
    else if(comando=="pmod"){
        if(arg1.empty() || arg2.empty() || arg3.empty()){
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona=stoi(arg1);
            string nomeProp= arg2;
            int valor=stoi(arg3);

            if(!simulacao.getHabitacao()) {
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }

            for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i,j);

                    if (zonaAtual && zonaAtual->getId() == id_zona) {
                        nomeProp[0] = (char)std::toupper(nomeProp[0]);
                        for (int k = 1; k < nomeProp.size(); k++) {
                            nomeProp[k] = (char)std::tolower(nomeProp[k]);
                        }

                        if(zonaAtual->setPropsByName(nomeProp,valor)==1){
                            w << set_color(10)<< "Propriedade alterada com sucesso\n";
                            return;
                        }else{
                            w << set_color(4)<< "Propriedade fora dos limites\n ";
                            return;
                        }

                    }
                }
            }
            w << set_color(4)<< "A Zona com o id ["<<id_zona<<"] nao existe\n";

        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO, argumento(s) invalido(s)]\n";
            return;
        }
    }
    else if (comando == "cnovo") {
        if (arg1.empty() || arg2.empty() || arg3.empty()) {
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona=stoi(arg1);
            string tipo_comp=arg2;
            string tipo_comp_in=arg3;
            bool zona_existe=false;

            if(!simulacao.getHabitacao()) {
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }

                for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
                    for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                        Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);
                        if(zonaAtual && zonaAtual->getId()==id_zona){
                            zona_existe=true;

                            if(zonaAtual->adicionaComponente(tipo_comp,tipo_comp_in,id_ap,id_s,id_p,instante)==1){
                                w << set_color(10)<<"[Componente adicionado com sucesso] \n";
                            }

                            if(tipo_comp=="a" & tipo_comp_in=="a" || tipo_comp=="a" & tipo_comp_in=="s"||tipo_comp=="a" & tipo_comp_in=="r"||tipo_comp=="a" & tipo_comp_in=="l"){ //aparecer nas zonas o sensor/aparelho e o seu id ao lado
                                windows[zonaAtual->getIndiceJanela()]<<"\n-"<<tipo_comp_in<<id_ap<<"-";
                                id_ap++;
                            }else if(tipo_comp=="s" & tipo_comp_in=="t" || tipo_comp=="s" & tipo_comp_in=="v"||tipo_comp=="s" & tipo_comp_in=="m"||tipo_comp=="s" & tipo_comp_in=="d"||tipo_comp=="s" & tipo_comp_in=="h" || tipo_comp=="s" & tipo_comp_in=="o"||tipo_comp=="s" & tipo_comp_in=="f"){
                                windows[zonaAtual->getIndiceJanela()]<<"\n-"<<tipo_comp_in<<id_s<<"-";
                                id_s++;
                            }else if(tipo_comp=="p"){
                                windows[zonaAtual->getIndiceJanela()]<<"\n-"<<tipo_comp<<id_p<<"-";
                                id_p++;
                            }
                        }
                    }
                }
            if(!zona_existe){
                w<< set_color(4)<<"[Zona nao existe ]\n";
            }

        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO, argumento(s) invalido(s)]\n";
            return;
        }
    }
    else if (comando == "crem") {
        if (arg1.empty() || arg2.empty() || arg3.empty()) {
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona=stoi(arg1);
            int id=stoi(arg3);
            bool zona_existe=false;

            if(!simulacao.getHabitacao()) {
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }

            for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);

                    if(zonaAtual && zonaAtual->getId()==id_zona){
                        zona_existe=true;

                        if(zonaAtual->removeComponente(arg2,id,id,id)==1){
                            w << set_color(10)<<"[Componente removido com sucesso]\n";
                            windows[zonaAtual->getIndiceJanela()].clear();
                            windows[zonaAtual->getIndiceJanela()] << zonaAtual->getConteudoZona();
                        }else{
                            w << set_color(4)<<"[ID de componente nao existe]\n";
                        }
                    }
                }
            }
            if(!zona_existe){
                w<< set_color(4)<<"[Zona nao existe]\n";
            }
        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO, argumento(s) invalido(s)]\n";
            return;
        }
    }
        //COMANDOS PARA PROCESSADORES DE REGRAS
    else if(comando == "rnova"){
        if (arg1.empty() || arg2.empty() || arg3.empty() || arg4.empty() || arg5.empty() || arg6.empty()) {
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona=stoi(arg1);
            int id_proc_regra=stoi(arg2);
            int id_sensor=stoi(arg4);
            int x=stoi(arg5);
            int y=stoi(arg6);

            bool zona_existe=false;
            bool proc_existe=false;
            bool sens_existe=false;

            if(!simulacao.getHabitacao()) {
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }

            for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);

                    if(zonaAtual){
                        zona_existe=true;
                    }
                    if(zonaAtual && zonaAtual->getNumProcessadores()>0){
                        for(int k=0;k<zonaAtual->getNumProcessadores();k++) {
                            if(zonaAtual->getId()==id_zona && zonaAtual->getProcessador(k)->getId_Proc()==id_proc_regra) {
                                    for(int z=0;z<zonaAtual->getNumSensores();z++) {
                                        if (zonaAtual->getSensor(z)->getId() == id_sensor && zonaAtual->getSensor(z) != nullptr) {
                                            if(zonaAtual->getProcessador(k)->adicionaRegra(arg3, id_r,x, y, zonaAtual->getSensor(z)->getValorMedido(),zonaAtual->getSensor(z))==1){
                                                w << set_color(10)<<"[Regra adicionada com sucesso]\n";
                                                id_r++;
                                                sens_existe=true;
                                                proc_existe=true;
                                            }else{
                                                sens_existe=true;
                                                proc_existe=true;
                                                w << set_color(4)<<"[Tipo de regra nao existe]\n";
                                            }

                                        }
                                    }

                            }
                        }
                    }
                }
            }

            if(!zona_existe){
                w<< set_color(4)<<"Zona nao existe\n";
                return;
            }

            if(!proc_existe || !sens_existe){
                w<< set_color(4)<<"Processador ou Sensor nao existem\n";
                return;
            }

        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO, argumento(s) invalido(s)]\n";
            return;
        }
    }
    else if(comando=="pmuda"){
        if (arg1.empty() || arg2.empty() || arg3.empty()) {
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona=stoi(arg1);
            int id_proc_regra=stoi(arg2);

            bool zona_existe=false;
            bool processadorID_encontrado=false;

            if(!simulacao.getHabitacao()) {
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }

            for(int i = 0; i < simulacao.getHabitacao()->getLinhas(); i++) {
                for(int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                    Zona* zonaAtual = simulacao.getHabitacao()->getZona(i, j);

                    if(zonaAtual && zonaAtual->getId() == id_zona) {
                        zona_existe = true;

                        bool processador_encontrado = false;
                        for(int k = 0; k < zonaAtual->getNumProcessadores(); k++) {
                            if(zonaAtual->getProcessador(k)->getId_Proc() == id_proc_regra) {
                                processador_encontrado = true;
                                zonaAtual->getProcessador(k)->setComando(arg3);
                                w << set_color(10) << "Comando do processador alterado com sucesso.\n";
                                break;
                            }
                        }

                        if(!processador_encontrado) {
                            w << set_color(4) << "[Processador com ID especificado nao encontrado na zona]\n";
                        }
                    }
                }
            }
            if(!zona_existe){
                w<< set_color(4)<<"[Zona nao existe]\n";
            }
        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO, argumento(s) invalido(s)]\n";
            return;
        }
    }
    else if(comando=="rlista"){
        if (arg1.empty() || arg2.empty()) {
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona=stoi(arg1);
            int id_proc_regra=stoi(arg2);
            bool proc_existe=false;
            bool zona_existe=false;

            if(!simulacao.getHabitacao()) {
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }

            for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);

                    if(zonaAtual && zonaAtual->getId()==id_zona) {
                        zona_existe = true;
                    }

                    if(zonaAtual && zonaAtual->getNumProcessadores()>0){
                        for(int k=0;k<zonaAtual->getNumProcessadores();k++){
                                if(zonaAtual->getId()==id_zona){
                                    if(zonaAtual->getProcessador(k)->getId_Proc()==id_proc_regra){
                                        proc_existe=true;
                                        w << set_color(10)<<"Regras do processador ID["<<id_proc_regra<<"]\n";
                                        w<< set_color(0)<<zonaAtual->getProcessador(k)->lista_regras();
                                    }
                                }
                        }

                    }
                }
            }
            if(!zona_existe){
                w<< set_color(4)<<"Zona nao existe\n";
                return;
            }
            if(!proc_existe){
                w<< set_color(4)<<"Processador de regras nao existe\n";
            }

        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO, argumento(s) invalido(s)]\n";
            return;
        }
    }
    else if(comando=="rrem"){
        if (arg1.empty() || arg2.empty() || arg3.empty()) {
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona=stoi(arg1);
            int id_proc_regra=stoi(arg2);
            int id_regra=stoi(arg3);
            bool zona_existe=false;
            bool id_proc_regra_existe=false;
            bool id_regra_existe=false;

            if(!simulacao.getHabitacao()) {
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }

            for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);

                    if(zonaAtual)
                        zona_existe=true;

                    if (zonaAtual && zonaAtual->getNumProcessadores() > 0) {
                        for (int k = 0; k < zonaAtual->getNumProcessadores(); k++) {
                            if (zonaAtual && zonaAtual->getId() == id_zona && zonaAtual->getProcessador(k)->getId_Proc() == id_proc_regra) {
                                id_proc_regra_existe=true;
                                if(zonaAtual->getProcessador(k)->removeRegra(id_regra)==1){
                                    w<< set_color(10)<<"Regra removida com sucesso\n";
                                    id_regra_existe=true;
                                }
                            }
                        }
                    }
                }
            }
            if(!zona_existe){
                w << set_color(4)<<"Zona nao existe\n";
                return;
            }
            if(!id_proc_regra_existe){
                w << set_color(4)<<"ID processador nao existe\n";
                return;
            }
            if(!id_regra_existe){
                w << set_color(4)<<"ID regra nao existe\n";
                return;
            }

        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO, argumento(s) invalido(s)]\n";
            return;
        }
    }
    else if(comando=="asoc"){
        if (arg1.empty() || arg2.empty() || arg3.empty()) {
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona=stoi(arg1);
            int id_proc_regra=stoi(arg2);
            int id_aparelho=stoi(arg3);
            bool zona_existe=false;
            bool aparelho_existe=false;
            bool proc_regra_existe=false;

            if(!simulacao.getHabitacao()) {
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }

            for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);

                    if(zonaAtual)
                        zona_existe=true;

                    if (zonaAtual && zonaAtual->getNumProcessadores() > 0 && zonaAtual->getId()==id_zona) {
                        for(int k=0;k<zonaAtual->getNumProcessadores();k++){
                            if(zonaAtual->getProcessador(k)->getId_Proc()==id_proc_regra){
                                proc_regra_existe=true;
                                for(int y=0;y<zonaAtual->getNumAparelhos();y++){
                                    if(zonaAtual->getAparelho(y)->getId()==id_aparelho){
                                        aparelho_existe=true;
                                        if((zonaAtual->getProcessador(k)->adicionaAparelho(zonaAtual->getAparelho(y)->getLetra(),zonaAtual->getAparelho(y)->isLigado(),zonaAtual->getAparelho(y)->getId(),instante))==1){
                                            w<< set_color(10)<<"Ligacao estabelecida com sucesso\n";
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if(!zona_existe){
                w << set_color(4)<<"Zona nao existe\n";
                return;
            }
            if(!proc_regra_existe){
                w << set_color(4)<<"ID processador nao existe\n";
                return;
            }
            if(!aparelho_existe){
                w << set_color(4)<<"ID aparelho nao existe\n";
                return;
            }

        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO, argumento(s) invalido(s)]\n";
            return;
        }
    }
    else if(comando=="ades"){
        if (arg1.empty() || arg2.empty() || arg3.empty()) {
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona=stoi(arg1);
            int id_proc_regra=stoi(arg2);
            int id_aparelho=stoi(arg3);
            bool zona_existe=false;
            bool aparelho_existe=false;
            bool proc_regra_existe=false;

            if(!simulacao.getHabitacao()) {
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }

            for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);

                    if(zonaAtual)
                        zona_existe=true;

                    if (zonaAtual && zonaAtual->getNumProcessadores() > 0 && zonaAtual->getId()==id_zona) {
                        for(int k=0;k<zonaAtual->getNumProcessadores();k++){
                            if(zonaAtual->getProcessador(k)->getId_Proc()==id_proc_regra){
                                proc_regra_existe=true;
                                if((zonaAtual->getProcessador(k)->removeAparelho(id_aparelho))==1){
                                    w<< set_color(10)<<"Ligacao removida com sucesso\n " ;
                                    aparelho_existe=true;
                                }
                            }
                        }
                    }
                }
            }

            if(!zona_existe){
                w << set_color(4)<<"Zona nao existe\n";
                return;
            }
            if(!proc_regra_existe){
                w << set_color(4)<<"ID processador nao existe\n";
                return;
            }
            if(!aparelho_existe){
                w << set_color(4)<<"ID aparelho nao existe\n";
                return;
            }
        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO, argumento(s) invalido(s)]\n";
            return;
        }
    }
        //COMANDOS PARA INTERAGIR COM APARELHOS
    else if(comando=="acom"){
        if (arg1.empty() || arg2.empty() || arg3.empty()) {
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona=stoi(arg1);
            int id_aparelho=stoi(arg2);
            bool zona_existe=false;
            bool aparelho_existe=false;

            if(!simulacao.getHabitacao()) {
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }

            for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);

                    if(zonaAtual)
                        zona_existe=true;

                    if (zonaAtual && zonaAtual->getNumAparelhos() > 0 && zonaAtual->getId()==id_zona) {
                        for(int k=0;k<zonaAtual->getNumAparelhos();k++){
                            if(zonaAtual->getAparelho(k)->getId()==id_aparelho){
                                aparelho_existe=true;
                                if(arg3=="liga"){
                                    zonaAtual->getAparelho(k)->setLigado(true);
                                    zonaAtual->getAparelho(k)->setInstante_ligou(instante);
                                    w<< set_color(10)<<"[Comando alterado com sucesso]\n ";
                                }else if(arg3=="desliga"){
                                    zonaAtual->getAparelho(k)->setLigado(false);
                                    zonaAtual->getAparelho(k)->setInstante_desligou(instante);
                                    zonaAtual->getAparelho(k)->setUltimo_Incremento(0);
                                    w<< set_color(10)<<"[Comando alterado com sucesso]\n";
                                }else w<< set_color(4)<<"[Comando nao reconhecido]\n";
                            }
                        }
                    }

                }
            }

            if(!zona_existe){
                w << set_color(4)<<"Zona nao existe\n";
                return;
            }

            if(!aparelho_existe){
                w << set_color(4)<<"ID aparelho nao existe\n";
                return;
            }

        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ERRO, argumento(s) invalido(s)]\n";
            return;
        }
    }
        //COMANDOS PARA CÓPIA/RECUPERAÇÃO DE PROCESSADORES DE REGRAS
    else if(comando=="psalva"){
        if (arg1.empty() || arg2.empty() || arg3.empty()) {
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        try{
            int id_zona=stoi(arg1);
            int id_proc_regra=stoi(arg2);
            std::string nome=arg3;

            bool proc_existe=false;
            bool zona_existe=false;

            if(!simulacao.getHabitacao()) {
                w << set_color(4) << "[Nao existe habitacao]\n";
                return;
            }

            for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
                for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                    Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);

                    if(zonaAtual && zonaAtual->getId()==id_zona) {
                        zona_existe = true;
                    }

                    if(zonaAtual && zonaAtual->getNumProcessadores()>0){
                        for(int k=0;k<zonaAtual->getNumProcessadores();k++){
                            //if(zonaAtual->getId()==id_zona){
                                if(zonaAtual->getProcessador(k)->getId_Proc()==id_proc_regra){
                                    proc_existe=true;
                                    if(simulacao.guardaProcessador(*zonaAtual->getProcessador(k),nome)==1){
                                        w <<set_color(10)<< "[Processador guardado com sucesso] \n";
                                    }else w <<set_color(4)<< "Ja existe um processador guardado com esse nome \n";

                                }
                            //}
                        }

                    }
                }
            }
            if(!zona_existe){
                w<< set_color(4)<<"Zona nao existe\n";
                return;
            }
            if(!proc_existe){
                w<< set_color(4)<<"Processador nao existe\n";
            }

        }
        catch(const std::invalid_argument &e){
            w << set_color(4) << "[ argumento(s) invalido(s)]\n";
            return;
        }
    }
    else if(comando=="prepoe"){
        if (arg1.empty()) {
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }
        bool processador_guardado_existe=false;
        bool processador_antigo_existe=false;
        bool zona_existe=false;

        for(int i=0;i<simulacao.getNumProcessadoresGuardados();i++){
            if(simulacao.getProcessadorGuardado(i).getNome()==arg1){
                processador_guardado_existe=true;

                for(int k=0;k<simulacao.getHabitacao()->getLinhas();k++) {
                    for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {
                        Zona *zonaAtual = simulacao.getHabitacao()->getZona(k, j);
                        if(zonaAtual && simulacao.getProcessadorGuardado(i).getIdZonaAssociada()==zonaAtual->getId()){
                            zona_existe=true;

                            if(zonaAtual->getNumProcessadores()!=0){
                                for(int y=0;y<zonaAtual->getNumProcessadores();y++){
                                    if(zonaAtual->getProcessador(y)->getId_Proc()==simulacao.getProcessadorGuardado(i).getId_Proc()){
                                        zonaAtual->substituiProc(simulacao.getProcessadorGuardado(i),zonaAtual);
                                    }else{
                                        zonaAtual->adicionaProcessador(simulacao.getProcessadorGuardado(i));
                                        windows[zonaAtual->getIndiceJanela()]<<"\n-"<<"p"<<simulacao.getProcessadorGuardado(i).getId_Proc()<<"-" ;
                                    }
                                }
                            }else if(zonaAtual->getNumProcessadores()==0){ //caso numero de processadores seja 0 nao precisa de ciclo para iterar sobre os processadores
                                zonaAtual->adicionaProcessador(simulacao.getProcessadorGuardado(i));
                                windows[zonaAtual->getIndiceJanela()]<<"\n-"<<"p"<<simulacao.getProcessadorGuardado(i).getId_Proc()<<"-" ;
                            }

                        }

                    }
                }
            }
        }

        if(!zona_existe){
            w<< set_color(4)<<"A zona ja nao existe\n";
            return;
        }

        if(!processador_guardado_existe){
            w<< set_color(4)<<"O processador nao existe na lista\n";
        }

        w <<set_color(10)<< "[Processador reposto com sucesso]\n";
    }
    else if(comando=="prem"){
        if (arg1.empty()) {
            w<<set_color(4)<<"[ERRO, insira argumentos corretamente]\n";
            return;
        }

        std::string nome=arg1;
        bool proc_existe = false;

        if(simulacao.removeProcGuardado(nome) == 1){
            proc_existe=true;
            w << set_color(10) << "[Processador eliminado com sucesso]\n";
        }

        if(!proc_existe){
            w << set_color(4) << "Processador nao encontrado\n";
        }
    }
    else if(comando=="plista"){
        if (simulacao.getNumProcessadoresGuardados()==0) {
            w << set_color(4) << "Nao existem processadores guardados" << "\n";
        } else {
            w << set_color(10) << "[Lista de processadores guardados:]\n";

            for (int i = 0; i < simulacao.getNumProcessadoresGuardados(); ++i) {
                Processador processadorInfo = simulacao.getProcessadorGuardado(i);
                w << set_color(0) << "\nNome: " << processadorInfo.getNome() << "\n";
                w << set_color(0) << "ID do Processador: " << processadorInfo.getId_Proc() << "\n";
                w << set_color(0) << "ID da Zona: " << processadorInfo.getIdZonaAssociada() << "\n";
                w << set_color(0) << "Regras:\n" << processadorInfo.lista_regras() << "\n";
                w << set_color(0) << "--------------" << "\n";
            }
        }
    }
    else if(comando=="clear"){
        w.clear();
    }
    else if(comando !="prox" && comando!="avanca"){
        w<<set_color(4)<<"[O comando nao foi encontrado]\n";
    }

    if(simulacao.getHabitacao()){
        std::string letraAparelho;

        for(int i=0;i<simulacao.getHabitacao()->getLinhas();i++) {
            for (int j = 0; j < simulacao.getHabitacao()->getColunas(); j++) {

                Zona *zonaAtual = simulacao.getHabitacao()->getZona(i, j);

                if(zonaAtual){
                    if(zonaAtual->getNumAparelhos() > 0){
                        for (int k = 0; k < zonaAtual->getNumAparelhos(); ++k) {
                            if(zonaAtual->getAparelho(k)->isLigado()){
                                letraAparelho = (char)std::toupper(zonaAtual->getAparelho(k)->getLetra()[0]);
                                zonaAtual->getAparelho(k)->setLetra(letraAparelho);
                                windows[zonaAtual->getIndiceJanela()].clear();
                                windows[zonaAtual->getIndiceJanela()] << zonaAtual->getConteudoZona();
                            }
                            else{
                                letraAparelho = (char)std::tolower(zonaAtual->getAparelho(k)->getLetra()[0]);
                                zonaAtual->getAparelho(k)->setLetra(letraAparelho);
                                windows[zonaAtual->getIndiceJanela()].clear();
                                windows[zonaAtual->getIndiceJanela()] << zonaAtual->getConteudoZona();
                            }
                        }
                    }
                }
            }
        }
    }

}