import modern_robotics as mr
import numpy as np
R13 = np.array([[-0.7071, 0, -0.7071], [0, 1, 0], [0.7071, 0, -0.7071]])
Rs2 = np.array([[-0.6964, 0.1736, 0.6964], [-0.1228, -0.9848, 0.1228], [0.7071, 0, 0.7071]])
R15 = np.array([[-0.9839, -0.1558, 0.0872], [-0.1564, 0.9877, 0], [-0.0861, -0.0136, -0.9962]])
R12 = np.array([[0.7071, 0, -0.7071], [0, 1, 0], [0.7071, 0, 0.7071]])
R34 = np.array([[0.6428, 0, -0.7660], [0, 1, 0], [0.7660, 0, 0.6428]])
Rs6 = np.array([[-0.1676,0.3250,-0.9308], [-0.0434,-0.9456,-0.3224],[-0.9849,-0.0136,0.1726]])
R6b = np.array([[-1, 0, 0], [0, 0, 1], [0, 1, 0]])

print(len(R6b))
w1=[0,0,1]
w2=[0,1,0]
w3=[0,1,0]
w4=[0,1,0]
w6=[0,1,0]
w5=[0,0,-1]
#Calculated by Rotation matrix cancelation rule
Rsb=np.dot(Rs6,R6b) 
print("The calculated Rsb is: ")
print(Rsb)
print("\n")
R21=mr.RotInv(R12)
R23=np.dot(R21,R13)
Rs1=np.dot(Rs2,R21)
Rs3=np.dot(Rs1,R13)
Rs4=np.dot(Rs3,R34)
Rs5=np.dot(Rs1,R15)
R43=mr.RotInv(R34)
R31=mr.RotInv(R13)
R35=np.dot(R31,R15)
R45=np.dot(R43,R35)
R51=mr.RotInv(R15)
R1s=mr.RotInv(Rs1)
R5s=np.dot(R51,R1s)
R56=np.dot(R5s,Rs6)

thetas=[]
omghats=[]
lst=[Rs1,R12,R23,R34,R45,R56]
for i in lst:
    wtheta=mr.MatrixLog3(i) #find so3 exponential coordinate
    omghattheta=mr.so3ToVec(wtheta) #convert so3 to 3-vector
    [omghat,theta] = mr.AxisAng3(omghattheta) #find rotation axis and joint angle
    thetas.append(theta)
    omghats.append(list(omghat))

print("axis of rotations are:")
for j in omghats:
    print(j)
print("\n")
print("joint angles are:")
print(thetas)



