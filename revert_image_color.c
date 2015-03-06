/* Given an image, revert the pixel colors */
#include <stdio.h>
#include <stdlib.h>

/* Struct que descreve o cabecalho do arquivo BMP */
#pragma pack(1)  // ensure structure is packed
typedef struct header
{
	char magic_number1;
	char magic_number2;
	unsigned int size_bmpfile;
	unsigned int application;
	unsigned int offset;
	unsigned int header;
	unsigned int width;
	unsigned int height;
	unsigned short int number_colorplanes;
	unsigned short int number_bitspixel;
	unsigned int bi_rgb;
	unsigned int size_rawbmpdata;
	unsigned int horizontal_resolution;
	unsigned int vertical_resolution;
	unsigned int number_colorspallete;
	unsigned int allcolorsimportant;
} Header;

/* Struct que descreve um pixel */
typedef struct pixel_rgb
{
	unsigned char B;
	unsigned char G;
	unsigned char R;
} Pixel;

/*
   Funcao le_cabecalho
Descricao:
Le os dados do cabecalho do arquivo BMP
*/

void le_cabecalho(Header *cabecalho, FILE* arq_entrada){
	fread(&cabecalho->magic_number1, sizeof(char), 1, arq_entrada);
	fread(&cabecalho->magic_number2, sizeof(char), 1, arq_entrada);
	fread(&cabecalho->size_bmpfile, sizeof(unsigned int), 1, arq_entrada);
	fread(&cabecalho->application, sizeof(unsigned int), 1, arq_entrada);
	fread(&cabecalho->offset, sizeof(unsigned int), 1, arq_entrada);
	fread(&cabecalho->header, sizeof(unsigned int), 1, arq_entrada);
	fread(&cabecalho->width, sizeof(unsigned int), 1, arq_entrada);
	fread(&cabecalho->height, sizeof(unsigned int), 1, arq_entrada);
	fread(&cabecalho->number_colorplanes, sizeof(unsigned short int), 1, arq_entrada);
	fread(&cabecalho->number_bitspixel, sizeof(unsigned short int), 1, arq_entrada);
	fread(&cabecalho->bi_rgb, sizeof(unsigned int), 1, arq_entrada);
	fread(&cabecalho->size_rawbmpdata, sizeof(unsigned int), 1, arq_entrada);
	fread(&cabecalho->horizontal_resolution, sizeof(unsigned int), 1, arq_entrada);
	fread(&cabecalho->vertical_resolution, sizeof(unsigned int), 1, arq_entrada);
	fread(&cabecalho->number_colorspallete, sizeof(unsigned int), 1, arq_entrada);
	fread(&cabecalho->allcolorsimportant, sizeof(unsigned int), 1, arq_entrada);

	printf("Magic number:%c%c\n", cabecalho->magic_number1, cabecalho->magic_number2);
	printf("Size of the BMP file:%u\n", cabecalho->size_bmpfile);
	printf("Application cabecalho:%u\n", cabecalho->application);
	printf("Offset: %u\n", cabecalho->offset);
	printf("Header: %u\n", cabecalho->header);
	printf("Width: %u\n", cabecalho->width);
	printf("Height: %u\n", cabecalho->height);
	printf("Number of color planes: %hu\n", cabecalho->number_colorplanes);
	printf("Number bits/pixel: %d \n", cabecalho->number_bitspixel);
	printf("BI_RGB: %u\n", cabecalho->bi_rgb);
	printf("Size of the raw BMP cabecalho: %u\n", cabecalho->size_rawbmpdata);
	printf("Horizontal resolution of the image: %u\n", cabecalho->horizontal_resolution);
	printf("Vertical resolution of the image: %u\n", cabecalho->vertical_resolution);
	printf("Number of colors in the palette: %u\n", cabecalho->number_colorspallete);
	printf("All colors are important: %u\n", cabecalho->allcolorsimportant);

}

/*  Funcao aloca_linhas
Descricao:
Para armazenar a matriz de pixels, são necessários "height" ponteiros para identificar os vetores das linhas.
Esta funcao aloca, portanto, o espaço de memória necessário para armazenar "height" ponteiros para vetores de estruturas Pixel.
Entrada:
height -> Dimensao vertical da matriz, i.e., o numero de linhas na matriz de pixels
Saida:
Ponteiro para uma regiao de memoria capaz de armazenar "height" ponteiros para (ponteiros para estruturas Pixel)
*/
struct pixel_rgb** aloca_linhas(int height){

	return malloc(height*sizeof(char));

}

/*  Funcao aloca_coluna
Descricao:
Para armazenar a matriz de pixels, são necessários "width" ponteiros para identificar os valores de uma coluna.
Esta funcao aloca, portanto, o espaço de memória necessário para armazenar "width" ponteiros para estruturas Pixel.
Entrada:
width -> Dimensao horizontal da matriz, i.e., o numero de colunas na matriz de pixels
Saida:
Ponteiro para uma regiao de memoria capaz de armazenar "width" ponteiros para estruturas Pixel
*/

struct pixel_rgb* aloca_coluna(int width){

