#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <tchar.h>
#include <windows.h>
#include <shlobj.h> // Para SHGetFolderPath

using namespace std;

/// <Constantes e globais>
const int LS = 260; // Tamanho limite de strings
char diretorioDeCriacao[LS];
size_t t = 0; // Controla o tamanho de exibi��o da fun��o de alterar notas

#ifndef UNLEN // Verifica se UNLEN j� foi definido
#define UNLEN 256
#endif

extern "C" {
	WINBASEAPI BOOL WINAPI GetUserNameA(LPSTR, LPDWORD); // Obter o nome do usu�rio associado ao processo
}

/// <Prot�tipos>
void chamaMenu();
int criarNota(_TCHAR  directoryPath[LS]);
void getUserName(char diretorioDeCriacao[]);
void geraDiretorio(char username[], char diretorioDeCriacao[]);
int dir(const _TCHAR* directoryPath);
void excluirArquivo(char diretorioDeCriacao[]);
void alteraNota(char diretorioDeCriacao[]);
void exibe_apos_alterar(char titulo[]);

/// <Objetos de manipula��o de arquivo>
ofstream fout;
ifstream fin;

/// <Fun��es>
int main() {
	chamaMenu();

	return 0;
}

void chamaMenu() {
	int opcao;
	_TCHAR directoryPath[LS]; //Armazena o diret�rio do desktop do usu�rio atual
	do {
		system("CLS");
		cout << "\tMenu de Opcoes" << endl
			<< " 1 - Criar nova nota" << endl
			<< " 2 - Excluir nota" << endl
			<< " 3 - Editar nota" << endl
			<< " 4 - Listar todas as notas" << endl
			<< " 5 - Sair" << endl
			<< " | Selecione uma opcao: ";
		cin >> opcao;
		switch (opcao) {
		case 1:
			system("CLS"); //Limpa a tela antes de entrar na fun��o selecionada
			criarNota(directoryPath);
			break;
		case 2:
			system("CLS");
			excluirArquivo(diretorioDeCriacao);
			break;
		case 3:
			system("CLS");
			alteraNota(diretorioDeCriacao);
			break;
		case 4:
			system("CLS");
			dir(directoryPath);
			break;
		case 5:
			break;
		default:
			cerr << "Opcao invalida. Tente novamente!" << endl;
		}
		system("PAUSE");
	} while (opcao != 5);
}

int criarNota(_TCHAR directoryPath[LS]) {
	char* titulo = new char[LS];
	char* diretorioDeCriacao = new char[LS];
	getUserName(diretorioDeCriacao); //Recebe de volta o diret�rio do arquivo
	cout << " | Qual o titulo da nota que deseja criar?" << endl;
	cin >> titulo;
	strcat(diretorioDeCriacao, titulo); //Concatena o diret�rio e o t�tulo
	strcat(diretorioDeCriacao, ".bin"); //Adiciona a extens�o do arquivo
	fout.open(diretorioDeCriacao, ios::binary); //Abrindo o arquivo neste diret�rio
	size_t tamanhoTitulo = strlen(titulo);
	fin.read(titulo, tamanhoTitulo);
	cout << "Nota criada com sucesso!" << endl;
	fout.close();

	// Obtendo o caminho do Desktop do usu�rio
	if (SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, directoryPath) != S_OK) {
		cerr << "Erro ao obter o diretorio do Desktop" << endl;
		return -1;
	}
	delete[] titulo;
	delete[] diretorioDeCriacao;
	return 0;
}

int dir(const _TCHAR* directoryPath) {

	cout << " | Notas no diretorio: " << endl;

	const basic_string<_TCHAR> targetExtension = _T(".bin"); // Definindo a extens�o dos arquivos

	WIN32_FIND_DATA findFileData; // Struct para armazenar os dados do arquivo encontrado
	HANDLE hFind = FindFirstFile((basic_string<_TCHAR>(directoryPath) + _T("\\*")).c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		cerr << "Erro ao abrir o diretorio | Tente criar uma nova nota" << endl;
		return -1;
	}

	do { // Fazendo a itera��o sobre os arquivos do diret�rio
		basic_string<_TCHAR> fileName = findFileData.cFileName; // Obt�m o nome do arquivo atual
		size_t extPos = fileName.find_last_of(_T('.')); // Encontra a posi��o do �ltimo ponto no nome do arquivo. Usado para identificar a extens�o

		if (extPos != basic_string<_TCHAR>::npos) { // Se a extens�o existe
			basic_string<_TCHAR> fileExtension = fileName.substr(extPos); // armazena ela em fileExtension

			if (fileExtension == targetExtension) { // Compara a extens�o do arquivo com a extens�o definida acima
				wcout << findFileData.cFileName << endl;
			}
		}
	} while (FindNextFile(hFind, &findFileData) != 0); // Executa enquanto existir arquivos a serem analisados

	FindClose(hFind); // Fecha o identificador
	return 0;
}

