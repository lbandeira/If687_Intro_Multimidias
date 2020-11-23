# ESP32 com Micropython v1.9.4

## Preparação do Ambiente

### Linux
#### Pip

```
   sudo apt install python-pip
```

#### Esptool

```
   pip install esptool
```
  [Esptool GitHub](https://github.com/espressif/esptool)

#### Picocom

```
   apt-get install picocom
```

#### Ampy

```
   apt-get install ampy
```


### Windows
#### Python

#### uPycraft

Esse programa é uma IDE micropython para Windows com design simples e facil de se utilizar. Para instalar, utilize este [link](https://raw.githubusercontent.com/DFRobot/uPyCraft/master/uPyCraft.exe)

Para melhor utilizar essa IDE, siga o [uPyCraft Tutorial](https://www.embarcados.com.br/micropython-no-esp8266/) e **selecione como firmware o .bin, que está na pasta base-firmware**

#### Putty

Para apenas acessar a porta serial e monitorar o dispositivo, utilizamos o Putty, o qual você pode baixar nesse [link](https://www.putty.org/)

## Como dar Build e Flash na placa

   #### Linux
   **1.** Conecte a placa no computador e pelo terminal vá ate a pasta esp32/Micropython

   **2.** Digite o seguinte comando:
```
   make all
```
Com esse comando, o programa irá instalar o firmware da pasta base-firmware e carregar os códigos que estão na pasta src

   #### Windows
   **1.** Conecte a placa no computador 

   **2.** Abra o uPyCraft

   **3.** Siga esse [Tutorial](https://www.embarcados.com.br/micropython-no-esp8266/) de como fazer upload na placa, mas utilize o firmware que se encontra na pasta base-firmware

## Como carregar os códigos na placa

   #### Linux
   **1.** Conecte a placa no computador e pelo terminal vá ate a pasta esp32

   **2.** Digite o seguinte comando:
```
   make build
```
Com esse comando, o programa irá carregar os códigos que estão na pasta src
  
  #### Windows

   Utilize o uPyCraft

## Como abrir o serial monitor

```
   picocom -b 115200 /dev/ttyUSB0
```

## Source (src)
   * main.py: Código principal




