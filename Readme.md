# Projeto EmbarcaTech 
## Monitoramento da Temperatura Interna e Externa com BitDogLab no Raspberry Pi Pico W

<div style="text-align: center;">

![Texto alternativo](/html_files/logo_embarcatech.png "EmbarcaTech")

</div>
<!-- TABLE OF CONTENTS --> 
<details open="open">
  <summary>Indície</summary>
  <ol>
    <li>
      <a href="#objetivo">Objetivo</a>
    </li>
    <li>
      <a href="#kit-de-desenvolvimento">Kit de Desenvolvimento</a>
    </li>
    <li>
      <a href="#sistema-de-arquivos">Sistema de Arquivos</a>
    </li>
    <li>
      <a href="#fluxograma-do-firmware">Fluxograma do Firmware</a>
    </li>
    <li>
      <a href="#esquemático">Esquemático</a>
    </li>
    <li>
      <a href="#anexos">Anexos</a>
    </li>
    <li>
      <a href="#sobre">Sobre</a>
    </li>
  </ol>
</details>

<!-- objetivo -->
### Objetivo

<p align="justify">O principal objetivo deste projeto é desenvolver um sistema de monitoramento eficiente e integrado para o Kit BitDogLab, com foco no Raspberry Pi Pico W, placa microcontroladora do Kit BitDogLab, que foi instalado um sensor externo DHT11 para medir a temperatura e umidade do ar, e um display OLED SSD1306, nativo do Kit, com comunicação I2C, protocolo de comunicação serial desenvolvido pela Philips, para exibir informações em tempo real. O projeto também inclui a criação de uma página Web, permitindo a visualização remota da temperatura interna, externa e da umidade do ar. Essas métricas serão analisadas para identificar condições ambientais que possam impactar negativamente no desempenho do microcontrolador, garantindo maior confiabilidade no processamento das informações. 
Além disso, o projeto fornecerá um exemplo de código funcional, permitindo que outros projetistas e desenvolvedores utilizem e adaptem as soluções para seus próprios projetos, promovendo a reutilização e a expansão do conhecimento técnico.
O projeto foi desenvolvido na linguagem de programação C, o código fonte principal, a página Web em HTML e Python para converter a página Web em hexadecimal.</p>

<!-- kit-de-desenvolvimento -->
### Kit de Desenvolvimento

O Kit de Desenvolvimento BitDogLab possui os seguintes componentes e sensores na placa:
</p>

•	1 Bateria;

•	1 Display OLED 128x64 pixels;

•	1 Matriz de LEDs coloridos RGB de 5 X 5;

•	1 Microfone;

•	1 Joystick;

•	2 Botões;

•	2 Buzzer;

•	1 Conector de expansão com 14 GPIO´s sendo 3(três) GND, 1(um) 3.3v e 1(um) 5v.

Abaixo temos uma imagem do kit:
 
![Texto alternativo](/doc/bitdoglab.png "BitDogLab")

<!-- sistema-de-arquivos -->
### Sistema de Arquivos

![Texto alternativo](/doc/arquivos.PNG "Arquivos")

O projeto é composto pelos seguintes arquivos:
 
•	build - na pasta build contém os arquivos após o projeto ser compilado;
<p align="justify">
•	html_files - na pasta html_files contém os arquivos da página HTML que exibirá os dados do Raspberry Pi Pico:
</p>
<p align="justify">
1. logo_embarcatech.png – é a imagem do Projeto EmbarcaTech inserida na página Web;
</p>

<p align="justify">
2. index.shtml – é o código HTML da página Web que exibirá os dados do Raspberry Pi Pico.
</p>
<p align="justify">
•	.gitignore – é o arquivo responsável por setar os arquivos que não serão enviado para o Git;
</p>

<p align="justify">
•	cgi.h - é onde são definidas as funções do Raspberry Pi Pico que irá realizar na página e as rotas das páginas Web. Ex.:  ligar um led;
</p>

<p align="justify">
•	CMakeLists.txt - ele é utilizado pelo CMake, uma ferramenta de automação de compilação que organiza e gerencia o processo de geração de arquivos de build para compilar o projeto. 
O que ele faz: 
</p>