// Altera a string, adicionando o nome do usu�rio
void getUserName(char diretorioDeCriacao[]) {
	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;

	if (!GetUserNameA(username, &username_len)) {
		cerr << "Erro ao obter o nome do usuario" << endl;
		return;
	}
	strcpy(diretorioDeCriacao, username);
	geraDiretorio(username, diretorioDeCriacao);
}

// Define o caminho onde o arquivo ser� salvo
void geraDiretorio(char username[], char diretorioDeCriacao[]) {
	strcpy(diretorioDeCriacao, ""); //Limpa o conte�do da vari�vel
	strcat(diretorioDeCriacao, "C:\\Users\\");
	strcat(diretorioDeCriacao, username);
	strcat(diretorioDeCriacao, "\\Desktop\\"); //Escolhi o Desktop como diret�rio padr�o
}

void excluirArquivo(char diretorioDeCriacao[]) {
	char* arquivo_a_Excluir = new char[LS];
	char* username = new char[LS];
	cout << " | Digite o nome do arquivo que deseja excluir [Sem a extensao]:";
	cin >> arquivo_a_Excluir;
	cout << " | Tem certeza?" << endl
		<< " 1 - Sim\t2 - Nao" << endl;
	int opcao;
	cin >> opcao;
	switch (opcao) {
	case 1:
		cout << " | Seguindo com a exclusao" << endl;
		break;
	case 2:
		cout << " | Exclusao cancelada" << endl;
		system("PAUSE");
		return;
	default:
		cerr << " | Opcao invalida. Cancelando." << endl;
		return;
	}
	getUserName(diretorioDeCriacao);
	strcat(diretorioDeCriacao, arquivo_a_Excluir);
	strcat(diretorioDeCriacao, ".bin");
	strcpy(arquivo_a_Excluir, diretorioDeCriacao); // arquivo_a_Excluir agora armazena o diret�rio completo, incluindo o nome e extens�o
	cout << arquivo_a_Excluir << endl;
	// Verifica se o arquivo existe e tenta excluir
	if (remove(arquivo_a_Excluir) != 0) {
		cerr << "Erro ao excluir o arquivo." << endl;
		return;
	}
	cout << "Arquivo excluido com sucesso." << endl;
	delete[] arquivo_a_Excluir;
	delete[] username;
}

void exibe_apos_alterar(char titulo[]) {
	char* leNota = new char[LS];
	fin.open(titulo, ios::binary);
	if (!fin.is_open()) {
		cerr << "Erro ao abrir o arquivo. Encerrando.";
		return;
	}
	fin.read(leNota, t); // L� os caracteres para dentro da string leNota
	for (int i = 0; i < t; i++) {
		cout << leNota[i]; // Exibe cada caractere separadamente, incluindo espa�os
	}
	cout << endl;
	fin.close();
	cout << " | Fim do arquivo!" << endl;
	delete[] leNota;
}

void alteraNota(char diretorioDeCriacao[]) {
	char* titulo = new char[LS];
	_TCHAR* directoryPathh = new _TCHAR[LS];
	getUserName(diretorioDeCriacao);
	if (SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, directoryPathh) != S_OK) {
		cerr << "Erro ao obter o diret�rio do Desktop" << endl;
		return;
	}
	dir(directoryPathh); //Acessando o diret�rio das notas
	cout << " | Digite o titulo da nota que deseja alterar:";
	cin >> titulo;
	char* guardaTitulo = new char[LS];
	strcpy(guardaTitulo, titulo);
	strcpy(titulo, diretorioDeCriacao);
	strcat(titulo, guardaTitulo);
	strcat(titulo, ".bin"); //O titulo agora armazena o diret�rio(a nota) que o usu�rio digitou
	fout.open(titulo, ios_base::app);
	if (!fout.is_open()) {
		cerr << "Erro ao abrir o arquivo. Encerrando.";
		return;
	}
	cout << " | Comece a editar!" << endl;
	string alterarNota;
	cin.ignore(); //Limpando o buffer para usar o getline
	getline(cin, alterarNota); //L� at� o usu�rio usar Enter
	fout.write(alterarNota.c_str(), alterarNota.size());
	fout.close();

	int opc;
	cout << " | Exibir nota?" << endl
		<< " 1 - Sim\t2 - Nao" << endl;
	cin >> opc;
	switch (opc) {
	case 1:
		exibe_apos_alterar(titulo);
		break;
	case 2:
		return;
	default:
		cerr << "Opcao invalida. Saindo.";
		return;
	}
	t += alterarNota.size(); // t armazena o tamanho total de caracteres na nota
	delete[] titulo;
	delete[] directoryPathh;
	delete[] guardaTitulo;
}