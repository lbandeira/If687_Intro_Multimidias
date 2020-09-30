# ###################################################
#            SALVUS ATAS-TEMP    V2.0.1            #
# Codigo principal do ATAS TEMP, o qual            #
# possui funcionalidades como:                     #
#    * Deepsleep ESP de 30min          |   OK      #
#    * Deepsleep SIGFOX                |   OK      #
#    * Alimentacao do sensor pelo pino |   OK      #
#    * Monitoramento de bateria        |   OK      #
#    * Checkin de sensores             |   OK      #
#    * Checkin do sigfox               |   OK      #
#                                                  # 
# Para excucao do codigo no picocom, utilize:      #
#                                                  #
#       exec(open("main.py").read())               #
#                                                  #
# ###################################################


import machine
import time
import dht
import onewire
from sigfox import Sigfox, message
from ds18x20 import DS18X20
from machine import Pin
from machine import ADC

valorR2 = 47000
valorR1 = 10000
AMOSTRAS = 2.0
DHT = False

def main():
#
# VERSAO DO CODIGO
#

    print('\033[33m'+'\n=============== VERSAO 2.0.1 ===============\n'+'\033[0;0m')

#
# Checkup do DEEPSLEEP
#
    start = time.ticks_ms()
    print("Time for booting: ", start)
    if machine.reset_cause() == machine.DEEPSLEEP_RESET:
        if machine.wake_reason() == machine.PIN_WAKE:
            print("Device woke up from deepsleep, because a interruption")
        elif machine.wake_reason() == 3:
            print("Device woke up from deepsleep, because of time")

    period = 1800000 # 3600000 = 1 hora | 1800000 = 30min | 60000 = 1min


#
# Inicialização dos pinos de controle
#
    pinSIGFOX = Pin(32, Pin.OUT)
    pinSIGFOX.on()

    pinTEMP = Pin(2, Pin.OUT)
    pinTEMP.on()
    
    pinADC = Pin(34, Pin.IN)
    adc2 = ADC(pinADC)
    
    try:
        print("Tentando o DHT\n")
        ds = dht.DHT22(machine.Pin(4))
        time.sleep(1) #Time to DHT start
        ds.measure()
        DHT = True
        print('\033[32m'+'DHT OK'+'\033[0;0m')
        temp = ds.temperature()
        hum = ds.humidity()
        bat = read_bat(adc2)

    except:
        print("Tentando o DS18b20\n")
        ds18b = DS18X20(onewire.OneWire(Pin(4)))
        addrs = ds18b.scan()
        DHT = False

        if not addrs:
            print('\033[31m'+'ERROR - SENSOR NAO ENCONTRADO'+'\033[0;0m')
            temp = 153.56 # 999 Hex =  Error - Sensor not found
            hum = 255 
            bat = read_bat(adc2)

        else:
            print('\033[32m'+'DS18b20 OK'+'\033[0;0m')
            hum = 0
            temp = read_temp_ds(ds18b, addrs)
            bat = read_bat(adc2)

#
# Debug
#    
    print('\033[33m'+'\n============== MENSAGEM SIGFOX =============='+'\033[0;0m')
    print('\nTemperatura', temp)
    print('\nUmidade', hum)
    print('\nBateria', bat)
    print('\033[33m''=============================================\n'+'\033[0;0m')


#
# Preparação e envio de mensagem
#
    send_message_sigfox(temp,hum,bat)

#
# Inicio do processo de DeepSleep
#
    tic = time.ticks_ms() - start
    print("Tempo do tic", tic)
    time.sleep(1)
    period -= tic
    pinTEMP.off()    
    print("\nDormindo...")
    pinSIGFOX.off()

    machine.deepsleep(period)
#
# FIM
#


#
#   Função de envio de mensagem para plataforma Sigfox
#
def send_message_sigfox(temperatura, umidade, bateria):

    # Ajuste de mensagem para envio de temperatura
    message["negative"] = 0
    if (temperatura < 0):
        temperatura = temperatura * -1
        message["negative"] = 1

    message["temperature"] = temperatura
    message["type"] = 0 # 0 periodic, 1 asyncron
    message["humidity"] = umidade
    message["door"] = 0 # 1 open, 0 close
    message["batery"] = bateria

    device = Sigfox()
    if (device.sigfox_is_alive()):
        device.sigfox_send_frame(message)
        time.sleep(1)
        resp = device.sigfox_deep()
        print('\033[32m'+'Dormindo sigfox: '+'\033[0;0m', resp)
    else:
        print('\033[31m'+'ERROR - SIGFOX NAO ENCONTRADO'+'\033[0;0m')


#
# Funcao de leitura de tensao de bateria baseada em amostras
#
def read_voltage(adc0):
    total = 0.0
    for i in range(0,AMOSTRAS):
        total += 1.0 * adc0.read()
        time.sleep(1)
    return total/AMOSTRAS

#
# Funcao de calculo de bateria 
#

def read_bat(adc):
    signal = read_voltage(adc)
    Vout =  (signal*1.1) / 4095
    Vin_ = (signal*5.1) / 4095
    bat = Vin_ * ((valorR1+valorR2) / valorR2)
    print('\033[33m'+'\n================ LOG BATERIA ================\n'+'\033[0;0m')
    print('Vout = ', Vout) #Tensao lida na porta adc2
    print('Vin = ', Vin_) #Tensao da porta adc2 * 5.1
    print('\033[33m'+'=============================================\n'+'\033[0;0m')
    return bat

def read_temp_ds(ds, addrss):
    ds.convert_temp()
    time.sleep(2)
    for addr in addrss:
        print (addr)
        temp = ds.read_temp(addr)
    return temp


main()
#exec(open("main.py").read())
# message["negative"] = 0
# message["temperature"] = 20
# message["type"] = 0 
# message["humidity"] = 58
# message["door"] = 0
# message["batery"] = 4.5
