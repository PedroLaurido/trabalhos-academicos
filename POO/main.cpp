#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "curses.h"
#include "Terminal.h"
#include <iomanip>
#include "cmd.h"
#include "Simulacao.h"

using namespace term;

void print_size(Terminal& t) {
    std::ostringstream o;
    o << "tamanho do terminal: " << std::setw(7) << t.getNumCols() << "x" << t.getNumRows();
    std::string str = o.str();
    t.clear();
    t << set_color(0) << move_to(t.getNumCols()-str.length(), t.getNumRows()-1) << str;
}

int main() {
    Simulacao simulacao;

    Terminal &t = Terminal::instance();
    string input;
    string comando,arg1,arg2,arg3,arg4,arg5,arg6;
    int instante=0,id_ap=1,id_s=1,id_p=1,id_r=1;


    for(int i=1; i<20; i++) {
        t.init_color(i, i, 0);
    }

    print_size(t);
    Window whabit = Window(2, 1, 115, 40);
    Window wcomand = Window(120, 1, 40, 40);

    do{

        whabit<< move_to(99 ,37)<<"Instante: "<<instante;

        t<< no_color() << move_to(0, 41) <<set_color(0)<<"\nInsira o comando: ";
        t>>input;

        istringstream iss(input);
        iss>>comando>>arg1>>arg2>>arg3>>arg4>>arg5>>arg6; //Separar o input em <comando><argumentos>

        if(comando == "exec")
        {
            if (arg1.empty()) {
                wcomand<< set_color(4) << "[ERRO, insira argumentos corretamente]";
                continue;
            }
            wcomand << set_color(0) <<"[A carregar ficheiro de texto com comandos...]\n" ;

            // Abrir o ficheiro
            ifstream file(arg1); // arg1 tem o nome do ficheiro
            if (!file) {
                wcomand << "Nao foi possivel abrir o arquivo.";
                continue;
            }

            // Ler e executar os comandos do ficheiro
            string linha_fich;
            while (getline(file, linha_fich)) {
                istringstream iss2(linha_fich);
                iss2 >> comando >> arg1 >> arg2 >> arg3 >> arg4 >> arg5 >> arg6;
                processarComando(comando, arg1, arg2, arg3, arg4,arg5,arg6,simulacao, wcomand,instante,id_ap,id_s,id_p,id_r);
                arg1.clear();
                arg2.clear();
                arg3.clear();
                arg4.clear();
            }
            file.close(); // Fechar o ficheiro após a leitura completa.
        }
        else{
            processarComando(comando, arg1, arg2, arg3, arg4,arg5,arg6, simulacao, wcomand,instante,id_ap,id_s,id_p,id_r);
            arg1.clear();
            arg2.clear();
            arg3.clear();
            arg4.clear();
        }

        move(42 , 0); // Move o cursor para o início da linha 42
        clrtoeol(); // Limpa a linha 42 da tela
        refresh(); // Atualiza a tela
    }while(comando!="sair");
    return 0;
}
