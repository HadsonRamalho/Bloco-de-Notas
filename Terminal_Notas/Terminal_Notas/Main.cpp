#include <iostream>
#include <fstream>

using namespace std;

/// <Protótipos>
void chamaMenu();

int main() {
	chamaMenu();

	return 0;
}

void chamaMenu() {
	int opcao;
	do {
		cout << "\tMenu de Opcoes" << endl
			<< " 1 - Criar nova nota" << endl
			<< " 2 - Excluir nota" << endl
			<< " 3 - Editar nota" << endl
			<< " 4 - Listar todas as notas" << endl
			<< " 5 - Sair" << endl;
		cin >> opcao;
	} while (opcao != 5);
}