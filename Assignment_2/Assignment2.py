import random
while(True):
    temp=random.uniform(15,55);humidity=random.uniform(10,20)
    print('Temperature(C):',round(temp,2),'Humidity(gm/m^3):',round(humidity,2))
    print('High temperature detected' if temp>40 else 'Its normal')
    print('\n')