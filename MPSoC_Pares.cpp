#include <bits/stdc++.h>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

//Modificando esta Função é possível mapear por Nodos Ímpares
int verificaPar(int x, int y){
    x = x%2;
    y = y%2;

    if(x == 0 && y == 0){
        return 1;
    }
    if(x != 0 && y == 0) {
        return 2;
    }
    if(x == 0 && y != 0){
        return 3;
    }
    if(x != 0 && y != 0){
        return 4;
    }
    else{
        return 0;
    }
}

int main() {

    //Iterar Matriz
    int srcX, srcY, trgX, trgY, numPacotes, x, y;

    ifstream arqTam("TrabalhoMapeamento/Test1.json");
    json Mesh = json::parse(arqTam);

    //Tamanho das Matrizes
    std::string x1 = Mesh["MPSOC_SIZE_X"];
    std::string y1 = Mesh["MPSOC_SIZE_Y"];

    int numLin = stoi(x1);
    int numCol = stoi(y1);

    //Auxiliares e Posições
    int posX = 0;
    int posY = 0;
    int auxX;
    int auxY;
    int auxPacotes = 0;
    int aux = 0;
    int auxR = 0;
    int posVector = 0;

    //Vectors
    vector<int> todasPosicoes;
    vector<int> vetNumPacotes;
    vector<std::string> todasTarefas;
    vector<std::string> tarefasRepetidas;

    //Strings retiradas dos Arquivos
    std::string tarefaOrigem_str;
    std::string tarefaDestino_str;
    std::string tarefaApp;
    std::string qtdStr;

    int qntdVezes;
    int tamGrafo;

    bool isRepetido = false;

    //Matrizes
    std::string matrizString[numLin][numCol];
    int matrizMPSoC[numLin][numCol];

    json testes = Mesh["TEST"];

    int tam = testes.size();

    for(int test=0; test < tam; test++){

        //Zerar Matrizes
        for(int linha=0;linha<numLin;linha++){
            for(int coluna=0;coluna<numCol;coluna++){
                matrizMPSoC[linha][coluna] = 0;
            }
        }

        for(int linha=0;linha<numLin;linha++){
            for(int coluna=0;coluna<numCol;coluna++){
                matrizString[linha][coluna] = "-";
            }
        }

        tarefaApp = testes[test]["APP"];
        qtdStr = testes[test]["QTD"];
        
        qntdVezes = stoi(qtdStr);

        ifstream arqAPP("TrabalhoMapeamento/Applications/" + tarefaApp + ".json");
        json grafo_tarefas = json::parse(arqAPP);

        tamGrafo = grafo_tarefas["grafo_tarefas"].size();

        cout << "Quantidades: " << qntdVezes << endl;
        cout << "Aplicacao: " << tarefaApp << endl;

        for(int vez = 1; vez <= qntdVezes; vez++){
            for(int grafo = 0; grafo < tamGrafo; grafo++){
                //Pega Tarefas
                std::string vezStr = "_" + to_string(vez);
                std::string placeholder = "+";

                tarefaOrigem_str = grafo_tarefas["grafo_tarefas"][grafo]["tarefa_origem"];
                tarefaDestino_str = grafo_tarefas["grafo_tarefas"][grafo]["tarefa_destino"];
                
                if(vez != 1){
                    tarefaOrigem_str = tarefaOrigem_str.append(vezStr);
                    tarefaDestino_str = tarefaDestino_str.append(vezStr);
                    placeholder = placeholder.append("_+");
                }

                //Salva em um Vector as tarefas, e no outro um Placeholder "+"
                //Caso for tarefa repetida, salva em ambos Vectors
                if(find(todasTarefas.begin(), todasTarefas.end(), tarefaOrigem_str) != todasTarefas.end()){
                    tarefasRepetidas.push_back(tarefaOrigem_str);
                } else {
                    tarefasRepetidas.push_back(placeholder);
                }
                
                todasTarefas.push_back(tarefaOrigem_str);

                if(find(todasTarefas.begin(), todasTarefas.end(), tarefaDestino_str) != todasTarefas.end()){
                    tarefasRepetidas.push_back(tarefaDestino_str);
                } else {
                    tarefasRepetidas.push_back(placeholder);
                }

                todasTarefas.push_back(tarefaDestino_str);

                vetNumPacotes.push_back(grafo_tarefas["grafo_tarefas"][grafo]["quantidade_pacotes"]);
            }
        }

        //Mapeia a Matriz com Strings e anota Posições
        for(int valorPassada = 1; valorPassada <= 4; valorPassada++){
            for(int i=0; i < numLin; i++){
                for(int j=0; j < numCol; j++){
                    if(verificaPar(i, j) == valorPassada){
                        if(aux < todasTarefas.size()){
                            if(aux > 0){
                                //Verifica se já não existe dentro da Matriz
                                if(todasTarefas[aux] == tarefasRepetidas[auxR]){
                                    isRepetido = true;
                                    
                                    //Procura sua posição na Matriz
                                    for(int auxX = 0; auxX < numLin; auxX++){
                                        for(int auxY = 0; auxY < numCol; auxY++){
                                            if(auxR < tarefasRepetidas.size()){
                                                if(matrizString[auxX][auxY] == tarefasRepetidas[auxR]){
                                                    posX = auxX;
                                                    posY = auxY;
                                                    todasPosicoes.push_back(posX);
                                                    todasPosicoes.push_back(posY);
                                                    cout << todasTarefas[aux] << " Repetido em: " << posX << "," << posY << endl;
                                                    aux++;
                                                    auxR++;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    //Diminui j(Eixo Y) para que tente inserir outro elemento na mesma posição
                                    j--;
                                }
                                if(!isRepetido){
                                    matrizString[i][j] = todasTarefas[aux];
                                    posX = i;
                                    posY = j;
                                    todasPosicoes.push_back(posX);
                                    todasPosicoes.push_back(posY);
                                    aux++;
                                    auxR++;
                                }


                            } else {
                                matrizString[i][j] = todasTarefas[aux];
                                posX = i;
                                posY = j;
                                todasPosicoes.push_back(posX);
                                todasPosicoes.push_back(posY);
                                aux++;
                                auxR++;
                            }
                            isRepetido = false;
                        }
                    }
                }
            }
        }
        aux = 0;
        auxR = 0;
        isRepetido = false;

        cout << "Matriz Mapeada: " << endl;

        for(int linha=0;linha<numLin;linha++){
            if(linha == 0){
                cout << "XY --------------------->" << endl;
            }
            cout << "| ";
            for(int coluna=0;coluna<numCol;coluna++){
                cout << "|" << matrizString[linha][coluna] << "|";
            }
            cout << endl;
            if(linha == (numLin - 1)){
                cout << "V " << endl;
            }
        }

        posVector = 0;
        auxPacotes = 0;

        for(int vez = 1; vez <= qntdVezes; vez++){
            for(int grafo = 0; grafo < tamGrafo; grafo++){
                srcX = todasPosicoes[posVector];
                srcY = todasPosicoes[posVector+1];
                trgX = todasPosicoes[posVector+2];
                trgY = todasPosicoes[posVector+3];
                //cout << srcX << "," << srcY << " ate " << trgX << "," << trgY << endl;

                numPacotes = vetNumPacotes[auxPacotes];

                if(srcX > trgX){
                    y=-1;
                    for(int i=srcX; i >= trgX; i--){
                        if(srcY > trgY){
                            if(y != trgY){
                                for(int j=srcY; j >= trgY; j--){
                                    //cout << "Proc[" << i << "][" << j << "] NAO EH O DESTINO" << endl;
                                    matrizMPSoC[i][j] += numPacotes;
                                    //cout << "Pacotes 1 = " << matrizMPSoC[i][j] << endl;

                                    y = j;
                                    if(j == trgY){
                                        i--;
                                        break;
                                    }

                                }
                            }
                        }
                        if(y != trgY){
                            for(int j=srcY; j <= trgY; j++){
                                //cout << "Proc[" << i << "][" << j << "] NAO EH O DESTINO" << endl;
                                matrizMPSoC[i][j] += numPacotes;
                               // cout << "Pacotes 2 = " << matrizMPSoC[i][j] << endl;

                                y = j;
                                if(j == trgY){
                                    i--;
                                    break;
                                }

                            }
                        }

                        if(i == trgX){
                            //cout << "Proc[" << i << "][" << y << "] EH O DESTINO" << endl;
                            matrizMPSoC[i][y] += numPacotes;
                            //cout << "Pacotes 3 = " << matrizMPSoC[i][y] << endl;
                            break;
                        } else if (i != trgX){
                            //cout << "Proc[" << i << "][" << y << "] NAO EH O DESTINO" << endl;
                            matrizMPSoC[i][y] += numPacotes;
                            //cout << "Pacotes 4 = " << matrizMPSoC[i][y] << endl;
                        }
                    }
                }
                else if(srcX < trgX){
                    for(int i=srcX; i <= trgX; i++){
                        if(srcY > trgY){
                            if(y != trgY){
                                for(int j=srcY; j >= trgY; j--){
                                    //cout << "Proc[" << i << "][" << j << "] NAO EH O DESTINO" << endl;
                                    matrizMPSoC[i][j] += numPacotes;
                                    //cout << "Pacotes = " << matrizMPSoC[i][j] << endl;

                                    y = j;
                                    if(j == trgY){
                                        i++;
                                        break;
                                    }
                                }
                            }
                        } else {
                            if(y != trgY){
                                for(int j=srcY; j <= trgY; j++){
                                    //cout << "Proc[" << i << "][" << j << "] NAO EH O DESTINO" << endl;
                                    matrizMPSoC[i][j] += numPacotes;
                                    //cout << "Pacotes = " << matrizMPSoC[i][j] << endl;

                                    y = j;
                                    if(j == trgY){
                                        i++;
                                        break;
                                    }
                                }
                            }
                        }

                        if(i == trgX){
                            //cout << "Proc[" << i << "][" << y << "] EH O DESTINO" << endl;
                            matrizMPSoC[i][y] += numPacotes;
                            //cout << "Pacotes = " << matrizMPSoC[i][y] << endl;
                            break;
                        } else if (i != trgX) {
                            //cout << "Proc[" << i << "][" << y << "] NAO EH O DESTINO" << endl;
                            matrizMPSoC[i][y] += numPacotes;
                            //cout << "Pacotes = " << matrizMPSoC[i][y] << endl;
                        }
                    }
                } else if(srcX == trgX){
                    for(int i=srcX; i <= trgX; i++){
                        if(srcY > trgY){
                            if(y != trgY){
                                for(int j=srcY; j >= trgY; j--){
                                    y = j;
                                    if(j == trgY){
                                        break;
                                    }
                                    //cout << "Proc[" << i << "][" << j << "] NAO EH O DESTINO" << endl;
                                    matrizMPSoC[i][j] += numPacotes;
                                    //cout << "Pacotes = " << matrizMPSoC[i][j] << endl;

                                }
                            }
                        } else {
                            if(y != trgY){
                                for(int j=srcY; j <= trgY; j++){
                                    y = j;
                                    if(j == trgY){
                                        break;
                                    }
                                    //cout << "Proc[" << i << "][" << j << "] NAO EH O DESTINO" << endl;
                                    matrizMPSoC[i][j] += numPacotes;
                                    //cout << "Pacotes = " << matrizMPSoC[i][j] << endl;

                                }
                            }
                        }

                        if(i == trgX){
                            //cout << "Proc[" << i << "][" << y << "] EH O DESTINO" << endl;
                            matrizMPSoC[i][y] += numPacotes;
                            //cout << "Pacotes = " << matrizMPSoC[i][y] << endl;
                            break;
                        } else if (i != trgX) {
                            //cout << "Proc[" << i << "][" << y << "] NAO EH O DESTINO" << endl;
                            matrizMPSoC[i][y] += numPacotes;
                            //cout << "Pacotes = " << matrizMPSoC[i][y] << endl;
                        }
                    }
                }

                posVector += 4;
                auxPacotes++;
            }
        }
        cout << "Matriz com Pacotes: " << endl;

        for(int linha=0;linha<numLin;linha++){
            if(linha == 0){
                cout << "XY --------------------->" << endl;
            }
            cout << "| ";
            for(int coluna=0;coluna<numCol;coluna++){
                cout << "|" << matrizMPSoC[linha][coluna] << "|";
            }
            cout << endl;
            if(linha == (numLin - 1)){
                cout << "V " << endl;
            }
        }

        todasTarefas.clear();
        tarefasRepetidas.clear();
        todasPosicoes.clear();
        vetNumPacotes.clear();
        cout << "<<<------Fim da Aplicacao "<< tarefaApp << "------>>>" << endl;
    }

  return 0;
}
