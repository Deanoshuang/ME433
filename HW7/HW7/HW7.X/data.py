# plot imu data to python 
import matplotlib.pyplot as plt 
import serial
ser = serial.Serial('COM3',230400)
print('Opening port: ')
print(ser.name)

read_samples = 10 # anything bigger than 1 to start out
ax = []
ay = []
az = []
gx = []
gy = []
gz = []
temp = []
pitch = []
roll = [] 
ser.write(b'\n')
while read_samples > 1:
    data_read = ser.read_until(b'\n',200) # get the data as bytes
    data_text = str(data_read,'utf-8') # turn the bytes to a string
    data = [float(i) for i in data_text.split()] 

    if(len(data)==10):
        read_samples = int(data[0])
        ax.append(data[1])
        ay.append(data[2])
        az.append(data[3])
        gx.append(data[4])
        gy.append(data[5])
        gz.append(data[6])
        temp.append(data[7])
        pitch.append(data[8])
        roll.append(data[9])
# plot it
t = range(len(ax)) # time array
plt.plot(t,ax,'ro-',t,ay,'bo-',t,az,'ko-')
plt.ylabel('Accelerations')
plt.xlabel('Samples')
plt.legend(['x','y','z'])
plt.show()

t = range(len(gx)) # time array
plt.plot(t,gx,'ro-',t,gy,'bo-',t,gz,'ko-')
plt.ylabel('O value')
plt.xlabel('Samples')
plt.legend(['Gx','Gy','Gz'])
plt.show()

t = range(len(temp)) # time array
plt.plot(t,temp,'ro-')
plt.ylabel('Temp')
plt.xlabel('Samples')
plt.show()

t = range(len(temp)) # time array
plt.plot(t,pitch,'ro-',t,roll,'bo-')
plt.ylabel('Pitch Roll Angles (Radians)')
plt.xlabel('Samples')
plt.legend(['Pitch','Roll'])
plt.show()

# be sure to close the port
ser.close()