	return malloc(width*sizeof(Pixel));

}

void le_matriz_pixels(Pixel** mat, Header header, FILE* arq_input)
{
	int i, j, k;
	char lixo;
	for(i=header.height-1; i>-1;i--)
	{
		for(j=0; j<header.width;j++)
		{
			fread(&mat[i][j], sizeof(Pixel), 1, arq_input);
		}

		//pegar o pading
		if(calcula_padding(header.width)!=0)
		{
			for(k=0;k<calcula_padding(header.width);k++)
			{
				fread(&lixo, sizeof (char), 1, arq_input);
			}
		}
	}
}

void InvertRGB(Pixel* in){
	in->R &= 255;
	in->R = 255 - in->R;

	in->G &= 255;
	in->G = 255 - in->G;

	in->B &= 255;
	in->B = 255 - in->B;
}

/*  Funcao calcula_padding
Descricao:
Por restricao do formato BMP todas as linhas da matriz de pixels devem necessariamente possuir um tamanho multiplo de 4.
Por isso, temos que calcular a quantidade de bytes a ser acrescida no final da linha para garantir essa restrição.
Desta forma, devemos calcular um valor X tal que "X" mais o "tamanho da linha" (sizeof(Pixel)*width) seja multiplo de 4.

Entrada:
width -> Dimensao horizontal da matriz, i.e., o numero de colunas na matriz de pixels
Saida:
quantidade de bytes extra a serem acrescidos ao final da linha para que ela tenha tamanho múltiplo de 4

*/
int calcula_padding(int width){
	int npad = (sizeof(Pixel)*width) & 3;
	if (npad)
		  npad = 4-npad;
	return npad;
}

/*
   Funcao aplica_filtro
Descricao:
Tendo a matriz de pixels mat preenchida, devemos inverter as cores de cada um dos pixels.
Lembrem-se:
1) A largura e a altura da matriz são dados do cabeçalho
2) Preto vale 0 0 0     e seu inverso (branco)  vale 255 255 255
2) Azul  vale 0 0 255   e seu inverso (laranja) vale 255 255 0

Ao final desta função, todos os valores
Entrada:
mat     -> Matriz bidimensional de estruturas Pixel
header  -> Estrutura de cabecalho do arquivo BMP
Saida:
void
*/
void aplica_filtro(Pixel** mat, Header header){
	int i, j;
	for(i=header.height-1; i>-1;i--)
	{
		for(j=0; j<header.width;j++)
		{
			InvertRGB(&mat[i][j]);
		}
	}
}

/*
   Funcao salva_versao_editada
Descricao:
De forma analoga a leitura, a gravacao deve ser feita na mesma ordem e da mesma forma.
Primeiramente os dados do cabecalho e em seguida os dados do mapa de bits.
Por fim, existem dois zeros que fecham o arquivo.

Dica para o cabecalho:
Mantenha o mesmo padrao e sequencia que foi utilizada na leitura

Dicas para o mapa de bits / matriz de pixels:
Lembre-se de colocar o pading no final de cada linha
Lembre-se que a imagem é escrita da ultima linha para a primeira

Entrada:
mat     -> Matriz bidimensional de estruturas Pixel
header  -> Estrutura de cabecalho do arquivo BMP
Saida:
void
*/

void salva_versao_editada(Header header, FILE* arq_output, Pixel** mat){
	int i, j, k;
	char lixo;

	fwrite(&header, sizeof(Header),1,arq_output);

	for(i=header.height-1; i>-1;i--)
	{
		for(j=0; j<header.width;j++)
		{
			fwrite(&mat[i][j],sizeof(Pixel),1,arq_output);
		}

		if(calcula_padding(header.width)!=0)
		{
			for(k=0;k<calcula_padding(header.width);k++)
			{
				fwrite(&lixo, sizeof (char), 1, arq_output);
			}
		}
	}
	fclose(arq_output);
}

int main (int argc, char *argv[])
{
	int i,j,k,padding;
	char nome_arquivo[50], x = 0;
	FILE *arq_input;
	FILE *arq_output;
	Pixel **mat;
	Header header;


	scanf("%s", nome_arquivo);
	arq_input = fopen (nome_arquivo, "r+b");
	if (arq_input == NULL)
	{
		printf("O arquivo nao pode ser aberto.\n");
		exit(2);
	}

	le_cabecalho(&header, arq_input);

	printf("Cabecalho lido\n");
	mat = aloca_linhas(header.height);

	for(i=0; i<header.height; i++) {
		mat[i] = aloca_coluna(header.width);
	}

	padding = calcula_padding(header.width);

	le_matriz_pixels(mat, header, arq_input);
	printf("leu matrix");

	fclose(arq_input);

	arq_output = fopen("inverso.bmp", "w+b");
	if (arq_output == NULL) {
		printf ("O arquivo nao pode ser aberto.\n");
		exit (1);
	}

	aplica_filtro(mat, header);
	salva_versao_editada(header, arq_output, mat);

	fclose(arq_output);
	for (i=0;i<header.width;i++)
	{
		free(mat[i]);
	}
	free(mat);

	return 0;

}