<p align="justify">
1. Configura o SDK do Pico: indica ao CMake onde encontrar o SDK do Raspberry Pi Pico, que contém funções e bibliotecas necessárias para programar o microcontrolador.
</p>

<p align="justify">
2. Define o projeto: estabelece o nome do projeto e suas propriedades, como o idioma de programação (geralmente C ou C++).
</p>

<p align="justify">
3. Especifica os arquivos fonte: lista os arquivos .c ou .cpp que fazem parte do projeto e devem ser compilados.
</p>

<p align="justify">
4. Inclui bibliotecas: adiciona dependências de bibliotecas externas ou internas (como drivers para periféricos ou funções padrão do Pico).
</p>

<p align="justify">
5. Define opções de compilação: configura flags do compilador, macros ou outros parâmetros específicos para a build.
</p>

<p align="justify">
6. Gera arquivos para o build system: cria os arquivos que o make (ou outro sistema) usará para compilar o projeto.
</p>

<p align="justify">
•	htmldata.c – este arquivo contém a página Web em formato hexadecimal;
</p>

<p align="justify">
•	lwipopts.h - é um componente essencial na configuração do lwIP (Lightweight IP), que  é uma pilha de protocolos de rede leve e eficiente, projetada especificamente para sistemas embarcados e dispositivos com recursos limitados, como microcontroladores. O lwipopts.h é usado para personalizar e ajustar o comportamento da pilha de protocolos de rede de acordo com as necessidades do projeto. Este arquivo permite configurar diversas opções, como recursos habilitados, tamanhos de buffers, e otimizações para memória e desempenho; 
</p>

<p align="justify">
•	makefsdata.py – é o executável em linguagem Python responsável por converter a página Web em hexadecimal gerando o arquivo htmldata.c;
</p>

<p align="justify">
•	pico_sdk_import.cmake - é um arquivo CMake script fornecido pelo SDK do Raspberry Pi Pico que facilita a configuração do ambiente de desenvolvimento, garantindo que todas as dependências e bibliotecas do Pico SDK sejam corretamente incluídas no processo de compilação do seu projeto;
</p>

<p align="justify">
•	ssd1306.c – é o drive responsável por configurar e acessar o display OLED, escrevendo os dados no mesmo;
</p>

<p align="justify">
•	ssd1306.h – é o arquivo cabeçalho do ssd1306.c, que contém declarações (não definições) de funções, variáveis globais, macros e classes. Essas declarações permitem que outros arquivos saibam o que existe em uma determinada biblioteca ou módulo;
</p>

<p align="justify">
•	ssi.h - é utilizado para inserir dados na páginas Web.
</p>

<!-- fluxograma-do-firmware -->
### Fluxograma do Firmware

Abaixo temos o fluxograma do sistema.

![Texto alternativo](/doc/fluxograma_url_codigo.png "Fluxograma")

<!-- esquemático -->
### Esquemático 

Esquema do circuito elétrico
![Texto alternativo](/doc/Esquematico.PNG "Fluxograma")

<!-- anexos -->
### Anexos

<p>Iniciando o Kit BitDogLab</p>

![Texto alternativo](/doc/01.png "Iniciando")

<p>Tela Inicial</p>

![Texto alternativo](/doc/02.png "Tela Inicial")

<p>Tela da Temperatura Externa</p>

![Texto alternativo](/doc/temp_ext.jpeg "Tela da Temperatura Externa")

<p>Tela da Umidade do Ar</p>

![Texto alternativo](/doc/04.png "Tela da Umidade do Ar")

<p>Tela do IP</p>

![Texto alternativo](/doc/05.png "Tela da IP")

<p>Tela da Página Web</p>

![Texto alternativo](/doc/06.png "Tela da IP")


<!-- sobre -->
## Sobre

Autor: Thiago Vilarinho Lemes <br>
LinkedIn <a href="https://www.linkedin.com/in/thiago-v-lemes-b1232727" target="_blank">Thiago Lemes</a><br>
e-mail: contatothiagolemes@gmail.com | lemes_vilarinho@yahoo.com.br