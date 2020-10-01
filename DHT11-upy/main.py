import dht
import machine

def main():

#
# Inicializacao do sensor
#
    d = dht.DHT11(machine.Pin(4))

#
# Realizacao da medicao
#
    d.measure()

    temp = d.temperature()
    hum = d.humidity()

#
# Debug
#    
    print('\033[33m'+'\n================ LEITURAS ================'+'\033[0;0m')
    print('\nTemperatura', temp)
    print('\nUmidade', hum)
    print('\033[33m''=============================================\n'+'\033[0;0m')


#
# FIM
#
main()
#exec(open("main.py").read())

