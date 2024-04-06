#include <iostream>
#include "controleLista.cpp"
using namespace std;



// Funcao para exibir o menu e obter a escolha do usuario
int mostrarMenu() {
    cout << "Menu:" << endl;
    cout << "0. Sair" << endl;
    cout << "1. adicionar" << endl;
    cout << "2. exibir" << endl;
    cout << "Escolha uma opcao: ";

    int escolha;
    cin >> escolha;

    return escolha;
}

int main() {
    Lista lista;
    bool sair = false;

    while (!sair) {
        int escolha = mostrarMenu();

        switch (escolha) {
            case 0:
                cout << "Saindo do programa." << endl;
                sair = true;
                break;
            case 1:
                cout << "Voce escolheu a opcao 1." << endl;
                    novoProduto(lista);
                break;
            case 2:
                cout << "Voce escolheu a opcao 2." << endl;
                // Faca o que precisa ser feito para a opcao 2
                exibir(lista);
                somaCarrinho(lista);
                break;
            case 3:
                cout << "Voce escolheu a opcao 3." << endl;
                // Faca o que precisa ser feito para a opcao 3
                break;
            default:
                cout << "Opcao invalida. Por favor, escolha uma opcao valida." << endl;
                break;
        }
    }

    return 0;
}
